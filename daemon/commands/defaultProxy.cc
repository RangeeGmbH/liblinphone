//
// Created by shucko on 16.01.24.
//

#include "defaultProxy.h"

using namespace std;

DefaultProxy::DefaultProxy() :
        DaemonCommand("defaultProxy", "defaultProxy <proxy_id>", "Get or set the default Proxy config") {
    addExample(make_unique<DaemonCommandExample>("unregister 2",
                                                 "Status: Ok"));
    addExample(make_unique<DaemonCommandExample>("unregister",
                                                 "Status: Ok"));
}

void DefaultProxy::exec(Daemon *app, const string& args) {
    LinphoneProxyConfig *cfg = NULL;
    string param;
    int pid;
    ostringstream ost;
    string proxysStr;

    istringstream ist(args);
    ist >> param;
    if (ist.fail()) {
        //get
        cfg = linphone_core_get_default_proxy_config(app->getCore());
        if (cfg == NULL) {
            proxysStr = "No Default Proxy";
            app->sendResponse(Response(COMMANDNAME_DEFAULTPROXY, proxysStr, Response::Ok));
            return;
        }
        else {
            proxysStr += "{ \"isAll\": false, \"proxies\": [ ";
            proxysStr += app->getJsonForProxys(cfg);
            proxysStr += " ] }";
            app->sendResponse(Response(COMMANDNAME_DEFAULTPROXY, proxysStr, Response::Ok));
        }
        return;
    }
     else {
        ist.clear();
        ist.str(param);
        ist >> pid;
        if (ist.fail()) {
            ost << "\"Incorrect parameter\"";
            app->sendResponse(Response(COMMANDNAME_DEFAULTPROXY, ost.str(), Response::Error));
            return;
        }
        cfg = app->findProxy(pid);
        if (cfg == NULL) {
            ost << "\"Incorrect parameter\"";
            app->sendResponse(Response(COMMANDNAME_DEFAULTPROXY, ost.str(), Response::Error));
            return;
        } else {
            //set
            cfg = app->findProxy(pid);
            linphone_core_set_default_proxy_config(app->getCore(), cfg);
            proxysStr += "{ \"isAll\": false, \"proxies\": [ ";
            proxysStr += app->getJsonForProxys(cfg);
            proxysStr += " ] }";
        }
        app->sendResponse(Response(COMMANDNAME_DEFAULTPROXY, proxysStr, Response::Ok));
    }
}
