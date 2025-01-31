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
#include <math.h>
#include "private.h"

using namespace std;

VolumeCommand::VolumeCommand() :
DaemonCommand("volume", "volume [get|set] [call <callid>|default|conference] [<value for speaker volume> <value for record volume>]",
                      "set or get the volume around value for speaker volume and record volume") {
    addExample(make_unique<DaemonCommandExample>("volume set default 0.55 0.55",
                                        "Status: OK\n"));
    addExample(make_unique<DaemonCommandExample>("volume get default",
                                        "Status: OK\n"));
    addExample(make_unique<DaemonCommandExample>("volume set call 1 0.55 0.55",
                                        "Status: OK\n"));
    addExample(make_unique<DaemonCommandExample>("volume get call 1",
                                        "Status: OK\n"));
    addExample(make_unique<DaemonCommandExample>("volume set conference 0.55 0.55",
                                        "Status: OK\n"));
    addExample(make_unique<DaemonCommandExample>("volume get conference",
                                        "Status: OK\n"));

}
void VolumeCommand::exec(Daemon *app, const string &args) {
    string callId;
    float inputVolume;
    float outputVolume;
    float ringerVolume;
    istringstream ist(args);
    string param;
    ist >> param;

    ostringstream ost;

    if (ist.fail()) {
        app->sendResponse(Response(COMMANDNAME_VOLUME, "Missing parameter", Response::Error));
    }
    if(param == "set") {
        ist >> param;
        if(param == "default") {
            ist >> outputVolume;
            ist >> inputVolume;
            ist >> ringerVolume;
            if (outputVolume > 1.0f) {
                outputVolume = 1.0f;
            }
            if (inputVolume > 1.0f) {
                inputVolume = 1.0f;
            }
            if (ringerVolume > 1.0f) {
                ringerVolume = 1.0f;
            }

            if (linphone_core_ready(app->getCore())) {
                //output
                linphone_core_set_playback_gain_db(app->getCore(), outputVolume);

                //input
                linphone_core_set_mic_gain_db(app->getCore(), inputVolume);

                //ringer
                linphone_core_set_ringer_gain_db(app->getCore(), ringerVolume);
            }

            ost << "{ \"isdefault\": true, \"volumes\": {"
                << "\"output\": " << linphone_core_get_playback_gain_db(app->getCore())
                << ", \"input\": " << linphone_core_get_mic_gain_db(app->getCore())
                << ", \"ringer\": " << linphone_core_get_ringer_gain_db(app->getCore())
                << " } }";

            app->sendResponse(Response(COMMANDNAME_VOLUME, ost.str(), Response::Ok));
        }
        if (param == "call") {
            ist >> param;
            callId = param;
            ist >> outputVolume;
            ist >> inputVolume;

            if (inputVolume > 1) {
                inputVolume = 1.0f;
            }
            if (outputVolume > 1) {
                outputVolume = 1.0f;
            }

            if (callId.empty()) {
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

            if (outputVolume >= 0.0f && inputVolume >= 0.0f) {
                std::string setCall;
                setCall = "{ \"isDefault\": false, \"call\": ";
                setCall += app->getJsonForCall(call);
                setCall += " }";
                app->sendResponse(Response(COMMANDNAME_VOLUME, setCall, Response::Ok));
            }
        }
        if (param == "conference") {
            ist >> outputVolume;
            ist >> inputVolume;

            if (inputVolume > 1) {
                inputVolume = 1.0f;
            }
            if (outputVolume > 1) {
                outputVolume = 1.0f;
            }
            LinphoneConference *conference =linphone_core_get_conference(app->getCore());
            if (conference == NULL) {
                ost << "No conference in progress. Can't set Volumes.";
                app->sendResponse(Response(COMMANDNAME_VOLUME, ost.str(), Response::Error));
                return;
            }
            else {
                linphone_conference_set_output_volume_gain(conference, outputVolume);
                linphone_conference_set_input_volume_gain(conference, inputVolume);

                outputVolume = linphone_conference_get_output_volume_gain(conference);
                inputVolume = linphone_conference_get_input_volume_gain(conference);

                if (outputVolume >= 0.0f && inputVolume >= 0.0f) {
                    std::string conferenceJSON;
                    conferenceJSON = app->getJsonForConference(conference);
                    app->sendResponse(Response(COMMANDNAME_VOLUME, conferenceJSON, Response::Ok));
                }
            }
        }
    }
    if (param == "get") {
        ist >> param;
        if (param == "default") {
            ost << "{ \"isdefault\": true, \"volumes\": {"
            << "\"output\": " << linphone_core_get_playback_gain_db(app->getCore())
            << ", \"input\": " << linphone_core_get_mic_gain_db(app->getCore())
            << ", \"ringer\": " << linphone_core_get_ringer_gain_db(app->getCore())
            << " } }";

            app->sendResponse(Response(COMMANDNAME_VOLUME, ost.str(), Response::Ok));
        }
        if (param == "call") {
            ist >> param;
            callId = param;
            if (callId.empty()) {
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

            if (outputVolume >= 0.0f && inputVolume >= 0.0f) {
                std::string getCALL;
                getCALL = "{ \"isDefault\": false, \"call\": ";
                getCALL += app->getJsonForCall(call);
                getCALL += " }";
                app->sendResponse(Response(COMMANDNAME_VOLUME, getCALL, Response::Ok));
            }
        }
        if (param == "conference") {
            LinphoneConference *conference =linphone_core_get_conference(app->getCore());
            if (conference == NULL) {
                ost << "No conference in progress. Can't get volumes.";
                app->sendResponse(Response(COMMANDNAME_VOLUME, ost.str(), Response::Error));
                return;
            }
            else {
                outputVolume = linphone_conference_get_output_volume_gain(conference);
                inputVolume = linphone_conference_get_input_volume_gain(conference);

                if (outputVolume >= 0.0f && inputVolume >= 0.0f) {
                    std::string conferenceJSON;
                    conferenceJSON = app->getJsonForConference(conference);
                    app->sendResponse(Response(COMMANDNAME_VOLUME, conferenceJSON, Response::Ok));
                }
            }
        }
    }
}