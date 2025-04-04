//
// Created by shucko on 16.01.24.
//

#include "defaultProxy.h"

using namespace std;

DefaultProxy::DefaultProxy()
    : DaemonCommand("defaultProxy", "defaultProxy <proxy_id>", "Get or set the default Proxy config") {
	addExample(make_unique<DaemonCommandExample>("unregister 2", "Status: Ok"));
	addExample(make_unique<DaemonCommandExample>("unregister", "Status: Ok"));
}

void DefaultProxy::exec(Daemon *app, const string &args) {
	LinphoneAccount *account = NULL;
	string param;
	int pid;
	ostringstream ost;
	string accountStr;

	istringstream ist(args);
	ist >> param;
	if (ist.fail()) {
		// get
		account = linphone_core_get_default_account(app->getCore());
		if (account == NULL) {
			accountStr = "No Default Proxy";
			app->sendResponse(Response(COMMANDNAME_DEFAULTPROXY, accountStr, Response::Ok));
			return;
		} else {
			const LinphoneAccountParams *params = linphone_account_get_params(account);
			accountStr += "{ \"isAll\": false, \"proxies\": [ ";
			accountStr += app->getJsonForAccountParams(params, account);
			accountStr += " ] }";
			app->sendResponse(Response(COMMANDNAME_DEFAULTPROXY, accountStr, Response::Ok));
		}
		return;
	} else {
		ist.clear();
		ist.str(param);
		ist >> pid;
		if (ist.fail()) {
			ost << "\"Incorrect parameter\"";
			app->sendResponse(Response(COMMANDNAME_DEFAULTPROXY, ost.str(), Response::Error));
			return;
		}
		account = app->findProxy(pid);
		if (account == NULL) {
			ost << "\"Incorrect parameter\"";
			app->sendResponse(Response(COMMANDNAME_DEFAULTPROXY, ost.str(), Response::Error));
			return;
		} else {
			// set
			linphone_core_set_default_account(app->getCore(), account);
			const LinphoneAccountParams *params = linphone_account_get_params(account);
			accountStr += "{ \"isAll\": false, \"proxies\": [ ";
			accountStr += app->getJsonForAccountParams(params, account);
			accountStr += " ] }";
		}
		app->sendResponse(Response(COMMANDNAME_DEFAULTPROXY, accountStr, Response::Ok));
	}
}
