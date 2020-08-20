/*
 * Copyright (c) 2010-2019 Belledonne Communications SARL.
 *
 * This file is part of Liblinphone.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "audio-codec-toggle.h"
#include "audio-codec-get.h"

using namespace std;

AudioCodecToggleCommand::AudioCodecToggleCommand(const char *name, const char *proto, const char *help, bool enable) :
		DaemonCommand(name, proto, help), mEnable(enable), mName(name) {
}

void AudioCodecToggleCommand::exec(Daemon *app, const string& args) {
	istringstream ist(args);
	const char * audio_codec_enable = "audio-codec-enable";
    const char * audio_codec_disable = "audio-codec-disable";
	if(mName == audio_codec_enable) {
        mName = audio_codec_enable;
	}
    if(mName == audio_codec_disable) {
        mName = audio_codec_disable;
    }

	if (ist.peek() == EOF) {
		app->sendResponse(Response(mName, "Missing parameter.",Response::Error));
	} else {
		string mime_type;
		PayloadType *pt = NULL;
		ist >> mime_type;
		PayloadTypeParser parser(app->getCore(), mime_type, true);
		if (!parser.successful()) {
			app->sendResponse(Response(mName, "Incorrect mime type format.", Response::Error));
			return;
		}
		if (!parser.all()) pt = parser.getPayloadType();

		int index = 0;
		for (const bctbx_list_t *node = linphone_core_get_audio_codecs(app->getCore()); node != NULL; node = bctbx_list_next(node)) {
			PayloadType *payload = reinterpret_cast<PayloadType*>(node->data);
			if (parser.all()) {
				linphone_core_enable_payload_type(app->getCore(), payload, mEnable);
			} else {
				if (pt == payload) {
					linphone_core_enable_payload_type(app->getCore(), payload, mEnable);
                    app->sendResponse(Response(mName, PayloadTypeResponse(app->getCore(), payload, index).getBody(), Response::Ok));
					return;
				}
			}
			++index;
		}
		if (parser.all()) {
			AudioCodecGetCommand getCommand;
			getCommand.exec(app, "");
		} else {
			app->sendResponse(Response(mName, "Audio codec not found.", Response::Error));
		}
	}
}

AudioCodecEnableCommand::AudioCodecEnableCommand() :
		AudioCodecToggleCommand("audio-codec-enable", "audio-codec-enable <payload_type_number>|<mime_type>|ALL",
					"Enable an audio codec.\n"
					"<mime_type> is of the form mime/rate/channels, eg. speex/16000/1", true) {
	addExample(new DaemonCommandExample("audio-codec-enable G722/8000/1",
						"Status: Ok\n\n"
						"Index: 9\n"
						"Payload-type-number: 9\n"
						"Clock-rate: 8000\n"
						"Bitrate: 64000\n"
						"Mime: G722\n"
						"Channels: 1\n"
						"Recv-fmtp: \n"
						"Send-fmtp: \n"
						"Enabled: true"));
	addExample(new DaemonCommandExample("audio-codec-enable 9",
						"Status: Ok\n\n"
						"Index: 9\n"
						"Payload-type-number: 9\n"
						"Clock-rate: 8000\n"
						"Bitrate: 64000\n"
						"Mime: G722\n"
						"Channels: 1\n"
						"Recv-fmtp: \n"
						"Send-fmtp: \n"
						"Enabled: true"));
}

AudioCodecDisableCommand::AudioCodecDisableCommand() :
		AudioCodecToggleCommand("audio-codec-disable", "audio-codec-disable <payload_type_number>|<mime_type>|ALL",
					"Disable an audio codec.\n"
					"<mime_type> is of the form mime/rate/channels, eg. speex/16000/1", false) {
	addExample(new DaemonCommandExample("audio-codec-disable G722/8000/1",
						"Status: Ok\n\n"
						"Index: 9\n"
						"Payload-type-number: 9\n"
						"Clock-rate: 8000\n"
						"Bitrate: 64000\n"
						"Mime: G722\n"
						"Channels: 1\n"
						"Recv-fmtp: \n"
						"Send-fmtp: \n"
						"Enabled: false"));
	addExample(new DaemonCommandExample("audio-codec-disable 9",
						"Status: Ok\n\n"
						"Index: 9\n"
						"Payload-type-number: 9\n"
						"Clock-rate: 8000\n"
						"Bitrate: 64000\n"
						"Mime: G722\n"
						"Channels: 1\n"
						"Recv-fmtp: \n"
						"Send-fmtp: \n"
						"Enabled: false"));
}
