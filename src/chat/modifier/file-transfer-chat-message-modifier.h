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

#ifndef _L_FILE_TRANSFER_CHAT_MESSAGE_MODIFIER_H_
#define _L_FILE_TRANSFER_CHAT_MESSAGE_MODIFIER_H_

#include <belle-sip/belle-sip.h>

#include "chat-message-modifier.h"
#include "utils/background-task.h"

// =============================================================================

LINPHONE_BEGIN_NAMESPACE

class ChatRoom;
class Core;
class FileContent;
class FileTransferContent;

class FileTransferChatMessageModifier : public ChatMessageModifier {
public:
	FileTransferChatMessageModifier(belle_http_provider_t *prov);
	~FileTransferChatMessageModifier();

	Result encode(const std::shared_ptr<ChatMessage> &message, int &errorCode) override;
	Result decode(const std::shared_ptr<ChatMessage> &message, int &errorCode) override;

	belle_http_request_t *getHttpRequest() const;
	void setHttpRequest(belle_http_request_t *request);

	int
	onSendBody(belle_sip_user_body_handler_t *bh, belle_sip_message_t *m, size_t offset, uint8_t *buffer, size_t *size);
	void onSendEnd(belle_sip_user_body_handler_t *bh);
	void fileUploadBackgroundTaskEnded();
	void fileTransferOnProgress(belle_sip_body_handler_t *bh, belle_sip_message_t *m, size_t offset, size_t total);
	void processResponseFromPostFile(const belle_http_response_event_t *event);
	void processIoErrorUpload(const belle_sip_io_error_event_t *event);
	void processAuthRequestedUpload(belle_sip_auth_event *event);

	void
	onRecvBody(belle_sip_user_body_handler_t *bh, belle_sip_message_t *m, size_t offset, uint8_t *buffer, size_t size);
	void onRecvEnd(belle_sip_user_body_handler_t *bh);
	void processResponseHeadersFromGetFile(const belle_http_response_event_t *event);
	void processAuthRequestedDownload(belle_sip_auth_event *event);
	void processIoErrorDownload(const belle_sip_io_error_event_t *event);
	void processResponseFromGetFile(const belle_http_response_event_t *event);

	bool downloadFile(const std::shared_ptr<ChatMessage> &message, FileTransferContent *fileTransferContent);
	void cancelFileTransfer();
	bool isFileTransferInProgressAndValid() const;
	std::string createFakeFileTransferFromUrl(const std::string &url);
	void fileUploadEndBackgroundTask();

	void parseFileTransferXmlIntoContent(const char *xml, FileTransferContent *fileTransferContent) const;
	std::string dumpFileTransferContentAsXmlString(const FileTransferContent *parsedXmlFileTransferContent,
	                                               const unsigned char *contentKey,
	                                               size_t contentKeySize,
	                                               const unsigned char *contentAuthTag,
	                                               size_t contentAuthTagSize,
	                                               const std::string &realFileName) const;

private:
	// Body handler is optional, but if set this method takes owneship of it, even in error cases.
	int uploadFile(belle_sip_body_handler_t *bh);
	// Body handler is optional, but if set this method takes owneship of it, even in error cases.
	int startHttpTransfer(const std::string &url,
	                      const std::string &action,
	                      belle_sip_body_handler_t *bh,
	                      belle_http_request_listener_callbacks_t *cbs);
	void fileUploadBeginBackgroundTask();

	void onDownloadFailed();
	void releaseHttpRequest();
	belle_sip_body_handler_t *prepare_upload_body_handler(std::shared_ptr<ChatMessage> message);

	std::string escapeFileName(const std::string &fileName) const;
	std::string unEscapeFileName(const std::string &fileName) const;

	std::weak_ptr<ChatMessage> chatMessage;
	FileContent *currentFileContentToTransfer = nullptr;
	FileTransferContent *currentFileTransferContent = nullptr;

	belle_http_request_t *httpRequest = nullptr;
	belle_http_request_listener_t *httpListener = nullptr;
	belle_http_provider_t *provider = nullptr;

	size_t lastNotifiedPercentage = 0;

	BackgroundTask bgTask;
};

LINPHONE_END_NAMESPACE

#endif // ifndef _L_FILE_TRANSFER_CHAT_MESSAGE_MODIFIER_H_
