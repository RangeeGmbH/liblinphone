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

#include "audio-codec-set.h"

#include "private.h"

/*hack, until this function comes to linphonecore*/
#define _payload_type_get_number(pt) ((long)(pt)->user_data)
#define _payload_type_set_number(pt, n) (pt)->user_data = (void *)(long)(n)

using namespace std;

AudioCodecSetCommand::AudioCodecSetCommand()
    : DaemonCommand(
          "audio-codec-set",
          "audio-codec-set <payload_type_number>|<mime_type> <property> <value>",
          "Set a property (number, clock_rate, recv_fmtp, send_fmtp, bitrate (in kbps/s)) of a codec. Numbering of "
          "payload type is automatically performed at startup, any change will be lost after restart.\n"
          "<mime_type> is of the form mime/rate/channels, eg. speex/16000/1") {
	addExample(make_unique<DaemonCommandExample>("audio-codec-set 9 number 18", "Status: Ok\n\n"
	                                                                            "Index: 10\n"
	                                                                            "Payload-type-number: 18\n"
	                                                                            "Clock-rate: 8000\n"
	                                                                            "Bitrate: 64000\n"
	                                                                            "Mime: G722\n"
	                                                                            "Channels: 1\n"
	                                                                            "Recv-fmtp: \n"
	                                                                            "Send-fmtp: \n"
	                                                                            "Enabled: false"));
	addExample(make_unique<DaemonCommandExample>("audio-codec-set G722/8000/1 number 9", "Status: Ok\n\n"
	                                                                                     "Index: 10\n"
	                                                                                     "Payload-type-number: 9\n"
	                                                                                     "Clock-rate: 8000\n"
	                                                                                     "Bitrate: 64000\n"
	                                                                                     "Mime: G722\n"
	                                                                                     "Channels: 1\n"
	                                                                                     "Recv-fmtp: \n"
	                                                                                     "Send-fmtp: \n"
	                                                                                     "Enabled: false"));
	addExample(make_unique<DaemonCommandExample>("audio-codec-set 9 clock_rate 16000", "Status: Ok\n\n"
	                                                                                   "Index: 10\n"
	                                                                                   "Payload-type-number: 9\n"
	                                                                                   "Clock-rate: 16000\n"
	                                                                                   "Bitrate: 64000\n"
	                                                                                   "Mime: G722\n"
	                                                                                   "Channels: 1\n"
	                                                                                   "Recv-fmtp: \n"
	                                                                                   "Send-fmtp: \n"
	                                                                                   "Enabled: false"));
}

static LinphonePayloadType *findPayload(LinphoneCore *lc, int payload_type, int *index) {
	bctbx_list_t *payloadTypes = linphone_core_get_audio_payload_types(lc);
	LinphonePayloadType *ret = nullptr;
	if (index) *index = 0;
	for (const bctbx_list_t *node = payloadTypes; node != NULL; node = bctbx_list_next(node)) {
		LinphonePayloadType *payload = static_cast<LinphonePayloadType *>(node->data);
		if (index) (*index)++;
		if (payload_type == linphone_payload_type_get_number(payload)) {
			ret = linphone_payload_type_ref(payload);
			break;
		}
	}
	bctbx_list_free_with_data(payloadTypes, (bctbx_list_free_func)linphone_payload_type_unref);
	return ret;
}

void AudioCodecSetCommand::exec(Daemon *app, const string &args) {
	istringstream ist(args);

	if (ist.peek() == EOF) {
	    app->sendResponse(Response(COMMANDNAME_AUDIO_CODEC_SET, "Missing parameters.", Response::Error));
		return;
	}

	string mime_type;
	ist >> mime_type;
	PayloadTypeParser parser(app->getCore(), mime_type);
	if (!parser.successful()) {
	    app->sendResponse(Response(COMMANDNAME_AUDIO_CODEC_SET, "Incorrect mime type format.", Response::Error));
		return;
	}
	string param;
	string value;
	ist >> param;
	if (ist.fail()) {
	    app->sendResponse(Response(COMMANDNAME_AUDIO_CODEC_SET, "Missing/Incorrect parameter(s).", Response::Error));
		return;
	}
	ist >> value;
	if (value.length() > 255) value.resize(255);

	LinphonePayloadType *payload = nullptr;
	if (parser.getPayloadType()) {
		payload =
		    linphone_core_get_payload_type(app->getCore(), linphone_payload_type_get_mime_type(parser.getPayloadType()),
		                                   linphone_payload_type_get_clock_rate(parser.getPayloadType()),
		                                   linphone_payload_type_get_channels(parser.getPayloadType()));
	}
	if (payload) {
		bool handled = false;
		if (param.compare("clock_rate") == 0) {
			if (value.length() > 0) {
				// setting the clock rate is not supported by liblinphone. What's the purpose of doing this ?'
				// linphone_payload_type_set_clock_rate(payload, atoi(value.c_str()));
				handled = false;
			}
		} else if (param.compare("recv_fmtp") == 0) {
			linphone_payload_type_set_recv_fmtp(payload, value.c_str());
			handled = true;
		} else if (param.compare("send_fmtp") == 0) {
			linphone_payload_type_set_send_fmtp(payload, value.c_str());
			handled = true;
		} else if (param.compare("number") == 0) {
			if (value.length() > 0) {
				int idx = atoi(value.c_str());
				LinphonePayloadType *conflict = NULL;
				if (idx != -1) {
					conflict = findPayload(app->getCore(), atoi(value.c_str()), NULL);
				}
				if (conflict) {
				    app->sendResponse(Response(COMMANDNAME_AUDIO_CODEC_SET, "New payload type number is already used.", Response::Error));
					linphone_payload_type_unref(conflict);
				} else {
					linphone_payload_type_set_number(payload, idx);
					app->sendResponse(Response(COMMANDNAME_AUDIO_CODEC_SET, PayloadTypeResponse(app->getCore(), payload, parser.getPosition()).getBody(),Response::Ok));
				}
				linphone_payload_type_unref(payload);
				return;
			}
		} else if (param.compare("bitrate") == 0) {
			linphone_payload_type_set_normal_bitrate(payload, atoi(value.c_str()));
			handled = true;
		}
		if (handled) {
		    app->sendResponse(Response(COMMANDNAME_AUDIO_CODEC_SET, PayloadTypeResponse(app->getCore(), payload, parser.getPosition()).getBody(), Response::Ok));
		} else {
		    app->sendResponse(Response(COMMANDNAME_AUDIO_CODEC_SET, "Invalid codec parameter.", Response::Error));
		}
		linphone_payload_type_unref(payload);
		return;
	}
	app->sendResponse(Response(COMMANDNAME_AUDIO_CODEC_SET, "Audio codec not found.", Response::Error));
}
