/*
 * Copyright (c) 2010-2022 Belledonne Communications SARL.
 *
 * This file is part of Liblinphone
 * (see https://gitlab.linphone.org/BC/public/liblinphone).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ptime.h"

using namespace std;

string PtimeCommand::getPtimeResponseStr(LinphoneCore *core, Direction dir) {
    ostringstream ost;
    switch (dir) {
        case Upload:
            ost << "Upload: " << linphone_core_get_upload_ptime(core) << "\n";
            break;
        case Download:
            ost << "Download: " << linphone_core_get_download_ptime(core) << "\n";
            break;
        case BothDirections:
            ost << "Upload: " << linphone_core_get_upload_ptime(core) << "\n";
            ost << "Download: " << linphone_core_get_download_ptime(core) << "\n";
            break;
    }
    return ost.str();
}


class PtimeResponse : public Response {
public:
	enum Direction { Upload = 0, Download = 1, BothDirections = 2 };
	PtimeResponse(LinphoneCore *core, Direction dir);
};

PtimeResponse::PtimeResponse(LinphoneCore *core, Direction dir) : Response() {
	ostringstream ost;
	switch (dir) {
		case Upload:
			ost << "Upload: " << linphone_core_get_upload_ptime(core) << "\n";
			break;
		case Download:
			ost << "Download: " << linphone_core_get_download_ptime(core) << "\n";
			break;
		case BothDirections:
			ost << "Upload: " << linphone_core_get_upload_ptime(core) << "\n";
			ost << "Download: " << linphone_core_get_download_ptime(core) << "\n";
			break;
	}
	setBody(ost.str());
}

PtimeCommand::PtimeCommand()
    : DaemonCommand(
          "ptime",
          "ptime [up|down] [<ms>]",
          "Set the upload or download ptime if ms is defined, otherwise return the current value of the ptime.") {
	addExample(make_unique<DaemonCommandExample>("ptime up 20", "Status: Ok\n\n"
	                                                            "Upload: 20"));
	addExample(make_unique<DaemonCommandExample>("ptime down 30", "Status: Ok\n\n"
	                                                              "Download: 30"));
	addExample(make_unique<DaemonCommandExample>("ptime up", "Status: Ok\n\n"
	                                                         "Upload: 20"));
	addExample(make_unique<DaemonCommandExample>("ptime", "Status: Ok\n\n"
	                                                      "Upload: 20\n"
	                                                      "Download: 30"));
}

void PtimeCommand::exec(Daemon *app, const string &args) {
	string direction;
	int ms;
	istringstream ist(args);
	ist >> direction;
	if (ist.fail()) {
		app->sendResponse(PtimeResponse(app->getCore(), PtimeResponse::BothDirections));
		return;
	}
	if (direction.compare("up") == 0) {
		if (!ist.eof()) {
			ist >> ms;
			if (ist.fail()) {
				app->sendResponse(Response(COMMANDNAME_PTIME,"Incorrect ms parameter.", Response::Error));
			}
			linphone_core_set_upload_ptime(app->getCore(), ms);
		}
		app->sendResponse(Response(COMMANDNAME_PTIME, getPtimeResponseStr(app->getCore(), Upload), Response::Ok));
	} else if (direction.compare("down") == 0) {
		if (!ist.eof()) {
			ist >> ms;
			if (ist.fail()) {
				app->sendResponse(Response(COMMANDNAME_PTIME,"Incorrect ms parameter.", Response::Error));
			}
			linphone_core_set_download_ptime(app->getCore(), ms);
		}
		app->sendResponse(PtimeResponse(app->getCore(), PtimeResponse::Download));
	} else {
		app->sendResponse(Response(COMMANDNAME_PTIME, "Missing/Incorrect parameter(s).", Response::Error));
	}
}
