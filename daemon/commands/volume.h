/*
 * Copyright (c) 2010-2019 Belledonne Communications SARL.
 * Copyright (c) 2019 Rangee GmbH.
 *
 * This file is part of Liblinphone.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * The code of this class was inspired by
 * https://stackoverflow.com/a/7661176
 */

#ifndef LINPHONE_SDK_VOLUME_H
#define LINPHONE_SDK_VOLUME_H

#include "daemon.h"

class volume: public DaemonCommand  {
public:
    volume();

    typedef enum {
        AUDIO_VOLUME_SET,
        AUDIO_VOLUME_GET,
    } audio_volume_action;

    long audioVolumeSet(audio_volume_action action, long* outvol, Daemon *app);
    long audioVolumeGet(audio_volume_action action, Daemon *app);

    void exec(Daemon *app, const std::string &args) override;
};


#endif //LINPHONE_SDK_VOLUME_H
