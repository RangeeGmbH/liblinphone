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

#include "call-mute.h"

using namespace std;

CallMuteCommand::CallMuteCommand() :
    DaemonCommand("call-mute", "call-mute [CALLID] [get|set] [input|output] ([0|1])", "Mute/unmute the microphone or speaker for call with CallID.")
{
	addExample(new DaemonCommandExample("call-mute set input 1 1",
										"Status: Ok\n\n"
										"Microphone Muted"));
	addExample(new DaemonCommandExample("call-mute set output 1 1",
                                        "Status: Ok\n\n"
                                        "Microphone Muted"));
	addExample(new DaemonCommandExample("call-mute get 1",
										"Status: Ok\n\n"
										"Microphone Muted"));
	addExample(new DaemonCommandExample("call-mute set input 1 0",
										"Status: Ok\n\n"
										"Microphone Unmuted"));
	addExample(new DaemonCommandExample("call-mute set output 1 0",
                                        "Status: Ok\n\n"
                                        "Microphone Unmuted"));
	addExample(new DaemonCommandExample("call-mute set input 1 1",
										"Status: Error\n\n"
                                        "Reason: No call with such id."));
}

void CallMuteCommand::exec(Daemon* app, const string& args)
{
    int callId;
    int muted;
    bool findCall;
    ostringstream ost;
    istringstream ist(args);
    string param;
    ist >> param;
    LinphoneCall *call;

    if (ist.fail()) {
        app->sendResponse(Response(COMMANDNAME_CALL_MUTE, "Missing parameter", Response::Error));
    }
    if (param == "set") {
        ist >> param;
        ist >> callId;
        ist >> muted;
        call = app->findCall(callId);
        if (call == NULL) {
            findCall = false;
        }
        else {
            findCall = true;
            if (param == "input") {
                ////set
                linphone_call_set_microphone_muted(call, (bool_t)muted);
            }
            if (param == "output") {
                ////set
                linphone_call_set_speaker_muted(call, (bool_t)muted);
            }
        }
        if(findCall) {
            ////get
            std::string callStr;
            callStr = "{ \"call\": ";
            callStr += app->getJsonForCall(call);
            callStr += " }";
            app->sendResponse(Response(COMMANDNAME_CALL_MUTE, callStr, Response::Ok));
        }
        else {
            ost << "No call with such id.";
            app->sendResponse(Response(COMMANDNAME_CALL_MUTE, ost.str(), Response::Error));
            return;
        }
    }
    if (param == "get") {
        ist >> callId;
        call = app->findCall(callId);
        if (call == NULL) {
            ost << "No call with such id.";
            app->sendResponse(Response(COMMANDNAME_CALL_MUTE, ost.str(), Response::Error));
            return;
        }
        else {
            std::string callStr;
            callStr = "{ \"call\": ";
            callStr += app->getJsonForCall(call);
            callStr += " }";
            app->sendResponse(Response(COMMANDNAME_CALL_MUTE, callStr, Response::Ok));
        }
    }
}
