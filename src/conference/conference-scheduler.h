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

#ifndef _L_CONFERENCE_SCHEDULER_H_
#define _L_CONFERENCE_SCHEDULER_H_

#include "c-wrapper/c-wrapper.h"
#include "core/core-accessor.h"
#include "linphone/api/c-types.h"
#include "linphone/types.h"
#include <belle-sip/object++.hh>
#include <chat/chat-message/chat-message-listener.h>
#include <chat/chat-room/chat-room-params.h>
#include <conference/conference-info.h>
#include <conference/session/call-session-listener.h>
#include <conference/session/call-session.h>

// =============================================================================

LINPHONE_BEGIN_NAMESPACE

class ConferenceSchedulerCbs;

class LINPHONE_PUBLIC ConferenceScheduler
    : public bellesip::HybridObject<LinphoneConferenceScheduler, ConferenceScheduler>,
      public CoreAccessor,
      public UserDataAccessor,
      public CallbacksHolder<ConferenceSchedulerCbs>,
      public CallSessionListener,
      public ChatMessageListener {
public:
	enum class State {
		Idle = LinphoneConferenceSchedulerStateIdle,
		Error = LinphoneConferenceSchedulerStateError,
		AllocationPending = LinphoneConferenceSchedulerStateAllocationPending,
		Ready = LinphoneConferenceSchedulerStateReady,
		Updating = LinphoneConferenceSchedulerStateUpdating
	};

	ConferenceScheduler(const std::shared_ptr<Core> &core);
	virtual ~ConferenceScheduler();

	void onCallSessionSetTerminated(const std::shared_ptr<CallSession> &session) override;
	void onCallSessionStateChanged(const std::shared_ptr<CallSession> &session,
	                               CallSession::State state,
	                               const std::string &message) override;
	void onChatMessageStateChanged(const std::shared_ptr<ChatMessage> &message, ChatMessage::State state) override;

	State getState() const;

	const std::shared_ptr<ConferenceInfo> getInfo() const;
	void cancelConference(const std::shared_ptr<ConferenceInfo> &info);
	void setInfo(const std::shared_ptr<ConferenceInfo> &info);

	void setConferenceAddress(const std::shared_ptr<Address> &conferenceAddress);

	void sendInvitations(std::shared_ptr<ChatRoomParams> chatRoomParams);

	const std::shared_ptr<Account> &getAccount() const;
	void setAccount(std::shared_ptr<Account> account);

private:
	void setState(State newState);
	std::string stateToString(State state);

	std::shared_ptr<ChatMessage> createInvitationChatMessage(std::shared_ptr<AbstractChatRoom> chatRoom,
	                                                         const std::shared_ptr<Address> participant,
	                                                         bool cancel);
	void fillCancelList(const ConferenceInfo::participant_list_t &oldList,
	                    const ConferenceInfo::participant_list_t &newList);

	ConferenceScheduler::State mState;
	std::shared_ptr<ConferenceInfo> mConferenceInfo = nullptr;
	std::shared_ptr<CallSession> mSession = nullptr;
	std::shared_ptr<Account> mAccount = nullptr;

	unsigned long mInvitationsSent = 0;
	std::list<std::shared_ptr<Address>> mInvitationsToSend;
	std::map<std::shared_ptr<Address>, int> mCancelToSend;
	std::list<std::shared_ptr<Address>> mInvitationsInError;
};

class ConferenceSchedulerCbs : public bellesip::HybridObject<LinphoneConferenceSchedulerCbs, ConferenceSchedulerCbs>,
                               public Callbacks {
public:
	LinphoneConferenceSchedulerCbsStateChangedCb getStateChanged() const;
	void setStateChanged(LinphoneConferenceSchedulerCbsStateChangedCb cb);
	LinphoneConferenceSchedulerCbsInvitationsSentCb getInvitationsSent() const;
	void setInvitationsSent(LinphoneConferenceSchedulerCbsInvitationsSentCb cb);

private:
	LinphoneConferenceSchedulerCbsStateChangedCb mStateChangedCb = nullptr;
	LinphoneConferenceSchedulerCbsInvitationsSentCb mInvitationsSent = nullptr;
};

std::ostream &operator<<(std::ostream &lhs, ConferenceScheduler::State s);

LINPHONE_END_NAMESPACE

#endif // ifndef _L_CONFERENCE_SCHEDULER_H_
