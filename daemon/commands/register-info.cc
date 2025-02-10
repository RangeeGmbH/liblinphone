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

#include "register-info.h"

#include <stdexcept>
#include <string>

#include "linphone/api/c-account-params.h"
#include "linphone/api/c-account.h"
#include "linphone/api/c-address.h"

using namespace std;

RegisterInfoCommand::RegisterInfoCommand():
        DaemonCommand("register-info", "register-info <register_id>|ALL",
                      "Get informations about one or more registrations.")
{
    addExample(make_unique<DaemonCommandExample>("register-info 1",
                                        "Status: Ok\n\n"
                                        "Id: 1\n"
                                        "Identity: sip:toto@sip.linphone.org\n"
                                        "Proxy: <sip:sip.linphone.org;transport=tls>\n"
                                        "Route: <sip:sip.linphone.org;transport=tls>\n"
                                        "State: LinphoneRegistrationOk"));
    addExample(make_unique<DaemonCommandExample>("register-info ALL",
                                        "Status: Ok\n\n"
                                        "Id: 1\n"
                                        "Identity: sip:toto@sip.linphone.org\n"
                                        "Proxy: <sip:sip.linphone.org;transport=tls>\n"
                                        "Route: <sip:sip.linphone.org;transport=tls>\n"
                                        "State: LinphoneRegistrationOk\n\n"
                                        "Id: 2\n"
                                        "Identity: sip:toto2@sip.linphone.org\n"
                                        "Proxy: <sip:sip.linphone.org;transport=udp>\n"
                                        "State: LinphoneRegistrationFailed"));
    addExample(make_unique<DaemonCommandExample>("register-info 3",
                                        "Status: Error\n"
                                        "Reason: No register with such id."));
}

void RegisterInfoCommand::exec(Daemon *app, const string &args) {
	string param;
	istringstream ist(args);
	ist >> param;
	ostringstream ost;
	if (ist.fail()) {
		app->sendResponse(Response("", "Missing parameter.", Response::Error));
		return;
	}
	if (param == "ALL") {
		for (int i = 1; i <= app->maxProxyId(); i++) {
			::LinphoneAccount *account = app->findProxy(i);
			if (account != NULL) {
				const LinphoneAccountParams *params = linphone_account_get_params(account);
				ost << "ProyyId: " << i << endl;
				ost << "ProxyAddress: " << linphone_account_params_get_server_addr(params) << endl;
				ost << "ProxyIdentity: " << linphone_account_params_get_identity(params) << endl;
				const bctbx_list_t *routes = linphone_account_params_get_routes_addresses(params);
				char *route = NULL;
				if (routes) route = linphone_address_as_string((LinphoneAddress *)routes->data);
				if (route != NULL) {
					ost << "Route: " << route << endl;
				}
				ost << "State: " << linphone_registration_state_to_string(linphone_account_get_state(account)) << endl;
			}
			else {
				app->sendResponse(Response(COMMANDNAME_REGISTER_INFO, "Invalid ID.", Response::Error));
			}
		}
		app->sendResponse(Response(COMMANDNAME_REGISTER_INFO, ost.str(), Response::Ok));
	} else {
		int id;
		try {
			id = atoi(param.c_str());
		} catch (invalid_argument &) {
			app->sendResponse(Response(COMMANDNAME_REGISTER_INFO, "Invalid ID.", Response::Error));
			return;
		} catch (out_of_range &) {
			app->sendResponse(Response(COMMANDNAME_REGISTER_INFO, "Out of range ID.", Response::Error));
			return;
		}
		::LinphoneAccount *account = app->findProxy(id);
		if (account == NULL) {
			app->sendResponse(Response(COMMANDNAME_REGISTER_INFO, "No register with such id.", Response::Error));
			return;
		}
		const LinphoneAccountParams *params = linphone_account_get_params(account);
		ost << "ProyyId: " << id << endl;
		ost << "ProxyAddress: " << linphone_account_params_get_server_addr(params) << endl;
		ost << "ProxyIdentity: " << linphone_account_params_get_identity(params) << endl;
		const bctbx_list_t *routes = linphone_account_params_get_routes_addresses(params);
		char *route = NULL;
		if (routes) route = linphone_address_as_string((LinphoneAddress *)routes->data);
		if (route != NULL) {
			ost << "Route: " << route << endl;
		}
		ost << "State: " << linphone_registration_state_to_string(linphone_account_get_state(account)) << endl;
		app->sendResponse(Response(COMMANDNAME_REGISTER_INFO, ost.str(), Response::Ok));
	}
}
