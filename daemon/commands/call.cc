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

#include "call.h"

using namespace std;

CallCommand::CallCommand() : DaemonCommand("call", "call <sip_address> [--early-media]", "Place a call.") {
	addExample(make_unique<DaemonCommandExample>("call daemon-test@sip.linphone.org", "Status: Ok\n\n"
	                                                                                  "Id: 1"));
	addExample(make_unique<DaemonCommandExample>("call daemon-test@sip.linphone.org --early-media", "Status: Ok\n\n"
	                                                                                                "Early media: Ok\n"
	                                                                                                "Id: 1"));
	addExample(make_unique<DaemonCommandExample>("call daemon-test@sip.linphone.org", "Status: Error\n"
	                                                                                  "Reason: Call creation failed."));
}

void CallCommand::exec(Daemon *app, const string &args) {
	string address;
	string early_media;
	LinphoneCall *call;
	Response resp;
	ostringstream ostr;
	bool bearly_media = false;

	istringstream ist(args);
	ist >> address;
	if (ist.fail()) {
	    ostr << "Missing address parameter.";
	    app->sendResponse(Response(COMMANDNAME_CALL, ostr.str(), Response::Error));
		return;
	}
	ist >> early_media;
	if (!ist.fail()) {
		LinphoneCallParams *cp = linphone_core_create_call_params(app->getCore(), NULL);
		if (early_media.compare("--early-media") == 0) {
			linphone_call_params_enable_early_media_sending(cp, TRUE);
			bearly_media = true;
		}
		call = linphone_core_invite_with_params(app->getCore(), address.c_str(), cp);
	} else {
		call = linphone_core_invite(app->getCore(), address.c_str());
	}

	if (call == NULL) {
	    ostr << "Call creation failed.";
	    app->sendResponse(Response(COMMANDNAME_CALL, ostr.str(), Response::Error));
		return;
	}

	if(bearly_media) {
	    ostr << "{ \"call\": { \"id\": " << app->updateCallId(call) << ", \"Early media\": \"Ok\" } }";
	}
	else {
	    std::ostringstream ost;
	    ostr << "{ \"call\": { \"id\": " << app->updateCallId(call) << " } }";
	}

	app->sendResponse(Response(COMMANDNAME_CALL, ostr.str(), Response::Ok));
}
