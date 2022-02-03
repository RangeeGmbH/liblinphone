//
// Created by stefanh on 02.11.21.
//

#include "maxCall.h"

MaxCallsCommand::MaxCallsCommand() :
DaemonCommand("maxcalls", "maxcalls",
              "set or get the maximum number of simultaneous calls Linphone core can manage at a time.") {
    addExample(new DaemonCommandExample("maxcalls",
                                        "Status: OK\n"));
    addExample(new DaemonCommandExample("maxcalls 1",
                                        "Status: OK\n"));
}

void MaxCallsCommand::exec(Daemon *app, const string &args) {
    istringstream ist(args);
    istringstream ivalue(args);
    string param;
    char ost[240];
    int value;
    ist >> param;
    if (param == "") {
        string maxCalls = to_string(linphone_core_get_max_calls(app->getCore()));
        sprintf(ost, "{ \"Maximum number of calls: \": \"%d\" }",  linphone_core_get_max_calls(app->getCore()));
        app->sendResponse(Response(COMMANDNAME_MAXCALLS, ost, Response::Ok));
    }
    else {
        ist >> param;
        ivalue >> value;
        linphone_core_set_max_calls(app->getCore(), value);
        sprintf(ost, "{ \"MaxCalls was set successfully. Maximum number of calls \": \"%d\" }", linphone_core_get_max_calls(app->getCore()));
        app->sendResponse(Response(COMMANDNAME_MAXCALLS, ost, Response::Ok));
    }
}