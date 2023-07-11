//
// Created by stefanh on 12.04.22.
//

#ifndef LINPHONE_SDK_FRIEND_H
#define LINPHONE_SDK_FRIEND_H

#include "daemon.h"
#define COMMANDNAME_FRIENDS "friends"

using namespace std;

class Friends: public DaemonCommand

{
public:
    Friends();
    void exec(Daemon *app, const std::string &args) override;
};


#endif //LINPHONE_SDK_FRIEND_H
