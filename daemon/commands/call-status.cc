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

#include "call-status.h"

using namespace std;

CallStatusCommand::CallStatusCommand() :
        DaemonCommand("call-status", "call-status [ALL] or [<call_id>]", "Return status of a call, or all calls") {
	addExample(new DaemonCommandExample("call-status 1",
						"Status: Ok\n\n"
						"State: LinphoneCallStreamsRunning\n"
						"From: <sip:daemon-test@sip.linphone.org>\n"
						"Direction: out\n"
						"Duration: 6"));
	addExample(new DaemonCommandExample("call-status 2",
						"Status: Error\n"
						"Reason: No call with such id."));
	addExample(new DaemonCommandExample("call-status",
						"Status: Error\n"
						"Reason: No current call available."));
}
void CallStatusCommand::exec(Daemon *app, const string& args) {
	LinphoneCore *lc = app->getCore();
	int cid;
	const MSList *elem;
	string param;
	LinphoneCall *call = NULL;
	istringstream ist(args);
	istringstream iparam(args);
	ist >> cid;
	iparam >> param;
	string ost;
	string callString;

	if(param != "ALL") {
	    // call-status 1
	    if (ist.fail()) {
	        call = linphone_core_get_current_call(lc);
	        if (call == NULL) {
	            string_format(ost, "\"No current call available.\"");
	            app->sendResponse(Response(COMMANDNAME_CALL_STATUS, ost, Response::Error));
	            return;
	        }
	    } else {
	        call = app->findCall(cid);
	        if (call == NULL) {
	            string_format(ost, "\"No call with such id.\"");
	            app->sendResponse(Response(COMMANDNAME_CALL_STATUS, ost, Response::Error));
	            return;
	        }
	    }
	    callString += "{ \"isALL\": false, \"calls\": [ ";
	    callString += app->getJsonForCall(call);
	    callString += " ]";

	    app->sendResponse(Response(COMMANDNAME_CALL_STATUS, callString, Response::Ok));
	}
	if(param == "ALL") {
	    //call-status ALL
	    elem = linphone_core_get_calls(app->getCore());
	    if (elem != NULL) {
	        call = (LinphoneCall*)elem->data;
	    }
	    if (call == NULL) {
	        app->sendResponse(Response(COMMANDNAME_CALL_STATUS, "No active call.", Response::Error));
	        return;
	    }
	    else{
	        callString += "{ \"isALL\": true, \"calls\": [ ";
                for (int index = 0; index < ms_list_size(elem); index++) {
                    LinphoneCall* lCall = (LinphoneCall*) bctbx_list_nth_data(elem,index);
                    callString += app->getJsonForCall(lCall);

                    if(index < ms_list_size(elem)-1) {
                        callString += ",";
                    }
                }
                callString += " ] }";
                app->sendResponse(Response(COMMANDNAME_CALL_STATUS, callString, Response::Ok));
	        }
	    return;
	}
}
