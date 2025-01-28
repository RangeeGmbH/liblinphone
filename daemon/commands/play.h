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

#ifndef LINPHONE_DAEMON_COMMAND_PLAY_H_
#define LINPHONE_DAEMON_COMMAND_PLAY_H_

#include "daemon.h"

#define COMMANDNAME_INCALL_PLAYER_START "incall-player-start"
#define COMMANDNAME_INCALL_PLAYER_STOP "incall-player-stop"
#define COMMANDNAME_INCALL_PLAYER_PAUSE "incall-player-pause"
#define COMMANDNAME_INCALL_PLAYER_RESUME "incall-player-resume"


class IncallPlayerStartCommand : public DaemonCommand {
public:
	IncallPlayerStartCommand();

	void exec(Daemon *app, const std::string &args) override;

private:
	static void onEof(LinphonePlayer *player);
};

class IncallPlayerStopCommand : public DaemonCommand {
public:
	IncallPlayerStopCommand();

	void exec(Daemon *app, const std::string &args) override;
};

class IncallPlayerPauseCommand : public DaemonCommand {
public:
	IncallPlayerPauseCommand();

	void exec(Daemon *app, const std::string &args) override;
};

class IncallPlayerResumeCommand : public DaemonCommand {
public:
	IncallPlayerResumeCommand();

	void exec(Daemon *app, const std::string &args) override;
};
#endif // LINPHONE_DAEMON_COMMAND_PLAY_H_
