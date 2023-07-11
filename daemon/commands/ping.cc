//
// Created by stefanh on 19.09.23.
//

#include "ping.h"
using namespace std;

PingCommand::PingCommand() : DaemonCommand("ping", "ping", "Ping the daemon.") {
    addExample(make_unique<DaemonCommandExample>(
            "ping", "Status: Ok \n\n"));
}

void PingCommand::exec(Daemon *app, BCTBX_UNUSED(const std::string &args)) {
    ostringstream ost;
    ost << "pong";
    app->sendResponse(Response(COMMANDNAME_PING, ost.str(), Response::Ok));
}