//
// Created by stefanh on 15.09.20.
//

#ifndef LINPHONE_SDK_ADDSOUNDCARDS_H
#define LINPHONE_SDK_ADDSOUNDCARDS_H


#include "daemon.h"
#define COMMANDNAME_SETSOUNDCARD "set-sound-card"

using namespace std;

class SetSoundCard: public DaemonCommand  {
public:
    SetSoundCard();
    void exec(Daemon *app, const std::string &args) override;
};


#endif //LINPHONE_SDK_ADDSOUNDCARDS_H
