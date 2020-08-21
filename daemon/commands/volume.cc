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
        DaemonCommand("volume", "volume",
                      "set the volume around value for ringer_dev, or capture_dev, or playback_dev, or get the current volume") {
    addExample(new DaemonCommandExample("volume set playback_dev 1",
                                        "Status: OK\n"));
    addExample(new DaemonCommandExample("volume set ringer_dev 1",
                                        "Status: OK\n"));
    addExample(new DaemonCommandExample("volume set capture_dev 1",
                                        "Status: OK\n"));
    addExample(new DaemonCommandExample("volume get playback_dev",
                                        "Status: OK\n"));
    addExample(new DaemonCommandExample("volume get ringer_dev",
                                        "Status: OK\n"));
    addExample(new DaemonCommandExample("volume get capture_dev",
                                        "Status: OK\n"));
}
void VolumeCommand::exec(Daemon *app, const string &args) {
    MSFactory* factory = app->getCore()->factory;
    MSSndCardManager *manager = ms_factory_get_snd_card_manager(factory);
    istringstream ist(args);
    string param;
    int value;
    ist >> param;
    if (ist.fail()) {
        app->sendResponse(Response(COMMANDNAME_VOLUME, "Missing parameter", Response::Error));
    }
    if (param == "set") {
        ist >> param;
        if(ist.fail()) {
            app->sendResponse(Response(COMMANDNAME_VOLUME, "Missing parameter", Response::Error));
        } else{
            if(param == "playback_dev"){
                ist >> value;
                if(ist.fail()) {
                    app->sendResponse(Response(COMMANDNAME_VOLUME, "Missing or wrong value", Response::Error));
                } else{
                    MSSndCard *play_card = ms_snd_card_manager_get_playback_card(manager, linphone_core_get_playback_device(app->getCore()));
                    ms_snd_card_set_level(play_card,MS_SND_CARD_PLAYBACK,value);
                    if(value >=0) {
                        app->sendResponse(Response(COMMANDNAME_VOLUME, "Volume was set successfully\n", Response::Ok));
                    }
                }
            }
            if(param == "ringer_dev"){
                ist >> value;
                if(ist.fail()) {
                    app->sendResponse(Response(COMMANDNAME_VOLUME, "Missing or wrong value", Response::Error));
                } else{
                    MSSndCard *ring_card = ms_snd_card_manager_get_playback_card(manager, linphone_core_get_ringer_device(app->getCore()));
                    ms_snd_card_set_level(ring_card,MS_SND_CARD_PLAYBACK,value);
                    if(value >=0) {
                        app->sendResponse(Response(COMMANDNAME_VOLUME, "Volume was set successfully\n", Response::Ok));
                    }
                }
            }
            if(param == "capture_dev"){
                ist >> value;
                if(ist.fail()) {
                    app->sendResponse(Response(COMMANDNAME_VOLUME, "Missing or wrong value", Response::Error));
                } else{
                    MSSndCard *capture_card = ms_snd_card_manager_get_capture_card(manager, linphone_core_get_capture_device(app->getCore()));
                    ms_snd_card_set_level(capture_card,MS_SND_CARD_CAPTURE,value);
                    if(value >=0) {
                        app->sendResponse(Response(COMMANDNAME_VOLUME, "Volume was set successfully\n", Response::Ok));
                    }
                }
            }
        }
    }
    if (param == "get") {
        ist >> param;
        if(ist.fail()) {
            app->sendResponse(Response(COMMANDNAME_VOLUME, "Missing parameter", Response::Error));
        } else{
            if(param == "playback_dev"){
                MSSndCard *play_card = ms_snd_card_manager_get_playback_card(manager, linphone_core_get_playback_device(app->getCore()));
                value = ms_snd_card_get_level(play_card,MS_SND_CARD_PLAYBACK);
                if(value >=0){
                    Response resp;
                    string volume = to_string(value);
                    string volumeErg = "Current playback volume: ";
                    volumeErg = volumeErg + volume + "\n";
                    app->sendResponse(Response(COMMANDNAME_VOLUME, volumeErg.c_str(), Response::Ok));
                }
            }
            if(param == "ringer_dev"){
                MSSndCard *ring_card = ms_snd_card_manager_get_playback_card(manager, linphone_core_get_ringer_device(app->getCore()));
                value = ms_snd_card_get_level(ring_card,MS_SND_CARD_PLAYBACK);
                if(value >=0){
                    Response resp;
                    string volume = to_string(value);
                    string volumeErg = "Current ring volume: ";
                    volumeErg = volumeErg + volume + "\n";
                    app->sendResponse(Response(COMMANDNAME_VOLUME, volumeErg.c_str(), Response::Ok));
                }
            }
            if(param == "capture_dev"){
                MSSndCard *capture_card = ms_snd_card_manager_get_capture_card(manager, linphone_core_get_capture_device(app->getCore()));
                value = ms_snd_card_get_level(capture_card,MS_SND_CARD_CAPTURE);
                if(value >=0){
                    Response resp;
                    string volume = to_string(value);
                    string volumeErg = "Current capture volume: ";
                    volumeErg = volumeErg + volume + "\n";
                    app->sendResponse(Response(COMMANDNAME_VOLUME, volumeErg.c_str(), Response::Ok));
                }
            }
        }
    }
    if (param != "get" && param != "set" && param != "" && param != "playback_dev" && param != "ringer_dev" && param != "capture_dev"){
        app->sendResponse(Response(COMMANDNAME_VOLUME, "Wrong parameter", Response::Error));
    }
}