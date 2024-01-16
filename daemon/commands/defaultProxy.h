//
// Created by shucko on 16.01.24.
//

#ifndef LINPHONE_SDK_DEFAULTPROXY_H
#define LINPHONE_SDK_DEFAULTPROXY_H


#include "daemon.h"

#define COMMANDNAME_DEFAULTPROXY "defaultProxy"

class DefaultProxy : public DaemonCommand {
public:
    DefaultProxy();

    void exec(Daemon *app, const std::string &args) override;
};


#endif //LINPHONE_SDK_DEFAULTPROXY_H
