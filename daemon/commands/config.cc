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

#include "configcommand.h"

using namespace std;


string ConfigGetCommand::getConfigResponseStr(const string& value) {
    ostringstream ost;
    ost << "Value: " << value ? value : "<unset>";
    return ost.str();
}

string ConfigSetCommand::getConfigResponseStr(const string& value) {
    ostringstream ost;
    ost << "Value: " << value ? value : "<unset>";
    return ost.str();
}

ConfigGetCommand::ConfigGetCommand() :
		DaemonCommand("config-get", "config-get <section> <key>",
				"Reads a configuration value from linphone's configuration database.") {
	addExample(new DaemonCommandExample("config-get rtp symmetric",
						"Status: Ok\n\n"
						"Value: <unset>"));
}

void ConfigGetCommand::exec(Daemon *app, const string& args) {
	string section,key;
	istringstream ist(args);
	ist >> section >> key;
	if (ist.fail()) {
		app->sendResponse(Response(COMMANDNAME_CONFIG_GET, "Missing section and/or key names.", Response::Error));
		return;
	}
	const char *read_value=lp_config_get_string(linphone_core_get_config(app->getCore()),section.c_str(),key.c_str(),NULL);
	if (read_value != NULL){
        app->sendResponse(Response(COMMANDNAME_CONFIG_GET, getConfigResponseStr(read_value)+"\n", Response::Ok));
	}
	else {
        ostringstream error;
        error << "value from section: " << section << "\n" << "with key: "<< key << "\n" << "was not found";
        app->sendResponse(Response(COMMANDNAME_CONFIG_GET, error.str(), Response::Error));
	}

}


ConfigSetCommand::ConfigSetCommand() :
		DaemonCommand("config-set", "config-set <section> <key> <value>",
				"Sets a configuration value into linphone's configuration database.") {
	addExample(new DaemonCommandExample("config-set rtp symmetric 1",
						"Status: Ok\n\n"
						"Value: 2"));
	addExample(new DaemonCommandExample("config-set rtp symmetric",
						"Status: Ok\n\n"
						"Value: <unset>"));
}

void ConfigSetCommand::exec(Daemon *app, const string& args) {
	string section,key,value;
	istringstream ist(args);
	ist >> section >> key;
	if (ist.fail()) {
		app->sendResponse(Response(COMMANDNAME_CONFIG_SET, "Missing section and/or key names.", Response::Error));
		return;
	}
	ist>>value;
	lp_config_set_string(linphone_core_get_config(app->getCore()), section.c_str(), key.c_str(), value.size()>0 ? value.c_str() : NULL);
	app->sendResponse(Response(COMMANDNAME_CONFIG_SET, getConfigResponseStr(value.c_str())+"\n", Response::Ok));
}

