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

#include "call-resume.h"

using namespace std;

CallResumeCommand::CallResumeCommand():
	DaemonCommand("call-resume", "call-resume [<call_id>]", "Pause a call (pause current if no id is specified).")
{
	addExample(new DaemonCommandExample("call-resume 1",
										"Status: Ok\n\n"
										"Call was resumed"));

	addExample(new DaemonCommandExample("call-resume 2",
										"Status: Error\n"
										"Reason: No call with such id."));

	addExample(new DaemonCommandExample("call-resume",
										"Status: Error\n"
										"Reason: No current call available."));
}

void CallResumeCommand::exec(Daemon* app, const string& args)
{
	LinphoneCore *lc = app->getCore();
	int cid;
	LinphoneCall *call = NULL;
	bool current = false;
	istringstream ist(args);
	ostringstream ost;
	ist >> cid;
	if (ist.fail()) {
		call = linphone_core_get_current_call(lc);
		current = true;
		if (call == NULL) {
		    ost << "\"No current call available.\"";
		    app->sendResponse(Response(COMMANDNAME_CALL_RESUME, ost.str(), Response::Error));
			return;
		}
	} else {
		call = app->findCall(cid);
		if (call == NULL) {
		    ost << "\"No call with such id.\"";
		    app->sendResponse(Response(COMMANDNAME_CALL_RESUME, ost.str(), Response::Error));
			return;
		}
	}

	if (linphone_call_resume(call) == 0) {
	    string currentStr = current ? "Current call was resumed" : "Call was resumed";
        ost << "\"" << currentStr << "\"";

	    app->sendResponse(Response(COMMANDNAME_CALL_RESUME, ost.str(), Response::Ok));
	} else {
	    ost << "\"Error pausing call\"";
	    app->sendResponse(Response(COMMANDNAME_CALL_RESUME, ost.str(), Response::Error));
	}
}
