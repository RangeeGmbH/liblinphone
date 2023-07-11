//
// Created by stefanh on 02.11.21.
//

#ifndef LINPHONE_SDK_MAXCALL_H
#define LINPHONE_SDK_MAXCALL_H

#include "daemon.h"
#define COMMANDNAME_MAXCALLS "maxcalls"

using namespace std;

class MaxCallsCommand: public DaemonCommand  {
public:
    MaxCallsCommand();
    void exec(Daemon *app, const std::string &args) override;
};


#endif //LINPHONE_SDK_MAXCALL_H
