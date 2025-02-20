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

#include "unregister.h"

using namespace std;

UnregisterCommand::UnregisterCommand()
    : DaemonCommand("unregister",
                    "unregister <register_id>|ALL",
                    "Unregister the daemon from the specified proxy or from all proxies.") {
	addExample(make_unique<DaemonCommandExample>("unregister 3", "Status: Error\n"
	                                                             "Reason: No register with such id."));
	addExample(make_unique<DaemonCommandExample>("unregister 2", "Status: Ok"));
	addExample(make_unique<DaemonCommandExample>("unregister ALL", "Status: Ok"));
}

void UnregisterCommand::exec(Daemon *app, const string &args) {
	LinphoneAccount *account = NULL;
	string param;
	int pid;
	ostringstream ost;
	string accountStr;

	istringstream ist(args);
	ist >> param;
	if (ist.fail()) {
		ost << "\"Missing parameter\"";
		app->sendResponse(Response(COMMANDNAME_UNREGISTER, ost.str(), Response::Error));
		return;
	}
	if (param.compare("ALL") == 0) {
		// Erase all proxies from config
		accountStr += "{ \"isAll\": true, \"proxies\": [ ";
		for (int i = 1; i <= app->maxProxyId(); i++) {
			account = app->findProxy(i);
			if (account != NULL) {
				const LinphoneAccountParams *params = linphone_account_get_params(account);
				accountStr += app->getJsonForAccountParams(params, account);
				linphone_core_remove_account(app->getCore(), account);
				if (i < app->maxProxyId() - 1) {
					accountStr += ",";
				}
			}
		}
		accountStr += " ] }";
		linphone_core_clear_proxy_config(app->getCore());
		app->sendResponse(Response(COMMANDNAME_UNREGISTER, accountStr, Response::Ok));
	} else {
		ist.clear();
		ist.str(param);
		ist >> pid;
		if (ist.fail()) {
			ost << "\"Incorrect parameter\"";
			app->sendResponse(Response(COMMANDNAME_UNREGISTER, ost.str(), Response::Error));
			return;
		}
		account = app->findProxy(pid);
		if (account == NULL) {
			ost << "\"No registe with such id.\"";
			app->sendResponse(Response(COMMANDNAME_UNREGISTER, ost.str(), Response::Error));
			return;
		} else {
			const LinphoneAccountParams *params = linphone_account_get_params(account);
			accountStr += "{ \"isAll\": false, \"proxies\": [ ";
			accountStr += app->getJsonForAccountParams(params, account);
			accountStr += " ] }";
		}
		linphone_core_remove_account(app->getCore(), account);
		app->sendResponse(Response(COMMANDNAME_UNREGISTER, accountStr, Response::Ok));
	}
}
