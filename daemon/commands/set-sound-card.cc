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

SetSoundCard::SetSoundCard() :
DaemonCommand("set-sound-card", "set-sound-card",
              "set the sound card for ringer_dev, or capture_dev, or playback_dev") {
    addExample(new DaemonCommandExample("set-sound-card set playback_dev <Sound Card Name>",
                                        "Status: OK\n"));
    addExample(new DaemonCommandExample("set-sound-card set ringer_dev <Sound Card Name>",
                                        "Status: OK\n"));
    addExample(new DaemonCommandExample("set-sound-card set capture_dev <Sound Card Name>",
                                        "Status: OK\n"));
}
void SetSoundCard::exec(Daemon *app, const string &args) {
    istringstream ist(args);
    string ost;
    string param;
    ist >> param;
    if (ist.fail()) {
        app->sendResponse(Response(COMMANDNAME_SETSOUNDCARD, "Missing parameter", Response::Error));
    }
    if(param == "default"){
        ist >> param;
        if (ist.fail()) {
            app->sendResponse(Response(COMMANDNAME_SETSOUNDCARD, "Missing parameter", Response::Error));
        }
        else {
            if(param == "output"){
                if (ist.fail()) {
                    app->sendResponse(Response(COMMANDNAME_SETSOUNDCARD, "Missing parameter", Response::Error));
                } else {
                    ist >> param;
                    if (ist.fail()) {
                        app->sendResponse(Response(COMMANDNAME_SETSOUNDCARD, "Missing or wrong value", Response::Error));
                    }
                    else {
                        // search card to set default playback /////////////////////////////
                        string output_dev = "default output";
                        std::string test = ist.str();
                        const std::string& soundCard = ist.str().substr(output_dev.length()+1, ist.str().length());

                        bctbx_list_t * deviceIt = linphone_core_get_extended_audio_devices(app->getCore());
                        while ( deviceIt != NULL ) {
                            LinphoneAudioDevice * pDevice = (LinphoneAudioDevice *) deviceIt->data;
                            std::string deviceName(linphone_audio_device_get_device_name(pDevice));
                            std::string driverName(linphone_audio_device_get_driver_name(pDevice));
                            std::string deviceAndDriver = driverName + ": " + deviceName;
                            if(deviceAndDriver == soundCard) {
                                //set default output
                                linphone_core_set_default_output_audio_device(app->getCore(), pDevice);
                            }

                            deviceIt = deviceIt->next;
                        }
                        bctbx_list_free_with_data(deviceIt, (void (*)(void *))linphone_audio_device_unref);
                        //////////////////////////////////////////////

                        // get default output card
                        const LinphoneAudioDevice *output_device = linphone_core_get_default_output_audio_device(app->getCore());
                        std::string deviceName(linphone_audio_device_get_device_name(output_device));
                        std::string driverName(linphone_audio_device_get_driver_name(output_device));
                        string_format(ost, "{ \"driver\": \"%s\", \"name\": \"%s\" }",  driverName.c_str(), deviceName.c_str());

                        if (!ist.str().empty() && ist.str() != "") {
                            app->sendResponse(
                                    Response(COMMANDNAME_SETSOUNDCARD, ost, Response::Ok));
                        }
                        param = output_dev;
                    }
                }
            }
            if(param == "input"){
                if (ist.fail()) {
                    app->sendResponse(Response(COMMANDNAME_SETSOUNDCARD, "Missing or wrong value", Response::Error));
                } else {
                    // search card to set default input /////////////////////////////
                    string input_dev = "input";
                    const std::string& soundCard = ist.str().substr(input_dev.length()+1, ist.str().length());

                    bctbx_list_t * deviceIt = linphone_core_get_extended_audio_devices(app->getCore());
                    while ( deviceIt != NULL ) {
                        LinphoneAudioDevice * pDevice = (LinphoneAudioDevice *) deviceIt->data;
                        std::string deviceName(linphone_audio_device_get_device_name(pDevice));
                        std::string driverName(linphone_audio_device_get_driver_name(pDevice));
                        std::string deviceAndDriver = driverName + ": " + deviceName;
                        if(deviceAndDriver == soundCard) {
                            //set default input
                            linphone_core_set_default_input_audio_device(app->getCore(), pDevice);
                        }

                        deviceIt = deviceIt->next;
                    }
                    bctbx_list_free_with_data(deviceIt, (void (*)(void *))linphone_audio_device_unref);
                    //////////////////////////////////////////////

                    // get default input card
                    const LinphoneAudioDevice *input_device = linphone_core_get_default_input_audio_device(app->getCore());
                    std::string deviceName(linphone_audio_device_get_device_name(input_device));
                    std::string driverName(linphone_audio_device_get_driver_name(input_device));
                    string_format(ost, "{ \"driver\": \"%s\", \"name\": \"%s\" }",  driverName.c_str(), deviceName.c_str());

                    if (!ist.str().empty() && ist.str() != "") {
                        app->sendResponse(
                                Response(COMMANDNAME_SETSOUNDCARD, ost, Response::Ok));
                    }
                    param = input_dev;
                }
            }
            if(param == "ringer") {
                if (ist.fail()) {
                    app->sendResponse(Response(COMMANDNAME_SETSOUNDCARD, "Missing or wrong value", Response::Error));
                } else {
                    string ringer_dev = "ringer_dev";
                    const std::string& soundCard = ist.str().substr(ringer_dev.length()+1, ist.str().length());
                    linphone_core_set_ringer_device(app->getCore(), soundCard.c_str());
                    if (!ist.str().empty() && ist.str() != "") {
                        app->sendResponse(
                                Response(COMMANDNAME_SETSOUNDCARD, "Soundcard was set successfully\n", Response::Ok));
                    }
                    param = ringer_dev;
                }
        }
    }
    /*else {

    }

        LinphoneCall *call = NULL;
        int cid;
        ist >> cid;
        call = app->findCall(ist);
        if (call == NULL) {
            sprintf(ost, "\"No call with such id.\"");
            app->sendResponse(Response(COMMANDNAME_CALL_STATUS, ost, Response::Error));
            return;
        }
        string callString;
        callString += "{ \"isALL\": false, \"calls\": [ ";
        callString += app->getJsonForCall(call);
        callString += " ]";
    }*/
    if (param != "default output" && param != "default input" && param != "capture_dev"){
        app->sendResponse(Response(COMMANDNAME_SETSOUNDCARD, "Wrong parameter", Response::Error));
    }
    }
}