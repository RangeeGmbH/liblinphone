//
// Created by stefanh on 23.06.22.
//

#ifndef LINPHONE_SDK_CONFERENCE_MUTE_H
#define LINPHONE_SDK_CONFERENCE_MUTE_H


#include "daemon.h"

#define COMMANDNAME_CONFERENCE_MUTE "conference-mute"

class ConferenceMuteCommand : public DaemonCommand
        {
        public:
            ConferenceMuteCommand();

            void exec(Daemon *app, const std::string& args) override;
        };

#endif //LINPHONE_SDK_CONFERENCE_MUTE_H
