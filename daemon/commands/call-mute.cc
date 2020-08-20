/*
 * Copyright (c) 2010-2019 Belledonne Communications SARL.
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

#include "call-mute.h"

using namespace std;

CallMuteCommand::CallMuteCommand() :
	DaemonCommand("call-mute", "call-mute 0|1", "Mute/unmute the microphone (1 to mute, 0 to unmute). No argument means MUTE.")
{
	addExample(new DaemonCommandExample("call-mute 1",
										"Status: Ok\n\n"
										"Microphone Muted"));
	addExample(new DaemonCommandExample("call-mute",
										"Status: Ok\n\n"
										"Microphone Muted"));
	addExample(new DaemonCommandExample("call-mute 0",
										"Status: Ok\n\n"
										"Microphone Unmuted"));
	addExample(new DaemonCommandExample("call-mute 1",
										"Status: Error\n\n"
										"Reason: No call in progress. Can't mute."));
}

void CallMuteCommand::exec(Daemon* app, const string& args)
{
	LinphoneCore *lc = app->getCore();
	int muted;
	LinphoneCall *call = linphone_core_get_current_call(lc);

	istringstream ist(args);
	ist >> muted;
	if (ist.fail() || (muted != 0)) {
		muted = TRUE;
        if (call == NULL) {
            app->sendResponse(Response(COMMANDNAME_CAll_MUTE, "No call in progress. Can't mute."));
            return;
        }
	} else {
        muted = FALSE;
        if (call == NULL) {
            app->sendResponse(Response(COMMANDNAME_CALL_MUTE_0, "No call in progress. Can't unmute."));
            return;
        }
	}
	linphone_core_enable_mic(lc, !muted);
    std::ostringstream buf;
    string micStatus = "MicStatus: ";
    string mutedStr = "Microphone Muted";
    string unmutedStr = "Microphone Unmuted";
    if(muted == TRUE) {
        buf << micStatus << mutedStr << "\n";
    }
    else {
        buf << micStatus << unmutedStr << "\n";
    }
	app->sendResponse(Response(muted ? COMMANDNAME_CAll_MUTE : COMMANDNAME_CALL_MUTE_0 ,buf.str(), Response::Ok));
}
