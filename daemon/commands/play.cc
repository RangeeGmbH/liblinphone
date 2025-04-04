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

#include "play.h"
#include "call.h"
#include <utility>
using namespace std;
#define VOIDPTR_TO_INT(p) ((int)(intptr_t)(p))

void IncallPlayerStartCommand::onEof(LinphonePlayer *player) {
	pair<int, Daemon *> *callPlayingData = (pair<int, Daemon *> *)linphone_player_get_user_data(player);
	Daemon *app = callPlayingData->second;
	int id = callPlayingData->first;
	app->callPlayingComplete(id);
	delete callPlayingData;
	linphone_player_set_user_data(player, NULL);
}

IncallPlayerStartCommand::IncallPlayerStartCommand()
    : DaemonCommand("incall-player-start",
                    "incall-player-start <filename> [<call_id>]",
                    "Play a WAV audio file or a MKV audio/video file. The played media stream will be sent through \n"
                    "the RTP session of the given call.\n"
                    "<filename> is the file to be played.\n") {
	addExample(make_unique<DaemonCommandExample>("incall-player-start /usr/local/share/sounds/linphone/hello8000.wav 1",
	                                             "Status: Ok\n"));
	addExample(make_unique<DaemonCommandExample>("incall-player-start /usr/local/share/sounds/linphone/hello8000.wav 1",
	                                             "Status: Error\n"
	                                             "Reason: No call with such id."));
	addExample(make_unique<DaemonCommandExample>("incall-player-start /usr/local/share/sounds/linphone/hello8000.wav",
	                                             "Status: Ok\n"));
	addExample(make_unique<DaemonCommandExample>("incall-player-start /usr/local/share/sounds/linphone/hello8000.wav",
	                                             "Status: Error\n"
	                                             "Reason: No active call."));
}

void IncallPlayerStartCommand::exec(Daemon *app, const string &args) {
	LinphoneCall *call = NULL;
	int cid;
	const MSList *elem;
	istringstream ist(args);
	string filename;

	ist >> filename;
	if (ist.eof() && (filename.length() == 0)) {
	    app->sendResponse(Response(COMMANDNAME_INCALL_PLAYER_START, "Missing filename parameter.", Response::Error));
		return;
	}
	if (ist.fail()) {
	    app->sendResponse(Response(COMMANDNAME_INCALL_PLAYER_START, "Incorrect filename parameter.", Response::Error));
		return;
	}

	ist >> cid;
	if (ist.fail()) {
		elem = linphone_core_get_calls(app->getCore());
		if (elem != NULL && elem->next == NULL) {
			call = (LinphoneCall *)elem->data;
		}
	} else {
		call = app->findCall(cid);
		if (call == NULL) {
		    app->sendResponse(Response(COMMANDNAME_INCALL_PLAYER_START, "No call with such id.", Response::Error));
			return;
		}
	}
	if (call == NULL) {
	    app->sendResponse(Response(COMMANDNAME_INCALL_PLAYER_START, "No active call.", Response::Error));
		return;
	}
	LinphonePlayer *p = linphone_call_get_player(call);

	LinphonePlayerCbs *cbs = linphone_factory_create_player_cbs(linphone_factory_get());

	pair<int, Daemon *> *callPlayingData = (pair<int, Daemon *> *)linphone_player_get_user_data(p);
	if (callPlayingData)
		callPlayingData = new pair<int, Daemon *>({VOIDPTR_TO_INT(linphone_call_get_user_data(call)), app});
	linphone_player_set_user_data(p, callPlayingData);
	linphone_player_cbs_set_eof_reached(cbs, onEof);
	linphone_player_add_callbacks(p, cbs);
	linphone_player_open(p, filename.c_str());
	linphone_player_start(p);
	app->sendResponse(Response(COMMANDNAME_INCALL_PLAYER_START, "", Response::Ok));
}

IncallPlayerStopCommand::IncallPlayerStopCommand()
    : DaemonCommand("incall-player-stop", "incall-player-stop [<call_id>]", "Close the opened file.\n") {
	addExample(make_unique<DaemonCommandExample>("incall-player-stop 1", "Status: Error\n"
	                                                                     "Reason: No call with such id."));
	addExample(make_unique<DaemonCommandExample>("incall-player-stop 1", "Status: Ok\n"));
	addExample(make_unique<DaemonCommandExample>("incall-player-stop", "Status: Ok\n"));
	addExample(make_unique<DaemonCommandExample>("incall-player-stop", "Status: Error\n"
	                                                                   "Reason: No active call."));
}

void IncallPlayerStopCommand::exec(Daemon *app, const string &args) {
	LinphoneCall *call = NULL;
	int cid;
	const MSList *elem;
	istringstream ist(args);
	ist >> cid;
	if (ist.fail()) {
		elem = linphone_core_get_calls(app->getCore());
		if (elem != NULL && elem->next == NULL) {
			call = (LinphoneCall *)elem->data;
		}
	} else {
		call = app->findCall(cid);
		if (call == NULL) {
		    app->sendResponse(Response(COMMANDNAME_INCALL_PLAYER_STOP, "No call with such id.", Response::Error));
			return;
		}
	}
	if (call == NULL) {
	    app->sendResponse(Response(COMMANDNAME_INCALL_PLAYER_STOP, "No active call.", Response::Error));
		return;
	}

	LinphonePlayer *p = linphone_call_get_player(call);

	linphone_player_close(p);
	app->sendResponse(Response());
	pair<int, Daemon *> *callPlayingData = (pair<int, Daemon *> *)linphone_player_get_user_data(p);
	if (callPlayingData) delete callPlayingData;
}

IncallPlayerPauseCommand::IncallPlayerPauseCommand()
    : DaemonCommand("incall-player-pause", "incall-player-pause [<call_id>]", "Pause the playing of a file.\n") {
	addExample(make_unique<DaemonCommandExample>("incall-player-pause 1", "Status: Error\n"
	                                                                      "Reason: No call with such id."));
	addExample(make_unique<DaemonCommandExample>("incall-player-pause 1", "Status: Ok\n"));
	addExample(make_unique<DaemonCommandExample>("incall-player-pause", "Status: Ok\n"));
	addExample(make_unique<DaemonCommandExample>("incall-player-pause", "Status: Error\n"
	                                                                    "Reason: No active call."));
}

void IncallPlayerPauseCommand::exec(Daemon *app, const string &args) {
	LinphoneCall *call = NULL;
	int cid;
	const MSList *elem;
	istringstream ist(args);
	ist >> cid;
	if (ist.fail()) {
		elem = linphone_core_get_calls(app->getCore());
		if (elem != NULL && elem->next == NULL) {
			call = (LinphoneCall *)elem->data;
		}
	} else {
		call = app->findCall(cid);
		if (call == NULL) {
		    app->sendResponse(Response(COMMANDNAME_INCALL_PLAYER_PAUSE, "No call with such id.", Response::Error));
			return;
		}
	}
	if (call == NULL) {
	    app->sendResponse(Response(COMMANDNAME_INCALL_PLAYER_PAUSE, "No active call.", Response::Error));
		return;
	}

	LinphonePlayer *p = linphone_call_get_player(call);
	linphone_player_pause(p);
	app->sendResponse(Response(COMMANDNAME_INCALL_PLAYER_PAUSE, "", Response::Ok));
}

IncallPlayerResumeCommand::IncallPlayerResumeCommand()
    : DaemonCommand("incall-player-resume", "incall-player-resume [<call_id>]", "Unpause the playing of a file.\n") {
	addExample(make_unique<DaemonCommandExample>("incall-player-resume 1", "Status: Error\n"
	                                                                       "Reason: No call with such id."));
	addExample(make_unique<DaemonCommandExample>("incall-player-resume 1", "Status: Ok\n"));
	addExample(make_unique<DaemonCommandExample>("incall-player-resume", "Status: Ok\n"));
	addExample(make_unique<DaemonCommandExample>("incall-player-resume", "Status: Error\n"
	                                                                     "Reason: No active call."));
}

void IncallPlayerResumeCommand::exec(Daemon *app, const string &args) {
	LinphoneCall *call = NULL;
	int cid;
	const MSList *elem;
	istringstream ist(args);
	ist >> cid;
	if (ist.fail()) {
		elem = linphone_core_get_calls(app->getCore());
		if (elem != NULL && elem->next == NULL) {
			call = (LinphoneCall *)elem->data;
		}
	} else {
		call = app->findCall(cid);
		if (call == NULL) {
		    app->sendResponse(Response(COMMANDNAME_INCALL_PLAYER_RESUME, "No call with such id.", Response::Error));
			return;
		}
	}
	if (call == NULL) {
	    app->sendResponse(Response(COMMANDNAME_INCALL_PLAYER_RESUME, "No active call.", Response::Error));
		return;
	}

	LinphonePlayer *p = linphone_call_get_player(call);
	linphone_player_start(p);
	app->sendResponse(Response(COMMANDNAME_INCALL_PLAYER_RESUME, "", Response::Ok));
}
