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

#include "register.h"
#include "linphone/core.h"
#include "linphone/lpconfig.h"
#include "private.h"
#include <string.h>

using namespace std;

RegisterCommand::RegisterCommand()
    : DaemonCommand("register",
                    "register <identity> <proxy_address> [<password>] [<userid>] [<realm>] [<parameter>]",
                    "Register the daemon to a SIP proxy. If one of the parameters <password>, <userid> and <realm> is "
                    "not needed, send the string \"NULL\"") {
	addExample(make_unique<DaemonCommandExample>(
	    "register sip:daemon-test@sip.linphone.org sip.linphone.org password bob linphone.org", "Status: Ok\n\n"
	                                                                                            "Id: 1"));
}
void RegisterCommand::exec(Daemon *app, const string &args) {
	LinphoneCore *lc = app->getCore();
	ostringstream ost;
	string identity;
	string proxy;
	string password;
	string userid;
	string realm;
	string parameter;
	istringstream ist(args);
	const char *cidentity;
	const char *cproxy;
	const char *cpassword = NULL;
	const char *cuserid = NULL;
	const char *crealm = NULL;
	const char *cparameter = NULL;
	string proxysStr;

	ist >> identity;
	ist >> proxy;
	ist >> password;
	ist >> userid;
	ist >> realm;
	ist >> parameter;
	if (proxy.empty()) {
	    ost << "\"Missing/Incorrect parameter(s).\"";
	    app->sendResponse(Response(COMMANDNAME_REGISTER, ost.str(), Response::Error));
		return;
	}
	cidentity = identity.c_str();
	cproxy = proxy.c_str();
	if (!password.empty() && (password.compare("NULL") != 0)) cpassword = password.c_str();
	if (!userid.empty() && (userid.compare("NULL") != 0)) cuserid = userid.c_str();
	if (!realm.empty() && (realm.compare("NULL") != 0)) crealm = realm.c_str();
	if (!parameter.empty()) cparameter = parameter.c_str();
	LinphoneProxyConfig *cfg = linphone_core_create_proxy_config(NULL);
	LinphoneAddress *from = linphone_address_new(cidentity);
	if (cpassword != NULL) {
		if (from != NULL) {
			LinphoneAuthInfo *info =
			    linphone_auth_info_new(linphone_address_get_username(from), cuserid, cpassword, NULL, crealm, NULL);
			linphone_core_add_auth_info(lc, info); /* Add authentication info to LinphoneCore */
			linphone_auth_info_unref(info);
		}
	}
	linphone_proxy_config_set_identity_address(cfg, from);
	if (from) linphone_address_unref(from);
	linphone_proxy_config_set_contact_parameters(cfg, cparameter);
	linphone_proxy_config_set_server_addr(cfg, cproxy);
	linphone_proxy_config_enable_register(cfg, TRUE);
	linphone_proxy_config_set_contact_parameters(cfg, cparameter);
	linphone_core_add_proxy_config(lc, cfg);

	cfg = app->findProxy(app->updateProxyId(cfg));
	proxysStr += "{ \"isAll\": false, \"proxies\": [ ";
	proxysStr += app->getJsonForProxys(cfg);
	proxysStr += " ]  }";

	app->sendResponse(Response(COMMANDNAME_REGISTER, proxysStr, Response::Ok));
}
