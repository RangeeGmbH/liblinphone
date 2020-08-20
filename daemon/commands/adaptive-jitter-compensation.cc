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

#include "adaptive-jitter-compensation.h"

string AdaptiveBufferCompensationCommand::AdaptiveBufferCompensationResponse(LinphoneCore *core, StreamType type) {
	bool enabled = false;
	ostringstream ost;
	switch (type) {
		case AudioStream:
			enabled = linphone_core_audio_adaptive_jittcomp_enabled(core) ? true : false;
			outputAdaptiveBufferCompensation(core, ost, "Audio", enabled);
			break;
		case VideoStream:
			enabled = linphone_core_video_adaptive_jittcomp_enabled(core) ? true : false;
			outputAdaptiveBufferCompensation(core, ost, "Video", enabled);
			break;
		case AllStreams:
			enabled = linphone_core_audio_adaptive_jittcomp_enabled(core) ? true : false;
			outputAdaptiveBufferCompensation(core, ost, "Audio", enabled);
			enabled = linphone_core_video_adaptive_jittcomp_enabled(core) ? true : false;
			outputAdaptiveBufferCompensation(core, ost, "Video", enabled);
			break;
	}
    return ost.str();
}

void AdaptiveBufferCompensationCommand::outputAdaptiveBufferCompensation(LinphoneCore *core, ostringstream &ost, const string& header, bool_t value) {
	ost << header << ": ";
	if (value) {
		ost << "enabled\n";
	} else {
		ost << "disabled\n";
	}
}

AdaptiveBufferCompensationCommand::AdaptiveBufferCompensationCommand() :
		DaemonCommand("adaptive-jitter-compensation", "adaptive-jitter-compensation [audio|video] [enable|disable]",
			"Enable or disable adaptive buffer compensation respectively with the 'enable' and 'disable' parameters for the specified stream, "
			"return the status of the use of adaptive buffer compensation without parameter.") {
	addExample(new DaemonCommandExample("adaptive-jitter-compensation audio",
						"Status: Ok\n\n"
						"Audio: enabled"));
	addExample(new DaemonCommandExample("adaptive-jitter-compensation video",
						"Status: Ok\n\n"
						"Video: disabled"));
	addExample(new DaemonCommandExample("adaptive-jitter-compensation",
						"Status: Ok\n\n"
						"Audio: enabled\n"
						"Video: disabled"));
	addExample(new DaemonCommandExample("adaptive-jitter-compensation video enable",
						"Status: Ok\n\n"
						"Video: enabled"));
}

void AdaptiveBufferCompensationCommand::exec(Daemon *app, const string& args) {
	string stream;
	string state;
	istringstream ist(args);

	ist >> stream;
	if (ist.fail()) {
		app->sendResponse(Response(COMMANDNAME_ADAPTIVE_JITTER_COMPENSATION, AdaptiveBufferCompensationResponse(app->getCore(), AdaptiveBufferCompensationCommand::AllStreams), Response::Ok));
		return;
	}

	ist >> state;
	if (ist.fail()) {
		if (stream.compare("audio") == 0) {
			app->sendResponse(Response(COMMANDNAME_ADAPTIVE_JITTER_COMPENSATION, AdaptiveBufferCompensationResponse(app->getCore(), AdaptiveBufferCompensationCommand::AudioStream), Response::Ok));
		} else if (stream.compare("video") == 0) {
			app->sendResponse(Response(COMMANDNAME_ADAPTIVE_JITTER_COMPENSATION, AdaptiveBufferCompensationResponse(app->getCore(), AdaptiveBufferCompensationCommand::VideoStream), Response::Ok));
		} else {
			app->sendResponse(Response(COMMANDNAME_ADAPTIVE_JITTER_COMPENSATION, "Incorrect stream parameter.", Response::Error));
		}
		return;
	}

    AdaptiveBufferCompensationCommand::StreamType type;
	bool enabled;
	if (stream.compare("audio") == 0) {
		type = AdaptiveBufferCompensationCommand::AudioStream;
	} else if (stream.compare("video") == 0) {
		type = AdaptiveBufferCompensationCommand::VideoStream;
	} else {
		app->sendResponse(Response(COMMANDNAME_ADAPTIVE_JITTER_COMPENSATION, "Incorrect stream parameter.", Response::Error));
		return;
	}
	if (state.compare("enable") == 0) {
		enabled = TRUE;
	} else if (state.compare("disable") == 0) {
		enabled = FALSE;
	} else {
		app->sendResponse(Response(COMMANDNAME_ADAPTIVE_JITTER_COMPENSATION, "Incorrect parameter.", Response::Error));
		return;
	}
	if (type == AdaptiveBufferCompensationCommand::AudioStream) {
		linphone_core_enable_audio_adaptive_jittcomp(app->getCore(), enabled);
	} else if (type == AdaptiveBufferCompensationCommand::VideoStream) {
		linphone_core_enable_video_adaptive_jittcomp(app->getCore(), enabled);
	}
	app->sendResponse(Response(COMMANDNAME_ADAPTIVE_JITTER_COMPENSATION, AdaptiveBufferCompensationResponse(app->getCore(), AdaptiveBufferCompensationCommand::AllStreams), Response::Ok));
}
