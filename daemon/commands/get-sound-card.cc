/*
 * Copyright (c) 2010-2019 Belledonne Communications SARL.
 * Copyright (c) 2019 Rangee GmbH.
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
 *
 * The code of this class was inspired by
 * https://stackoverflow.com/a/7661176
 */

#include "get-sound-card.h"
#include <math.h>
#include "private.h"

using namespace std;

GetSoundCard::GetSoundCard() :
DaemonCommand("get-sound-card", "get-sound-card default|CallID output|input|ringer",
              "get the sound card for default or Call of ringer, input, or output sound card") {
    addExample(make_unique<DaemonCommandExample>("get-sound-card default output <Sound Card Name>",
                                        "Status: OK\n"));
    addExample(make_unique<DaemonCommandExample>("get-sound-card default input <Sound Card Name>",
                                        "Status: OK\n"));
    addExample(make_unique<DaemonCommandExample>("get-sound-card default ringer <Sound Card Name>",
                                        "Status: OK\n"));
    addExample(make_unique<DaemonCommandExample>("get-sound-card <CallID> output <Sound Card Name>",
                                        "Status: OK\n"));
    addExample(make_unique<DaemonCommandExample>("get-sound-card <CallID> input <Sound Card Name>",
                                        "Status: OK\n"));
}
void GetSoundCard::exec(Daemon *app, const string &args) {
    istringstream ist(args);
    string ost;
    ostringstream ostStr;
    string param;
    ist >> param;
    if (ist.fail()) {
        app->sendResponse(Response(COMMANDNAME_GETSOUNDCARD, "Missing parameter", Response::Error));
    }
    if(param == "default"){
        // get default output card
        const LinphoneAudioDevice *output_device = linphone_core_get_default_output_audio_device(app->getCore());
        ost = "{ \"isDefault\": true, \"output\": " + app->getJsonForAudioDevice(output_device) + ", ";

        // get default input card
        const LinphoneAudioDevice *input_device = linphone_core_get_default_input_audio_device(app->getCore());

        ost += "\"input\": " + app->getJsonForAudioDevice(input_device) + ", ";

        string ringer_dev = "default ringer";
        // get ringer
        const std::string& idString_Ringer = linphone_core_get_ringer_device(app->getCore());
        ost += "\"ringer\": " + app->getJsonForAudioDevice(app->findAudioDevice(idString_Ringer)) +  "  } ";

        if (!ist.str().empty() && ist.str() != "") {
            app->sendResponse(
                    Response(COMMANDNAME_GETSOUNDCARD, ost, Response::Ok));
        }
    }
    else {
        LinphoneCall *call = NULL;
        stringstream ss;
        ss << param;
        int cid;
        ss >> cid;
        ist >> param;
        call = app->findCall(cid);
        if (call == NULL) {
            ostStr << "\"No call with such id.\"";
            app->sendResponse(Response(COMMANDNAME_GETSOUNDCARD, ostStr.str(), Response::Error));
            return;
        }
        if(param == "output"){
            if (ist.fail()) {
                app->sendResponse(Response(COMMANDNAME_GETSOUNDCARD, "Missing parameter", Response::Error));
            } else {
                const LinphoneAudioDevice *output_device = linphone_call_get_output_audio_device(call);

                ost = "{ \"isDefault\": false, \"output\": " + app->getJsonForAudioDevice(output_device);

                string callString;
                callString += "{ \"isAll\": false, \"calls\": [ ";
                callString += app->getJsonForCall(call);
                callString += " ] ";
                callString += ost;
                callString += " }";

                if (!ist.str().empty() && ist.str() != "") {
                    app->sendResponse(
                            Response(COMMANDNAME_GETSOUNDCARD, callString, Response::Ok));
                }
            }
        }
        if(param == "input"){
            if (ist.fail()) {
                app->sendResponse(Response(COMMANDNAME_GETSOUNDCARD, "Missing parameter", Response::Error));
            } else {
                const LinphoneAudioDevice *input_device = linphone_call_get_input_audio_device(call);
                ost = app->getJsonForAudioDevice(input_device);

                string callString;
                callString += "{ \"isAll\": false, \"calls\": [ ";
                callString += app->getJsonForCall(call);
                callString += " ] ";
                callString += ost;
                callString += " }";

                if (!ist.str().empty() && ist.str() != "") {
                    app->sendResponse(
                            Response(COMMANDNAME_GETSOUNDCARD, callString, Response::Ok));
                }
            }
        }
    }
}