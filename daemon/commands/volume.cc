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

#include "volume.h"
#include <unistd.h>
#include <fcntl.h>
#include <regex>
#include <alsa/asoundlib.h>
#include <math.h>
#include "private.h"

using namespace std;

VolumeCommand::VolumeCommand() :
DaemonCommand("volume", "volume <call-iD> [<value for speaker volume> <value for record volume>]",
              "set or get the volume around value for speaker volume and record volume") {
    addExample(new DaemonCommandExample("volume 1",
                                        "Status: OK\n"));
    addExample(new DaemonCommandExample("volume 1 0.xx or 1.00 0.xx or 1.00",
                                        "Status: OK\n"));
}
void VolumeCommand::exec(Daemon *app, const string &args) {
    string callId;
    float inputVolume;
    float outputVolume;
    istringstream ist(args);
    string param;
    ist >> param;

    ostringstream ost;

    if (ist.fail()) {
        app->sendResponse(Response(COMMANDNAME_VOLUME, "Missing parameter", Response::Error));
    }
    if(param == "set") {
        ist >> param;
        if(param == "default"){
            ist >> inputVolume;
            ist >> outputVolume;
            //output
            app->getCore()->sound_conf.soft_play_lev=outputVolume;
            if (linphone_core_ready(app->getCore())){
                linphone_config_set_float(app->getCore()->config,"sound","output_volume",app->getCore()->sound_conf.soft_play_lev);
            }
            //input
            app->getCore()->sound_conf.soft_mic_lev=inputVolume;
            if (linphone_core_ready(app->getCore())){
                linphone_config_set_float(app->getCore()->config,"sound","input_volume",app->getCore()->sound_conf.soft_mic_lev);
            }
            ost << "{ \"isdefault\": true, \"volumes\" : {" << "\"output\": " <<  linphone_config_get_float(app->getCore()->config,"sound","output_volume",0)
            << ", \"input\": " << linphone_config_get_float(app->getCore()->config,"sound","input_volume",0) << "  }  }";

            app->sendResponse(Response(COMMANDNAME_VOLUME, ost.str(), Response::Ok));
        }
        if(param == "call"){
            ist >> param;
            callId = param;
            ist >> inputVolume;
            ist >> outputVolume;

            if(callId.empty()) {
                ost << "Missing parameter.";
                std::string commandName = COMMANDNAME_VOLUME;
                app->sendResponse(Response(commandName, ost.str(), Response::Error));
                return;
            }
            LinphoneCall *call = NULL;
            call = app->findCall(std::stoi(callId));
            if (call == NULL) {
                ost << "No call with such id.";
                std::string commandName = COMMANDNAME_VOLUME;
                app->sendResponse(Response(commandName, ost.str(), Response::Error));
                return;
            }
            linphone_call_set_speaker_volume_gain(call, outputVolume);
            linphone_call_set_microphone_volume_gain(call, inputVolume);

            outputVolume = linphone_call_get_speaker_volume_gain(call);
            inputVolume = linphone_call_get_microphone_volume_gain(call);

            if(outputVolume >=0.0 && inputVolume >=0.0) {
                std::string setCall;
                setCall = "{ \"isDefault\": false, \"call\": ";
                setCall += app->getJsonForCall(call);
                setCall += " }";
                app->sendResponse(Response(COMMANDNAME_VOLUME, setCall, Response::Ok));
            }
        }
    }
    if(param == "get") {
        ist >> param;
        if(param == "default") {
            ost << " { \"isdefault\": true, \"volumes\" : {" << "\"output\": " <<  linphone_config_get_float(app->getCore()->config,"sound","output_volume",0)
            << ", \"input\": " << linphone_config_get_float(app->getCore()->config,"sound","input_volume",0) << "  }  }";
            app->sendResponse(Response(COMMANDNAME_VOLUME, ost.str(), Response::Ok));
        }
        if(param == "call") {
            ist >> param;
            callId = param;
            if(callId.empty()) {
                ost << "Missing parameter.";
                std::string commandName = COMMANDNAME_VOLUME;
                app->sendResponse(Response(commandName, ost.str(), Response::Error));
                return;
            }
            LinphoneCall *call = NULL;
            call = app->findCall(std::stoi(callId));
            if (call == NULL) {
                ost << "No call with such id.";
                std::string commandName = COMMANDNAME_VOLUME;
                app->sendResponse(Response(commandName, ost.str(), Response::Error));
                return;
            }
            outputVolume = linphone_call_get_speaker_volume_gain(call);
            inputVolume = linphone_call_get_microphone_volume_gain(call);

            if(outputVolume >=0.0 && inputVolume >=0.0) {
                std::string getCALL;
                getCALL = "{ \"isDefault\": false, \"call\": ";
                getCALL += app->getJsonForCall(call);
                getCALL += " }";
                app->sendResponse(Response(COMMANDNAME_VOLUME, getCALL, Response::Ok));
            }
        }
    }
}