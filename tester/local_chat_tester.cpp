/*
 * copyright (c) 2010-2023 belledonne communications sarl.
 *
 * This file is part of Liblinphone
 * (see https://gitlab.linphone.org/BC/public/liblinphone).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "conference/participant.h"
#include "core/core-p.h"
#include "liblinphone_tester.h"
#include "local_conference_tester_functions.h"

namespace LinphoneTest {

static void group_chat_room_creation_server(void) {
	Focus focus("chloe_rc");
	{ // to make sure focus is destroyed after clients.
		ClientConference marie("marie_rc", focus.getIdentity());
		ClientConference pauline("pauline_rc", focus.getIdentity());
		ClientConference laure("laure_tcp_rc", focus.getIdentity());

		focus.registerAsParticipantDevice(marie);
		focus.registerAsParticipantDevice(pauline);
		focus.registerAsParticipantDevice(laure);

		bctbx_list_t *coresList = bctbx_list_append(NULL, focus.getLc());
		coresList = bctbx_list_append(coresList, marie.getLc());
		coresList = bctbx_list_append(coresList, pauline.getLc());
		coresList = bctbx_list_append(coresList, laure.getLc());

		Address paulineAddr = pauline.getIdentity();
		Address laureAddr = laure.getIdentity();
		bctbx_list_t *participantsAddresses = bctbx_list_append(NULL, linphone_address_ref(paulineAddr.toC()));
		participantsAddresses = bctbx_list_append(participantsAddresses, linphone_address_ref(laureAddr.toC()));

		stats initialMarieStats = marie.getStats();
		stats initialPaulineStats = pauline.getStats();
		stats initialLaureStats = laure.getStats();

		// Marie creates a new group chat room
		const char *initialSubject = "Colleagues @work";
		LinphoneChatRoom *marieCr =
		    create_chat_room_client_side(coresList, marie.getCMgr(), &initialMarieStats, participantsAddresses,
		                                 initialSubject, FALSE, LinphoneChatRoomEphemeralModeDeviceManaged);
		const LinphoneAddress *confAddr = linphone_chat_room_get_conference_address(marieCr);

		// Check that the chat room is correctly created on Pauline's side and that the participants are added
		LinphoneChatRoom *paulineCr = check_creation_chat_room_client_side(
		    coresList, pauline.getCMgr(), &initialPaulineStats, confAddr, initialSubject, 2, FALSE);
		LinphoneChatRoom *laureCr = check_creation_chat_room_client_side(coresList, laure.getCMgr(), &initialLaureStats,
		                                                                 confAddr, initialSubject, 2, FALSE);

		// Marie now changes the subject
		const char *newSubject = "Let's go drink a beer #party";
		linphone_chat_room_set_subject(marieCr, newSubject);
		BC_ASSERT_TRUE(wait_for_list(coresList, &marie.getStats().number_of_subject_changed,
		                             initialMarieStats.number_of_subject_changed + 1, liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &pauline.getStats().number_of_subject_changed,
		                             initialPaulineStats.number_of_subject_changed + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &laure.getStats().number_of_subject_changed,
		                             initialLaureStats.number_of_subject_changed + 1, liblinphone_tester_sip_timeout));
		BC_ASSERT_STRING_EQUAL(linphone_chat_room_get_subject(marieCr), newSubject);
		BC_ASSERT_STRING_EQUAL(linphone_chat_room_get_subject(paulineCr), newSubject);
		BC_ASSERT_STRING_EQUAL(linphone_chat_room_get_subject(laureCr), newSubject);

		// Bring Laure offline and remove her from the chat room
		// As Laure is offline, she is not notified of the removal
		linphone_core_set_network_reachable(laure.getLc(), FALSE);
		BC_ASSERT_FALSE(linphone_core_is_network_reachable(laure.getLc()));
		CoreManagerAssert({focus, marie, pauline, laure}).waitUntil(std::chrono::seconds(1), [] { return false; });
		LinphoneParticipant *laureParticipant = linphone_chat_room_find_participant(marieCr, laureAddr.toC());
		BC_ASSERT_PTR_NOT_NULL(laureParticipant);
		linphone_chat_room_remove_participant(marieCr, laureParticipant);

		BC_ASSERT_TRUE(wait_for_list(coresList, &marie.getStats().number_of_participants_removed,
		                             initialMarieStats.number_of_participants_removed + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &pauline.getStats().number_of_participants_removed,
		                             initialPaulineStats.number_of_participants_removed + 1,
		                             liblinphone_tester_sip_timeout));

		// Check that Laure's conference is not destroyed
		BC_ASSERT_EQUAL(laure.getStats().number_of_LinphoneConferenceStateTerminated,
		                initialLaureStats.number_of_LinphoneConferenceStateTerminated, int, "%d");

		coresList = bctbx_list_remove(coresList, focus.getLc());
		// Restart flexisip
		focus.reStart();
		coresList = bctbx_list_append(coresList, focus.getLc());

		for (auto chatRoom : focus.getCore().getChatRooms()) {

			BC_ASSERT_EQUAL(linphone_chat_room_get_nb_participants(L_GET_C_BACK_PTR(chatRoom)), 3, int, "%d");
		}

		BC_ASSERT_FALSE(wait_for_list(coresList, &laure.getStats().number_of_LinphoneConferenceStateTerminated,
		                              initialLaureStats.number_of_LinphoneConferenceStateTerminated + 1, 3000));

		// Laure comes back online and its chatroom is expected to be deleted
		linphone_core_set_network_reachable(laure.getLc(), TRUE);
		LinphoneAddress *laureDeviceAddress = linphone_address_clone(
		    linphone_proxy_config_get_contact(linphone_core_get_default_proxy_config(laure.getLc())));
		// Notify chat room that a participant has registered
		focus.notifyParticipantDeviceRegistration(linphone_chat_room_get_conference_address(marieCr),
		                                          laureDeviceAddress);
		linphone_address_unref(laureDeviceAddress);
		BC_ASSERT_TRUE(wait_for_list(coresList, &laure.getStats().number_of_LinphoneConferenceStateTerminated,
		                             initialLaureStats.number_of_LinphoneConferenceStateTerminated + 1,
		                             liblinphone_tester_sip_timeout));

		for (auto chatRoom : focus.getCore().getChatRooms()) {
			BC_ASSERT_EQUAL(linphone_chat_room_get_nb_participants(L_GET_C_BACK_PTR(chatRoom)), 3, int, "%d");
		}

		linphone_chat_room_leave(paulineCr);
	}
}

static void group_chat_room_server_deletion(void) {
	Focus focus("chloe_rc");
	{ // to make sure focus is destroyed after clients.
		ClientConference marie("marie_rc", focus.getIdentity());
		ClientConference pauline("pauline_rc", focus.getIdentity());
		ClientConference pauline2("pauline_rc", focus.getIdentity());

		focus.registerAsParticipantDevice(marie);
		focus.registerAsParticipantDevice(pauline);
		focus.registerAsParticipantDevice(pauline2);

		bctbx_list_t *coresList = bctbx_list_append(NULL, focus.getLc());
		coresList = bctbx_list_append(coresList, marie.getLc());
		coresList = bctbx_list_append(coresList, pauline.getLc());
		coresList = bctbx_list_append(coresList, pauline2.getLc());
		Address paulineAddr = pauline.getIdentity();
		bctbx_list_t *participantsAddresses = bctbx_list_append(NULL, linphone_address_ref(paulineAddr.toC()));

		stats initialFocusStats = focus.getStats();
		stats initialMarieStats = marie.getStats();
		stats initialPaulineStats = pauline.getStats();
		stats initialPauline2Stats = pauline2.getStats();

		// Marie creates a new group chat room
		const char *initialSubject = "Colleagues #together";
		LinphoneChatRoom *marieCr =
		    create_chat_room_client_side(coresList, marie.getCMgr(), &initialMarieStats, participantsAddresses,
		                                 initialSubject, FALSE, LinphoneChatRoomEphemeralModeDeviceManaged);
		const LinphoneAddress *confAddr = linphone_chat_room_get_conference_address(marieCr);

		// Check that the chat room is correctly created on Pauline's side and that the participants are added
		LinphoneChatRoom *paulineCr = check_creation_chat_room_client_side(
		    coresList, pauline.getCMgr(), &initialPaulineStats, confAddr, initialSubject, 1, FALSE);

		LinphoneChatRoom *pauline2Cr = check_creation_chat_room_client_side(
		    coresList, pauline2.getCMgr(), &initialPauline2Stats, confAddr, initialSubject, 1, FALSE);

		/*BC_ASSERT_TRUE(*/ CoreManagerAssert({focus, marie, pauline, pauline2}).wait([&focus] {
			for (auto chatRoom : focus.getCore().getChatRooms()) {
				for (auto participant : chatRoom->getParticipants()) {
					const auto &devices = participant->getDevices();
					if (devices.size() > 0) {
						return false;
					}
					for (auto device : devices)
						if (device->getState() != ParticipantDevice::State::Present) {
							return false;
						}
				}
			}
			return true;
		}) /*)*/;

		LinphoneChatMessage *msg = linphone_chat_room_create_message_from_utf8(marieCr, "message blabla");
		linphone_chat_message_send(msg);
		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, pauline, pauline2}).wait([msg] {
			return (linphone_chat_message_get_state(msg) == LinphoneChatMessageStateDelivered);
		}));
		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, pauline, pauline2}).wait([paulineCr] {
			return linphone_chat_room_get_unread_messages_count(paulineCr) == 1;
		}));
		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, pauline, pauline2}).wait([pauline2Cr] {
			return linphone_chat_room_get_unread_messages_count(pauline2Cr) == 1;
		}));

		for (auto chatRoom : focus.getCore().getChatRooms()) {
			for (auto participant : chatRoom->getParticipants()) {
				//  force deletion by removing devices
				std::shared_ptr<Address> participantAddress = participant->getAddress();
				linphone_chat_room_set_participant_devices(L_GET_C_BACK_PTR(chatRoom), participantAddress->toC(), NULL);
			}
		}

		// wait until chatroom is deleted server side
		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, pauline, pauline2}).wait([&focus] {
			return focus.getCore().getChatRooms().size() == 0;
		}));

		// wait bit more to detect side effect if any
		CoreManagerAssert({focus, marie, pauline, pauline2}).waitUntil(chrono::seconds(2), [] { return false; });

		BC_ASSERT_EQUAL(focus.getStats().number_of_LinphoneConferenceStateDeleted,
		                initialFocusStats.number_of_LinphoneConferenceStateDeleted + 1, int, "%d");

		// Clean db from chat room
		linphone_chat_message_unref(msg);

		bctbx_list_free(coresList);
	}
}

static void group_chat_room_server_deletion_with_rmt_lst_event_handler(void) {
	Focus focus("chloe_rc");
	{ // to make sure focus is destroyed after clients.
		ClientConference marie("marie_rc", focus.getIdentity());
		ClientConference pauline("pauline_rc", focus.getIdentity());

		focus.registerAsParticipantDevice(marie);
		focus.registerAsParticipantDevice(pauline);

		bctbx_list_t *coresList = bctbx_list_append(NULL, focus.getLc());
		coresList = bctbx_list_append(coresList, marie.getLc());
		coresList = bctbx_list_append(coresList, pauline.getLc());
		Address paulineAddr = pauline.getIdentity();
		bctbx_list_t *participantsAddresses = bctbx_list_append(NULL, linphone_address_ref(paulineAddr.toC()));

		stats initialMarieStats = marie.getStats();
		stats initialPaulineStats = pauline.getStats();

		// Marie creates a new group chat room
		const char *initialSubject = "Colleagues (characters: $ £ çà)";
		LinphoneChatRoom *marieCr =
		    create_chat_room_client_side(coresList, marie.getCMgr(), &initialMarieStats, participantsAddresses,
		                                 initialSubject, FALSE, LinphoneChatRoomEphemeralModeDeviceManaged);
		const LinphoneAddress *confAddr = linphone_chat_room_get_conference_address(marieCr);

		// Check that the chat room is correctly created on Pauline's side and that the participants are added
		LinphoneChatRoom *paulineCr = check_creation_chat_room_client_side(
		    coresList, pauline.getCMgr(), &initialPaulineStats, confAddr, initialSubject, 1, FALSE);

		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, pauline}).wait([&focus] {
			for (auto chatRoom : focus.getCore().getChatRooms()) {
				for (auto participant : chatRoom->getParticipants()) {
					for (auto device : participant->getDevices())
						if (device->getState() != ParticipantDevice::State::Present) {
							return false;
						}
				}
			}
			return true;
		}));

		LinphoneChatMessage *msg = linphone_chat_room_create_message_from_utf8(marieCr, "message blabla");
		linphone_chat_message_send(msg);
		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, pauline}).wait([msg] {
			return (linphone_chat_message_get_state(msg) == LinphoneChatMessageStateDelivered);
		}));
		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, pauline}).wait([paulineCr] {
			return linphone_chat_room_get_unread_messages_count(paulineCr) == 1;
		}));

		// now with simulate foregound/backgroud switch to get a remote event handler list instead of a simple remote
		// event handler
		linphone_core_enter_background(pauline.getLc());
		linphone_config_set_bool(linphone_core_get_config(pauline.getLc()), "misc",
		                         "conference_event_package_force_full_state", TRUE);
		CoreManagerAssert({focus, marie, pauline}).waitUntil(std::chrono::seconds(1), [] { return false; });

		unsigned int paulineCrNo = (unsigned int)bctbx_list_size(linphone_core_get_chat_rooms(pauline.getLc()));
		coresList = bctbx_list_remove(coresList, pauline.getLc());
		pauline.reStart();
		coresList = bctbx_list_append(coresList, pauline.getLc());

		// Wait for chat rooms to be recovered from the main DB
		BC_ASSERT_TRUE(wait_for_list(coresList, &pauline.getStats().number_of_LinphoneConferenceStateCreated,
		                             paulineCrNo, liblinphone_tester_sip_timeout));
		char *paulineDeviceIdentity = linphone_core_get_device_identity(pauline.getLc());
		LinphoneAddress *paulineLocalAddr = linphone_address_new(paulineDeviceIdentity);
		bctbx_free(paulineDeviceIdentity);
		paulineCr = pauline.searchChatRoom(paulineLocalAddr, confAddr);
		linphone_address_unref(paulineLocalAddr);
		BC_ASSERT_PTR_NOT_NULL(paulineCr);

		CoreManagerAssert({focus, marie, pauline}).waitUntil(std::chrono::seconds(1), [] { return false; });

		CoreManagerAssert({focus, marie}).waitUntil(std::chrono::seconds(2), [] { return false; });

		for (auto chatRoom : focus.getCore().getChatRooms()) {
			for (auto participant : chatRoom->getParticipants()) {
				//  force deletion by removing devices
				std::shared_ptr<Address> participantAddress = participant->getAddress();
				linphone_chat_room_set_participant_devices(L_GET_C_BACK_PTR(chatRoom), participantAddress->toC(), NULL);
			}
		}

		// wait until chatroom is deleted server side
		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, pauline}).wait([&focus] {
			return focus.getCore().getChatRooms().size() == 0;
		}));

		// wait bit more to detect side effect if any
		CoreManagerAssert({focus, marie, pauline}).waitUntil(chrono::seconds(2), [] { return false; });

		// to avoid creation attempt of a new chatroom
		auto config = focus.getDefaultProxyConfig();
		linphone_proxy_config_edit(config);
		linphone_proxy_config_set_conference_factory_uri(config, NULL);
		linphone_proxy_config_done(config);

		linphone_chat_message_unref(msg);

		bctbx_list_free(coresList);
	}
}

static void group_chat_room_with_client_removed_added(void) {
	Focus focus("chloe_rc");
	{ // to make sure focus is destroyed after clients.
		ClientConference marie("marie_rc", focus.getIdentity());
		ClientConference michelle("michelle_rc", focus.getIdentity());
		ClientConference pauline("pauline_rc", focus.getIdentity());

		focus.registerAsParticipantDevice(marie);
		focus.registerAsParticipantDevice(michelle);
		focus.registerAsParticipantDevice(pauline);

		bctbx_list_t *coresList = bctbx_list_append(NULL, focus.getLc());
		coresList = bctbx_list_append(coresList, marie.getLc());
		coresList = bctbx_list_append(coresList, michelle.getLc());
		coresList = bctbx_list_append(coresList, pauline.getLc());
		bctbx_list_t *participantsAddresses = NULL;
		Address michelleAddr = michelle.getIdentity();
		participantsAddresses = bctbx_list_append(participantsAddresses, linphone_address_ref(michelleAddr.toC()));
		Address paulineAddr = pauline.getIdentity();
		participantsAddresses = bctbx_list_append(participantsAddresses, linphone_address_ref(paulineAddr.toC()));

		stats initialMarieStats = marie.getStats();
		stats initialMichelleStats = michelle.getStats();
		stats initialPaulineStats = pauline.getStats();

		// Marie creates a new group chat room
		const char *initialSubject = "Colleagues (characters: $ £ çà)";
		LinphoneChatRoom *marieCr = create_chat_room_client_side_with_expected_number_of_participants(
		    coresList, marie.getCMgr(), &initialMarieStats, participantsAddresses, initialSubject, 2, FALSE,
		    LinphoneChatRoomEphemeralModeDeviceManaged);
		const LinphoneAddress *confAddr = linphone_chat_room_get_conference_address(marieCr);

		// Check that the chat room is correctly created on Michelle's side and that the participants are added
		LinphoneChatRoom *michelleCr = check_creation_chat_room_client_side(
		    coresList, michelle.getCMgr(), &initialMichelleStats, confAddr, initialSubject, 2, FALSE);

		// Check that the chat room is correctly created on Pauline's side and that the participants are added
		LinphoneChatRoom *paulineCr = check_creation_chat_room_client_side(
		    coresList, pauline.getCMgr(), &initialPaulineStats, confAddr, initialSubject, 2, FALSE);

		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, pauline, michelle}).wait([&focus] {
			for (auto chatRoom : focus.getCore().getChatRooms()) {
				for (auto participant : chatRoom->getParticipants()) {
					for (auto device : participant->getDevices())
						if (device->getState() != ParticipantDevice::State::Present) {
							return false;
						}
				}
			}
			return true;
		}));

		BC_ASSERT_TRUE(wait_for_list(coresList, &michelle.getStats().number_of_LinphoneConferenceStateCreated,
		                             initialMichelleStats.number_of_LinphoneConferenceStateCreated + 1,
		                             liblinphone_tester_sip_timeout));

		// Marie now changes the subject
		const char *newSubject = "Let's go drink a beer";
		linphone_chat_room_set_subject(marieCr, newSubject);
		BC_ASSERT_TRUE(wait_for_list(coresList, &marie.getStats().number_of_subject_changed,
		                             initialMarieStats.number_of_subject_changed + 1, liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &michelle.getStats().number_of_subject_changed,
		                             initialMichelleStats.number_of_subject_changed + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &pauline.getStats().number_of_subject_changed,
		                             initialPaulineStats.number_of_subject_changed + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_STRING_EQUAL(linphone_chat_room_get_subject(marieCr), newSubject);
		BC_ASSERT_STRING_EQUAL(linphone_chat_room_get_subject(paulineCr), newSubject);
		BC_ASSERT_STRING_EQUAL(linphone_chat_room_get_subject(michelleCr), newSubject);

		BC_ASSERT_EQUAL(linphone_chat_room_get_nb_participants(marieCr), 2, int, "%d");
		BC_ASSERT_EQUAL(linphone_chat_room_get_nb_participants(paulineCr), 2, int, "%d");
		BC_ASSERT_EQUAL(linphone_chat_room_get_nb_participants(michelleCr), 2, int, "%d");

		initialMarieStats = marie.getStats();
		initialMichelleStats = michelle.getStats();
		initialPaulineStats = pauline.getStats();

		ClientConference michelle2("michelle_rc", focus.getIdentity());
		stats initialMichelle2Stats = michelle2.getStats();
		coresList = bctbx_list_append(coresList, michelle2.getLc());
		focus.registerAsParticipantDevice(michelle2);
		// Notify chat room that a participant has registered

		bctbx_list_t *devices = NULL;
		const LinphoneAddress *deviceAddr = linphone_proxy_config_get_contact(michelle.getDefaultProxyConfig());
		LinphoneParticipantDeviceIdentity *identity =
		    linphone_factory_create_participant_device_identity(linphone_factory_get(), deviceAddr, "");
		bctbx_list_t *specs = linphone_core_get_linphone_specs_list(michelle.getLc());
		linphone_participant_device_identity_set_capability_descriptor_2(identity, specs);
		bctbx_list_free_with_data(specs, ms_free);
		devices = bctbx_list_append(devices, identity);

		deviceAddr = linphone_proxy_config_get_contact(michelle2.getDefaultProxyConfig());
		identity = linphone_factory_create_participant_device_identity(linphone_factory_get(), deviceAddr, "");
		specs = linphone_core_get_linphone_specs_list(michelle2.getLc());
		linphone_participant_device_identity_set_capability_descriptor_2(identity, specs);
		bctbx_list_free_with_data(specs, ms_free);
		devices = bctbx_list_append(devices, identity);

		for (auto chatRoom : focus.getCore().getChatRooms()) {
			linphone_chat_room_set_participant_devices(L_GET_C_BACK_PTR(chatRoom), michelle.getCMgr()->identity,
			                                           devices);
		}
		bctbx_list_free_with_data(devices, (bctbx_list_free_func)belle_sip_object_unref);

		LinphoneChatRoom *michelle2Cr = check_creation_chat_room_client_side(
		    coresList, michelle2.getCMgr(), &initialMichelle2Stats, confAddr, newSubject, 2, FALSE);
		BC_ASSERT_TRUE(wait_for_list(coresList, &michelle2.getStats().number_of_LinphoneConferenceStateCreated,
		                             initialMichelle2Stats.number_of_LinphoneConferenceStateCreated + 1,
		                             liblinphone_tester_sip_timeout));

		for (auto chatRoom : focus.getCore().getChatRooms()) {
			BC_ASSERT_EQUAL(linphone_chat_room_get_nb_participants(L_GET_C_BACK_PTR(chatRoom)), 3, int, "%d");
		}

		BC_ASSERT_TRUE(wait_for_list(coresList, &marie.getStats().number_of_participant_devices_added,
		                             initialMarieStats.number_of_participant_devices_added + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &pauline.getStats().number_of_participant_devices_added,
		                             initialPaulineStats.number_of_participant_devices_added + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &michelle.getStats().number_of_participant_devices_added,
		                             initialMichelleStats.number_of_participant_devices_added + 1,
		                             liblinphone_tester_sip_timeout));

		LinphoneAddress *michelle2Contact =
		    linphone_address_clone(linphone_proxy_config_get_contact(michelle2.getDefaultProxyConfig()));
		ms_message("%s is restarting its core", linphone_address_as_string(michelle2Contact));
		linphone_address_unref(michelle2Contact);
		coresList = bctbx_list_remove(coresList, michelle2.getLc());
		michelle2.reStart();
		michelle2.setupMgrForConference();
		coresList = bctbx_list_append(coresList, michelle2.getLc());

		BC_ASSERT_TRUE(wait_for_list(coresList, &michelle2.getStats().number_of_LinphoneConferenceStateCreated, 1,
		                             liblinphone_tester_sip_timeout));
		LinphoneAddress *michelleDeviceAddr =
		    linphone_address_clone(linphone_proxy_config_get_contact(michelle2.getDefaultProxyConfig()));
		michelle2Cr = michelle2.searchChatRoom(michelleDeviceAddr, confAddr);
		BC_ASSERT_PTR_NOT_NULL(michelle2Cr);

		if (michelle2Cr) {
			BC_ASSERT_EQUAL(linphone_chat_room_get_nb_participants(michelle2Cr), 2, int, "%d");
			BC_ASSERT_STRING_EQUAL(linphone_chat_room_get_subject(michelle2Cr), newSubject);
		}

		BC_ASSERT_EQUAL(linphone_chat_room_get_nb_participants(marieCr), 2, int, "%d");
		BC_ASSERT_STRING_EQUAL(linphone_chat_room_get_subject(marieCr), newSubject);

		BC_ASSERT_EQUAL(linphone_chat_room_get_nb_participants(michelleCr), 2, int, "%d");
		BC_ASSERT_STRING_EQUAL(linphone_chat_room_get_subject(michelleCr), newSubject);

		BC_ASSERT_EQUAL(linphone_chat_room_get_nb_participants(paulineCr), 2, int, "%d");
		BC_ASSERT_STRING_EQUAL(linphone_chat_room_get_subject(paulineCr), newSubject);

		linphone_core_delete_chat_room(michelle2.getLc(), michelle2Cr);
		BC_ASSERT_TRUE(wait_for_list(coresList, &michelle2.getStats().number_of_LinphoneConferenceStateDeleted,
		                             initialMichelle2Stats.number_of_LinphoneConferenceStateDeleted + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &marie.getStats().number_of_participants_removed,
		                             initialMarieStats.number_of_participant_devices_removed + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &pauline.getStats().number_of_participants_removed,
		                             initialPaulineStats.number_of_participant_devices_removed + 1,
		                             liblinphone_tester_sip_timeout));

		initialMarieStats = marie.getStats();
		initialMichelleStats = michelle.getStats();
		initialPaulineStats = pauline.getStats();

		Address michelle2Addr = michelle2.getIdentity();
		linphone_chat_room_add_participant(marieCr, linphone_address_ref(michelle2Addr.toC()));

		michelle2Cr = check_creation_chat_room_client_side(coresList, michelle2.getCMgr(), &initialMichelle2Stats,
		                                                   confAddr, newSubject, 2, FALSE);
		BC_ASSERT_TRUE(wait_for_list(coresList, &michelle2.getStats().number_of_LinphoneConferenceStateCreated,
		                             initialMichelle2Stats.number_of_LinphoneConferenceStateCreated + 1,
		                             liblinphone_tester_sip_timeout));

		BC_ASSERT_TRUE(wait_for_list(coresList, &marie.getStats().number_of_participant_devices_added,
		                             initialMarieStats.number_of_participant_devices_added + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &pauline.getStats().number_of_participant_devices_added,
		                             initialPaulineStats.number_of_participant_devices_added + 1,
		                             liblinphone_tester_sip_timeout));

		LinphoneChatMessage *msg = linphone_chat_room_create_message_from_utf8(marieCr, "message blabla");
		linphone_chat_message_send(msg);
		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, pauline, michelle, michelle2}).wait([msg] {
			return (linphone_chat_message_get_state(msg) == LinphoneChatMessageStateDelivered);
		}));
		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, pauline, michelle, michelle2}).wait([michelleCr] {
			return linphone_chat_room_get_unread_messages_count(michelleCr) == 1;
		}));
		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, pauline, michelle, michelle2}).wait([michelle2Cr] {
			return linphone_chat_room_get_unread_messages_count(michelle2Cr) == 1;
		}));
		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, pauline, michelle, michelle2}).wait([paulineCr] {
			return linphone_chat_room_get_unread_messages_count(paulineCr) == 1;
		}));
		linphone_chat_message_unref(msg);

		CoreManagerAssert({focus, marie, pauline, michelle, michelle2}).waitUntil(std::chrono::seconds(3), [] {
			return false;
		});

		for (auto chatRoom : focus.getCore().getChatRooms()) {
			for (auto participant : chatRoom->getParticipants()) {
				//  force deletion by removing devices
				std::shared_ptr<Address> participantAddress = participant->getAddress();
				linphone_chat_room_set_participant_devices(L_GET_C_BACK_PTR(chatRoom), participantAddress->toC(), NULL);
			}
		}

		// wait until chatroom is deleted server side
		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, pauline, michelle, michelle2}).wait([&focus] {
			return focus.getCore().getChatRooms().size() == 0;
		}));

		// wait bit more to detect side effect if any
		CoreManagerAssert({focus, marie, pauline, michelle, michelle2}).waitUntil(chrono::seconds(2), [] {
			return false;
		});

		// to avoid creation attempt of a new chatroom
		auto config = focus.getDefaultProxyConfig();
		linphone_proxy_config_edit(config);
		linphone_proxy_config_set_conference_factory_uri(config, NULL);
		linphone_proxy_config_done(config);

		linphone_address_unref(michelleDeviceAddr);
		bctbx_list_free(coresList);
	}
}

static void group_chat_room_with_client_deletes_chatroom_after_restart(void) {
	Focus focus("chloe_rc");
	{ // to make sure focus is destroyed after clients.
		bool encrypted = false;
		ClientConference marie("marie_rc", focus.getIdentity(), encrypted);
		ClientConference michelle("michelle_rc", focus.getIdentity(), encrypted);
		ClientConference pauline("pauline_rc", focus.getIdentity(), encrypted);
		ClientConference laure("laure_tcp_rc", focus.getIdentity(), encrypted);

		focus.registerAsParticipantDevice(marie);
		focus.registerAsParticipantDevice(michelle);
		focus.registerAsParticipantDevice(laure);
		focus.registerAsParticipantDevice(pauline);

		stats marie_stat = marie.getStats();
		stats pauline_stat = pauline.getStats();
		stats laure_stat = laure.getStats();
		stats michelle_stat = michelle.getStats();
		bctbx_list_t *coresList = bctbx_list_append(NULL, focus.getLc());
		coresList = bctbx_list_append(coresList, marie.getLc());
		coresList = bctbx_list_append(coresList, pauline.getLc());
		coresList = bctbx_list_append(coresList, laure.getLc());
		coresList = bctbx_list_append(coresList, michelle.getLc());

		bctbx_list_t *participantsAddresses = NULL;
		Address michelleAddr = michelle.getIdentity();
		participantsAddresses = bctbx_list_append(participantsAddresses, linphone_address_ref(michelleAddr.toC()));
		Address laureAddr = laure.getIdentity();
		participantsAddresses = bctbx_list_append(participantsAddresses, linphone_address_ref(laureAddr.toC()));
		Address paulineAddr = pauline.getIdentity();
		participantsAddresses = bctbx_list_append(participantsAddresses, linphone_address_ref(paulineAddr.toC()));

		// Marie creates a new group chat room
		const char *initialSubject = "Colleagues (characters: $ £ çà)";
		LinphoneChatRoom *marieCr = create_chat_room_client_side_with_expected_number_of_participants(
		    coresList, marie.getCMgr(), &marie_stat, participantsAddresses, initialSubject, 3, encrypted,
		    LinphoneChatRoomEphemeralModeDeviceManaged);
		BC_ASSERT_PTR_NOT_NULL(marieCr);
		const LinphoneAddress *confAddr = linphone_chat_room_get_conference_address(marieCr);
		char *conference_address = linphone_address_as_string(confAddr);

		// Check that the chat room is correctly created on Michelle's side and that the participants are added
		LinphoneChatRoom *michelleCr = check_creation_chat_room_client_side(
		    coresList, michelle.getCMgr(), &michelle_stat, confAddr, initialSubject, 3, FALSE);
		BC_ASSERT_PTR_NOT_NULL(michelleCr);

		// Check that the chat room is correctly created on Pauline's side and that the participants are added
		LinphoneChatRoom *paulineCr = check_creation_chat_room_client_side(coresList, pauline.getCMgr(), &pauline_stat,
		                                                                   confAddr, initialSubject, 3, FALSE);
		BC_ASSERT_PTR_NOT_NULL(paulineCr);

		// Check that the chat room is correctly created on Laure's side and that the participants are added
		LinphoneChatRoom *laureCr = check_creation_chat_room_client_side(coresList, laure.getCMgr(), &laure_stat,
		                                                                 confAddr, initialSubject, 3, FALSE);
		BC_ASSERT_PTR_NOT_NULL(laureCr);

		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, pauline, michelle, laure}).wait([&focus] {
			for (auto chatRoom : focus.getCore().getChatRooms()) {
				for (auto participant : chatRoom->getParticipants()) {
					for (auto device : participant->getDevices())
						if (device->getState() != ParticipantDevice::State::Present) {
							return false;
						}
				}
			}
			return true;
		}));

		BC_ASSERT_TRUE(wait_for_list(coresList, &michelle.getStats().number_of_LinphoneConferenceStateCreated,
		                             michelle_stat.number_of_LinphoneConferenceStateCreated + 1,
		                             liblinphone_tester_sip_timeout));

		BC_ASSERT_TRUE(wait_for_list(coresList, &laure.getStats().number_of_LinphoneConferenceStateCreated,
		                             laure_stat.number_of_LinphoneConferenceStateCreated + 1,
		                             liblinphone_tester_sip_timeout));

		BC_ASSERT_TRUE(wait_for_list(coresList, &pauline.getStats().number_of_LinphoneConferenceStateCreated,
		                             pauline_stat.number_of_LinphoneConferenceStateCreated + 1,
		                             liblinphone_tester_sip_timeout));

		// wait bit more to detect side effect if any
		CoreManagerAssert({focus, marie, pauline, michelle, laure}).waitUntil(chrono::seconds(5), [] { return false; });

		ms_message("%s reinitializes its core", linphone_core_get_identity(laure.getLc()));
		coresList = bctbx_list_remove(coresList, laure.getLc());
		linphone_core_manager_reinit(laure.getCMgr());

		stats focus_stat = focus.getStats();
		marie_stat = marie.getStats();
		pauline_stat = pauline.getStats();
		laure_stat = laure.getStats();
		michelle_stat = michelle.getStats();

		ms_message("%s starts again its core", linphone_core_get_identity(laure.getLc()));
		linphone_core_manager_start(laure.getCMgr(), TRUE);
		setup_mgr_for_conference(laure.getCMgr(), NULL);
		coresList = bctbx_list_append(coresList, laure.getLc());

		BC_ASSERT_TRUE(wait_for_list(coresList, &laure.getStats().number_of_LinphoneRegistrationOk,
		                             laure_stat.number_of_LinphoneRegistrationOk + 1, liblinphone_tester_sip_timeout));

		BC_ASSERT_TRUE(wait_for_list(coresList, &laure.getStats().number_of_LinphoneSubscriptionActive,
		                             laure_stat.number_of_LinphoneSubscriptionActive + 1,
		                             liblinphone_tester_sip_timeout));

		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, pauline, michelle, laure}).wait([&laure] {
			if (laure.getCore().getChatRooms().size() < 1) {
				return false;
			}
			for (auto chatRoom : laure.getCore().getChatRooms()) {
				if (chatRoom->getState() != ChatRoom::State::Created) {
					return false;
				}
			}
			return true;
		}));

		const LinphoneAddress *laureDeviceAddr =
		    linphone_proxy_config_get_contact(linphone_core_get_default_proxy_config(laure.getLc()));
		laureCr = linphone_core_search_chat_room(laure.getLc(), NULL, laureDeviceAddr, confAddr, NULL);
		BC_ASSERT_PTR_NOT_NULL(laureCr);

		if (laureCr) {
			linphone_core_delete_chat_room(laure.getLc(), laureCr);
		}

		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, pauline, michelle, laure}).wait([&laure] {
			return (laure.getCore().getChatRooms().size() == 0);
		}));

		BC_ASSERT_TRUE(wait_for_list(coresList, &focus.getStats().number_of_participants_removed,
		                             focus_stat.number_of_participants_removed + 1, liblinphone_tester_sip_timeout));

		BC_ASSERT_TRUE(wait_for_list(coresList, &marie.getStats().number_of_participants_removed,
		                             marie_stat.number_of_participants_removed + 1, liblinphone_tester_sip_timeout));

		BC_ASSERT_TRUE(wait_for_list(coresList, &pauline.getStats().number_of_participants_removed,
		                             pauline_stat.number_of_participants_removed + 1, liblinphone_tester_sip_timeout));

		BC_ASSERT_TRUE(wait_for_list(coresList, &michelle.getStats().number_of_participants_removed,
		                             michelle_stat.number_of_participants_removed + 1, liblinphone_tester_sip_timeout));

		for (auto chatRoom : focus.getCore().getChatRooms()) {
			for (auto participant : chatRoom->getParticipants()) {
				//  force deletion by removing devices
				std::shared_ptr<Address> participantAddress = participant->getAddress();
				linphone_chat_room_set_participant_devices(L_GET_C_BACK_PTR(chatRoom), participantAddress->toC(), NULL);
			}
		}

		// wait until chatroom is deleted server side
		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, pauline, michelle, laure}).wait([&focus] {
			return focus.getCore().getChatRooms().size() == 0;
		}));

		// wait bit more to detect side effect if any
		CoreManagerAssert({focus, marie, pauline, michelle, laure}).waitUntil(chrono::seconds(2), [] { return false; });

		// to avoid creation attempt of a new chatroom
		auto config = focus.getDefaultProxyConfig();
		linphone_proxy_config_edit(config);
		linphone_proxy_config_set_conference_factory_uri(config, NULL);
		linphone_proxy_config_done(config);

		ms_free(conference_address);
		bctbx_list_free(coresList);
	}
}

static void group_chat_room_with_client_restart(void) {
	group_chat_room_with_client_restart_base(false);
}

static void group_chat_room_with_client_registering_with_short_register_expires(void) {
	Focus focus("chloe_rc");
	{ // to make sure focus is destroyed after clients.
		bool_t encrypted = FALSE;
		ClientConference marie("marie_rc", focus.getIdentity(), encrypted);
		ClientConference michelle("michelle_rc", focus.getIdentity(), encrypted);
		ClientConference berthe("berthe_rc", focus.getIdentity(), encrypted);

		focus.registerAsParticipantDevice(marie);
		focus.registerAsParticipantDevice(michelle);
		focus.registerAsParticipantDevice(berthe);

		bctbx_list_t *coresList = bctbx_list_append(NULL, focus.getLc());
		coresList = bctbx_list_append(coresList, marie.getLc());
		coresList = bctbx_list_append(coresList, michelle.getLc());
		coresList = bctbx_list_append(coresList, berthe.getLc());
		bctbx_list_t *participantsAddresses = NULL;
		Address michelleAddr = michelle.getIdentity();
		participantsAddresses = bctbx_list_append(participantsAddresses, linphone_address_ref(michelleAddr.toC()));
		Address bertheAddr = berthe.getIdentity();
		participantsAddresses = bctbx_list_append(participantsAddresses, linphone_address_ref(bertheAddr.toC()));

		stats initialMarieStats = marie.getStats();
		stats initialMichelleStats = michelle.getStats();
		stats initialBertheStats = berthe.getStats();
		//
		// Marie creates a new group chat room
		const char *initialSubject = "Colleagues (characters: $ £ çà)";
		LinphoneChatRoom *marieCr = create_chat_room_client_side_with_expected_number_of_participants(
		    coresList, marie.getCMgr(), &initialMarieStats, participantsAddresses, initialSubject, 2, encrypted,
		    LinphoneChatRoomEphemeralModeDeviceManaged);
		const LinphoneAddress *confAddr = linphone_chat_room_get_conference_address(marieCr);

		// Check that the chat room is correctly created on Michelle's side and that the participants are added
		LinphoneChatRoom *michelleCr = check_creation_chat_room_client_side(
		    coresList, michelle.getCMgr(), &initialMichelleStats, confAddr, initialSubject, 2, FALSE);

		LinphoneChatRoom *bertheCr = check_creation_chat_room_client_side(
		    coresList, berthe.getCMgr(), &initialBertheStats, confAddr, initialSubject, 2, FALSE);

		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, michelle, berthe}).wait([&focus] {
			for (const auto &chatRoom : focus.getCore().getChatRooms()) {
				for (const auto &participant : chatRoom->getParticipants()) {
					for (const auto &device : participant->getDevices()) {
						if (device->getState() != ParticipantDevice::State::Present) {
							return false;
						}
					}
				}
			}
			return true;
		}));

		BC_ASSERT_TRUE(wait_for_list(coresList, &michelle.getStats().number_of_LinphoneConferenceStateCreated,
		                             initialMichelleStats.number_of_LinphoneConferenceStateCreated + 1,
		                             liblinphone_tester_sip_timeout));

		BC_ASSERT_TRUE(wait_for_list(coresList, &berthe.getStats().number_of_LinphoneConferenceStateCreated,
		                             initialBertheStats.number_of_LinphoneConferenceStateCreated + 1,
		                             liblinphone_tester_sip_timeout));

		// Marie now changes the subject
		const char *newSubject = "Let's go drink a beer";
		linphone_chat_room_set_subject(marieCr, newSubject);
		BC_ASSERT_TRUE(wait_for_list(coresList, &marie.getStats().number_of_subject_changed,
		                             initialMarieStats.number_of_subject_changed + 1, liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &michelle.getStats().number_of_subject_changed,
		                             initialMichelleStats.number_of_subject_changed + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &berthe.getStats().number_of_subject_changed,
		                             initialBertheStats.number_of_subject_changed + 1, liblinphone_tester_sip_timeout));
		BC_ASSERT_STRING_EQUAL(linphone_chat_room_get_subject(marieCr), newSubject);
		BC_ASSERT_STRING_EQUAL(linphone_chat_room_get_subject(michelleCr), newSubject);
		BC_ASSERT_STRING_EQUAL(linphone_chat_room_get_subject(bertheCr), newSubject);

		BC_ASSERT_EQUAL(linphone_chat_room_get_nb_participants(marieCr), 2, int, "%d");
		BC_ASSERT_EQUAL(linphone_chat_room_get_nb_participants(michelleCr), 2, int, "%d");
		BC_ASSERT_EQUAL(linphone_chat_room_get_nb_participants(bertheCr), 2, int, "%d");

		const std::initializer_list<std::reference_wrapper<ConfCoreManager>> cores2{focus, marie, michelle, berthe};
		for (const ConfCoreManager &core : cores2) {
			BC_ASSERT_TRUE(
			    CoreManagerAssert({focus, marie, michelle, berthe}).waitUntil(chrono::seconds(10), [&focus, &core] {
				    return checkChatroom(focus, core, -1);
			    }));
		};

		initialMichelleStats = michelle.getStats();
		stats initialFocusStats = focus.getStats();

		int expires = 1;
		LinphoneAddress *michelleContact = linphone_address_clone(
		    linphone_proxy_config_get_contact(linphone_core_get_default_proxy_config(michelle.getLc())));
		char *michelleContactString = linphone_address_as_string(michelleContact);
		ms_message("%s is registering again with expires set to %0d seconds", michelleContactString, expires);
		ms_free(michelleContactString);
		linphone_core_set_network_reachable(michelle.getLc(), FALSE);
		LinphoneAccount *account = linphone_core_get_default_account(michelle.getLc());
		const LinphoneAccountParams *account_params = linphone_account_get_params(account);
		LinphoneAccountParams *new_account_params = linphone_account_params_clone(account_params);
		linphone_account_params_set_expires(new_account_params, expires);
		linphone_account_set_params(account, new_account_params);
		linphone_account_params_unref(new_account_params);
		linphone_core_set_network_reachable(michelle.getLc(), TRUE);
		BC_ASSERT_TRUE(wait_for_list(coresList, &michelle.getStats().number_of_LinphoneRegistrationOk,
		                             initialMichelleStats.number_of_LinphoneRegistrationOk + 1,
		                             liblinphone_tester_sip_timeout));

		// We expect that the client sends 2 subscriptions to the server:
		// - one from the call onNetworkReacable which fails because the DNS resolution of sip.example.org has not been
		// done yet
		// - the second one upon reception of 200 Ok Registration Successful
		BC_ASSERT_TRUE(wait_for_list(coresList, &michelle.getStats().number_of_LinphoneSubscriptionOutgoingProgress,
		                             initialMichelleStats.number_of_LinphoneSubscriptionOutgoingProgress + 2,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &michelle.getStats().number_of_LinphoneSubscriptionTerminated,
		                             initialMichelleStats.number_of_LinphoneSubscriptionTerminated + 1,
		                             liblinphone_tester_sip_timeout));

		// Send many back to back registers to verify that the server is only sent one SUBSCRIBE
		for (int cnt = 0; cnt < 10; cnt++) {
			stats initialMichelleStats2 = michelle.getStats();
			linphone_core_refresh_registers(michelle.getLc());

			BC_ASSERT_TRUE(wait_for_list(coresList, &michelle.getStats().number_of_LinphoneRegistrationRefreshing,
			                             initialMichelleStats2.number_of_LinphoneRegistrationRefreshing + 1,
			                             liblinphone_tester_sip_timeout));
			BC_ASSERT_TRUE(wait_for_list(coresList, &michelle.getStats().number_of_LinphoneRegistrationOk,
			                             initialMichelleStats2.number_of_LinphoneRegistrationOk + 1,
			                             liblinphone_tester_sip_timeout));
		}

		// Verify that only one subscription is sent out to the conference server
		BC_ASSERT_TRUE(wait_for_list(coresList, &focus.getStats().number_of_LinphoneSubscriptionIncomingReceived,
		                             initialFocusStats.number_of_LinphoneSubscriptionIncomingReceived + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_FALSE(wait_for_list(coresList, &michelle.getStats().number_of_LinphoneSubscriptionOutgoingProgress,
		                              initialMichelleStats.number_of_LinphoneSubscriptionOutgoingProgress + 3, 5000));
		BC_ASSERT_FALSE(wait_for_list(coresList, &focus.getStats().number_of_LinphoneSubscriptionIncomingReceived,
		                              initialFocusStats.number_of_LinphoneSubscriptionIncomingReceived + 2, 1000));

		michelleCr = linphone_core_search_chat_room(michelle.getLc(), NULL, michelleContact, confAddr, NULL);
		BC_ASSERT_PTR_NOT_NULL(michelleCr);

		const std::initializer_list<std::reference_wrapper<ConfCoreManager>> cores{focus, marie, michelle, berthe};
		for (const ConfCoreManager &core : cores) {
			BC_ASSERT_TRUE(checkChatroom(focus, core, -1));
			for (auto chatRoom : core.getCore().getChatRooms()) {
				BC_ASSERT_EQUAL(chatRoom->getParticipants().size(), ((focus.getLc() == core.getLc())) ? 3 : 2, size_t,
				                "%zu");
				BC_ASSERT_STRING_EQUAL(chatRoom->getSubject().c_str(), newSubject);
			}
		};

		LinphoneChatMessage *msg = linphone_chat_room_create_message_from_utf8(michelleCr, "back with you");
		linphone_chat_message_send(msg);
		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, michelle, berthe}).wait([msg] {
			return (linphone_chat_message_get_state(msg) == LinphoneChatMessageStateDelivered);
		}));
		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, michelle, berthe}).wait([marieCr] {
			return linphone_chat_room_get_unread_messages_count(marieCr) == 1;
		}));
		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, michelle, berthe}).wait([bertheCr] {
			return linphone_chat_room_get_unread_messages_count(bertheCr) == 1;
		}));
		linphone_chat_message_unref(msg);
		msg = NULL;

		msg = linphone_chat_room_create_message_from_utf8(marieCr, "welcome back");
		linphone_chat_message_send(msg);
		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, michelle, berthe}).wait([msg] {
			return (linphone_chat_message_get_state(msg) == LinphoneChatMessageStateDelivered);
		}));
		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, michelle, berthe}).wait([bertheCr] {
			return linphone_chat_room_get_unread_messages_count(bertheCr) == 2;
		}));
		linphone_chat_message_unref(msg);
		msg = NULL;

		CoreManagerAssert({focus, marie, michelle, berthe}).waitUntil(std::chrono::seconds(2), [] { return false; });

		for (auto chatRoom : focus.getCore().getChatRooms()) {
			for (auto participant : chatRoom->getParticipants()) {
				//  force deletion by removing devices
				std::shared_ptr<Address> participantAddress = participant->getAddress();
				linphone_chat_room_set_participant_devices(L_GET_C_BACK_PTR(chatRoom), participantAddress->toC(), NULL);
			}
		}

		// wait until chatroom is deleted server side
		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, michelle}).wait([&focus] {
			return focus.getCore().getChatRooms().size() == 0;
		}));

		// wait bit more to detect side effect if any
		CoreManagerAssert({focus, marie, michelle, berthe}).waitUntil(chrono::seconds(2), [] { return false; });

		// to avoid creation attempt of a new chatroom
		LinphoneProxyConfig *config = linphone_core_get_default_proxy_config(focus.getLc());
		linphone_proxy_config_edit(config);
		linphone_proxy_config_set_conference_factory_uri(config, NULL);
		linphone_proxy_config_done(config);

		linphone_address_unref(michelleContact);
		bctbx_list_free(coresList);
	}
}

static void group_chat_room_with_client_restart_removed_from_server(void) {
	Focus focus("chloe_rc");
	{ // to make sure focus is destroyed after clients.
		bool_t encrypted = FALSE;
		ClientConference marie("marie_rc", focus.getIdentity(), encrypted);
		ClientConference michelle("michelle_rc", focus.getIdentity(), encrypted);
		ClientConference berthe("berthe_rc", focus.getIdentity(), encrypted);

		focus.registerAsParticipantDevice(marie);
		focus.registerAsParticipantDevice(michelle);
		focus.registerAsParticipantDevice(berthe);

		bctbx_list_t *coresList = bctbx_list_append(NULL, focus.getLc());
		coresList = bctbx_list_append(coresList, marie.getLc());
		coresList = bctbx_list_append(coresList, michelle.getLc());
		coresList = bctbx_list_append(coresList, berthe.getLc());
		bctbx_list_t *participantsAddresses = NULL;
		Address michelleAddr = michelle.getIdentity();
		participantsAddresses = bctbx_list_append(participantsAddresses, linphone_address_ref(michelleAddr.toC()));
		Address bertheAddr = berthe.getIdentity();
		participantsAddresses = bctbx_list_append(participantsAddresses, linphone_address_ref(bertheAddr.toC()));

		stats initialMarieStats = marie.getStats();
		stats initialMichelleStats = michelle.getStats();
		stats initialBertheStats = berthe.getStats();
		//
		// Marie creates a new group chat room
		const char *initialSubject = "Colleagues (characters: $ £ çà)";
		LinphoneChatRoom *marieCr = create_chat_room_client_side_with_expected_number_of_participants(
		    coresList, marie.getCMgr(), &initialMarieStats, participantsAddresses, initialSubject, 2, encrypted,
		    LinphoneChatRoomEphemeralModeDeviceManaged);
		const LinphoneAddress *confAddr = linphone_chat_room_get_conference_address(marieCr);

		// Check that the chat room is correctly created on Michelle's side and that the participants are added
		LinphoneChatRoom *michelleCr = check_creation_chat_room_client_side(
		    coresList, michelle.getCMgr(), &initialMichelleStats, confAddr, initialSubject, 2, FALSE);

		LinphoneChatRoom *bertheCr = check_creation_chat_room_client_side(
		    coresList, berthe.getCMgr(), &initialBertheStats, confAddr, initialSubject, 2, FALSE);

		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, michelle, berthe}).wait([&focus] {
			for (const auto &chatRoom : focus.getCore().getChatRooms()) {
				for (const auto &participant : chatRoom->getParticipants()) {
					for (const auto &device : participant->getDevices()) {
						if (device->getState() != ParticipantDevice::State::Present) {
							return false;
						}
					}
				}
			}
			return true;
		}));

		BC_ASSERT_TRUE(wait_for_list(coresList, &michelle.getStats().number_of_LinphoneConferenceStateCreated,
		                             initialMichelleStats.number_of_LinphoneConferenceStateCreated + 1,
		                             liblinphone_tester_sip_timeout));

		BC_ASSERT_TRUE(wait_for_list(coresList, &berthe.getStats().number_of_LinphoneConferenceStateCreated,
		                             initialBertheStats.number_of_LinphoneConferenceStateCreated + 1,
		                             liblinphone_tester_sip_timeout));

		// Marie now changes the subject
		const char *newSubject = "Let's go drink a beer";
		linphone_chat_room_set_subject(marieCr, newSubject);
		BC_ASSERT_TRUE(wait_for_list(coresList, &marie.getStats().number_of_subject_changed,
		                             initialMarieStats.number_of_subject_changed + 1, liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &michelle.getStats().number_of_subject_changed,
		                             initialMichelleStats.number_of_subject_changed + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &berthe.getStats().number_of_subject_changed,
		                             initialBertheStats.number_of_subject_changed + 1, liblinphone_tester_sip_timeout));
		BC_ASSERT_STRING_EQUAL(linphone_chat_room_get_subject(marieCr), newSubject);
		BC_ASSERT_STRING_EQUAL(linphone_chat_room_get_subject(michelleCr), newSubject);
		BC_ASSERT_STRING_EQUAL(linphone_chat_room_get_subject(bertheCr), newSubject);

		BC_ASSERT_EQUAL(linphone_chat_room_get_nb_participants(marieCr), 2, int, "%d");
		BC_ASSERT_EQUAL(linphone_chat_room_get_nb_participants(michelleCr), 2, int, "%d");
		BC_ASSERT_EQUAL(linphone_chat_room_get_nb_participants(bertheCr), 2, int, "%d");

		const std::initializer_list<std::reference_wrapper<ConfCoreManager>> cores2{focus, marie, michelle, berthe};
		for (const ConfCoreManager &core : cores2) {
			BC_ASSERT_TRUE(
			    CoreManagerAssert({focus, marie, michelle, berthe}).waitUntil(chrono::seconds(10), [&focus, &core] {
				    return checkChatroom(focus, core, -1);
			    }));
		};

		initialMichelleStats = michelle.getStats();

		LinphoneAddress *michelleContact = linphone_address_clone(
		    linphone_proxy_config_get_contact(linphone_core_get_default_proxy_config(michelle.getLc())));
		ms_message("%s is restarting its core", linphone_address_as_string(michelleContact));
		coresList = bctbx_list_remove(coresList, michelle.getLc());
		// Restart michelle
		michelle.reStart();

		bctbx_list_t *devices = NULL;
		LinphoneParticipantDeviceIdentity *identity =
		    linphone_factory_create_participant_device_identity(linphone_factory_get(), michelleContact, "");
		bctbx_list_t *specs = linphone_core_get_linphone_specs_list(michelle.getLc());
		linphone_participant_device_identity_set_capability_descriptor_2(identity, specs);
		bctbx_list_free_with_data(specs, ms_free);
		devices = bctbx_list_append(devices, identity);
		// Remove and then add Michelle's device to simulate a new registration to the chat room
		// The conference server will therefore send an INVITE to Michelle thinking that she is not yet part of the
		// chatroom
		for (auto chatRoom : focus.getCore().getChatRooms()) {
			linphone_chat_room_set_participant_devices(L_GET_C_BACK_PTR(chatRoom), michelle.getCMgr()->identity, NULL);
			linphone_chat_room_set_participant_devices(L_GET_C_BACK_PTR(chatRoom), michelle.getCMgr()->identity,
			                                           devices);
		}
		bctbx_list_free_with_data(devices, (bctbx_list_free_func)belle_sip_object_unref);

		setup_mgr_for_conference(michelle.getCMgr(), NULL);
		coresList = bctbx_list_append(coresList, michelle.getLc());

		BC_ASSERT_TRUE(wait_for_list(coresList, &michelle.getStats().number_of_LinphoneConferenceStateCreated,
		                             initialMichelleStats.number_of_LinphoneConferenceStateCreated + 1,
		                             liblinphone_tester_sip_timeout));

		// Verify that only one subscription is sent out to the conference server
		BC_ASSERT_TRUE(wait_for_list(coresList, &michelle.getStats().number_of_LinphoneSubscriptionOutgoingProgress, 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_FALSE(
		    wait_for_list(coresList, &michelle.getStats().number_of_LinphoneSubscriptionOutgoingProgress, 2, 5000));

		michelleCr = linphone_core_search_chat_room(michelle.getLc(), NULL, michelleContact, confAddr, NULL);
		BC_ASSERT_PTR_NOT_NULL(michelleCr);

		const std::initializer_list<std::reference_wrapper<ConfCoreManager>> cores{focus, marie, michelle, berthe};
		for (const ConfCoreManager &core : cores) {
			BC_ASSERT_TRUE(checkChatroom(focus, core, -1));
			for (auto chatRoom : core.getCore().getChatRooms()) {
				BC_ASSERT_EQUAL(chatRoom->getParticipants().size(), ((focus.getLc() == core.getLc())) ? 3 : 2, size_t,
				                "%zu");
				BC_ASSERT_STRING_EQUAL(chatRoom->getSubject().c_str(), newSubject);
			}
		};

		LinphoneChatMessage *msg = linphone_chat_room_create_message_from_utf8(michelleCr, "back with you");
		linphone_chat_message_send(msg);
		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, michelle, berthe}).wait([msg] {
			return (linphone_chat_message_get_state(msg) == LinphoneChatMessageStateDelivered);
		}));
		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, michelle, berthe}).wait([marieCr] {
			return linphone_chat_room_get_unread_messages_count(marieCr) == 1;
		}));
		linphone_chat_message_unref(msg);
		msg = NULL;

		msg = linphone_chat_room_create_message_from_utf8(marieCr, "welcome back");
		linphone_chat_message_send(msg);
		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, michelle, berthe}).wait([msg] {
			return (linphone_chat_message_get_state(msg) == LinphoneChatMessageStateDelivered);
		}));
		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, michelle, berthe}).wait([michelleCr] {
			return linphone_chat_room_get_unread_messages_count(michelleCr) == 1;
		}));
		linphone_chat_message_unref(msg);
		msg = NULL;

		CoreManagerAssert({focus, marie, michelle, berthe}).waitUntil(std::chrono::seconds(2), [] { return false; });

		for (auto chatRoom : focus.getCore().getChatRooms()) {
			for (auto participant : chatRoom->getParticipants()) {
				//  force deletion by removing devices
				std::shared_ptr<Address> participantAddress = participant->getAddress();
				linphone_chat_room_set_participant_devices(L_GET_C_BACK_PTR(chatRoom), participantAddress->toC(), NULL);
			}
		}

		// wait until chatroom is deleted server side
		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, michelle}).wait([&focus] {
			return focus.getCore().getChatRooms().size() == 0;
		}));

		// wait bit more to detect side effect if any
		CoreManagerAssert({focus, marie, michelle, berthe}).waitUntil(chrono::seconds(2), [] { return false; });

		// to avoid creation attempt of a new chatroom
		LinphoneProxyConfig *config = linphone_core_get_default_proxy_config(focus.getLc());
		linphone_proxy_config_edit(config);
		linphone_proxy_config_set_conference_factory_uri(config, NULL);
		linphone_proxy_config_done(config);

		linphone_address_unref(michelleContact);
		bctbx_list_free(coresList);
	}
}

static void group_chat_room_with_client_removed_while_stopped_base(bool_t use_remote_event_list_handler) {
	Focus focus("chloe_rc");
	{ // to make sure focus is destroyed after clients.
		bool_t encrypted = FALSE;
		ClientConference marie("marie_rc", focus.getIdentity(), encrypted);
		ClientConference michelle("michelle_rc", focus.getIdentity(), encrypted);
		ClientConference berthe("berthe_rc", focus.getIdentity(), encrypted);

		focus.registerAsParticipantDevice(marie);
		focus.registerAsParticipantDevice(michelle);
		focus.registerAsParticipantDevice(berthe);

		bctbx_list_t *coresList = bctbx_list_append(NULL, focus.getLc());
		coresList = bctbx_list_append(coresList, marie.getLc());
		coresList = bctbx_list_append(coresList, michelle.getLc());
		coresList = bctbx_list_append(coresList, berthe.getLc());
		bctbx_list_t *participantsAddresses = NULL;
		Address michelleAddr = michelle.getIdentity();
		participantsAddresses = bctbx_list_append(participantsAddresses, linphone_address_ref(michelleAddr.toC()));
		Address bertheAddr = berthe.getIdentity();
		participantsAddresses = bctbx_list_append(participantsAddresses, linphone_address_ref(bertheAddr.toC()));

		stats initialMarieStats = marie.getStats();
		stats initialMichelleStats = michelle.getStats();
		stats initialBertheStats = berthe.getStats();
		//
		// Marie creates a new group chat room
		const char *initialSubject = "Colleagues (characters: $ £ çà)";
		LinphoneChatRoom *marieCr = create_chat_room_client_side_with_expected_number_of_participants(
		    coresList, marie.getCMgr(), &initialMarieStats, participantsAddresses, initialSubject, 2, encrypted,
		    LinphoneChatRoomEphemeralModeDeviceManaged);
		const LinphoneAddress *confAddr = linphone_chat_room_get_conference_address(marieCr);

		// Check that the chat room is correctly created on Michelle's side and that the participants are added
		LinphoneChatRoom *michelleCr = check_creation_chat_room_client_side(
		    coresList, michelle.getCMgr(), &initialMichelleStats, confAddr, initialSubject, 2, FALSE);

		LinphoneChatRoom *bertheCr = check_creation_chat_room_client_side(
		    coresList, berthe.getCMgr(), &initialBertheStats, confAddr, initialSubject, 2, FALSE);

		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, michelle, berthe}).wait([&focus] {
			for (const auto &chatRoom : focus.getCore().getChatRooms()) {
				for (const auto &participant : chatRoom->getParticipants()) {
					for (const auto &device : participant->getDevices()) {
						if (device->getState() != ParticipantDevice::State::Present) {
							return false;
						}
					}
				}
			}
			return true;
		}));

		BC_ASSERT_TRUE(wait_for_list(coresList, &michelle.getStats().number_of_LinphoneConferenceStateCreated,
		                             initialMichelleStats.number_of_LinphoneConferenceStateCreated + 1,
		                             liblinphone_tester_sip_timeout));

		BC_ASSERT_TRUE(wait_for_list(coresList, &berthe.getStats().number_of_LinphoneConferenceStateCreated,
		                             initialBertheStats.number_of_LinphoneConferenceStateCreated + 1,
		                             liblinphone_tester_sip_timeout));

		// Marie now changes the subject
		const char *newSubject = "Let's go drink a beer";
		linphone_chat_room_set_subject(marieCr, newSubject);
		BC_ASSERT_TRUE(wait_for_list(coresList, &marie.getStats().number_of_subject_changed,
		                             initialMarieStats.number_of_subject_changed + 1, liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &michelle.getStats().number_of_subject_changed,
		                             initialMichelleStats.number_of_subject_changed + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &berthe.getStats().number_of_subject_changed,
		                             initialBertheStats.number_of_subject_changed + 1, liblinphone_tester_sip_timeout));
		BC_ASSERT_STRING_EQUAL(linphone_chat_room_get_subject(marieCr), newSubject);
		BC_ASSERT_STRING_EQUAL(linphone_chat_room_get_subject(michelleCr), newSubject);
		BC_ASSERT_STRING_EQUAL(linphone_chat_room_get_subject(bertheCr), newSubject);

		BC_ASSERT_EQUAL(linphone_chat_room_get_nb_participants(marieCr), 2, int, "%d");
		BC_ASSERT_EQUAL(linphone_chat_room_get_nb_participants(michelleCr), 2, int, "%d");
		BC_ASSERT_EQUAL(linphone_chat_room_get_nb_participants(bertheCr), 2, int, "%d");

		const std::initializer_list<std::reference_wrapper<ConfCoreManager>> cores2{focus, marie, michelle, berthe};
		for (const ConfCoreManager &core : cores2) {
			BC_ASSERT_TRUE(
			    CoreManagerAssert({focus, marie, michelle, berthe}).waitUntil(chrono::seconds(10), [&focus, &core] {
				    return checkChatroom(focus, core, -1);
			    }));
		};

		initialMarieStats = marie.getStats();
		initialMichelleStats = michelle.getStats();
		initialBertheStats = berthe.getStats();

		LinphoneAddress *michelleContact = linphone_address_clone(
		    linphone_proxy_config_get_contact(linphone_core_get_default_proxy_config(michelle.getLc())));

		// Restart Michelle
		char *uuid = NULL;
		if (linphone_config_get_string(linphone_core_get_config(michelle.getLc()), "misc", "uuid", NULL)) {
			uuid = bctbx_strdup(
			    linphone_config_get_string(linphone_core_get_config(michelle.getLc()), "misc", "uuid", NULL));
		}

		ms_message("%s stops its core", linphone_core_get_identity(michelle.getLc()));
		coresList = bctbx_list_remove(coresList, michelle.getLc());
		linphone_core_manager_stop(michelle.getCMgr());

		ms_message("All %s's devices are removed", linphone_address_as_string(michelleContact));
		for (auto chatRoom : focus.getCore().getChatRooms()) {
			auto participant =
			    chatRoom->findParticipant(Address::toCpp(michelle.getCMgr()->identity)->getSharedFromThis());
			BC_ASSERT_PTR_NOT_NULL(participant);
			if (participant) {
				const auto &devices = participant->getDevices();
				BC_ASSERT_GREATER_STRICT(devices.size(), 0, size_t, "%zu");
				for (const auto &device : devices) {
					auto deviceAddress = device->getAddress();
					ms_message("Delete device %s from the database", deviceAddress->toString().c_str());
					L_GET_PRIVATE_FROM_C_OBJECT(focus.getLc())
					    ->mainDb->deleteChatRoomParticipantDevice(chatRoom, device);
				}
				ClientConference::deleteAllDevices(participant);
			}
		}

		// Marie removes Michelle from the chat room
		LinphoneParticipant *michelleParticipant = linphone_chat_room_find_participant(marieCr, michelleContact);
		BC_ASSERT_PTR_NOT_NULL(michelleParticipant);
		linphone_chat_room_remove_participant(marieCr, michelleParticipant);

		BC_ASSERT_TRUE(wait_for_list(coresList, &marie.getStats().number_of_participants_removed,
		                             initialMarieStats.number_of_participants_removed + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &berthe.getStats().number_of_participants_removed,
		                             initialBertheStats.number_of_participants_removed + 1,
		                             liblinphone_tester_sip_timeout));

		ms_message("%s is restarting its core", linphone_core_get_identity(focus.getLc()));
		coresList = bctbx_list_remove(coresList, focus.getLc());

		// Restart flexisip
		focus.reStart();
		coresList = bctbx_list_append(coresList, focus.getLc());

		// For a SUBSCRIBE from the clients
		const std::initializer_list<std::reference_wrapper<ConfCoreManager>> cores3{marie, berthe};
		for (ConfCoreManager &core : cores3) {
			stats initialStats = core.getStats();
			linphone_core_set_network_reachable(core.getLc(), FALSE);
			BC_ASSERT_TRUE(wait_for_list(coresList, &core.getStats().number_of_LinphoneSubscriptionTerminated,
			                             initialStats.number_of_LinphoneSubscriptionTerminated + 1,
			                             liblinphone_tester_sip_timeout));
			linphone_core_set_network_reachable(core.getLc(), TRUE);
			BC_ASSERT_TRUE(wait_for_list(coresList, &core.getStats().number_of_LinphoneRegistrationOk,
			                             initialStats.number_of_LinphoneRegistrationOk + 1,
			                             liblinphone_tester_sip_timeout));
			BC_ASSERT_TRUE(wait_for_list(coresList, &core.getStats().number_of_LinphoneSubscriptionActive,
			                             initialStats.number_of_LinphoneSubscriptionActive + 1,
			                             liblinphone_tester_sip_timeout));
			BC_ASSERT_FALSE(wait_for_list(coresList, &core.getStats().number_of_participant_devices_added,
			                              initialStats.number_of_participant_devices_added + 1, 2000));
			BC_ASSERT_FALSE(wait_for_list(coresList, &core.getStats().number_of_participants_added,
			                              initialStats.number_of_participants_added + 1, 2000));
		}

		ms_message("%s starts again its core", linphone_address_as_string(michelleContact));
		linphone_core_manager_configure(michelle.getCMgr());
		if (use_remote_event_list_handler) {
			LinphoneAccount *account = linphone_core_get_default_account(michelle.getLc());
			const LinphoneAccountParams *account_params = linphone_account_get_params(account);
			LinphoneAccountParams *new_account_params = linphone_account_params_clone(account_params);
			linphone_account_params_set_conference_factory_uri(new_account_params,
			                                                   focus.getIdentity().asString().c_str());
			linphone_account_set_params(account, new_account_params);
			linphone_account_params_unref(new_account_params);
		}

		// Make sure gruu is preserved
		linphone_config_set_string(linphone_core_get_config(michelle.getLc()), "misc", "uuid", uuid);
		linphone_core_manager_start(michelle.getCMgr(), TRUE);
		coresList = bctbx_list_append(coresList, michelle.getLc());

		if (uuid) {
			bctbx_free(uuid);
		}
		setup_mgr_for_conference(michelle.getCMgr(), NULL);
		coresList = bctbx_list_append(coresList, michelle.getLc());
		if (use_remote_event_list_handler) {
			BC_ASSERT_TRUE(wait_for_list(coresList, &michelle.getStats().number_of_LinphoneSubscriptionActive,
			                             initialMichelleStats.number_of_LinphoneSubscriptionActive + 1,
			                             liblinphone_tester_sip_timeout));
		} else {
			BC_ASSERT_FALSE(wait_for_list(coresList, &michelle.getStats().number_of_LinphoneSubscriptionActive,
			                              initialMichelleStats.number_of_LinphoneSubscriptionActive + 1, 2000));
			BC_ASSERT_TRUE(wait_for_list(coresList, &michelle.getStats().number_of_LinphoneSubscriptionError,
			                             initialMichelleStats.number_of_LinphoneSubscriptionError + 1,
			                             liblinphone_tester_sip_timeout));
		}

		michelleCr = linphone_core_search_chat_room(michelle.getLc(), NULL, michelleContact, confAddr, NULL);
		BC_ASSERT_PTR_NOT_NULL(michelleCr);

		// wait until chatroom is deleted Michelle's side
		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, michelle, berthe}).wait([&michelle] {
			return michelle.getCore().getChatRooms().size() == 1;
		}));

		initialMarieStats = marie.getStats();
		initialBertheStats = berthe.getStats();

		// A second device for Berthe is added in order to verify that the server will not send a NOTIFY full state
		// where Michelle is still a participant but she has no devices associated
		ClientConference berthe2("berthe_rc", focus.getIdentity(), encrypted);
		stats initialBerthe2Stats = berthe2.getStats();
		coresList = bctbx_list_append(coresList, berthe2.getLc());

		LinphoneAddress *berthe2Contact = linphone_address_clone(
		    linphone_proxy_config_get_contact(linphone_core_get_default_proxy_config(berthe2.getLc())));
		ms_message("%s is adding device %s", linphone_core_get_identity(focus.getLc()),
		           linphone_address_as_string(berthe2Contact));
		focus.registerAsParticipantDevice(berthe2);

		// Notify chat room that a participant has registered
		bctbx_list_t *devices = NULL;
		bctbx_list_t *specs = linphone_core_get_linphone_specs_list(berthe.getLc());
		const LinphoneAddress *deviceAddr =
		    linphone_proxy_config_get_contact(linphone_core_get_default_proxy_config(berthe.getLc()));
		LinphoneParticipantDeviceIdentity *identity =
		    linphone_factory_create_participant_device_identity(linphone_factory_get(), deviceAddr, "");
		linphone_participant_device_identity_set_capability_descriptor_2(identity, specs);
		bctbx_list_free_with_data(specs, ms_free);
		devices = bctbx_list_append(devices, identity);

		specs = linphone_core_get_linphone_specs_list(berthe2.getLc());
		deviceAddr = linphone_proxy_config_get_contact(linphone_core_get_default_proxy_config(berthe2.getLc()));
		identity = linphone_factory_create_participant_device_identity(linphone_factory_get(), deviceAddr, "");
		linphone_participant_device_identity_set_capability_descriptor_2(identity, specs);
		bctbx_list_free_with_data(specs, ms_free);
		devices = bctbx_list_append(devices, identity);

		for (auto chatRoom : focus.getCore().getChatRooms()) {
			linphone_chat_room_set_participant_devices(L_GET_C_BACK_PTR(chatRoom), berthe.getCMgr()->identity, devices);
		}
		bctbx_list_free_with_data(devices, (bctbx_list_free_func)belle_sip_object_unref);

		LinphoneChatRoom *berthe2Cr = check_creation_chat_room_client_side(
		    coresList, berthe2.getCMgr(), &initialBerthe2Stats, confAddr, newSubject, 1, FALSE);
		BC_ASSERT_PTR_NOT_NULL(berthe2Cr);
		BC_ASSERT_TRUE(wait_for_list(coresList, &berthe2.getStats().number_of_LinphoneConferenceStateCreated,
		                             initialBerthe2Stats.number_of_LinphoneConferenceStateCreated + 1,
		                             liblinphone_tester_sip_timeout));

		BC_ASSERT_TRUE(wait_for_list(coresList, &marie.getStats().number_of_participant_devices_added,
		                             initialMarieStats.number_of_participant_devices_added + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &berthe.getStats().number_of_participant_devices_added,
		                             initialBertheStats.number_of_participant_devices_added + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_FALSE(wait_for_list(coresList, &michelle.getStats().number_of_participant_devices_added,
		                              initialMichelleStats.number_of_participant_devices_added + 1, 2000));

		const std::initializer_list<std::reference_wrapper<ConfCoreManager>> cores{focus, marie, berthe, berthe2};
		for (const ConfCoreManager &core : cores) {
			BC_ASSERT_TRUE(checkChatroom(focus, core, -1));
			for (auto chatRoom : core.getCore().getChatRooms()) {
				BC_ASSERT_EQUAL(chatRoom->getParticipants().size(), ((focus.getLc() == core.getLc())) ? 2 : 1, size_t,
				                "%zu");
				BC_ASSERT_STRING_EQUAL(chatRoom->getSubject().c_str(), newSubject);
			}
		};

		BC_ASSERT_FALSE(wait_for_list(coresList, &michelle.getStats().number_of_LinphoneConferenceStateCreated,
		                              initialMichelleStats.number_of_LinphoneConferenceStateCreated + 1,
		                              liblinphone_tester_sip_timeout));

		LinphoneChatMessage *msg = linphone_chat_room_create_message_from_utf8(michelleCr, "back with you");
		linphone_chat_message_send(msg);
		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, michelle, berthe, berthe2}).wait([msg] {
			return (linphone_chat_message_get_state(msg) == LinphoneChatMessageStateNotDelivered);
		}));
		linphone_chat_message_unref(msg);
		msg = NULL;

		msg = linphone_chat_room_create_message_from_utf8(marieCr, "Michelle, I can't receive your messages .... :(");
		linphone_chat_message_send(msg);
		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, michelle, berthe, berthe2}).wait([msg] {
			return (linphone_chat_message_get_state(msg) == LinphoneChatMessageStateDelivered);
		}));
		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, michelle, berthe, berthe2}).wait([bertheCr] {
			return linphone_chat_room_get_unread_messages_count(bertheCr) == 1;
		}));
		if (berthe2Cr) {
			BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, michelle, berthe, berthe2}).wait([berthe2Cr] {
				return linphone_chat_room_get_unread_messages_count(berthe2Cr) == 1;
			}));
		}
		linphone_chat_message_unref(msg);
		msg = NULL;

		CoreManagerAssert({focus, marie, michelle, berthe}).waitUntil(std::chrono::seconds(2), [] { return false; });

		for (auto chatRoom : focus.getCore().getChatRooms()) {
			for (auto participant : chatRoom->getParticipants()) {
				//  force deletion by removing devices
				auto participantAddress = participant->getAddress();
				linphone_chat_room_set_participant_devices(L_GET_C_BACK_PTR(chatRoom), participantAddress->toC(), NULL);
			}
		}

		// wait until chatroom is deleted server side
		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, michelle, berthe, berthe2}).wait([&focus] {
			return focus.getCore().getChatRooms().size() == 0;
		}));

		// wait bit more to detect side effect if any
		CoreManagerAssert({focus, marie, michelle, berthe, berthe2}).waitUntil(chrono::seconds(2), [] {
			return false;
		});

		// to avoid creation attempt of a new chatroom
		LinphoneProxyConfig *config = linphone_core_get_default_proxy_config(focus.getLc());
		linphone_proxy_config_edit(config);
		linphone_proxy_config_set_conference_factory_uri(config, NULL);
		linphone_proxy_config_done(config);

		linphone_address_unref(michelleContact);
		bctbx_list_free(coresList);
	}
}

static void group_chat_room_with_client_removed_while_stopped_remote_list_event_handler(void) {
	group_chat_room_with_client_removed_while_stopped_base(TRUE);
}

static void group_chat_room_with_client_removed_while_stopped_no_remote_list_event_handler(void) {
	group_chat_room_with_client_removed_while_stopped_base(TRUE);
}

static void group_chat_room_with_creator_without_groupchat_capability_in_register(void) {
	Focus focus("chloe_rc");
	{ // to make sure focus is destroyed after clients.
		bool_t encrypted = FALSE;
		ClientConference marie("marie_rc", focus.getIdentity(), encrypted);
		ClientConference marie2("marie_rc", focus.getIdentity(), encrypted);
		ClientConference michelle("michelle_rc", focus.getIdentity(), encrypted);
		ClientConference berthe("berthe_rc", focus.getIdentity(), encrypted);

		stats initialMarieStats = marie.getStats();
		stats initialMarie2Stats = marie2.getStats();
		stats initialMichelleStats = michelle.getStats();
		stats initialBertheStats = berthe.getStats();
		stats initialFocusStats = focus.getStats();

		bctbx_list_t *coresList = bctbx_list_append(NULL, focus.getLc());
		coresList = bctbx_list_append(coresList, marie.getLc());
		coresList = bctbx_list_append(coresList, michelle.getLc());
		coresList = bctbx_list_append(coresList, berthe.getLc());

		focus.registerAsParticipantDevice(marie2);
		focus.registerAsParticipantDevice(michelle);
		focus.registerAsParticipantDevice(berthe);

		bctbx_list_t *participantsAddresses = NULL;
		Address michelleAddr = michelle.getIdentity();
		participantsAddresses = bctbx_list_append(participantsAddresses, linphone_address_ref(michelleAddr.toC()));
		Address bertheAddr = berthe.getIdentity();
		participantsAddresses = bctbx_list_append(participantsAddresses, linphone_address_ref(bertheAddr.toC()));

		// Marie creates a new group chat room
		const char *initialSubject = "Colleagues (characters: $ £ çà)";

		LinphoneChatRoomParams *params = linphone_core_create_default_chat_room_params(marie.getLc());
		linphone_chat_room_params_enable_encryption(params, encrypted);
		linphone_chat_room_params_set_ephemeral_mode(params, LinphoneChatRoomEphemeralModeDeviceManaged);
		linphone_chat_room_params_set_backend(params, LinphoneChatRoomBackendFlexisipChat);
		linphone_chat_room_params_enable_group(params, TRUE);
		linphone_chat_room_params_set_subject(params, initialSubject);
		LinphoneChatRoom *marieCr =
		    linphone_core_create_chat_room_6(marie.getLc(), params, NULL, participantsAddresses);
		linphone_chat_room_params_unref(params);
		BC_ASSERT_PTR_NOT_NULL(marieCr);
		// linphone_core_create_chat_room_6 takes a ref to the chatroom
		if (marieCr) linphone_chat_room_unref(marieCr);

		BC_ASSERT_TRUE(wait_for_list(coresList, &focus.getStats().number_of_LinphoneConferenceStateCreated,
		                             initialFocusStats.number_of_LinphoneConferenceStateCreated + 1,
		                             liblinphone_tester_sip_timeout));

		size_t numberFocusChatroom = focus.getCore().getChatRooms().size();
		BC_ASSERT_GREATER_STRICT(numberFocusChatroom, 0, size_t, "%zu");
		LinphoneAddress *confAddr = (numberFocusChatroom > 0)
		                                ? linphone_address_clone(linphone_chat_room_get_conference_address(
		                                      L_GET_C_BACK_PTR(focus.getCore().getChatRooms().front())))
		                                : NULL;

		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, marie2, michelle, berthe}).wait([&focus] {
			for (const auto &chatRoom : focus.getCore().getChatRooms()) {
				for (const auto &participant : chatRoom->getParticipants()) {
					if (participant->getDevices().size() != 1) {
						return false;
					}
					for (const auto &device : participant->getDevices()) {
						if (device->getState() != ParticipantDevice::State::Present) {
							return false;
						}
					}
				}
			}
			return true;
		}));

		BC_ASSERT_TRUE(wait_for_list(coresList, &marie.getStats().number_of_LinphoneConferenceStateCreationFailed,
		                             initialMarieStats.number_of_LinphoneConferenceStateCreationFailed + 1,
		                             liblinphone_tester_sip_timeout));

		BC_ASSERT_TRUE(wait_for_list(coresList, &focus.getStats().number_of_LinphoneConferenceStateDeleted,
		                             initialFocusStats.number_of_LinphoneConferenceStateDeleted + 1,
		                             liblinphone_tester_sip_timeout));

		BC_ASSERT_TRUE(wait_for_list(coresList, &marie.getStats().number_of_LinphoneChatRoomSessionError,
		                             initialMarieStats.number_of_LinphoneChatRoomSessionError + 1,
		                             liblinphone_tester_sip_timeout));

		BC_ASSERT_TRUE(wait_for_list(coresList, &marie.getStats().number_of_LinphoneConferenceStateDeleted,
		                             initialMarieStats.number_of_LinphoneConferenceStateDeleted + 1,
		                             liblinphone_tester_sip_timeout));

		BC_ASSERT_FALSE(wait_for_list(coresList, &michelle.getStats().number_of_LinphoneConferenceStateCreated,
		                              initialMichelleStats.number_of_LinphoneConferenceStateCreated + 1, 2000));

		BC_ASSERT_FALSE(wait_for_list(coresList, &berthe.getStats().number_of_LinphoneConferenceStateCreated,
		                              initialBertheStats.number_of_LinphoneConferenceStateCreated + 1, 2000));

		BC_ASSERT_FALSE(wait_for_list(coresList, &marie2.getStats().number_of_LinphoneConferenceStateCreated,
		                              initialMarie2Stats.number_of_LinphoneConferenceStateCreated + 1, 2000));

		const std::initializer_list<std::reference_wrapper<ConfCoreManager>> cores{focus, marie, marie2, michelle,
		                                                                           berthe};
		for (const ConfCoreManager &core : cores) {
			const LinphoneAddress *deviceAddr = linphone_proxy_config_get_contact(core.getDefaultProxyConfig());
			LinphoneChatRoom *cr = linphone_core_search_chat_room(core.getLc(), NULL, deviceAddr, confAddr, NULL);
			BC_ASSERT_PTR_NULL(cr);
		};

		CoreManagerAssert({focus, marie, marie2, michelle, berthe}).waitUntil(std::chrono::seconds(2), [] {
			return false;
		});

		for (auto chatRoom : focus.getCore().getChatRooms()) {
			for (auto participant : chatRoom->getParticipants()) {
				//  force deletion by removing devices
				std::shared_ptr<Address> participantAddress = participant->getAddress();
				linphone_chat_room_set_participant_devices(L_GET_C_BACK_PTR(chatRoom), participantAddress->toC(), NULL);
			}
		}

		// wait until chatroom is deleted server side
		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, marie2, michelle, berthe}).wait([&focus] {
			return focus.getCore().getChatRooms().size() == 0;
		}));

		// wait bit more to detect side effect if any
		CoreManagerAssert({focus, marie, marie2, michelle, berthe}).waitUntil(chrono::seconds(2), [] { return false; });

		// to avoid creation attempt of a new chatroom
		LinphoneProxyConfig *config = linphone_core_get_default_proxy_config(focus.getLc());
		linphone_proxy_config_edit(config);
		linphone_proxy_config_set_conference_factory_uri(config, NULL);
		linphone_proxy_config_done(config);

		linphone_address_unref(confAddr);
		bctbx_list_free(coresList);
	}
}

static void group_chat_room_with_creator_without_groupchat_capability(void) {
	Focus focus("chloe_rc");
	{ // to make sure focus is destroyed after clients.
		bool_t encrypted = FALSE;
		ClientConference marie("marie_rc", focus.getIdentity(), encrypted);
		ClientConference michelle("michelle_rc", focus.getIdentity(), encrypted);
		ClientConference berthe("berthe_rc", focus.getIdentity(), encrypted);

		bctbx_list_t *coresList = bctbx_list_append(NULL, focus.getLc());
		coresList = bctbx_list_append(coresList, marie.getLc());
		coresList = bctbx_list_append(coresList, michelle.getLc());
		coresList = bctbx_list_append(coresList, berthe.getLc());

		stats initialMarieStats = marie.getStats();
		linphone_core_set_network_reachable(marie.getLc(), FALSE);
		linphone_core_remove_linphone_spec(marie.getLc(), "groupchat");
		linphone_core_remove_linphone_spec(marie.getLc(), "conference");
		linphone_core_set_network_reachable(marie.getLc(), TRUE);
		BC_ASSERT_TRUE(wait_for_list(coresList, &marie.getStats().number_of_LinphoneRegistrationOk,
		                             initialMarieStats.number_of_LinphoneRegistrationOk + 1,
		                             liblinphone_tester_sip_timeout));

		focus.registerAsParticipantDevice(marie);
		focus.registerAsParticipantDevice(michelle);
		focus.registerAsParticipantDevice(berthe);

		bctbx_list_t *participantsAddresses = NULL;
		Address michelleAddr = michelle.getIdentity();
		participantsAddresses = bctbx_list_append(participantsAddresses, linphone_address_ref(michelleAddr.toC()));
		Address bertheAddr = berthe.getIdentity();
		participantsAddresses = bctbx_list_append(participantsAddresses, linphone_address_ref(bertheAddr.toC()));

		stats initialFocusStats = focus.getStats();

		// Marie creates a new group chat room
		const char *initialSubject = "Colleagues (characters: $ £ çà)";

		LinphoneChatRoomParams *params = linphone_core_create_default_chat_room_params(marie.getLc());
		linphone_chat_room_params_enable_encryption(params, encrypted);
		linphone_chat_room_params_set_ephemeral_mode(params, LinphoneChatRoomEphemeralModeDeviceManaged);
		linphone_chat_room_params_set_backend(params, LinphoneChatRoomBackendFlexisipChat);
		linphone_chat_room_params_enable_group(params, TRUE);
		linphone_chat_room_params_set_subject(params, initialSubject);
		LinphoneChatRoom *chatRoom =
		    linphone_core_create_chat_room_6(marie.getLc(), params, NULL, participantsAddresses);
		linphone_chat_room_params_unref(params);
		BC_ASSERT_PTR_NOT_NULL(chatRoom);

		// linphone_core_create_chat_room_6 takes a ref to the chatroom
		if (chatRoom) linphone_chat_room_unref(chatRoom);

		BC_ASSERT_TRUE(wait_for_list(coresList, &focus.getStats().number_of_LinphoneConferenceStateCreationFailed,
		                             initialFocusStats.number_of_LinphoneConferenceStateCreationFailed + 1,
		                             liblinphone_tester_sip_timeout));

		BC_ASSERT_TRUE(wait_for_list(coresList, &marie.getStats().number_of_LinphoneConferenceStateCreationFailed,
		                             initialMarieStats.number_of_LinphoneConferenceStateCreationFailed + 1,
		                             liblinphone_tester_sip_timeout));

		BC_ASSERT_TRUE(wait_for_list(coresList, &focus.getStats().number_of_LinphoneConferenceStateDeleted,
		                             initialFocusStats.number_of_LinphoneConferenceStateDeleted + 1,
		                             liblinphone_tester_sip_timeout));

		BC_ASSERT_TRUE(wait_for_list(coresList, &marie.getStats().number_of_LinphoneChatRoomSessionError,
		                             initialMarieStats.number_of_LinphoneChatRoomSessionError + 1,
		                             liblinphone_tester_sip_timeout));

		// Clean db from chat room
		linphone_core_manager_delete_chat_room(marie.getCMgr(), chatRoom, coresList);

		BC_ASSERT_TRUE(wait_for_list(coresList, &marie.getStats().number_of_LinphoneConferenceStateDeleted,
		                             initialMarieStats.number_of_LinphoneConferenceStateDeleted + 1,
		                             liblinphone_tester_sip_timeout));

		bctbx_list_free(coresList);
	}
}

static void group_chat_room_with_invite_error(void) {
	group_chat_room_with_sip_errors_base(true, false, false);
}

static void group_chat_room_with_subscribe_error(void) {
	group_chat_room_with_sip_errors_base(false, true, false);
}

static void group_chat_room_bulk_notify_to_participant(void) {
	Focus focus("chloe_rc");
	{ // to make sure focus is destroyed after clients.
		ClientConference marie("marie_rc", focus.getIdentity());
		ClientConference pauline("pauline_rc", focus.getIdentity());
		ClientConference michelle("michelle_rc", focus.getIdentity());

		focus.registerAsParticipantDevice(marie);
		focus.registerAsParticipantDevice(pauline);
		focus.registerAsParticipantDevice(michelle);

		bctbx_list_t *coresList = bctbx_list_append(NULL, focus.getLc());
		coresList = bctbx_list_append(coresList, marie.getLc());
		coresList = bctbx_list_append(coresList, pauline.getLc());
		coresList = bctbx_list_append(coresList, michelle.getLc());
		Address paulineAddr = pauline.getIdentity();
		bctbx_list_t *participantsAddresses = bctbx_list_append(NULL, linphone_address_ref(paulineAddr.toC()));
		Address michelleAddr = michelle.getIdentity();
		participantsAddresses = bctbx_list_append(participantsAddresses, linphone_address_ref(michelleAddr.toC()));

		stats initialMarieStats = marie.getStats();
		stats initialPaulineStats = pauline.getStats();
		stats initialMichelleStats = michelle.getStats();

		// Marie creates a new group chat room
		const char *initialSubject = "Colleagues (characters: $ £ çà)";
		LinphoneChatRoom *marieCr =
		    create_chat_room_client_side(coresList, marie.getCMgr(), &initialMarieStats, participantsAddresses,
		                                 initialSubject, FALSE, LinphoneChatRoomEphemeralModeDeviceManaged);
		const LinphoneAddress *confAddr = linphone_chat_room_get_conference_address(marieCr);

		// Check that the chat room is correctly created on Pauline's side and that the participants are added
		LinphoneChatRoom *paulineCr = check_creation_chat_room_client_side(
		    coresList, pauline.getCMgr(), &initialPaulineStats, confAddr, initialSubject, 2, FALSE);
		LinphoneChatRoom *michelleCr = check_creation_chat_room_client_side(
		    coresList, michelle.getCMgr(), &initialMichelleStats, confAddr, initialSubject, 2, FALSE);

		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, pauline, michelle}).wait([&focus] {
			for (auto chatRoom : focus.getCore().getChatRooms()) {
				for (auto participant : chatRoom->getParticipants()) {
					for (auto device : participant->getDevices())
						if (device->getState() != ParticipantDevice::State::Present) {
							return false;
						}
				}
			}
			return true;
		}));

		BC_ASSERT_TRUE(wait_for_list(coresList, &pauline.getStats().number_of_LinphoneConferenceStateCreated,
		                             initialPaulineStats.number_of_LinphoneConferenceStateCreated + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &michelle.getStats().number_of_LinphoneConferenceStateCreated,
		                             initialMichelleStats.number_of_LinphoneConferenceStateCreated + 1,
		                             liblinphone_tester_sip_timeout));

		// Marie now changes the subject
		const char *newSubject = "Let's go drink a beer";
		linphone_chat_room_set_subject(marieCr, newSubject);
		BC_ASSERT_TRUE(wait_for_list(coresList, &marie.getStats().number_of_subject_changed,
		                             initialMarieStats.number_of_subject_changed + 1, liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &pauline.getStats().number_of_subject_changed,
		                             initialPaulineStats.number_of_subject_changed + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &michelle.getStats().number_of_subject_changed,
		                             initialMichelleStats.number_of_subject_changed + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_STRING_EQUAL(linphone_chat_room_get_subject(marieCr), newSubject);
		BC_ASSERT_STRING_EQUAL(linphone_chat_room_get_subject(paulineCr), newSubject);
		BC_ASSERT_STRING_EQUAL(linphone_chat_room_get_subject(michelleCr), newSubject);

		// Pauline goes offline
		linphone_core_set_network_reachable(pauline.getLc(), FALSE);

		// Adding Laure
		ClientConference laure("laure_tcp_rc", focus.getIdentity());
		coresList = bctbx_list_append(coresList, laure.getLc());
		focus.registerAsParticipantDevice(laure);

		initialMarieStats = marie.getStats();
		initialPaulineStats = pauline.getStats();
		initialMichelleStats = michelle.getStats();
		stats initialLaureStats = laure.getStats();

		Address laureAddr = laure.getIdentity();
		participantsAddresses = bctbx_list_append(NULL, linphone_address_ref(laureAddr.toC()));
		linphone_chat_room_add_participants(marieCr, participantsAddresses);

		LinphoneChatRoom *laureCr = check_creation_chat_room_client_side(coresList, laure.getCMgr(), &initialLaureStats,
		                                                                 confAddr, newSubject, 3, FALSE);
		BC_ASSERT_PTR_NOT_NULL(laureCr);

		BC_ASSERT_TRUE(wait_for_list(coresList, &laure.getStats().number_of_LinphoneConferenceStateCreationPending,
		                             initialLaureStats.number_of_LinphoneConferenceStateCreationPending + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &laure.getStats().number_of_LinphoneConferenceStateCreated,
		                             initialLaureStats.number_of_LinphoneConferenceStateCreated + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &laure.getStats().number_of_LinphoneChatRoomConferenceJoined,
		                             initialLaureStats.number_of_LinphoneChatRoomConferenceJoined + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &marie.getStats().number_of_participants_added,
		                             initialMarieStats.number_of_participants_added + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_FALSE(wait_for_list(coresList, &pauline.getStats().number_of_participants_added,
		                              initialPaulineStats.number_of_participants_added + 1, 3000));
		BC_ASSERT_TRUE(wait_for_list(coresList, &michelle.getStats().number_of_participants_added,
		                             initialMichelleStats.number_of_participants_added + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &marie.getStats().number_of_participant_devices_added,
		                             initialMarieStats.number_of_participant_devices_added + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_FALSE(wait_for_list(coresList, &pauline.getStats().number_of_participant_devices_added,
		                              initialPaulineStats.number_of_participant_devices_added + 1, 3000));
		BC_ASSERT_TRUE(wait_for_list(coresList, &michelle.getStats().number_of_participant_devices_added,
		                             initialMichelleStats.number_of_participant_devices_added + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_EQUAL(linphone_chat_room_get_nb_participants(marieCr), 3, int, "%d");
		BC_ASSERT_EQUAL(linphone_chat_room_get_nb_participants(paulineCr), 2, int, "%d");
		BC_ASSERT_EQUAL(linphone_chat_room_get_nb_participants(michelleCr), 3, int, "%d");

		// Wait a little bit to detect side effects
		CoreManagerAssert({focus, marie, laure, pauline, michelle}).waitUntil(std::chrono::seconds(2), [] {
			return false;
		});

		initialMarieStats = marie.getStats();
		initialPaulineStats = pauline.getStats();
		initialMichelleStats = michelle.getStats();
		initialLaureStats = laure.getStats();

		// Marie now changes the subject again
		const char *newSubject2 = "Seriously, ladies... Tonight we go out";
		linphone_chat_room_set_subject(marieCr, newSubject2);

		BC_ASSERT_TRUE(wait_for_list(coresList, &marie.getStats().number_of_subject_changed,
		                             initialMarieStats.number_of_subject_changed + 1, liblinphone_tester_sip_timeout));
		BC_ASSERT_FALSE(wait_for_list(coresList, &pauline.getStats().number_of_subject_changed,
		                              initialPaulineStats.number_of_subject_changed + 1, 3000));
		BC_ASSERT_TRUE(wait_for_list(coresList, &laure.getStats().number_of_subject_changed,
		                             initialLaureStats.number_of_subject_changed + 1, liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &michelle.getStats().number_of_subject_changed,
		                             initialMichelleStats.number_of_subject_changed + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_STRING_EQUAL(linphone_chat_room_get_subject(marieCr), newSubject2);
		BC_ASSERT_STRING_EQUAL(linphone_chat_room_get_subject(paulineCr), newSubject);
		BC_ASSERT_STRING_EQUAL(linphone_chat_room_get_subject(michelleCr), newSubject2);
		BC_ASSERT_STRING_EQUAL(linphone_chat_room_get_subject(laureCr), newSubject2);

		initialMarieStats = marie.getStats();
		initialPaulineStats = pauline.getStats();
		initialMichelleStats = michelle.getStats();
		initialLaureStats = laure.getStats();

		char *laureDeviceIdentity = linphone_core_get_device_identity(laure.getLc());
		LinphoneAddress *laureLocalAddr = linphone_address_new(laureDeviceIdentity);
		BC_ASSERT_PTR_NOT_NULL(laureLocalAddr);
		bctbx_free(laureDeviceIdentity);

		// Focus deletes Laure's device
		for (auto chatRoom : focus.getCore().getChatRooms()) {
			std::shared_ptr<Participant> participant =
			    chatRoom->findParticipant(Address::toCpp(laureLocalAddr)->getSharedFromThis());
			BC_ASSERT_PTR_NOT_NULL(participant);
			if (participant) {
				//  force deletion by removing devices
				std::shared_ptr<Address> participantAddress = participant->getAddress();
				// Do not use laureLocalAddr because it has a GRUU
				linphone_chat_room_set_participant_devices(L_GET_C_BACK_PTR(chatRoom), participantAddress->toC(), NULL);
			}
		}

		// Marie removes Laure from the chat room
		LinphoneParticipant *laureParticipant = linphone_chat_room_find_participant(marieCr, laureLocalAddr);
		BC_ASSERT_PTR_NOT_NULL(laureParticipant);
		linphone_chat_room_remove_participant(marieCr, laureParticipant);

		linphone_address_unref(laureLocalAddr);

		BC_ASSERT_FALSE(wait_for_list(coresList, &laure.getStats().number_of_LinphoneConferenceStateTerminated,
		                              initialLaureStats.number_of_LinphoneConferenceStateTerminated + 1, 3000));

		BC_ASSERT_TRUE(wait_for_list(coresList, &marie.getStats().number_of_participant_devices_removed,
		                             initialMarieStats.number_of_participant_devices_removed + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_FALSE(wait_for_list(coresList, &pauline.getStats().number_of_participant_devices_removed,
		                              initialPaulineStats.number_of_participant_devices_removed + 1, 3000));
		BC_ASSERT_TRUE(wait_for_list(coresList, &michelle.getStats().number_of_participant_devices_removed,
		                             initialMichelleStats.number_of_participant_devices_removed + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &marie.getStats().number_of_participants_removed,
		                             initialMarieStats.number_of_participants_removed + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_FALSE(wait_for_list(coresList, &pauline.getStats().number_of_participants_removed,
		                              initialPaulineStats.number_of_participants_removed + 1, 3000));
		BC_ASSERT_TRUE(wait_for_list(coresList, &michelle.getStats().number_of_participants_removed,
		                             initialMichelleStats.number_of_participants_removed + 1,
		                             liblinphone_tester_sip_timeout));

		BC_ASSERT_EQUAL(linphone_chat_room_get_nb_participants(marieCr), 2, int, "%d");
		BC_ASSERT_EQUAL(linphone_chat_room_get_nb_participants(paulineCr), 2, int, "%d");

		// Wait a little bit to detect side effects
		CoreManagerAssert({focus, marie, pauline, michelle}).waitUntil(std::chrono::seconds(2), [] { return false; });

		initialPaulineStats = pauline.getStats();
		// Pauline comes up online
		linphone_core_set_network_reachable(pauline.getLc(), TRUE);

		BC_ASSERT_TRUE(wait_for_list(coresList, &pauline.getStats().number_of_LinphoneRegistrationOk,
		                             initialPaulineStats.number_of_LinphoneRegistrationOk + 1,
		                             liblinphone_tester_sip_timeout));

		// Check that Pauline receives the backlog of events occurred while she was offline
		BC_ASSERT_TRUE(wait_for_list(coresList, &pauline.getStats().number_of_participants_added,
		                             initialPaulineStats.number_of_participants_added + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &pauline.getStats().number_of_participant_devices_added,
		                             initialPaulineStats.number_of_participant_devices_added + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &pauline.getStats().number_of_subject_changed,
		                             initialPaulineStats.number_of_subject_changed + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &pauline.getStats().number_of_participant_devices_removed,
		                             initialPaulineStats.number_of_participant_devices_removed + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &pauline.getStats().number_of_participants_removed,
		                             initialPaulineStats.number_of_participants_removed + 1,
		                             liblinphone_tester_sip_timeout));

		BC_ASSERT_EQUAL(linphone_chat_room_get_nb_participants(paulineCr), 2, int, "%d");
		BC_ASSERT_STRING_EQUAL(linphone_chat_room_get_subject(paulineCr), newSubject2);

		CoreManagerAssert({focus, marie, pauline, michelle}).waitUntil(std::chrono::seconds(2), [] { return false; });

		for (auto chatRoom : focus.getCore().getChatRooms()) {
			for (auto participant : chatRoom->getParticipants()) {
				//  force deletion by removing devices
				std::shared_ptr<Address> participantAddress = participant->getAddress();
				linphone_chat_room_set_participant_devices(L_GET_C_BACK_PTR(chatRoom), participantAddress->toC(), NULL);
			}
		}

		// wait until chatroom is deleted server side
		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, pauline}).wait([&focus] {
			return focus.getCore().getChatRooms().size() == 0;
		}));

		// wait bit more to detect side effect if any
		CoreManagerAssert({focus, marie, pauline, michelle}).waitUntil(std::chrono::seconds(2), [] { return false; });

		// to avoid creation attempt of a new chatroom
		auto config = focus.getDefaultProxyConfig();
		linphone_proxy_config_edit(config);
		linphone_proxy_config_set_conference_factory_uri(config, NULL);
		linphone_proxy_config_done(config);

		bctbx_list_free(coresList);
	}
}

static void one_to_one_chatroom_exhumed_while_offline(void) {
	Focus focus("chloe_rc");
	{ // to make sure focus is destroyed after clients.
		ClientConference marie("marie_rc", focus.getIdentity());
		ClientConference pauline("pauline_rc", focus.getIdentity());

		focus.registerAsParticipantDevice(marie);
		focus.registerAsParticipantDevice(pauline);

		bctbx_list_t *coresList = bctbx_list_append(NULL, focus.getLc());
		coresList = bctbx_list_append(coresList, marie.getLc());
		coresList = bctbx_list_append(coresList, pauline.getLc());
		Address paulineAddr = pauline.getIdentity();
		bctbx_list_t *participantsAddresses = bctbx_list_append(NULL, linphone_address_ref(paulineAddr.toC()));

		stats initialMarieStats = marie.getStats();
		stats initialPaulineStats = pauline.getStats();

		// Marie creates a new one to one chat room
		const char *initialSubject = "one to one with Pauline";
		LinphoneChatRoom *marieCr =
		    create_chat_room_client_side(coresList, marie.getCMgr(), &initialMarieStats, participantsAddresses,
		                                 initialSubject, FALSE, LinphoneChatRoomEphemeralModeDeviceManaged);
		BC_ASSERT_PTR_NOT_NULL(marieCr);
		LinphoneAddress *confAddr = linphone_address_clone(linphone_chat_room_get_conference_address(marieCr));
		BC_ASSERT_PTR_NOT_NULL(confAddr);

		// Check that the chat room is correctly created on Pauline's side and that the participants are added
		LinphoneChatRoom *paulineCr = check_creation_chat_room_client_side(
		    coresList, pauline.getCMgr(), &initialPaulineStats, confAddr, initialSubject, 1, FALSE);

		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, pauline}).wait([&focus] {
			for (auto chatRoom : focus.getCore().getChatRooms()) {
				for (auto participant : chatRoom->getParticipants()) {
					for (auto device : participant->getDevices())
						if (device->getState() != ParticipantDevice::State::Present) {
							return false;
						}
				}
			}
			return true;
		}));

		BC_ASSERT_TRUE(wait_for_list(coresList, &pauline.getStats().number_of_LinphoneConferenceStateCreated,
		                             initialPaulineStats.number_of_LinphoneConferenceStateCreated + 1,
		                             liblinphone_tester_sip_timeout));

		// Pauline goes offline
		linphone_core_set_network_reachable(pauline.getLc(), FALSE);

		LinphoneChatMessage *marieMsg1 = linphone_chat_room_create_message_from_utf8(marieCr, "Long live the C++ !");
		linphone_chat_message_send(marieMsg1);
		BC_ASSERT_TRUE(wait_for_list(coresList, &marie.getStats().number_of_LinphoneMessageSent,
		                             initialMarieStats.number_of_LinphoneMessageSent + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_FALSE(wait_for_list(coresList, &pauline.getStats().number_of_LinphoneMessageReceived,
		                              initialPaulineStats.number_of_LinphoneMessageReceived + 1, 3000));
		linphone_chat_message_unref(marieMsg1);

		int marieMsgs = linphone_chat_room_get_history_size(marieCr);
		BC_ASSERT_EQUAL(marieMsgs, 1, int, "%d");
		// Pauline didn't received the message as she was offline
		int paulineMsgs = linphone_chat_room_get_history_size(paulineCr);
		BC_ASSERT_EQUAL(paulineMsgs, 0, int, "%d");

		// Wait a little bit to detect side effects
		CoreManagerAssert({focus, marie, pauline}).waitUntil(std::chrono::seconds(2), [] { return false; });

		// Marie deletes the chat room
		// Pauline cannot now this because she is offline
		linphone_core_manager_delete_chat_room(marie.getCMgr(), marieCr, coresList);
		BC_ASSERT_TRUE(wait_for_list(coresList, &marie.getStats().number_of_LinphoneConferenceStateTerminated,
		                             initialMarieStats.number_of_LinphoneConferenceStateTerminated + 1,
		                             liblinphone_tester_sip_timeout));

		initialMarieStats = marie.getStats();
		initialPaulineStats = pauline.getStats();

		paulineAddr = pauline.getIdentity();
		participantsAddresses = bctbx_list_append(NULL, linphone_address_ref(paulineAddr.toC()));

		marieCr = create_chat_room_client_side(coresList, marie.getCMgr(), &initialMarieStats, participantsAddresses,
		                                       initialSubject, FALSE, LinphoneChatRoomEphemeralModeDeviceManaged);
		BC_ASSERT_PTR_NOT_NULL(marieCr);
		LinphoneAddress *exhumedConfAddrPtr = (LinphoneAddress *)linphone_chat_room_get_conference_address(marieCr);
		BC_ASSERT_PTR_NOT_NULL(exhumedConfAddrPtr);
		LinphoneAddress *exhumedConfAddr = NULL;
		if (exhumedConfAddrPtr) {
			exhumedConfAddr =
			    linphone_address_clone((LinphoneAddress *)linphone_chat_room_get_conference_address(marieCr));
			BC_ASSERT_PTR_NOT_NULL(exhumedConfAddr);
			if (exhumedConfAddr) {
				BC_ASSERT_FALSE(linphone_address_equal(confAddr, exhumedConfAddr));
			}
		}

		BC_ASSERT_EQUAL((int)marie.getCore().getChatRooms().size(), 1, int, "%d");
		BC_ASSERT_EQUAL(linphone_chat_room_get_nb_participants(marieCr), 1, int, "%d");

		BC_ASSERT_EQUAL(linphone_chat_room_get_nb_participants(paulineCr), 1, int, "%d");

		// Wait a little bit to detect side effects
		CoreManagerAssert({focus, marie, pauline}).waitUntil(std::chrono::seconds(2), [] { return false; });

		initialPaulineStats = pauline.getStats();
		// Pauline comes up online
		linphone_core_set_network_reachable(pauline.getLc(), TRUE);

		BC_ASSERT_TRUE(wait_for_list(coresList, &pauline.getStats().number_of_LinphoneRegistrationOk,
		                             initialPaulineStats.number_of_LinphoneRegistrationOk + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &pauline.getStats().number_of_LinphoneChatRoomConferenceJoined,
		                             initialPaulineStats.number_of_LinphoneChatRoomConferenceJoined + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_EQUAL((int)pauline.getCore().getChatRooms().size(), 1, int, "%d");

		char *paulineDeviceIdentity = linphone_core_get_device_identity(pauline.getLc());
		LinphoneAddress *paulineDeviceAddr = linphone_address_new(paulineDeviceIdentity);
		bctbx_free(paulineDeviceIdentity);
		auto newPaulineCr = pauline.searchChatRoom(paulineDeviceAddr, confAddr);
		linphone_address_unref(paulineDeviceAddr);
		BC_ASSERT_PTR_NOT_NULL(newPaulineCr);
		BC_ASSERT_PTR_EQUAL(newPaulineCr, paulineCr);

		if (newPaulineCr) {
			LinphoneAddress *paulineNewConfAddr =
			    linphone_address_ref((LinphoneAddress *)linphone_chat_room_get_conference_address(newPaulineCr));
			BC_ASSERT_PTR_NOT_NULL(paulineNewConfAddr);
			if (paulineNewConfAddr) {
				BC_ASSERT_FALSE(linphone_address_equal(confAddr, paulineNewConfAddr));
				if (exhumedConfAddr) {
					BC_ASSERT_TRUE(linphone_address_equal(exhumedConfAddr, paulineNewConfAddr));
				}
			}
			linphone_address_unref(paulineNewConfAddr);

			BC_ASSERT_EQUAL(linphone_chat_room_get_nb_participants(newPaulineCr), 1, int, "%d");
			BC_ASSERT_STRING_EQUAL(linphone_chat_room_get_subject(newPaulineCr), initialSubject);

			BC_ASSERT_TRUE(wait_for_list(coresList, &pauline.getStats().number_of_LinphoneMessageReceived,
			                             initialPaulineStats.number_of_LinphoneMessageReceived + 1,
			                             liblinphone_tester_sip_timeout));
			paulineMsgs = linphone_chat_room_get_history_size(newPaulineCr);
			BC_ASSERT_EQUAL(paulineMsgs, 1, int, "%d");

			LinphoneChatMessage *paulineMsg =
			    linphone_chat_room_create_message_from_utf8(newPaulineCr, "Sorry I was offline :(");
			linphone_chat_message_send(paulineMsg);
			BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, pauline}).wait([paulineMsg] {
				return (linphone_chat_message_get_state(paulineMsg) == LinphoneChatMessageStateDelivered);
			}));
			BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, pauline}).wait([marieCr] {
				return linphone_chat_room_get_unread_messages_count(marieCr) == 1;
			}));

			// Since Marie has deleted the chat room, she lost all messages she sent before deleting it
			marieMsgs = linphone_chat_room_get_history_size(marieCr);
			BC_ASSERT_EQUAL(marieMsgs, 1, int, "%d");
			paulineMsgs = linphone_chat_room_get_history_size(newPaulineCr);
			BC_ASSERT_EQUAL(paulineMsgs, 2, int, "%d");

			LinphoneChatMessage *marieMsg = linphone_chat_room_create_message_from_utf8(marieCr, "exhumed!!");
			linphone_chat_message_send(marieMsg);
			BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, pauline}).wait([marieMsg] {
				return (linphone_chat_message_get_state(marieMsg) == LinphoneChatMessageStateDelivered);
			}));
			BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, pauline}).wait([newPaulineCr]() mutable {
				return linphone_chat_room_get_unread_messages_count(newPaulineCr) == 2;
			}));
			linphone_chat_message_unref(marieMsg);

			marieMsgs = linphone_chat_room_get_history_size(marieCr);
			BC_ASSERT_EQUAL(marieMsgs, 2, int, "%d");
			paulineMsgs = linphone_chat_room_get_history_size(newPaulineCr);
			BC_ASSERT_EQUAL(paulineMsgs, 3, int, "%d");
		}

		linphone_address_unref(exhumedConfAddr);

		CoreManagerAssert({focus, marie, pauline}).waitUntil(std::chrono::seconds(1), [] { return false; });

		CoreManagerAssert({focus, marie}).waitUntil(std::chrono::seconds(2), [] { return false; });

		initialMarieStats = marie.getStats();
		initialPaulineStats = pauline.getStats();

		linphone_core_manager_delete_chat_room(marie.getCMgr(), marieCr, coresList);
		BC_ASSERT_TRUE(wait_for_list(coresList, &marie.getStats().number_of_LinphoneConferenceStateTerminated,
		                             initialMarieStats.number_of_LinphoneConferenceStateTerminated + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &pauline.getStats().number_of_LinphoneConferenceStateTerminated,
		                             initialPaulineStats.number_of_LinphoneConferenceStateTerminated + 1,
		                             liblinphone_tester_sip_timeout));

		// wait bit more to detect side effect if any
		CoreManagerAssert({focus, marie, pauline}).waitUntil(chrono::seconds(2), [] { return false; });

		// to avoid creation attempt of a new chatroom
		auto config = focus.getDefaultProxyConfig();
		linphone_proxy_config_edit(config);
		linphone_proxy_config_set_conference_factory_uri(config, NULL);
		linphone_proxy_config_done(config);

		bctbx_list_free(coresList);
	}
}

static void multidomain_group_chat_room(void) {
	Focus focusExampleDotOrg("chloe_rc");
	Focus focusAuth1DotExampleDotOrg("arthur_rc");
	{ // to make sure focus is destroyed after clients.
		ClientConference marie("marie_rc", focusExampleDotOrg.getIdentity());
		ClientConference pauline("pauline_rc", focusExampleDotOrg.getIdentity());
		ClientConference michelle("michelle_rc", focusExampleDotOrg.getIdentity());

		focusExampleDotOrg.registerAsParticipantDevice(marie);
		focusExampleDotOrg.registerAsParticipantDevice(pauline);
		focusExampleDotOrg.registerAsParticipantDevice(michelle);

		bctbx_list_t *coresList = bctbx_list_append(NULL, focusExampleDotOrg.getLc());
		coresList = bctbx_list_append(coresList, marie.getLc());
		coresList = bctbx_list_append(coresList, pauline.getLc());
		coresList = bctbx_list_append(coresList, michelle.getLc());
		Address paulineAddr = pauline.getIdentity();
		bctbx_list_t *participantsAddresses = bctbx_list_append(NULL, linphone_address_ref(paulineAddr.toC()));
		Address michelleAddr = michelle.getIdentity();
		participantsAddresses = bctbx_list_append(participantsAddresses, linphone_address_ref(michelleAddr.toC()));

		stats initialMarieStats = marie.getStats();
		stats initialPaulineStats = pauline.getStats();
		stats initialMichelleStats = michelle.getStats();

		// Marie creates a new group chat room
		const char *initialSubject = "Colleagues";
		LinphoneChatRoom *marieCr =
		    create_chat_room_client_side(coresList, marie.getCMgr(), &initialMarieStats, participantsAddresses,
		                                 initialSubject, FALSE, LinphoneChatRoomEphemeralModeDeviceManaged);
		LinphoneAddress *confAddr = linphone_address_clone(linphone_chat_room_get_conference_address(marieCr));

		// Check that the chat room is correctly created on Pauline's side and that the participants are added
		LinphoneChatRoom *paulineCr = check_creation_chat_room_client_side(
		    coresList, pauline.getCMgr(), &initialPaulineStats, confAddr, initialSubject, 2, FALSE);

		LinphoneChatRoom *michelleCr = check_creation_chat_room_client_side(
		    coresList, michelle.getCMgr(), &initialMichelleStats, confAddr, initialSubject, 2, FALSE);

		if (paulineCr || michelleCr) {
			// throw BCTBX_EXCEPTION << "Cannot create chatroom giving up";
			// goto end;
		}
		BC_ASSERT_TRUE(CoreManagerAssert({focusExampleDotOrg, marie, pauline, michelle}).wait([&focusExampleDotOrg] {
			for (auto chatRoom : focusExampleDotOrg.getCore().getChatRooms()) {
				for (auto participant : chatRoom->getParticipants()) {
					for (auto device : participant->getDevices())
						if (device->getState() != ParticipantDevice::State::Present) {
							return false;
						}
				}
			}
			return true;
		}));

		LinphoneChatMessage *msg = linphone_chat_room_create_message_from_utf8(marieCr, "message blabla");
		linphone_chat_message_send(msg);
		BC_ASSERT_TRUE(CoreManagerAssert({focusExampleDotOrg, marie, pauline, michelle}).wait([msg] {
			return (linphone_chat_message_get_state(msg) == LinphoneChatMessageStateDelivered);
		}));
		BC_ASSERT_TRUE(CoreManagerAssert({focusExampleDotOrg, marie, pauline, michelle}).wait([paulineCr] {
			return linphone_chat_room_get_unread_messages_count(paulineCr) == 1;
		}));
		BC_ASSERT_TRUE(CoreManagerAssert({focusExampleDotOrg, marie, pauline, michelle}).wait([michelleCr] {
			return linphone_chat_room_get_unread_messages_count(michelleCr) == 1;
		}));
		linphone_chat_message_unref(msg);

		// now change focus in order to get conference with multiple domain.
		focusAuth1DotExampleDotOrg.registerAsParticipantDevice(marie);
		focusAuth1DotExampleDotOrg.registerAsParticipantDevice(pauline);
		focusAuth1DotExampleDotOrg.registerAsParticipantDevice(michelle);

		// change conference factory uri
		Address focusAuth1DotExampleDotOrgFactoryAddress = focusAuth1DotExampleDotOrg.getIdentity();
		marie.configureCoreForConference(focusAuth1DotExampleDotOrgFactoryAddress);
		pauline.configureCoreForConference(focusAuth1DotExampleDotOrgFactoryAddress);
		michelle.configureCoreForConference(focusAuth1DotExampleDotOrgFactoryAddress);

		coresList = bctbx_list_append(coresList, focusAuth1DotExampleDotOrg.getLc());
		initialMarieStats = marie.getStats();
		initialPaulineStats = pauline.getStats();
		initialMichelleStats = michelle.getStats();
		participantsAddresses = bctbx_list_append(NULL, linphone_address_ref(paulineAddr.toC()));
		participantsAddresses = bctbx_list_append(participantsAddresses, linphone_address_ref(michelleAddr.toC()));
		LinphoneChatRoom *marieCrfocusAuth1DotExampleDotOrg =
		    create_chat_room_client_side(coresList, marie.getCMgr(), &initialMarieStats, participantsAddresses,
		                                 initialSubject, FALSE, LinphoneChatRoomEphemeralModeDeviceManaged);
		LinphoneAddress *confAddrfocusAuth1DotExampleDotOrg =
		    linphone_address_clone(linphone_chat_room_get_conference_address(marieCrfocusAuth1DotExampleDotOrg));

		// Check that the chat room is correctly created on Pauline's side and that the participants are added
		LinphoneChatRoom *paulineCrfocusAuth1DotExampleDotOrg =
		    check_creation_chat_room_client_side(coresList, pauline.getCMgr(), &initialPaulineStats,
		                                         confAddrfocusAuth1DotExampleDotOrg, initialSubject, 2, FALSE);
		BC_ASSERT_PTR_NOT_NULL(paulineCrfocusAuth1DotExampleDotOrg);
		LinphoneChatRoom *michelleCrfocusAuth1DotExampleDotOrg =
		    check_creation_chat_room_client_side(coresList, michelle.getCMgr(), &initialMichelleStats,
		                                         confAddrfocusAuth1DotExampleDotOrg, initialSubject, 2, FALSE);
		BC_ASSERT_PTR_NOT_NULL(michelleCrfocusAuth1DotExampleDotOrg);

		BC_ASSERT_TRUE(CoreManagerAssert({focusAuth1DotExampleDotOrg, marie, pauline, michelle})
		                   .wait([&focusAuth1DotExampleDotOrg] {
			                   for (auto chatRoom : focusAuth1DotExampleDotOrg.getCore().getChatRooms()) {
				                   for (auto participant : chatRoom->getParticipants()) {
					                   for (auto device : participant->getDevices())
						                   if (device->getState() != ParticipantDevice::State::Present) {
							                   return false;
						                   }
				                   }
			                   }
			                   return true;
		                   }));

		msg = linphone_chat_room_create_message_from_utf8(marieCrfocusAuth1DotExampleDotOrg, "message blabla");
		linphone_chat_message_send(msg);
		BC_ASSERT_TRUE(CoreManagerAssert({focusAuth1DotExampleDotOrg, marie, pauline, michelle}).wait([msg] {
			return (linphone_chat_message_get_state(msg) == LinphoneChatMessageStateDelivered);
		}));

		// great, now I want to see what happened if marie restart.
		coresList = bctbx_list_remove(coresList, marie.getLc());
		marie.reStart();
		marie.setupMgrForConference();
		coresList = bctbx_list_append(coresList, marie.getLc());

		// Retrieve chat room
		LinphoneAddress *marieDeviceAddr =
		    linphone_address_clone(linphone_proxy_config_get_contact(marie.getDefaultProxyConfig()));
		marieCr = marie.searchChatRoom(marieDeviceAddr, confAddr);
		BC_ASSERT_PTR_NOT_NULL(marieCr);
		marieCrfocusAuth1DotExampleDotOrg = marie.searchChatRoom(marieDeviceAddr, confAddrfocusAuth1DotExampleDotOrg);
		BC_ASSERT_PTR_NOT_NULL(marieCrfocusAuth1DotExampleDotOrg);

		CoreManagerAssert({focusExampleDotOrg, marie, pauline, michelle}).waitUntil(chrono::seconds(2), [] {
			return false;
		});

		BC_ASSERT_TRUE(wait_for_list(coresList, &marie.getStats().number_of_LinphoneSubscriptionActive, 2,
		                             liblinphone_tester_sip_timeout));

		ClientConference laure("laure_tcp_rc", focusExampleDotOrg.getIdentity());
		coresList = bctbx_list_append(coresList, laure.getLc());
		Address laureAddr = laure.getIdentity();
		focusExampleDotOrg.registerAsParticipantDevice(laure);

		initialMarieStats = marie.getStats();
		initialPaulineStats = pauline.getStats();
		initialMichelleStats = michelle.getStats();
		stats initialLaureStats = laure.getStats();

		participantsAddresses = bctbx_list_append(NULL, linphone_address_ref(laureAddr.toC()));
		linphone_chat_room_add_participants(marieCr, participantsAddresses);

		BC_ASSERT_TRUE(wait_for_list(coresList, &laure.getStats().number_of_LinphoneConferenceStateCreationPending,
		                             initialLaureStats.number_of_LinphoneConferenceStateCreationPending + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &laure.getStats().number_of_LinphoneConferenceStateCreated,
		                             initialLaureStats.number_of_LinphoneConferenceStateCreated + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &laure.getStats().number_of_LinphoneChatRoomConferenceJoined,
		                             initialLaureStats.number_of_LinphoneChatRoomConferenceJoined + 1,
		                             liblinphone_tester_sip_timeout));

		LinphoneChatRoom *laureCr = check_creation_chat_room_client_side(coresList, laure.getCMgr(), &initialLaureStats,
		                                                                 confAddr, initialSubject, 3, FALSE);
		BC_ASSERT_PTR_NOT_NULL(laureCr);

		BC_ASSERT_TRUE(wait_for_list(coresList, &marie.getStats().number_of_participants_added,
		                             initialMarieStats.number_of_participants_added + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &pauline.getStats().number_of_participants_added,
		                             initialPaulineStats.number_of_participants_added + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &michelle.getStats().number_of_participants_added,
		                             initialMichelleStats.number_of_participants_added + 1,
		                             liblinphone_tester_sip_timeout));

		BC_ASSERT_EQUAL(linphone_chat_room_get_nb_participants(marieCr), 3, int, "%d");
		BC_ASSERT_EQUAL(linphone_chat_room_get_nb_participants(paulineCr), 3, int, "%d");
		BC_ASSERT_EQUAL(linphone_chat_room_get_nb_participants(michelleCr), 3, int, "%d");

		BC_ASSERT_TRUE(wait_for_list(coresList, &pauline.getStats().number_of_NotifyReceived,
		                             initialPaulineStats.number_of_NotifyReceived + 1, liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &michelle.getStats().number_of_NotifyReceived,
		                             initialMichelleStats.number_of_NotifyReceived + 1,
		                             liblinphone_tester_sip_timeout));

		focusAuth1DotExampleDotOrg.registerAsParticipantDevice(laure);
		laure.configureCoreForConference(focusAuth1DotExampleDotOrgFactoryAddress);

		participantsAddresses = bctbx_list_append(NULL, linphone_address_ref(laureAddr.toC()));
		linphone_chat_room_add_participants(marieCrfocusAuth1DotExampleDotOrg, participantsAddresses);
		BC_ASSERT_TRUE(wait_for_list(coresList, &laure.getStats().number_of_LinphoneConferenceStateCreationPending,
		                             initialLaureStats.number_of_LinphoneConferenceStateCreationPending + 2,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &laure.getStats().number_of_LinphoneConferenceStateCreated,
		                             initialLaureStats.number_of_LinphoneConferenceStateCreated + 2,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &laure.getStats().number_of_LinphoneChatRoomConferenceJoined,
		                             initialLaureStats.number_of_LinphoneChatRoomConferenceJoined + 2,
		                             liblinphone_tester_sip_timeout));

		LinphoneChatRoom *laureCrfocusAuth1DotExampleDotOrg =
		    check_creation_chat_room_client_side(coresList, laure.getCMgr(), &initialLaureStats,
		                                         confAddrfocusAuth1DotExampleDotOrg, initialSubject, 3, FALSE);
		BC_ASSERT_PTR_NOT_NULL(laureCrfocusAuth1DotExampleDotOrg);

		BC_ASSERT_TRUE(wait_for_list(coresList, &marie.getStats().number_of_participants_added,
		                             initialMarieStats.number_of_participants_added + 2,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &pauline.getStats().number_of_participants_added,
		                             initialPaulineStats.number_of_participants_added + 2,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &michelle.getStats().number_of_participants_added,
		                             initialMichelleStats.number_of_participants_added + 2,
		                             liblinphone_tester_sip_timeout));

		BC_ASSERT_EQUAL(linphone_chat_room_get_nb_participants(marieCrfocusAuth1DotExampleDotOrg), 3, int, "%d");
		BC_ASSERT_EQUAL(linphone_chat_room_get_nb_participants(paulineCrfocusAuth1DotExampleDotOrg), 3, int, "%d");
		BC_ASSERT_EQUAL(linphone_chat_room_get_nb_participants(michelleCrfocusAuth1DotExampleDotOrg), 3, int, "%d");

		linphone_chat_message_unref(msg);

		linphone_address_unref(marieDeviceAddr);
		bctbx_list_free(coresList);
	}
}

static void one_to_one_group_chat_room_deletion_by_server_client(void) {
	one_to_one_group_chat_room_deletion_by_server_client_base(FALSE);
}

static void group_chat_room_add_participant_with_invalid_address(void) {
	Focus focus("chloe_rc");
	{ // to make sure focus is destroyed after clients.
		ClientConference marie("marie_rc", focus.getIdentity());
		ClientConference pauline("pauline_rc", focus.getIdentity());
		ClientConference michelle("michelle_rc", focus.getIdentity());

		focus.registerAsParticipantDevice(marie);
		focus.registerAsParticipantDevice(pauline);
		focus.registerAsParticipantDevice(michelle);

		bctbx_list_t *coresList = bctbx_list_append(NULL, focus.getLc());
		coresList = bctbx_list_append(coresList, marie.getLc());
		coresList = bctbx_list_append(coresList, pauline.getLc());
		coresList = bctbx_list_append(coresList, michelle.getLc());
		Address paulineAddr = pauline.getIdentity();
		bctbx_list_t *participantsAddresses = bctbx_list_append(NULL, linphone_address_ref(paulineAddr.toC()));
		Address invalidAddr = Address();
		participantsAddresses = bctbx_list_append(participantsAddresses, linphone_address_ref(invalidAddr.toC()));
		Address michelleAddr = michelle.getIdentity();
		participantsAddresses = bctbx_list_append(participantsAddresses, linphone_address_ref(michelleAddr.toC()));

		stats initialMarieStats = marie.getStats();
		stats initialPaulineStats = pauline.getStats();
		stats initialMichelleStats = michelle.getStats();

		// Marie creates a new group chat room
		const char *initialSubject = "Colleagues";
		LinphoneChatRoom *marieCr = create_chat_room_client_side_with_expected_number_of_participants(
		    coresList, marie.getCMgr(), &initialMarieStats, participantsAddresses, initialSubject, 2, FALSE,
		    LinphoneChatRoomEphemeralModeDeviceManaged);
		const LinphoneAddress *confAddr = linphone_chat_room_get_conference_address(marieCr);

		// Check that the chat room is correctly created on Pauline's side and that the participants are added
		LinphoneChatRoom *paulineCr = check_creation_chat_room_client_side(
		    coresList, pauline.getCMgr(), &initialPaulineStats, confAddr, initialSubject, 2, FALSE);
		LinphoneChatRoom *michelleCr = check_creation_chat_room_client_side(
		    coresList, michelle.getCMgr(), &initialMichelleStats, confAddr, initialSubject, 2, FALSE);

		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, pauline, michelle}).wait([&focus] {
			for (auto chatRoom : focus.getCore().getChatRooms()) {
				for (auto participant : chatRoom->getParticipants()) {
					for (auto device : participant->getDevices())
						if (device->getState() != ParticipantDevice::State::Present) {
							return false;
						}
				}
			}
			return true;
		}));

		BC_ASSERT_TRUE(wait_for_list(coresList, &pauline.getStats().number_of_LinphoneConferenceStateCreated,
		                             initialPaulineStats.number_of_LinphoneConferenceStateCreated + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &michelle.getStats().number_of_LinphoneConferenceStateCreated,
		                             initialMichelleStats.number_of_LinphoneConferenceStateCreated + 1,
		                             liblinphone_tester_sip_timeout));

		// Marie now changes the subject
		const char *newSubject = "Let's go drink a beer";
		linphone_chat_room_set_subject(marieCr, newSubject);
		BC_ASSERT_TRUE(wait_for_list(coresList, &marie.getStats().number_of_subject_changed,
		                             initialMarieStats.number_of_subject_changed + 1, liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &pauline.getStats().number_of_subject_changed,
		                             initialPaulineStats.number_of_subject_changed + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_TRUE(wait_for_list(coresList, &michelle.getStats().number_of_subject_changed,
		                             initialMichelleStats.number_of_subject_changed + 1,
		                             liblinphone_tester_sip_timeout));
		BC_ASSERT_STRING_EQUAL(linphone_chat_room_get_subject(marieCr), newSubject);
		BC_ASSERT_STRING_EQUAL(linphone_chat_room_get_subject(paulineCr), newSubject);
		BC_ASSERT_STRING_EQUAL(linphone_chat_room_get_subject(michelleCr), newSubject);

		BC_ASSERT_EQUAL(linphone_chat_room_get_nb_participants(marieCr), 2, int, "%d");
		BC_ASSERT_EQUAL(linphone_chat_room_get_nb_participants(paulineCr), 2, int, "%d");
		BC_ASSERT_EQUAL(linphone_chat_room_get_nb_participants(michelleCr), 2, int, "%d");

		initialMarieStats = marie.getStats();
		initialPaulineStats = pauline.getStats();
		initialMichelleStats = michelle.getStats();

		linphone_chat_room_add_participant(marieCr, linphone_address_ref(invalidAddr.toC()));

		BC_ASSERT_FALSE(wait_for_list(coresList, &marie.getStats().number_of_participants_added,
		                              initialMarieStats.number_of_participants_added + 1, 5000));
		BC_ASSERT_FALSE(wait_for_list(coresList, &marie.getStats().number_of_participant_devices_added,
		                              initialMarieStats.number_of_participant_devices_added + 1, 1000));
		BC_ASSERT_FALSE(wait_for_list(coresList, &marie.getStats().number_of_participant_devices_joined,
		                              initialMarieStats.number_of_participant_devices_joined + 1, 1000));
		BC_ASSERT_FALSE(wait_for_list(coresList, &pauline.getStats().number_of_participants_added,
		                              initialPaulineStats.number_of_participants_added + 1, 1000));
		BC_ASSERT_FALSE(wait_for_list(coresList, &pauline.getStats().number_of_participant_devices_added,
		                              initialPaulineStats.number_of_participant_devices_added + 1, 1000));
		BC_ASSERT_FALSE(wait_for_list(coresList, &pauline.getStats().number_of_participant_devices_joined,
		                              initialPaulineStats.number_of_participant_devices_joined + 1, 1000));
		BC_ASSERT_FALSE(wait_for_list(coresList, &michelle.getStats().number_of_participants_added,
		                              initialMichelleStats.number_of_participants_added + 1, 1000));
		BC_ASSERT_FALSE(wait_for_list(coresList, &michelle.getStats().number_of_participant_devices_added,
		                              initialMichelleStats.number_of_participant_devices_added + 1, 1000));
		BC_ASSERT_FALSE(wait_for_list(coresList, &michelle.getStats().number_of_participant_devices_joined,
		                              initialMichelleStats.number_of_participant_devices_joined + 1, 1000));

		BC_ASSERT_EQUAL(linphone_chat_room_get_nb_participants(marieCr), 2, int, "%d");
		BC_ASSERT_EQUAL(linphone_chat_room_get_nb_participants(paulineCr), 2, int, "%d");
		BC_ASSERT_EQUAL(linphone_chat_room_get_nb_participants(michelleCr), 2, int, "%d");

		CoreManagerAssert({focus, marie, pauline, michelle}).waitUntil(std::chrono::seconds(1), [] { return false; });

		CoreManagerAssert({focus, marie}).waitUntil(std::chrono::seconds(2), [] { return false; });

		for (auto chatRoom : focus.getCore().getChatRooms()) {
			for (auto participant : chatRoom->getParticipants()) {
				//  force deletion by removing devices
				std::shared_ptr<Address> participantAddress = participant->getAddress();
				linphone_chat_room_set_participant_devices(L_GET_C_BACK_PTR(chatRoom), participantAddress->toC(), NULL);
			}
		}

		// wait until chatroom is deleted server side
		BC_ASSERT_TRUE(CoreManagerAssert({focus, marie, pauline, michelle}).wait([&focus] {
			return focus.getCore().getChatRooms().size() == 0;
		}));

		// wait bit more to detect side effect if any
		CoreManagerAssert({focus, marie, pauline, michelle}).waitUntil(chrono::seconds(2), [] { return false; });

		// to avoid creation attempt of a new chatroom
		auto config = focus.getDefaultProxyConfig();
		linphone_proxy_config_edit(config);
		linphone_proxy_config_set_conference_factory_uri(config, NULL);
		linphone_proxy_config_done(config);

		linphone_address_unref(invalidAddr.toC());
		bctbx_list_free(coresList);
	}
}

static void group_chat_room_with_only_participant_with_invalid_address(void) {
	Focus focus("chloe_rc");
	{ // to make sure focus is destroyed after clients.
		ClientConference marie("marie_rc", focus.getIdentity());

		focus.registerAsParticipantDevice(marie);

		bctbx_list_t *coresList = bctbx_list_append(NULL, focus.getLc());
		coresList = bctbx_list_append(coresList, marie.getLc());
		Address invalidAddr = Address();
		bctbx_list_t *participantsAddresses = bctbx_list_append(NULL, linphone_address_ref(invalidAddr.toC()));

		stats initialMarieStats = marie.getStats();

		// Marie creates a new group chat room
		const char *initialSubject = "Colleagues";

		LinphoneChatRoomParams *chatRoomParams = linphone_core_create_default_chat_room_params(marie.getLc());
		linphone_chat_room_params_enable_encryption(chatRoomParams, FALSE);
		linphone_chat_room_params_set_backend(chatRoomParams, LinphoneChatRoomBackendFlexisipChat);
		linphone_chat_room_params_enable_group(chatRoomParams, TRUE);
		LinphoneChatRoom *marieCr = create_chat_room_client_side_with_expected_number_of_participants(
		    coresList, marie.getCMgr(), &initialMarieStats, participantsAddresses, initialSubject, 0, FALSE,
		    LinphoneChatRoomEphemeralModeDeviceManaged);
		linphone_chat_room_params_unref(chatRoomParams);
		BC_ASSERT_PTR_NOT_NULL(marieCr);

		// Check that the chat room has not been created
		BC_ASSERT_FALSE(wait_for_list(coresList, &marie.getStats().number_of_LinphoneConferenceStateCreated,
		                              initialMarieStats.number_of_LinphoneConferenceStateCreated + 1, 3000));
		BC_ASSERT_FALSE(wait_for_list(coresList, &marie.getStats().number_of_LinphoneChatRoomConferenceJoined,
		                              initialMarieStats.number_of_LinphoneChatRoomConferenceJoined + 1, 1000));
		BC_ASSERT_TRUE(wait_for_list(coresList, &marie.getStats().number_of_LinphoneConferenceStateCreationFailed,
		                             initialMarieStats.number_of_LinphoneConferenceStateCreationFailed + 1,
		                             liblinphone_tester_sip_timeout));

		// to avoid creation attempt of a new chatroom
		auto config = focus.getDefaultProxyConfig();
		linphone_proxy_config_edit(config);
		linphone_proxy_config_set_conference_factory_uri(config, NULL);
		linphone_proxy_config_done(config);

		bctbx_list_free(coresList);
	}
}

} // namespace LinphoneTest

static test_t local_conference_chat_tests[] = {
    TEST_ONE_TAG("Group chat room creation local server",
                 LinphoneTest::group_chat_room_creation_server,
                 "LeaksMemory"), /* beacause of coreMgr restart*/
    TEST_NO_TAG("Group chat Server chat room deletion", LinphoneTest::group_chat_room_server_deletion),
    TEST_ONE_TAG("Group chat with client removed added",
                 LinphoneTest::group_chat_room_with_client_removed_added,
                 "LeaksMemory"), /* beacause of coreMgr restart*/
    TEST_ONE_TAG("Group chat with client restart",
                 LinphoneTest::group_chat_room_with_client_restart,
                 "LeaksMemory"), /* beacause of coreMgr restart*/
    TEST_NO_TAG("Group chat with client registering with a short REGISTER expires",
                LinphoneTest::group_chat_room_with_client_registering_with_short_register_expires),
    TEST_ONE_TAG("Group chat with client restart and removed from server",
                 LinphoneTest::group_chat_room_with_client_restart_removed_from_server,
                 "LeaksMemory"), /* beacause of coreMgr restart*/
    TEST_ONE_TAG("Group chat with client removed while stopped (Remote Conference List Event Handler)",
                 LinphoneTest::group_chat_room_with_client_removed_while_stopped_remote_list_event_handler,
                 "LeaksMemory"), /* beacause of coreMgr restart*/
    TEST_ONE_TAG("Group chat with client removed while stopped (No Remote Conference List Event Handler)",
                 LinphoneTest::group_chat_room_with_client_removed_while_stopped_no_remote_list_event_handler,
                 "LeaksMemory"), /* beacause of coreMgr restart*/
    TEST_NO_TAG("Group chat with creator without groupchat capability",
                LinphoneTest::group_chat_room_with_creator_without_groupchat_capability),
    TEST_NO_TAG("Group chat with creator without groupchat capability in register",
                LinphoneTest::group_chat_room_with_creator_without_groupchat_capability_in_register),
    TEST_NO_TAG("Group chat room bulk notify to participant",
                LinphoneTest::group_chat_room_bulk_notify_to_participant), /* because of network up and down*/
    TEST_ONE_TAG("One to one chatroom exhumed while participant is offline",
                 LinphoneTest::one_to_one_chatroom_exhumed_while_offline,
                 "LeaksMemory"), /* because of network up and down*/
    TEST_ONE_TAG("Group chat Server chat room deletion with remote list event handler",
                 LinphoneTest::group_chat_room_server_deletion_with_rmt_lst_event_handler,
                 "LeaksMemory"), /* because of coreMgr restart*/
    TEST_ONE_TAG("One to one group chat deletion initiated by server and client",
                 LinphoneTest::one_to_one_group_chat_room_deletion_by_server_client,
                 "LeaksMemory"), /* because of network up and down */
    TEST_ONE_TAG("Group chat room deletes chatroom after restart",
                 LinphoneTest::group_chat_room_with_client_deletes_chatroom_after_restart,
                 "LeaksMemory"), /* because of network up and down */
    TEST_ONE_TAG("Multi domain chatroom",
                 LinphoneTest::multidomain_group_chat_room,
                 "LeaksMemory") /* because of coreMgr restart*/
};

static test_t local_conference_chat_error_tests[] = {
    TEST_ONE_TAG("Group chat with INVITE session error",
                 LinphoneTest::group_chat_room_with_invite_error,
                 "LeaksMemory"), /* because of network up and down */
    TEST_ONE_TAG("Group chat with SUBSCRIBE session error",
                 LinphoneTest::group_chat_room_with_subscribe_error,
                 "LeaksMemory"), /* because of network up and down */
    TEST_NO_TAG("Group chat Add participant with invalid address",
                LinphoneTest::group_chat_room_add_participant_with_invalid_address),
    TEST_NO_TAG("Group chat Only participant with invalid address",
                LinphoneTest::group_chat_room_with_only_participant_with_invalid_address),

};

test_suite_t local_conference_test_suite_chat = {"Local conference tester (Chat)",
                                                 NULL,
                                                 NULL,
                                                 liblinphone_tester_before_each,
                                                 liblinphone_tester_after_each,
                                                 sizeof(local_conference_chat_tests) /
                                                     sizeof(local_conference_chat_tests[0]),
                                                 local_conference_chat_tests};

test_suite_t local_conference_test_suite_chat_error = {"Local conference tester (Chat error)",
                                                       NULL,
                                                       NULL,
                                                       liblinphone_tester_before_each,
                                                       liblinphone_tester_after_each,
                                                       sizeof(local_conference_chat_error_tests) /
                                                           sizeof(local_conference_chat_error_tests[0]),
                                                       local_conference_chat_error_tests};
