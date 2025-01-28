//
// Created by stefanh on 19.09.23.
//

#include "daemon.h"


#define COMMANDNAME_PING "ping"


class PingCommand : public DaemonCommand{
public:
    PingCommand();

    void exec(Daemon *app, const std::string &args) override;
};
