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

#include "terminate.h"

using namespace std;

TerminateCommand::TerminateCommand() :
DaemonCommand("terminate", "terminate [ALL] or [<call_id>]", "Terminate a call or Terminate all calls.") {
    addExample(make_unique<DaemonCommandExample>("terminate 2",
                                        "Status: Error\n"
                                        "Reason: No call with such id."));
    addExample(make_unique<DaemonCommandExample>("terminate 1",
                                        "Status: Ok\n"));
    addExample(make_unique<DaemonCommandExample>("terminate",
                                        "Status: Ok\n"));
    addExample(make_unique<DaemonCommandExample>("terminate all",
                                        "Status: Ok\n"));
    addExample(make_unique<DaemonCommandExample>("terminate",
                                        "Status: Error\n"
                                        "Reason: No active call."));
}
void TerminateCommand::exec(Daemon *app, const string& args) {
    string param;
    int cid;
    istringstream ist(args);
    const MSList *elem;
    LinphoneCall *call = NULL;
    ostringstream ost;
    ist >> param;
    // terminate
    if (ist.fail()) {
        elem = linphone_core_get_calls(app->getCore());
        if (elem != NULL && elem->next == NULL) {
            call = (LinphoneCall*)elem->data;
        }
        if (call == NULL) {
            ost << "\"No active call.\"";
            app->sendResponse(Response(COMMANDNAME_TERMINATE, ost.str(), Response::Error));
            return;
        }
        linphone_call_terminate(call);
        ost << "{ \"CallId\": " << "\"" << app->updateCallId(call) << "\"" << " }";
        app->sendResponse(Response(COMMANDNAME_TERMINATE, ost.str(), Response::Ok));
        return;
    }
    // terminate ALL
    if(param == "ALL"){
        elem = linphone_core_get_calls(app->getCore());
        if (elem != NULL) {
            call = (LinphoneCall*)elem->data;
        }
        if (call == NULL) {
            ost << "No active call.";
            app->sendResponse(Response(COMMANDNAME_TERMINATE, ost.str(), Response::Error));
            return;
        }
        else{
            string callString;
            callString += "{ \"isAll\": true, \"calls\": [ ";
            for (int index = 0; index < ms_list_size(elem); index++) {
                LinphoneCall* lCall = (LinphoneCall*) bctbx_list_nth_data(elem,index);
                callString += app->getJsonForCall(lCall);

                if(index < ms_list_size(elem)-1) {
                    callString += ",";
                }
            }
            callString += " ] }";
            LinphoneCore *lc = app->getCore();
            linphone_core_terminate_all_calls(lc);
            app->sendResponse(Response(COMMANDNAME_TERMINATE, callString, Response::Ok));
        }
        return;
    }
    ist.seekg(0);
    ist >> cid;
    // terminate 1
    call = app->findCall(cid);
    if (call == NULL) {
        ost << "No call with such id. CallId: " << cid;
        app->sendResponse(Response(COMMANDNAME_TERMINATE, ost.str(), Response::Error));
        return;
    }
    linphone_call_terminate(call);
}
