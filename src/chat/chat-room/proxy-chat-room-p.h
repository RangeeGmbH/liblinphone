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

#ifndef _L_PROXY_CHAT_ROOM_P_H_
#define _L_PROXY_CHAT_ROOM_P_H_

#include "abstract-chat-room-p.h"
#include "proxy-chat-room.h"

// =============================================================================

LINPHONE_BEGIN_NAMESPACE

class ProxyChatRoomPrivate : public AbstractChatRoomPrivate {
public:
	inline void setCreationTime(time_t creationTime) override {
		chatRoom->getPrivate()->setCreationTime(creationTime);
	}

	inline void setLastUpdateTime(time_t lastUpdateTime) override {
		chatRoom->getPrivate()->setLastUpdateTime(lastUpdateTime);
	}

	inline void sendChatMessage(const std::shared_ptr<ChatMessage> &chatMessage) override {
		chatRoom->getPrivate()->sendChatMessage(chatMessage);
	}

	inline void onChatMessageSent(const std::shared_ptr<ChatMessage> &chatMessage) override {
		chatRoom->getPrivate()->onChatMessageSent(chatMessage);
	}

	inline void addEvent(const std::shared_ptr<EventLog> &eventLog) override {
		chatRoom->getPrivate()->addEvent(eventLog);
	}

	inline void addTransientEvent(const std::shared_ptr<EventLog> &eventLog) override {
		chatRoom->getPrivate()->addTransientEvent(eventLog);
	}

	inline void removeTransientEvent(const std::shared_ptr<EventLog> &eventLog) override {
		chatRoom->getPrivate()->removeTransientEvent(eventLog);
	}

	inline void addTransientChatMessage(const std::shared_ptr<ChatMessage> &message) override {
		chatRoom->getPrivate()->addTransientChatMessage(message);
	}

	inline void removeTransientChatMessage(const std::shared_ptr<ChatMessage> &message) override {
		chatRoom->getPrivate()->removeTransientChatMessage(message);
	}

	inline std::list<std::shared_ptr<ChatMessage>> getTransientChatMessages() override {
		return chatRoom->getPrivate()->getTransientChatMessages();
	}

	inline void setIsEmpty(const bool empty) override {
		chatRoom->getPrivate()->setIsEmpty(empty);
	}

	inline void sendDeliveryNotifications(const std::shared_ptr<ChatMessage> &chatMessage) override {
		chatRoom->getPrivate()->sendDeliveryNotifications(chatMessage);
	}

	inline void notifyChatMessageReceived(const std::shared_ptr<ChatMessage> &chatMessage) override {
		chatRoom->getPrivate()->notifyChatMessageReceived(chatMessage);
	}

	inline void notifyAggregatedChatMessages() override {
		chatRoom->getPrivate()->notifyAggregatedChatMessages();
	}

	inline void notifyUndecryptableChatMessageReceived(const std::shared_ptr<ChatMessage> &chatMessage) override {
		chatRoom->getPrivate()->notifyUndecryptableChatMessageReceived(chatMessage);
	}

	inline LinphoneReason onSipMessageReceived(SalOp *op, const SalMessage *message) override {
		return chatRoom->getPrivate()->onSipMessageReceived(op, message);
	}

	inline void onChatMessageReceived(const std::shared_ptr<ChatMessage> &chatMessage) override {
		chatRoom->getPrivate()->onChatMessageReceived(chatMessage);
	}

	inline void realtimeTextReceived(uint32_t character, const std::shared_ptr<Call> &call) override {
		chatRoom->getPrivate()->realtimeTextReceived(character, call);
	}

	inline void setCallId(const std::string &value) override {
		chatRoom->getPrivate()->setCallId(value);
	};

	inline virtual bool isSubscriptionUnderWay() const override {
		return chatRoom->getPrivate()->isSubscriptionUnderWay();
	}

	inline virtual void addPendingMessage(const std::shared_ptr<ChatMessage> &chatMessage) override {
		chatRoom->getPrivate()->addPendingMessage(chatMessage);
	}

	void setupProxy();
	void teardownProxy();

	std::shared_ptr<AbstractChatRoom> chatRoom;

	L_DECLARE_PUBLIC(ProxyChatRoom);
};

LINPHONE_END_NAMESPACE

#endif // ifndef _L_PROXY_CHAT_ROOM_P_H_
