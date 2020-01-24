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
#include "private.h"

using namespace std;

VolumeCommand::VolumeCommand() :
        DaemonCommand("volume", "volume set value | volume get",
                      "set the volume around value or get the current volume") {
    addExample(new DaemonCommandExample("value set 1",
                                        "Status: OK\n"));
    addExample(new DaemonCommandExample("value get",
                                        "Status: OK\n"));
}

long VolumeCommand::audioVolumeSet(audio_volume_action action, long *outvol, Daemon *app) {
    snd_mixer_t *handle;
    snd_mixer_elem_t *elem;
    snd_mixer_selem_id_t *sid;

    //static const char *mix_name = "Master";
    static const char *card = "";
    //const char *read_value = "";
    char **cardname;
    cardname = new char*[0];
    //int cardindex;


    int totalCards = 0;   // No cards found yet
    int cardNum = -1;     // Start with first card
    int err;

    for (;;) {
        // Get next sound card's card number.
        if ((err = snd_card_next(&cardNum)) < 0) {
            fprintf(stderr, "Can't get the next card number: %s\n",
                    snd_strerror(err));
            break;
        }
        snd_card_get_name(cardNum, cardname);
        if (cardNum < 0)
            // No more cards
            break;

        ++totalCards;   // Another card found, so bump the count
    }



    switch (action){
        case audio_volume_action::AUDIO_VOLUME_SET_PLAYBACK_DEVICE:
            //read_value = lp_config_get_string(linphone_core_get_config(app->getCore()),"sound","playback_dev_id",NULL);
            //mix_name = read_value;
            //card  = lp_config_get_string(linphone_core_get_config(app->getCore()),"sound","playback_dev_id",NULL);

            card = "Microsoft LifeChat LX-3000";
            snd_card_get_name(2, cardname);
            //cardindex = snd_card_get_index("2");

            break;
        case audio_volume_action::AUDIO_VOLUME_GET_PLAYBACK_DEVICE:
            break;
    }

    //static const char *mix_name = "Master";
    //static const char* mix_name = "Capture";
    //static const char *card = "default";
//    static int mix_index = cardindex;
    int ret = 0;

    snd_mixer_selem_id_alloca(&sid);

    //sets simple-mixer index and name
    //snd_mixer_selem_id_set_index(sid, mix_index);
    //snd_mixer_selem_id_set_name(sid, mix_name);

    if ((snd_mixer_open(&handle, 0)) < 0) {
        *outvol = -1;
        return *outvol;
    }
    if ((snd_mixer_attach(handle, card)) < 0) {
        snd_mixer_close(handle);
        *outvol = -2;
        return *outvol;
    }
    if ((snd_mixer_selem_register(handle, NULL, NULL)) < 0) {
        snd_mixer_close(handle);
        *outvol = -3;
        return *outvol;
    }
    ret = snd_mixer_load(handle);
    if (ret < 0) {
        snd_mixer_close(handle);
        *outvol = -4;
        return *outvol;
    }
    elem = snd_mixer_find_selem(handle, sid);
    if (!elem) {
        snd_mixer_close(handle);
        *outvol = -5;
        return *outvol;
    }
    long minv = 0;
    long maxv = 0;

    snd_mixer_selem_get_playback_volume_range(elem, &minv, &maxv);
    if (action == AUDIO_VOLUME_SET_PLAYBACK_DEVICE) {
        //if(*outvol < 0 || *outvol > VOLUME_BOUND) // out of bounds
        //   return -7;

        //*outvol = (*outvol * (maxv - minv) / (100-1)) + minv;
        *outvol = (*outvol * (maxv - minv) / 100) + minv;

        if (snd_mixer_selem_set_playback_volume(elem, SND_MIXER_SCHN_FRONT_RIGHT, *outvol) < 0) {
            snd_mixer_close(handle);
            return *outvol = -8;
        }
        if (snd_mixer_selem_set_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, *outvol) < 0) {
            snd_mixer_close(handle);
            return *outvol = -9;
        }
    }
    snd_mixer_close(handle);
    return *outvol;
}

long VolumeCommand::audioVolumeGet(audio_volume_action action, Daemon *app) {
    long outvol = 0;
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

    if ((snd_mixer_open(&handle, 0)) < 0) {
        return -1;
    }
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
    if (action == AUDIO_VOLUME_GET_PLAYBACK_DEVICE) {
        // if (snd_mixer_selem_has_playback_switch(elem)) {
        //      snd_mixer_selem_set_playback_switch_all(elem, 0);
        //}
        //if(snd_mixer_selem_get_capture_volume(elem, SND_MIXER_SCHN_FRONT_RIGHT, outvol) < 0) {
        //    snd_mixer_close(handle);
        //    return -6;
        //}

        //if(snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_UNKNOWN, outvol) < 0) {
        if (snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_FRONT_RIGHT, &outvol) < 0) {
            snd_mixer_close(handle);
            return -6;
        }
        float floatVolume = 100 * (float) outvol / (float) maxv;
        floatVolume = round(floatVolume);
        long erg = (long) floatVolume;
        snd_mixer_close(handle);
        return erg;
    }
    return -7;
}

void VolumeCommand::exec(Daemon *app, const string &args) {

    MSFactory* factory = app->getCore()->factory;
    MSSndCardManager *manager = ms_factory_get_snd_card_manager(factory);
    //MSSndCard *capture_card = ms_snd_card_manager_get_card(manager, linphone_core_get_capture_device(app->getCore()));
    MSSndCard *play_card = ms_snd_card_manager_get_card(manager, linphone_core_get_playback_device(app->getCore()));


    LinphoneCore *lc = app->getCore();
    const char *devid;
    //devid=lp_config_get_string(lc->config,"sound","playback_dev_id",NULL);
    devid=lp_config_get_string(linphone_core_get_config(app->getCore()),"sound","playback_dev_id",NULL);
    //linphone_core_set_playback_device(lc,devid);
    MSSndCard *play_card2 = ms_snd_card_manager_get_card(manager, devid);

    //play_card2 = get_card_from_string_id(devid, 0, factory);
    //play_card2 = ms_snd_card_get_string_id(devid, 0, factory);

    //lc->sound_conf.play_lev = (char)level;

    play_card2=lc->sound_conf.play_sndcard;

    printf("%s", play_card2->name);


   // ms_snd_card_set_level()


    /*MSSndCard *sndcard;
    lc->sound_conf.play_lev = (char)level;
    sndcard=lc->sound_conf.play_sndcard;
    if (sndcard) ms_snd_card_set_level(sndcard,MS_SND_CARD_PLAYBACK,level);*/

    const char *read_value = "";
    read_value = lp_config_get_string(linphone_core_get_config(app->getCore()),"sound","playback_dev_id",NULL);
    printf("%s", read_value);
    Response resp;
    resp.setBody(play_card->name);
    app->sendResponse(resp);


    istringstream ist(args);
    string param;
    long value;
    ist >> param;
    if (ist.fail()) {
        Response resp;
        resp.setBody("Status: Error\nReason: Missing parameter");
        app->sendResponse(resp);
    }
    if (param == "set") {
        ist >> param;
        if(ist.fail()) {
            Response resp;
            resp.setBody("Status: Error\nReason: Missing parameter");
            app->sendResponse(resp);
        } else{
            if(param == "playback_dev"){
                ist >> value;
                if(ist.fail()) {
                    Response resp;
                    resp.setBody("Status: Error\nReason: Missing or wrong value");
                    app->sendResponse(resp);
                } else{
                    audio_volume_action action = audio_volume_action::AUDIO_VOLUME_SET_PLAYBACK_DEVICE;
                    value = audioVolumeSet(action, &value, app);
                    if(value >=0){
                        Response resp;
                        string respMessage = "Volume was set successfully";
                        resp.setBody(respMessage.c_str());
                        app->sendResponse(resp);
                    }
                }
            }
        }
    }
    if (param == "get") {
        value = audioVolumeGet(AUDIO_VOLUME_GET_PLAYBACK_DEVICE, app);
        if(value >=0){
            Response resp;
            string volume = to_string(value);
            string volumeErg = "Current volume: ";
            volumeErg = volumeErg + volume;
            resp.setBody(volumeErg.c_str());
            app->sendResponse(resp);
        }
    }
    if (param != "get" && param != "set" && param != "" && param != "playback_dev"){
        Response resp;
        resp.setBody("Status: Error\nReason: Wrong parameter");
        app->sendResponse(resp);
    }
}