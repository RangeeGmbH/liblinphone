/*
 * Copyright (c) 2010-2019 Belledonne Communications SARL.
 * Copyright (c) 2019-2020 Rangee GmbH.
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

#include "terminate.h"

using namespace std;

TerminateCommand::TerminateCommand() :
		DaemonCommand("terminate", "terminate [ALL] or [<call_id>]", "Terminate a call or Terminate all calls.") {
	addExample(new DaemonCommandExample("terminate 2",
						"Status: Error\n"
						"Reason: No call with such id."));
	addExample(new DaemonCommandExample("terminate 1",
						"Status: Ok\n"));
	addExample(new DaemonCommandExample("terminate",
						"Status: Ok\n"));
    addExample(new DaemonCommandExample("terminate all",
                                        "Status: Ok\n"));
	addExample(new DaemonCommandExample("terminate",
						"Status: Error\n"
						"Reason: No active call."));
}
void TerminateCommand::exec(Daemon *app, const string& args) {
    string param;
    int cid;
    istringstream ist(args);
    const MSList *elem;
    LinphoneCall *call = NULL;
    ist >> param;
    // terminate
    if (ist.fail()) {
        elem = linphone_core_get_calls(app->getCore());
        if (elem != NULL && elem->next == NULL) {
            call = (LinphoneCall*)elem->data;
        }
        if (call == NULL) {
            app->sendResponse(Response("No active call."));
            return;
        }
        linphone_call_terminate(call);
        app->sendResponse(Response());
        return;
    }
    // terminate ALL
    if(param == "ALL"){
        elem = linphone_core_get_calls(app->getCore());
        if (elem != NULL && elem->next == NULL) {
            call = (LinphoneCall*)elem->data;
        }
        if (call == NULL) {
            app->sendResponse(Response("No active call."));
            return;
        }
        else{
            LinphoneCore *lc = app->getCore();
            linphone_core_terminate_all_calls(lc);
            app->sendResponse(Response());
        }
        return;
    }
    ist.seekg(0);
    ist >> cid;
    // terminate 1
    call = app->findCall(cid);
    if (call == NULL) {
        app->sendResponse(Response("No call with such id."));
        return;
    }
    linphone_call_terminate(call);
    app->sendResponse(Response());
}
