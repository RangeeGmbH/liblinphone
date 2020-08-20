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

#include "jitterbuffer.h"
#include <iostream>

#include <mediastreamer2/msrtp.h>
#include <private.h>

string JitterBufferCommand::getJitterBufferCommandResponseStr(LinphoneCore *lc,bool audio, bool video){
    ostringstream ostr;
    if (audio)
    ostr<<"audio-jitter-buffer-size: "<<linphone_core_get_audio_jittcomp(lc)<<endl;
    if (video)
    ostr<<"video-jitter-buffer-size: "<<linphone_core_get_video_jittcomp(lc)<<endl;
    return ostr.str();
}


JitterBufferCommand::JitterBufferCommand() : DaemonCommand("jitter-buffer",
	"jitter-buffer [audio|video] [size <milliseconds>]",
	"Control jitter buffer parameters.\n"
	"jitter-buffer [audio|video] size <milliseconds>: sets the nominal jitter buffer size in milliseconds. Has no effect in a running call.\n"
	"jitter-buffer [audio|video]: gets the nominal jitter buffer size."
	){
	addExample(new DaemonCommandExample("jitter-buffer","Status: Ok\n\n"
				"audio-jitter-buffer-size: 60\nvideo-jitter-buffer-size: 60\n"));
	addExample(new DaemonCommandExample("jitter-buffer audio","Status: Ok\n\n"
				"audio-jitter-buffer-size: 60\n"));
	addExample(new DaemonCommandExample("jitter-buffer audio size 80","Status: Ok\n\n"
				"audio-jitter-buffer-size: 80\n"));
}


void JitterBufferCommand::exec(Daemon *app, const string& args) {
	istringstream istr(args);
	string arg1;
	istr >> arg1;
	if (istr.fail()){
		app->sendResponse(Response(COMMANDNAME_JITTER_BUFFER, getJitterBufferCommandResponseStr(app->getCore(), true, true), Response::Ok));
		return;
	}
	if (arg1 != "audio" && arg1 != "video") {
		app->sendResponse(Response(COMMANDNAME_JITTER_BUFFER, "Invalid argument.", Response::Error));
		return;
	}
	string arg2;
	istr >> arg2;
	if (istr.fail()) {
		app->sendResponse(Response(COMMANDNAME_JITTER_BUFFER, getJitterBufferCommandResponseStr(app->getCore(), arg1 == "audio", arg1 == "video"), Response::Ok));
		return;
	}
	if (arg2 == "size") {
		int arg3;
		istr >> arg3;
		if (istr.fail()) {
			app->sendResponse(Response(COMMANDNAME_JITTER_BUFFER, "Bad command argument.", Response::Error));
			return;
		}
		if (arg1 == "audio")
			linphone_core_set_audio_jittcomp(app->getCore(), arg3);
		else if (arg1=="video")
			linphone_core_set_video_jittcomp(app->getCore(), arg3);
	}
	app->sendResponse(Response(COMMANDNAME_JITTER_BUFFER, getJitterBufferCommandResponseStr(app->getCore(), arg1 == "audio", arg1 == "video"), Response::Ok));
}

JitterBufferResetCommand::JitterBufferResetCommand() : DaemonCommand("jitter-buffer-reset",
	"jitter-buffer-reset call|stream <id> [audio|video]" ,
	"Reset the RTP jitter buffer for a given call or stream id and stream type."
	){
	addExample(new DaemonCommandExample("jitter-buffer-reset call 3 audio",
		"Status: Ok\n"));
	addExample(new DaemonCommandExample("jitter-buffer-reset stream 12",
		"Status: Ok\n"));
}


void JitterBufferResetCommand::exec(Daemon *app, const string& args) {
	istringstream istr(args);
	string arg1;
	istr >> arg1;
	if (istr.fail()) {
		app->sendResponse(Response(COMMANDNAME_JITTER_BUFFER_RESET, "Missing arguments", Response::Error));
		return;
	}
	if (arg1 != "call" && arg1 != "stream") {
		app->sendResponse(Response(COMMANDNAME_JITTER_BUFFER_RESET, "Invalid command syntax.", Response::Error));
		return;
	}
	int arg2;
	istr >> arg2;
	if (istr.fail()) {
		app->sendResponse(Response(COMMANDNAME_JITTER_BUFFER_RESET, "Missing call or stream id.", Response::Error));
		return;
	}
	MSFilter *rtprecv = NULL;
	if (arg1 == "call") {
		LinphoneCall *call = app->findCall(arg2);
		string streamtype;
		if (call == NULL) {
			app->sendResponse(Response(COMMANDNAME_JITTER_BUFFER_RESET, "No such call id", Response::Error));
			return;
		}
		istr >> streamtype;
		if (streamtype == "video") {
			VideoStream *vstream = reinterpret_cast<VideoStream *>(linphone_call_get_stream(call, LinphoneStreamTypeVideo));
			rtprecv = vstream ? vstream->ms.rtprecv : NULL;
		} else {
			AudioStream *astream = reinterpret_cast<AudioStream *>(linphone_call_get_stream(call, LinphoneStreamTypeAudio));
			rtprecv = astream ? astream->ms.rtprecv : NULL;
		}
	} else {
		AudioStream *stream = app->findAudioStream(arg2);
		if (stream == NULL) {
			app->sendResponse(Response(COMMANDNAME_JITTER_BUFFER_RESET, "No such stream id", Response::Error));
			return;
		}
		rtprecv = stream->ms.rtprecv;
	}
	if (rtprecv == NULL) {
		app->sendResponse(Response(COMMANDNAME_JITTER_BUFFER_RESET, "No such active stream", Response::Error));
		return;
	}
	ms_filter_call_method_noarg(rtprecv, MS_RTP_RECV_RESET_JITTER_BUFFER);
	app->sendResponse(Response(COMMANDNAME_JITTER_BUFFER_RESET, "", Response::Ok));
}


