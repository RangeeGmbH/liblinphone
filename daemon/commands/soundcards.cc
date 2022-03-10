/*
 * Copyright (c) 2010-2019 Belledonne Communications SARL.
 * Copyright (c) 2019-2020 Rangee GmbH.
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

#include "soundcards.h"

using namespace std;

SoundcardCommand::SoundcardCommand() :
        DaemonCommand("soundcards", "soundcards", "list all soundcards") {
    addExample(new DaemonCommandExample("soundcards",
                                        "Status: OK\n"));
}

void SoundcardCommand::exec(Daemon *app, const string &args) {
    LinphoneCore *lc = app->getCore();
    linphone_core_reload_sound_devices(lc);
    ostringstream ost;
    bctbx_list_t * deviceIt = linphone_core_get_extended_audio_devices(app->getCore());
    vector <std::string> v;
    while ( deviceIt != NULL ) {
        LinphoneAudioDevice * pDevice = (LinphoneAudioDevice *) deviceIt->data;
        std::string deviceName(linphone_audio_device_get_device_name(pDevice));
        std::string driverName(linphone_audio_device_get_driver_name(pDevice));
        std::string deviceAndDriver = driverName + ": " + deviceName;

        std::string vectorStr = "";
        vectorStr = app->getJsonForAudioDevice(pDevice);
        v.push_back(vectorStr);

        deviceIt = deviceIt->next;
    }

    std::string soundCards = app->join(v, ", ");
    ost << "{ \"soundcards\": [" << soundCards.c_str() << "] }";
    app->sendResponse(Response(COMMANDNAME_SOUNDCARDS, ost.str(), Response::Ok));
}