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

#ifndef _L_LOCAL_CONFERENCE_H_
#define _L_LOCAL_CONFERENCE_H_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "conference.h"
#include "event/event-subscribe.h"

// =============================================================================

LINPHONE_BEGIN_NAMESPACE

class LINPHONE_PUBLIC LocalConference : public Conference {
	friend class ServerGroupChatRoomPrivate;
	friend class ServerGroupChatRoom;

public:
	LocalConference(const std::shared_ptr<Core> &core,
	                const std::shared_ptr<Address> &myAddress,
	                CallSessionListener *listener,
	                const std::shared_ptr<ConferenceParams> params,
	                ConferenceListener *confListener = nullptr);
	virtual ~LocalConference();

	void subscribeReceived(const std::shared_ptr<EventSubscribe> &event);

	virtual bool isIn() const override;

	virtual std::shared_ptr<ConferenceParticipantEvent> notifyParticipantAdded(
	    time_t creationTime, const bool isFullState, const std::shared_ptr<Participant> &participant) override;
	virtual std::shared_ptr<ConferenceParticipantEvent> notifyParticipantRemoved(
	    time_t creationTime, const bool isFullState, const std::shared_ptr<Participant> &participant) override;
	virtual std::shared_ptr<ConferenceParticipantEvent>
	notifyParticipantSetAdmin(time_t creationTime,
	                          const bool isFullState,
	                          const std::shared_ptr<Participant> &participant,
	                          bool isAdmin) override;
	virtual std::shared_ptr<ConferenceSubjectEvent>
	notifySubjectChanged(time_t creationTime, const bool isFullState, const std::string subject) override;
	virtual std::shared_ptr<ConferenceEphemeralMessageEvent>
	notifyEphemeralModeChanged(time_t creationTime, const bool isFullState, const EventLog::Type type) override;
	virtual std::shared_ptr<ConferenceEphemeralMessageEvent>
	notifyEphemeralMessageEnabled(time_t creationTime, const bool isFullState, const bool enable) override;
	virtual std::shared_ptr<ConferenceEphemeralMessageEvent>
	notifyEphemeralLifetimeChanged(time_t creationTime, const bool isFullState, const long lifetime) override;
	virtual std::shared_ptr<ConferenceParticipantDeviceEvent>
	notifyParticipantDeviceAdded(time_t creationTime,
	                             const bool isFullState,
	                             const std::shared_ptr<Participant> &participant,
	                             const std::shared_ptr<ParticipantDevice> &participantDevice) override;
	virtual std::shared_ptr<ConferenceParticipantDeviceEvent>
	notifyParticipantDeviceRemoved(time_t creationTime,
	                               const bool isFullState,
	                               const std::shared_ptr<Participant> &participant,
	                               const std::shared_ptr<ParticipantDevice> &participantDevice) override;
	virtual std::shared_ptr<ConferenceParticipantDeviceEvent>
	notifyParticipantDeviceStateChanged(time_t creationTime,
	                                    const bool isFullState,
	                                    const std::shared_ptr<Participant> &participant,
	                                    const std::shared_ptr<ParticipantDevice> &participantDevice) override;

	virtual std::shared_ptr<ConferenceAvailableMediaEvent>
	notifyAvailableMediaChanged(time_t creationTime,
	                            const bool isFullState,
	                            const std::map<ConferenceMediaCapabilities, bool> mediaCapabilities) override;
	virtual std::shared_ptr<ConferenceParticipantDeviceEvent>
	notifyParticipantDeviceMediaCapabilityChanged(time_t creationTime,
	                                              const bool isFullState,
	                                              const std::shared_ptr<Participant> &participant,
	                                              const std::shared_ptr<ParticipantDevice> &participantDevice) override;

	virtual void notifyFullState() override;
	virtual std::shared_ptr<Call> getCall() const override;

protected:
#ifdef HAVE_ADVANCED_IM
	std::shared_ptr<LocalConferenceEventHandler> eventHandler;
#endif

private:
	L_DISABLE_COPY(LocalConference);
};

LINPHONE_END_NAMESPACE

#endif // ifndef _L_LOCAL_CONFERENCE_H_
