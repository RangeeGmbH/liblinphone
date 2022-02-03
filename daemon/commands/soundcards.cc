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

void SoundcardCommand::exec(Daemon *app, const string& args) {
    LinphoneCore *lc = app->getCore();
    linphone_core_reload_sound_devices(lc);
    const char **dev;
    dev=linphone_core_get_sound_devices(lc);
    int i;
    char ost[1024];
    vector<std::string> v;
    for(i=0; dev[i]!=NULL; ++i){
        std::string vectorStr = "\"" + to_string(i) + " " + dev[i] + "\"";
        v.push_back(vectorStr);
    }
    std::string soundCards =  app->join(v, ", ");
    sprintf(ost, "[%s]", soundCards.c_str());

    app->sendResponse(Response(COMMANDNAME_SOUNDCARDS, ost, Response::Ok));
}