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
    string param;
    ist >> param;
    if (ist.fail()) {
        app->sendResponse(Response(COMMANDNAME_SETSOUNDCARD, "Missing parameter", Response::Error));
    }
    if(param == "playback_dev"){
        ist >> param;
        if (ist.fail()) {
            app->sendResponse(Response(COMMANDNAME_SETSOUNDCARD, "Missing parameter", Response::Error));
        } else {
            if (ist.fail()) {
                app->sendResponse(Response(COMMANDNAME_SETSOUNDCARD, "Missing or wrong value", Response::Error));
            } else {
                string playback_dev = "playback_dev";
                const std::string& soundCard = ist.str().substr(playback_dev.length()+1, ist.str().length());
                linphone_core_set_playback_device(app->getCore(), soundCard.c_str());
                if (!ist.str().empty() && ist.str() != "") {
                    app->sendResponse(
                            Response(COMMANDNAME_SETSOUNDCARD, "Soundcard was set successfully\n", Response::Ok));
                }
                param = playback_dev;
            }
        }
    }
    if (param == "ringer_dev") {
        ist >> param;
        if (ist.fail()) {
            app->sendResponse(Response(COMMANDNAME_SETSOUNDCARD, "Missing parameter", Response::Error));
        } else {
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
    if(param == "capture_dev"){
        ist >> param;
        if (ist.fail()) {
            app->sendResponse(Response(COMMANDNAME_SETSOUNDCARD, "Missing parameter", Response::Error));
        } else {
            if (ist.fail()) {
                app->sendResponse(Response(COMMANDNAME_SETSOUNDCARD, "Missing or wrong value", Response::Error));
            } else {
                string capture_dev = "capture_dev";
                const std::string& soundCard = ist.str().substr(capture_dev.length()+1, ist.str().length());
                linphone_core_set_capture_device(app->getCore(), soundCard.c_str());
                if (!ist.str().empty() && ist.str() != "") {
                    app->sendResponse(
                            Response(COMMANDNAME_SETSOUNDCARD, "Soundcard was set successfully\n", Response::Ok));
                }
                param = capture_dev;
            }
        }
    }
    if (param != "playback_dev" && param != "ringer_dev" && param != "capture_dev"){
        app->sendResponse(Response(COMMANDNAME_SETSOUNDCARD, "Wrong parameter", Response::Error));
    }
}