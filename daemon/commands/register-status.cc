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

#include "register-status.h"

using namespace std;
RegisterStatusCommand::RegisterStatusCommand() :
		DaemonCommand("register-status", "register-status <register_id>|ALL", "Return status of a registration or of all registrations.") {
	addExample(new DaemonCommandExample("register-status 1",
						"Status: Ok\n\n"
						"Id: 1\n"
						"State: LinphoneRegistrationOk"));
	addExample(new DaemonCommandExample("register-status ALL",
						"Status: Ok\n\n"
						"Id: 1\n"
						"State: LinphoneRegistrationOk\n\n"
						"Id: 2\n"
						"State: LinphoneRegistrationFailed"));
	addExample(new DaemonCommandExample("register-status 3",
						"Status: Error\n"
						"Reason: No register with such id."));
}

void RegisterStatusCommand::exec(Daemon *app, const string& args) {
	LinphoneProxyConfig *cfg = NULL;
    ostringstream ost;
	string param;
	int pid;

	istringstream ist(args);
	ist >> param;
	if (ist.fail()) {
		app->sendResponse(Response("Missing parameter.", COMMANDNAME_REGISTER_STATUS, Response::Error));
		return;
	}
	if (param.compare("ALL") == 0) {
		for (int i = 1; i <= app->maxProxyId(); i++) {
			cfg = app->findProxy(i);
			if (cfg != NULL) {
                ost << "ProyyId: " << i << "\n" << "ProxyAddress: " << linphone_proxy_config_get_server_addr(cfg) << "\n" << "ProxyIdentity: " << linphone_proxy_config_get_identity(cfg) << "\n" << "State: " << linphone_registration_state_to_string(linphone_proxy_config_get_state(cfg)) << "\n";
            }
		}
		app->sendResponse(Response(ost.str(), COMMANDNAME_REGISTER_STATUS, Response::Ok));
	} else {
		ist.clear();
		ist.str(param);
		ist >> pid;
		if (ist.fail()) {
			app->sendResponse(Response("Incorrect parameter.", COMMANDNAME_REGISTER_STATUS, Response::Error));
			return;
		}
		cfg = app->findProxy(pid);
		if (cfg == NULL) {
			app->sendResponse(Response("No register with such id.", COMMANDNAME_REGISTER_STATUS, Response::Error));
			return;
		}
        ost << "ProyyId: " << pid << "\n" << "ProxyAddress: " << linphone_proxy_config_get_server_addr(cfg) << "\n" << "ProxyIdentity: " << linphone_proxy_config_get_identity(cfg);
		app->sendResponse(Response(ost.str(), COMMANDNAME_REGISTER_STATUS, Response::Ok));
	}
}
