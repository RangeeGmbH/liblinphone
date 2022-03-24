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

#include "set-sound-card.h"
#include <unistd.h>
#include <fcntl.h>
#include <regex>
#include <alsa/asoundlib.h>
#include <math.h>
#include "private.h"

using namespace std;


LinphoneAudioDevice * SetSoundCard::getLinphoneAudioDevice(Daemon *app, std::string idString) {
    string ost;
    LinphoneAudioDevice * pDevice = app->findAudioDevice(idString);
    if(pDevice == nullptr) {
        ost = "Try to set card: " + idString + " for default was not found in device list";
        app->sendResponse(Response(COMMANDNAME_SETSOUNDCARD, ost, Response::Error));
    }
    return pDevice;
}

SetSoundCard::SetSoundCard() :
DaemonCommand("set-sound-card", "set-sound-card default|CallID output|input|ringer <Sound Card Name>",
              "set the sound card for default or Call of ringer, input, or output sound card") {
    addExample(new DaemonCommandExample("set-sound-card default output <Sound Card Name>",
                                        "Status: OK\n"));
    addExample(new DaemonCommandExample("set-sound-card default input <Sound Card Name>",
                                        "Status: OK\n"));
    addExample(new DaemonCommandExample("set-sound-card default ringer <Sound Card Name>",
                                        "Status: OK\n"));
    addExample(new DaemonCommandExample("set-sound-card <CallID> output <Sound Card Name>",
                                        "Status: OK\n"));
    addExample(new DaemonCommandExample("set-sound-card <CallID> input <Sound Card Name>",
                                        "Status: OK\n"));
}
void SetSoundCard::exec(Daemon *app, const string &args) {
    istringstream ist(args);
    string ost;
    ostringstream ostStr;
    string param;
    ist >> param;
    if (ist.fail()) {
        app->sendResponse(Response(COMMANDNAME_SETSOUNDCARD, "Missing parameter", Response::Error));
    }
    if(param == "default") {
        ist >> param;
        if (ist.fail()) {
            app->sendResponse(Response(COMMANDNAME_SETSOUNDCARD, "Missing parameter", Response::Error));
        }
        else {
            LinphoneAudioDevice * pDevice_Output;
            LinphoneAudioDevice * pDevice_Input;
            LinphoneAudioDevice * pDevice_Ringer;

            // search
            if(param == "output") {
                string output_dev = "default output";
                const std::string& soundCard_Output = ist.str().substr(output_dev.length()+1, ist.str().length());

                pDevice_Output = getLinphoneAudioDevice(app, soundCard_Output);

                if(pDevice_Output == nullptr)  {
                    return;
                }
                linphone_core_set_default_output_audio_device(app->getCore(), pDevice_Output);
            }
            else if(param == "input") {
                string input_dev = "default input";
                const std::string& soundCard_Input = ist.str().substr(input_dev.length()+1, ist.str().length());

                pDevice_Input = getLinphoneAudioDevice(app, soundCard_Input);
                if(pDevice_Input == nullptr)  {
                    return;
                }
                linphone_core_set_default_input_audio_device(app->getCore(), pDevice_Input);;
            }
            else if(param == "ringer") {
                string ringer_dev = "default ringer";
                const std::string& soundCard_Ringer = ist.str().substr(ringer_dev.length()+1, ist.str().length());

                pDevice_Ringer = getLinphoneAudioDevice(app, soundCard_Ringer);

                if(pDevice_Ringer == nullptr)  {
                    return;
                }
                linphone_core_set_ringer_device(app->getCore(), linphone_audio_device_get_id(pDevice_Ringer));
            }

            // get...
            bctbx_list_t * deviceIt = linphone_core_get_extended_audio_devices(app->getCore());
            vector <std::string> v;
            while ( deviceIt != NULL ) {
                LinphoneAudioDevice * pDevice = (LinphoneAudioDevice *) deviceIt->data;
                std::string vectorStr = "";
                vectorStr = app->getJsonForAudioDevice(pDevice);
                v.push_back(vectorStr);

                deviceIt = deviceIt->next;
            }
            bctbx_list_free_with_data(deviceIt, (void (*)(void *))linphone_audio_device_unref);

            std::string soundCards = app->join(v, ", ");
            ost = "{ \"soundcards\": [" + soundCards + "] }";
            app->sendResponse(Response(COMMANDNAME_SETSOUNDCARD, ost, Response::Ok));
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
            app->sendResponse(Response(COMMANDNAME_SETSOUNDCARD, ostStr.str(), Response::Error));
            return;
        }
        if(param == "output"){
            if (ist.fail()) {
                app->sendResponse(Response(COMMANDNAME_SETSOUNDCARD, "Missing parameter", Response::Error));
            } else {

                //set.....
                string output_dev = "output";
                const std::string& soundCard = ist.str().substr(ist.str().find(output_dev)+output_dev.length()+1, ist.str().length());
                LinphoneAudioDevice * pDevice;
                pDevice = getLinphoneAudioDevice(app,soundCard);
                if(pDevice == nullptr)  {
                    return;
                }
                linphone_call_set_output_audio_device(call, pDevice);
                ////////////
                //get.....

                string callString;
                callString += "{ \"isAll\": false, \"calls\": [ ";
                callString += app->getJsonForCall(call);
                callString += " ]";
                callString += " }";

                if (!ist.str().empty() && ist.str() != "") {
                    app->sendResponse(
                            Response(COMMANDNAME_SETSOUNDCARD, callString, Response::Ok));
                }
                param = output_dev;
            }
        }
        if(param == "input"){
            if (ist.fail()) {
                app->sendResponse(Response(COMMANDNAME_SETSOUNDCARD, "Missing parameter", Response::Error));
            } else {

                //set.....
                string input_dev = "input";
                const std::string& soundCard = ist.str().substr(ist.str().find(input_dev)+input_dev.length()+1, ist.str().length());
                LinphoneAudioDevice * pDevice;
                pDevice = getLinphoneAudioDevice(app,soundCard);
                if(pDevice == nullptr)  {
                    return;
                }
                ////////////
                //get....

                string callString;
                callString += "{ \"isAll\": false, \"calls\": [ ";
                callString += app->getJsonForCall(call);
                callString += " ]";
                callString += " }";

                if (!ist.str().empty() && ist.str() != "") {
                    app->sendResponse(
                            Response(COMMANDNAME_SETSOUNDCARD, callString, Response::Ok));
                }
                param = input_dev;
            }
        }
    }
}