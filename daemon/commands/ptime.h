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

#ifndef LINPHONE_DAEMON_COMMAND_PTIME_H_
#define LINPHONE_DAEMON_COMMAND_PTIME_H_

#include "daemon.h"

using namespace std;

#define COMMANDNAME_PTIME "ptime"

class PtimeCommand: public DaemonCommand {
public:
	PtimeCommand();
	enum Direction {
	    Upload = 0,
	    Download = 1,
	    BothDirections = 2
	};

	void exec(Daemon *app, const std::string& args) override;
private:
    string getPtimeResponseStr(LinphoneCore *core, Direction dir);
};

#endif // LINPHONE_DAEMON_COMMAND_PTIME_H_
