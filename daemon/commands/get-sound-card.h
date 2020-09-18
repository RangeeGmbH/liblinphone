//
// Created by stefanh on 17.09.20.
//

#ifndef LINPHONE_SDK_GETSOUNDCARD_H
#define LINPHONE_SDK_GETSOUNDCARD_H


#include "daemon.h"
#define COMMANDNAME_GETSOUNDCARD "get-sound-card"

using namespace std;

class GetSoundCard: public DaemonCommand  {
public:
    GetSoundCard();
    void exec(Daemon *app, const std::string &args) override;
};


#endif //LINPHONE_SDK_GETSOUNDCARD_H
