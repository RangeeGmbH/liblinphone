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
#include <unistd.h>
#include <fcntl.h>
#include <regex>
#include <alsa/asoundlib.h>
#include <math.h>
#include "private.h"

using namespace std;

GetSoundCard::GetSoundCard() :
        DaemonCommand("get-sound-card", "get-sound-card",
                      "set the sound device for ringer_dev, or capture_dev, or playback_dev") {
    addExample(new DaemonCommandExample("get-sound-card playback_dev",
                                        "Status: OK\n"));
    addExample(new DaemonCommandExample("get-sound-card ringer_dev",
                                        "Status: OK\n"));
    addExample(new DaemonCommandExample("get-sound-card capture_dev",
                                        "Status: OK\n"));
}
void GetSoundCard::exec(Daemon *app, const string &args) {
    istringstream ist(args);
    string param;
    ist >> param;
    if (ist.fail()) {
        app->sendResponse(Response(COMMANDNAME_GETSOUNDCARD, "Missing parameter", Response::Error));
    }
    if(param == "playback_dev"){
        Response resp;
        string playbackCardName = linphone_core_get_playback_device(app->getCore());
        string outputStr = "Current Playback Device: ";
        outputStr = outputStr + playbackCardName + "\n";
        app->sendResponse(Response(COMMANDNAME_GETSOUNDCARD, outputStr.c_str(), Response::Ok));
    }
    if (param == "ringer_dev") {
        Response resp;
        string ringerCardName = linphone_core_get_ringer_device(app->getCore());
        string outputStr = "Current Ringer Device: ";
        outputStr = outputStr + ringerCardName + "\n";
        app->sendResponse(Response(COMMANDNAME_GETSOUNDCARD, outputStr.c_str(), Response::Ok));
    }
    if(param == "capture_dev"){
        Response resp;
        string captureCardName = linphone_core_get_capture_device(app->getCore());
        string outputStr = "Current Capture Device: ";
        outputStr = outputStr + captureCardName + "\n";
        app->sendResponse(Response(COMMANDNAME_GETSOUNDCARD, outputStr.c_str(), Response::Ok));
    }

    if (param != "playback_dev" && param != "ringer_dev" && param != "capture_dev"){
        app->sendResponse(Response(COMMANDNAME_GETSOUNDCARD, "Wrong parameter", Response::Error));
    }
}