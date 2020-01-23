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

#include "volume.h"
#include <unistd.h>
#include <fcntl.h>
#include <regex>
#include <alsa/asoundlib.h>
#include <math.h>

using namespace std;

volume::volume() :
        DaemonCommand("volume", "volume set value | volume get",
                      "set the volume around value or get the current volume") {
    addExample(new DaemonCommandExample("value set 1",
                                        "Status: OK\n"));
    addExample(new DaemonCommandExample("value get",
                                        "Status: OK\n"));
}

int volume::audio_volume(audio_volume_action action, long *outvol, Daemon *app) {
    snd_mixer_t *handle;
    snd_mixer_elem_t *elem;
    snd_mixer_selem_id_t *sid;

    static const char *mix_name = "Master";
    //static const char* mix_name = "Capture";
    static const char *card = "default";
    static int mix_index = 0;
    int ret = 0;

    snd_mixer_selem_id_alloca(&sid);

    //sets simple-mixer index and name
    snd_mixer_selem_id_set_index(sid, mix_index);
    snd_mixer_selem_id_set_name(sid, mix_name);

    if ((snd_mixer_open(&handle, 0)) < 0)
        return -1;
    if ((snd_mixer_attach(handle, card)) < 0) {
        snd_mixer_close(handle);
        return -2;
    }
    if ((snd_mixer_selem_register(handle, NULL, NULL)) < 0) {
        snd_mixer_close(handle);
        return -3;
    }
    ret = snd_mixer_load(handle);
    if (ret < 0) {
        snd_mixer_close(handle);
        return -4;
    }
    elem = snd_mixer_find_selem(handle, sid);
    if (!elem) {
        snd_mixer_close(handle);
        return -5;
    }
    long minv = 0;
    long maxv = 0;

    snd_mixer_selem_get_playback_volume_range(elem, &minv, &maxv);

    if (action == AUDIO_VOLUME_GET) {

        // if (snd_mixer_selem_has_playback_switch(elem)) {
        //      snd_mixer_selem_set_playback_switch_all(elem, 0);
        //}
        //if(snd_mixer_selem_get_capture_volume(elem, SND_MIXER_SCHN_FRONT_RIGHT, outvol) < 0) {
        //    snd_mixer_close(handle);
        //    return -6;
        //}

        //if(snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_UNKNOWN, outvol) < 0) {
        if (snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_FRONT_RIGHT, outvol) < 0) {
            snd_mixer_close(handle);
            return -6;
        }
        float floatVolume = 100 * (float) (*outvol) / (float) maxv;
        floatVolume = round(floatVolume);
        long erg = (long) floatVolume;
        Response resp;
        string volume = to_string(erg);
        string volumeErg = "Current volume: ";
        volumeErg = volumeErg + volume;
        resp.setBody(volumeErg.c_str());
        app->sendResponse(resp);
    } else if (action == AUDIO_VOLUME_SET) {
        //if(*outvol < 0 || *outvol > VOLUME_BOUND) // out of bounds
        //   return -7;

        //*outvol = (*outvol * (maxv - minv) / (100-1)) + minv;
        *outvol = (*outvol * (maxv - minv) / 100) + minv;

        if (snd_mixer_selem_set_playback_volume(elem, SND_MIXER_SCHN_FRONT_RIGHT, *outvol) < 0) {
            snd_mixer_close(handle);
            return -8;
        }
        if (snd_mixer_selem_set_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, *outvol) < 0) {
            snd_mixer_close(handle);
            return -9;
        }
        Response resp;
        string respMessage = "Volume was set successfuly";
        resp.setBody(respMessage.c_str());
        app->sendResponse(resp);
    }
    snd_mixer_close(handle);
    return 0;
}

void volume::exec(Daemon *app, const string &args) {
    istringstream ist(args);
    string value;
    std::size_t foundSet = ist.str().find("set ", 0);
    std::size_t foundGet = ist.str().find("get", 0);
    if (foundSet != std::string::npos) {
        value = ist.str().substr(4, ist.str().length());
        const regex r("^([0-9][0-9]{0,999})$");
        smatch sm;
        if (regex_search(value, sm, r)) {
            long valueLong = stol(value);
            audio_volume(AUDIO_VOLUME_SET, &valueLong, app);
        }
    }
    if (foundGet != std::string::npos) {
        long valueLong = 0;
        audio_volume(AUDIO_VOLUME_GET, &valueLong, app);
    }
}