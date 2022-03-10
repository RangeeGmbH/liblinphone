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
                        // set default output /////////////////////////////
                        string output_dev = "default output";
                        std::string test = ist.str();
                        const std::string& soundCard = ist.str().substr(output_dev.length()+1, ist.str().length());

                        bctbx_list_t * deviceIt = linphone_core_get_extended_audio_devices(app->getCore());
                        LinphoneAudioDevice * pDevice = app->findAudioDevice(deviceIt, soundCard);
                        if(pDevice == nullptr) {
                            ost = "Try to set card: " + soundCard + " default output was not found in device list";
                            app->sendResponse(Response(COMMANDNAME_SETSOUNDCARD, ost, Response::Error));
                        }
                        else {
                            linphone_core_set_default_output_audio_device(app->getCore(), pDevice);
                            bctbx_list_free_with_data(deviceIt, (void (*)(void *))linphone_audio_device_unref);

                            // get default output card
                            const LinphoneAudioDevice *output_device = linphone_core_get_default_output_audio_device(app->getCore());
                            ost = app->getJsonForAudioDevice(output_device);

                            if (!ist.str().empty() && ist.str() != "") {
                                app->sendResponse(
                                        Response(COMMANDNAME_SETSOUNDCARD, ost, Response::Ok));
                            }
                            param = output_dev;
                        }
                    }
                }
            }
            if(param == "input"){
                if (ist.fail()) {
                    app->sendResponse(Response(COMMANDNAME_SETSOUNDCARD, "Missing or wrong value", Response::Error));
                } else {
                    // search card to set default input /////////////////////////////
                    string input_dev = "default input";
                    const std::string& soundCard = ist.str().substr(input_dev.length()+1, ist.str().length());

                    bctbx_list_t * deviceIt = linphone_core_get_extended_audio_devices(app->getCore());
                    LinphoneAudioDevice * pDevice = app->findAudioDevice(deviceIt, soundCard);
                    if(pDevice == nullptr) {
                        ost = "Try to set card: " + soundCard + " for default input was not found in device list";
                        app->sendResponse(Response(COMMANDNAME_SETSOUNDCARD, ost, Response::Error));
                    }
                    else {
                        linphone_core_set_default_input_audio_device(app->getCore(), pDevice);
                        bctbx_list_free_with_data(deviceIt, (void (*)(void *))linphone_audio_device_unref);

                        // get default input card
                        const LinphoneAudioDevice *input_device = linphone_core_get_default_input_audio_device(app->getCore());

                        ost = app->getJsonForAudioDevice(input_device);

                        if (!ist.str().empty() && ist.str() != "") {
                            app->sendResponse(
                                    Response(COMMANDNAME_SETSOUNDCARD, ost, Response::Ok));
                        }
                        param = input_dev;
                    }
                }
            }
            if(param == "ringer") {
                if (ist.fail()) {
                    app->sendResponse(Response(COMMANDNAME_SETSOUNDCARD, "Missing or wrong value", Response::Error));
                } else {
                    string ringer_dev = "default ringer";
                    // set ringer ( old set device function )
                    const std::string& soundCard = ist.str().substr(ringer_dev.length()+1, ist.str().length());
                    linphone_core_set_ringer_device(app->getCore(), soundCard.c_str());

                    // get ringer
                    bctbx_list_t * deviceIt = linphone_core_get_extended_audio_devices(app->getCore());
                    LinphoneAudioDevice * pDevice = app->findAudioDevice(deviceIt, soundCard);
                    if(pDevice == nullptr) {
                        ost = "Try to set card: " + soundCard + " for default ringer was not found in device list";
                        app->sendResponse(Response(COMMANDNAME_SETSOUNDCARD, ost, Response::Error));
                    }
                    else {
                        ost = app->getJsonForAudioDevice(pDevice);
                        bctbx_list_free_with_data(deviceIt, (void (*)(void *))linphone_audio_device_unref);

                        if (!ist.str().empty() && ist.str() != "") {
                            app->sendResponse(
                                    Response(COMMANDNAME_SETSOUNDCARD, ost, Response::Ok));
                        }
                        param = ringer_dev;
                    }
                }
            }
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

                bctbx_list_t * deviceIt = linphone_core_get_extended_audio_devices(app->getCore());
                LinphoneAudioDevice * pDevice = app->findAudioDevice(deviceIt, soundCard);
                if ( pDevice == nullptr ) {
                    ost = "Try to set card: " + soundCard +  " for output was not found in device list";
                    string callString;
                    callString += "{ \"isAll\": false, \"calls\": [ ";
                    callString += app->getJsonForCall(call);
                    callString += " ] ";
                    callString += ost;
                    callString += " }";
                    app->sendResponse(Response(COMMANDNAME_SETSOUNDCARD, callString, Response::Error));
                }
                //linphone_call_set_output_audio_device(call, app->findAudioDevice(deviceIt, soundCard));
                linphone_call_set_output_audio_device(call, pDevice);
                bctbx_list_free_with_data(deviceIt, (void (*)(void *))linphone_audio_device_unref);
                ////////////

                const LinphoneAudioDevice *output_device = linphone_call_get_output_audio_device(call);
                ost = app->getJsonForAudioDevice(output_device);

                string callString;
                callString += "{ \"isAll\": false, \"calls\": [ ";
                callString += app->getJsonForCall(call);
                callString += " ] ";
                callString += ost;
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

                bctbx_list_t * deviceIt = linphone_core_get_extended_audio_devices(app->getCore());
                LinphoneAudioDevice * pDevice = app->findAudioDevice(deviceIt, soundCard);
                if ( pDevice == nullptr ) {
                    ost = "Try to set card: " + soundCard +  " for input was not found in device list";
                    string callString;
                    callString += "{ \"isAll\": false, \"calls\": [ ";
                    callString += app->getJsonForCall(call);
                    callString += " ] ";
                    callString += ost;
                    callString += " }";
                    app->sendResponse(Response(COMMANDNAME_SETSOUNDCARD, callString, Response::Error));
                }
                linphone_call_set_input_audio_device(call, pDevice);
                bctbx_list_free_with_data(deviceIt, (void (*)(void *))linphone_audio_device_unref);
                ////////////

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
                            Response(COMMANDNAME_SETSOUNDCARD, callString, Response::Ok));
                }
                param = input_dev;
            }
        }
    }
}