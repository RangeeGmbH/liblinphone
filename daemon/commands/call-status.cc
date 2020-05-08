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
    addExample(new DaemonCommandExample("call-status ALL",
                                        "Status: Ok\n"));
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

    if(param != "ALL") {
        // call-status 1

        if (ist.fail()) {
            call = linphone_core_get_current_call(lc);
            if (call == NULL) {
                app->sendResponse(Response("No current call available.", COMMANDNAME_CALL_STATUS, Response::Error));
                return;
            }
        } else {
            call = app->findCall(cid);
            if (call == NULL) {
                app->sendResponse(Response("No call with such id.", COMMANDNAME_CALL_STATUS, Response::Error));
                return;
            }
        }
        LinphoneCallState call_state = LinphoneCallIdle;
        call_state = linphone_call_get_state(call);

        LinphoneCallLog *callLog = linphone_call_get_call_log(call);
        const LinphoneAddress *fromAddr = linphone_call_log_get_from_address(callLog);
        char *fromStr = linphone_address_as_string(fromAddr);

        const char *flag;
        bool_t in_conference;
        in_conference=(linphone_call_get_conference(call) != NULL);
        flag=in_conference ? "Conferencing" : "";

        ostringstream ostr;

        ostr << "CallId: " << app->updateCallId(call) << "\n";
        ostr << "State: " << linphone_call_state_to_string(call_state) << "\n";

        switch (call_state) {
            case LinphoneCallOutgoingInit:
            case LinphoneCallOutgoingProgress:
            case LinphoneCallOutgoingRinging:
            case LinphoneCallPaused:
            case LinphoneCallStreamsRunning:
            case LinphoneCallConnected:
            case LinphoneCallIncomingReceived:
                ostr << "From: " << fromStr << "\n";
                ostr << "Direction: " << ((linphone_call_get_dir(call) == LinphoneCallOutgoing) ? "out" : "in") << "\n";
                ostr << "Duration: " << linphone_call_get_duration(call) << "\n";
                ostr << flag << "\n";
                break;
            default:
                break;
        }
        app->sendResponse(Response(ostr.str(), COMMANDNAME_CALL_STATUS, Response::Ok));
    }
    if(param == "ALL") {
        //call-status ALL
        elem = linphone_core_get_calls(app->getCore());
        if (elem != NULL) {
            call = (LinphoneCall*)elem->data;
        }
        if (call == NULL) {
            app->sendResponse(Response("No active call.", COMMANDNAME_CALL_STATUS, Response::Error));
            return;
        }
        else{
            for (int index = 0; index < ms_list_size(elem); index++) {
                LinphoneCall* lCall = (LinphoneCall*) bctbx_list_nth_data(elem,index);

                LinphoneCallState call_state = LinphoneCallIdle;
                call_state = linphone_call_get_state(lCall);

                LinphoneCallLog *callLog = linphone_call_get_call_log(lCall);
                const LinphoneAddress *fromAddr = linphone_call_log_get_from_address(callLog);
                char *fromStr = linphone_address_as_string(fromAddr);

                const char *flag;
                bool_t in_conference;
                in_conference=(linphone_call_get_conference(lCall) != NULL);
                flag=in_conference ? "Conferencing" : "";

                ostringstream ostr;

                ostr << "CallId: " << app->updateCallId(lCall) << "\n";
                ostr << "State: " << linphone_call_state_to_string(call_state) << "\n";

                switch (call_state) {
                    case LinphoneCallPaused:
                    case LinphoneCallStreamsRunning:
                    case LinphoneCallConnected:
                    case LinphoneCallIncomingReceived:
                    case LinphoneCallOutgoingInit:
                    case LinphoneCallOutgoingProgress:
                    case LinphoneCallOutgoingRinging:
                        ostr << "From: " << fromStr << "\n";
                        ostr << "Direction: " << ((linphone_call_get_dir(lCall) == LinphoneCallOutgoing) ? "out" : "in") << "\n";
                        ostr << "Duration: " << linphone_call_get_duration(call) << "\n";
                        ostr << flag << "\n";
                        break;
                    default:
                        break;
                }
                app->sendResponse(Response(ostr.str(), COMMANDNAME_CALL_STATUS, Response::Ok));
            }
        }
        return;
    }

}
