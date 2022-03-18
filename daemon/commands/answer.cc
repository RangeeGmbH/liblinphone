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

#include "answer.h"

using namespace std;

AnswerCommand::AnswerCommand() :
		DaemonCommand("answer", "answer <call_id>", "Answer an incoming call.") {
	addExample(new DaemonCommandExample("answer 3",
						"Status: Error\n"
						"Reason: No call with such id."));
	addExample(new DaemonCommandExample("answer 2",
						"Status: Error\n"
						"Reason: Can't accept this call."));
	addExample(new DaemonCommandExample("answer 1",
						"Status: Ok"));
	addExample(new DaemonCommandExample("answer",
						"Status: Ok"));
	addExample(new DaemonCommandExample("answer",
						"Status: Error\n"
						"Reason: No call to accept."));
}

void AnswerCommand::exec(Daemon *app, const string& args) {
	LinphoneCore *lc = app->getCore();
	int cid;
	LinphoneCall *call;
	istringstream ist(args);
	ist >> cid;
	std::ostringstream stringStream;
	if (ist.fail()) {
		for (const MSList* elem = linphone_core_get_calls(lc); elem != NULL; elem = elem->next) {
			call = (LinphoneCall*)elem->data;
			LinphoneCallState cstate = linphone_call_get_state(call);
			if (cstate == LinphoneCallIncomingReceived || cstate == LinphoneCallIncomingEarlyMedia) {
				if (linphone_call_accept(call) == 0) {
				    string callString;
				    callString += "{ \"isAll\": false, \"calls\": [ ";
				    callString += app->getJsonForCall(call);
				    callString += " ] }";
				    app->sendResponse(Response(COMMANDNAME_ANSWER, callString, Response::Ok));
					return;
				}
			}
		}
	} else {
		call = app->findCall(cid);
		if (call == NULL) {
		    stringStream << "\"No call with such id\"";
		    app->sendResponse(Response(COMMANDNAME_ANSWER, stringStream.str(), Response::Error));
			return;
		}

		LinphoneCallState cstate = linphone_call_get_state(call);
		if (cstate == LinphoneCallIncomingReceived || cstate == LinphoneCallIncomingEarlyMedia) {
			if (linphone_call_accept(call) == 0) {
			    string callString;
			    callString += "{ \"isAll\": false, \"calls\": [ ";
			    callString += app->getJsonForCall(call);
			    callString += " ] }";
			    app->sendResponse(Response(COMMANDNAME_ANSWER, callString, Response::Ok));
				return;
			}
		}
		app->sendResponse(Response(COMMANDNAME_ANSWER, "Can't accept this call.", Response::Error));
		return;
	}

	app->sendResponse(Response(COMMANDNAME_ANSWER, "No call to accept.", Response::Error));
}
