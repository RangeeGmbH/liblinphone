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

#ifndef LINPHONE_DAEMON_COMMAND_CONFIG_H_
#define LINPHONE_DAEMON_COMMAND_CONFIG_H_

#include "daemon.h"

using namespace std;

#define COMMANDNAME_CONFIG_GET "config-get"
#define COMMANDNAME_CONFIG_SET "config-set"

class ConfigGetCommand: public DaemonCommand {
public:
    ConfigGetCommand();

    void exec(Daemon *app, const std::string& args) override;

private:
    string getConfigResponseStr(const string& value);
};

class ConfigSetCommand: public DaemonCommand {
public:
    ConfigSetCommand();

    void exec(Daemon *app, const std::string& args) override;

private:
    string getConfigResponseStr(const string& value);
};

#endif // LINPHONE_DAEMON_COMMAND_CONFIG_H_
