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
DaemonCommand("volume", "volume <call-iD> [<value for speaker volume> <value for record volume>]",
              "set or get the volume around value for speaker volume and record volume") {
    addExample(new DaemonCommandExample("volume 1",
                                        "Status: OK\n"));
    addExample(new DaemonCommandExample("volume 1 0.xx or 1.00 0.xx or 1.00",
                                        "Status: OK\n"));
}
void VolumeCommand::exec(Daemon *app, const string &args) {
    string callId;
    float playbackVolume;
    float recordVolume;
    istringstream ist(args);

    //ist >> playbackVolume;
    //ist >> recordVolume;

    string param;
    ist >> param;

    ostringstream ost;

    if (ist.fail()) {
        app->sendResponse(Response(COMMANDNAME_VOLUME, "Missing parameter", Response::Error));
    }
    if(param == "default"){
        ist >> playbackVolume;
        ist >> recordVolume;
        if (ist.fail()) {
            app->sendResponse(Response(COMMANDNAME_VOLUME, "Missing parameter", Response::Error));
        }
        else {
            //output
            linphone_core_set_playback_gain_db(app->getCore(), playbackVolume);
            linphone_core_set_mic_gain_db(app->getCore(), recordVolume);
            /*app->getCore()->sound_conf.soft_play_lev=app->linearToDb(playbackVolume);
            if (linphone_core_ready(app->getCore())){
                linphone_config_set_float(app->getCore()->config,"sound","playback_gain_db",app->getCore()->sound_conf.soft_play_lev);
            }*/
            //input
            /*app->getCore()->sound_conf.soft_mic_lev=app->linearToDb(recordVolume);
            if (linphone_core_ready(app->getCore())){
                linphone_config_set_float(app->getCore()->config,"sound","mic_gain_db",app->getCore()->sound_conf.soft_mic_lev);
            }*/
        }
    }
    else {
        ist >> callId;
        ist >> playbackVolume;
        ist >> recordVolume;

        if(callId.empty()) {
            ost << "Missing parameter.";
            std::string commandName = COMMANDNAME_VOLUME;
            app->sendResponse(Response(commandName, ost.str(), Response::Error));
            return;
        }
        LinphoneCall *call = NULL;
        call = app->findCall(std::stoi(callId));
        if (call == NULL) {
            ost << "No call with such id.";
            std::string commandName = COMMANDNAME_VOLUME;
            app->sendResponse(Response(commandName, ost.str(), Response::Error));
            return;
        }
#ifdef test
        if(!playbackVolume.empty() && !recordVolume.empty()) {
            int speakerVolume;
            speakerVolume = std::stoi(playbackVolume);
            if( speakerVolume == 100 ) {
                playbackVolume = "1.00";
            }
            else if ( speakerVolume < 100 ) {
                playbackVolume = "0." + to_string(speakerVolume);
            }

            int micVolume;
            micVolume = std::stoi(recordVolume);
            if( micVolume == 100 ) {
                recordVolume = "1.00";
            }
            else if ( micVolume < 100 ) {
                recordVolume = "0." + to_string(micVolume);
            }

            linphone_call_set_speaker_volume_gain(call, std::stof(playbackVolume));
            linphone_call_set_microphone_volume_gain(call, std::stof(recordVolume));
        }
        float playbackVolumeFloat = linphone_call_get_speaker_volume_gain(call);
        char strPlayback[40];
        sprintf(strPlayback, "%.2f", playbackVolumeFloat);
        sscanf(strPlayback, "%f", &playbackVolumeFloat);
        float recordVolumeFloat = linphone_call_get_microphone_volume_gain(call);
        char strRecord[40];
        sprintf(strRecord, "%.2f", recordVolumeFloat);
        sscanf(strRecord, "%f", &recordVolumeFloat);

        if(playbackVolumeFloat >=0 && recordVolumeFloat >=0){
            Response resp;
            string volumePlayback = to_string(playbackVolumeFloat);
            string volumeErgPlayback = "Current playback volume: ";
            volumeErgPlayback = volumeErgPlayback + volumePlayback + "\n";
            string volumeRecord = to_string(recordVolumeFloat);
            string volumeErgRecord = "Current record volume: ";
            volumeErgRecord = volumeErgRecord + volumeRecord + "\n";
            string callId = "CallID: ";
            string callIdErg = callId + to_string(app->updateCallId(call)) + "\n";

            ost << "{ \"Current playback volume\": " << "\"" << volumePlayback.c_str() << "\"" << ", \"Current record volume\": " << "\"" << volumeRecord.c_str()
            << "\"" << ",  \"CallID\": " << "\"" << to_string(app->updateCallId(call)).c_str() << "\"" << " }";
            app->sendResponse(Response(COMMANDNAME_VOLUME, ost.str(), Response::Ok));
        }
#endif
    }
}