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
 */

#include "soundcards.h"

using namespace std;

AnswerSoundcard::AnswerSoundcard() :
        DaemonCommand("soundcards", "soundcards", "list all soundcards") {
    addExample(new DaemonCommandExample("soundcards",
                                        "Status: OK\n"));
}

void AnswerSoundcard::exec(Daemon *app, const string& args) {
    LinphoneCore *lc = app->getCore();
    const char **dev;
    dev=linphone_core_get_sound_devices(lc);
    int i;
    ostringstream ost;
    for(i=0; dev[i]!=NULL; ++i){
        ost << i << " " << dev[i] << "\n";
    }
    Response resp;
    resp.setBody(ost.str().c_str());
    app->sendResponse(resp);
}