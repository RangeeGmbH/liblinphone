/*
 * Copyright (c) 2010-2022 Belledonne Communications SARL.
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

#include <bctoolbox/defs.h>

#include "belle-sip/belle-sip.h"

#include "ortp/b64.h"

#include "linphone/core.h"
#include "linphone/lpconfig.h"
#include "linphone/wrapper_utils.h"
#include "private.h"
#include <linphone/utils/utils.h>

#include "c-wrapper/c-wrapper.h"
#include "call/call.h"
#include "chat/chat-room/abstract-chat-room-p.h"
#include "chat/chat-room/basic-chat-room.h"
#include "chat/chat-room/chat-room-params.h"
#ifdef HAVE_ADVANCED_IM
#include "chat/chat-room/client-group-chat-room.h"
#include "chat/chat-room/client-group-to-basic-chat-room.h"
#endif
#include "content/content-type.h"
#include "core/core-p.h"
#include "linphone/api/c-chat-room-params.h"

using namespace std;

void linphone_core_disable_chat(LinphoneCore *lc, LinphoneReason deny_reason) {
	lc->chat_deny_code = deny_reason;
}

void linphone_core_enable_chat(LinphoneCore *lc) {
	lc->chat_deny_code = LinphoneReasonNone;
}

bool_t linphone_core_chat_enabled(const LinphoneCore *lc) {
	return lc->chat_deny_code != LinphoneReasonNone;
}

const bctbx_list_t *linphone_core_get_chat_rooms(LinphoneCore *lc) {
	if (lc->chat_rooms) bctbx_list_free_with_data(lc->chat_rooms, (bctbx_list_free_func)linphone_chat_room_unref);
	lc->chat_rooms = L_GET_RESOLVED_C_LIST_FROM_CPP_LIST(L_GET_CPP_PTR_FROM_C_OBJECT(lc)->getChatRooms());
	return lc->chat_rooms;
}

LinphoneChatRoom *linphone_core_create_client_group_chat_room(LinphoneCore *lc, const char *subject, bool_t fallback) {
	return linphone_core_create_client_group_chat_room_2(lc, subject, fallback, FALSE);
}

// Deprecated see linphone_core_create_chat_room_6
LinphoneChatRoom *linphone_core_create_client_group_chat_room_2(LinphoneCore *lc,
                                                                const char *subject,
                                                                bool_t fallback,
                                                                bool_t encrypted) {
	return L_GET_C_BACK_PTR(
	    L_GET_PRIVATE_FROM_C_OBJECT(lc)->createClientGroupChatRoom(L_C_TO_STRING(subject), !!fallback, !!encrypted));
}

// Deprecated see linphone_core_create_chat_room_6
LinphoneChatRoom *linphone_core_create_chat_room(LinphoneCore *lc,
                                                 const LinphoneChatRoomParams *params,
                                                 const LinphoneAddress *localAddr,
                                                 const char *subject,
                                                 const bctbx_list_t *participants) {
	LinphoneChatRoomParams *params2 = linphone_chat_room_params_clone(params);
	linphone_chat_room_params_set_subject(params2, subject);
	LinphoneChatRoom *result = linphone_core_create_chat_room_6(lc, params2, localAddr, participants);
	linphone_chat_room_params_unref(params2);
	return result;
}

// Deprecated see linphone_core_create_chat_room_6
LinphoneChatRoom *linphone_core_create_chat_room_2(LinphoneCore *lc,
                                                   const LinphoneChatRoomParams *params,
                                                   const char *subject,
                                                   const bctbx_list_t *participants) {
	LinphoneChatRoomParams *params2 = linphone_chat_room_params_clone(params);
	linphone_chat_room_params_set_subject(params2, subject);
	LinphoneChatRoom *result = linphone_core_create_chat_room_6(lc, params2, NULL, participants);
	linphone_chat_room_params_unref(params2);
	return result;
}

// Deprecated see linphone_core_create_chat_room_6
LinphoneChatRoom *
linphone_core_create_chat_room_3(LinphoneCore *lc, const char *subject, const bctbx_list_t *participants) {
	LinphoneChatRoomParams *params = linphone_core_create_default_chat_room_params(lc);
	linphone_chat_room_params_set_subject(params, subject);
	LinphoneChatRoom *result = linphone_core_create_chat_room_6(lc, params, NULL, participants);
	linphone_chat_room_params_unref(params);
	return result;
}

// Deprecated see linphone_core_create_chat_room_6
LinphoneChatRoom *linphone_core_create_chat_room_4(LinphoneCore *lc,
                                                   const LinphoneChatRoomParams *params,
                                                   const LinphoneAddress *localAddr,
                                                   const LinphoneAddress *participant) {
	bctbx_list_t *paricipants = bctbx_list_prepend(NULL, (LinphoneAddress *)participant);
	LinphoneChatRoom *result = linphone_core_create_chat_room_6(lc, params, localAddr, paricipants);
	bctbx_list_free(paricipants);
	return result;
}

// Deprecated see linphone_core_create_chat_room_6
LinphoneChatRoom *linphone_core_create_chat_room_5(LinphoneCore *lc, const LinphoneAddress *participant) {
	bctbx_list_t *paricipants = bctbx_list_prepend(NULL, (LinphoneAddress *)participant);
	LinphoneChatRoomParams *params = linphone_core_create_default_chat_room_params(lc);
	LinphoneChatRoom *result = linphone_core_create_chat_room_6(lc, params, NULL, paricipants);
	linphone_chat_room_params_unref(params);
	bctbx_list_free(paricipants);
	return result;
}

LinphoneChatRoom *linphone_core_create_chat_room_6(LinphoneCore *lc,
                                                   const LinphoneChatRoomParams *params,
                                                   const LinphoneAddress *localAddr,
                                                   const bctbx_list_t *participants) {
	shared_ptr<LinphonePrivate::ChatRoomParams> chatRoomParams =
	    params ? LinphonePrivate::ChatRoomParams::toCpp(params)->clone()->toSharedPtr() : nullptr;
	// If a participant has an invalid address, the pointer to its address is NULL.
	// For the purpose of building an std::list from a bctbx_list_t, replace it by an empty Address (that is invalid)
	const list<std::shared_ptr<LinphonePrivate::Address>> participantsList =
	    LinphonePrivate::Address::getCppListFromCList(participants);
	bool withGruu = chatRoomParams ? chatRoomParams->getChatRoomBackend() ==
	                                     LinphonePrivate::ChatRoomParams::ChatRoomBackend::FlexisipChat
	                               : false;
	shared_ptr<const LinphonePrivate::Address> identityAddress =
	    localAddr ? LinphonePrivate::Address::toCpp(localAddr)->getSharedFromThis()
	              : L_GET_PRIVATE_FROM_C_OBJECT(lc)->getDefaultLocalAddress(nullptr, withGruu);
	shared_ptr<LinphonePrivate::AbstractChatRoom> room =
	    L_GET_PRIVATE_FROM_C_OBJECT(lc)->createChatRoom(chatRoomParams, identityAddress, participantsList);
	if (room) {
		auto cRoom = L_GET_C_BACK_PTR(room);
		linphone_chat_room_ref(cRoom);
		return cRoom;
	}
	return NULL;
}

LinphoneChatRoom *linphone_core_search_chat_room(const LinphoneCore *lc,
                                                 const LinphoneChatRoomParams *params,
                                                 const LinphoneAddress *localAddr,
                                                 const LinphoneAddress *remoteAddr,
                                                 const bctbx_list_t *participants) {
	shared_ptr<LinphonePrivate::ChatRoomParams> chatRoomParams =
	    params ? LinphonePrivate::ChatRoomParams::toCpp(params)->clone()->toSharedPtr() : nullptr;
	const list<std::shared_ptr<LinphonePrivate::Address>> participantsList =
	    LinphonePrivate::Address::getCppListFromCList(participants);
	bool withGruu = chatRoomParams ? chatRoomParams->getChatRoomBackend() ==
	                                     LinphonePrivate::ChatRoomParams::ChatRoomBackend::FlexisipChat
	                               : false;
	shared_ptr<const LinphonePrivate::Address> identityAddress =
	    localAddr ? LinphonePrivate::Address::toCpp(localAddr)->getSharedFromThis()
	              : L_GET_PRIVATE_FROM_C_OBJECT(lc)->getDefaultLocalAddress(nullptr, withGruu);
	shared_ptr<const LinphonePrivate::Address> remoteAddress =
	    remoteAddr ? LinphonePrivate::Address::toCpp(remoteAddr)->getSharedFromThis() : nullptr;
	shared_ptr<LinphonePrivate::AbstractChatRoom> room = L_GET_PRIVATE_FROM_C_OBJECT(lc)->searchChatRoom(
	    chatRoomParams, identityAddress, remoteAddress, participantsList);
	if (room) return L_GET_C_BACK_PTR(room);
	return NULL;
}

LinphoneChatRoomParams *linphone_core_create_default_chat_room_params(LinphoneCore *lc) {
	auto params = LinphonePrivate::ChatRoomParams::getDefaults(L_GET_CPP_PTR_FROM_C_OBJECT(lc)->getSharedFromThis());
	params->ref();
	return params->toC();
}

static LinphoneChatRoomParams *_linphone_core_create_default_chat_room_params() {
	auto params = LinphonePrivate::ChatRoomParams::getDefaults();
	params->ref();
	return params->toC();
}

LinphoneChatRoom *_linphone_core_create_server_group_chat_room(LinphoneCore *lc, LinphonePrivate::SalCallOp *op) {
	return _linphone_server_group_chat_room_new(lc, op);
}

void linphone_core_delete_chat_room(LinphoneCore *, LinphoneChatRoom *cr) {
	L_GET_CPP_PTR_FROM_C_OBJECT(cr)->deleteFromDb();
}

// Deprecated see linphone_core_search_chat_room
LinphoneChatRoom *linphone_core_get_chat_room(LinphoneCore *lc, const LinphoneAddress *peerAddr) {
	return linphone_core_get_chat_room_2(lc, peerAddr, NULL);
}

// Deprecated see linphone_core_search_chat_room
LinphoneChatRoom *
linphone_core_get_chat_room_2(LinphoneCore *lc, const LinphoneAddress *peer_addr, const LinphoneAddress *local_addr) {
	LinphoneChatRoom *result = linphone_core_search_chat_room(lc, NULL, local_addr, peer_addr, NULL);
	if (result == NULL) {
		bctbx_list_t *paricipants = bctbx_list_prepend(NULL, (LinphoneAddress *)peer_addr);
		LinphoneChatRoomParams *params = linphone_core_create_default_chat_room_params(lc);
		linphone_chat_room_params_set_backend(params, LinphoneChatRoomBackendBasic);
		linphone_chat_room_params_enable_group(params, FALSE);
		result = linphone_core_create_chat_room_6(lc, params, local_addr, paricipants);
		if (result) linphone_chat_room_unref(result);
		linphone_chat_room_params_unref(params);
		bctbx_list_free(paricipants);
	}
	return result;
}

// Deprecated see linphone_core_search_chat_room
LinphoneChatRoom *linphone_core_get_chat_room_from_uri(LinphoneCore *lc, const char *to) {
	LinphoneAddress *addr = linphone_core_interpret_url(lc, to);
	LinphoneChatRoom *room = linphone_core_get_chat_room(lc, addr);
	if (addr) linphone_address_unref(addr);
	return room;
}

// Deprecated see linphone_core_search_chat_room
LinphoneChatRoom *linphone_core_find_chat_room(const LinphoneCore *lc,
                                               const LinphoneAddress *peer_addr,
                                               const LinphoneAddress *local_addr) {
	LinphoneChatRoom *result = linphone_core_search_chat_room(lc, NULL, local_addr, peer_addr, NULL);
	return result;
}

// Deprecated see linphone_core_search_chat_room
LinphoneChatRoom *linphone_core_find_one_to_one_chat_room(const LinphoneCore *lc,
                                                          const LinphoneAddress *local_addr,
                                                          const LinphoneAddress *participant_addr) {
	bctbx_list_t *paricipants = bctbx_list_prepend(NULL, (LinphoneAddress *)participant_addr);
	LinphoneChatRoomParams *params = _linphone_core_create_default_chat_room_params();
	linphone_chat_room_params_set_backend(params, LinphoneChatRoomBackendFlexisipChat);
	linphone_chat_room_params_enable_group(params, FALSE);
	LinphoneChatRoom *result = linphone_core_search_chat_room(lc, params, local_addr, NULL, paricipants);
	if (!result) {
		linphone_chat_room_params_set_backend(params, LinphoneChatRoomBackendBasic);
		result = linphone_core_search_chat_room(lc, params, local_addr, participant_addr, NULL);
	}
	linphone_chat_room_params_unref(params);
	bctbx_list_free(paricipants);
	return result;
}

// Deprecated see linphone_core_search_chat_room
LinphoneChatRoom *linphone_core_find_one_to_one_chat_room_2(const LinphoneCore *lc,
                                                            const LinphoneAddress *local_addr,
                                                            const LinphoneAddress *participant_addr,
                                                            bool_t encrypted) {
	bctbx_list_t *paricipants = bctbx_list_prepend(NULL, (LinphoneAddress *)participant_addr);
	LinphoneChatRoomParams *params = _linphone_core_create_default_chat_room_params();
	linphone_chat_room_params_set_backend(params, LinphoneChatRoomBackendFlexisipChat);
	linphone_chat_room_params_enable_group(params, FALSE);
	linphone_chat_room_params_enable_encryption(params, encrypted);
	LinphoneChatRoom *result = linphone_core_search_chat_room(lc, params, local_addr, NULL, paricipants);
	if (!result && !encrypted) {
		linphone_chat_room_params_set_backend(params, LinphoneChatRoomBackendBasic);
		result = linphone_core_search_chat_room(lc, params, local_addr, participant_addr, NULL);
	}
	linphone_chat_room_params_unref(params);
	bctbx_list_free(paricipants);
	return result;
}

LinphoneReason linphone_core_message_received(LinphoneCore *lc, LinphonePrivate::SalOp *op, const SalMessage *sal_msg) {
	return L_GET_CPP_PTR_FROM_C_OBJECT(lc)->onSipMessageReceived(op, sal_msg);
}

unsigned int linphone_chat_message_store(BCTBX_UNUSED(LinphoneChatMessage *msg)) {
	// DO nothing, just for old JNI compat...
	return 1;
}
