/*
 * Copyright (c) 2010-2019 Belledonne Communications SARL.
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
 */

#include "conference.h"

using namespace std;

ConferenceCommand::ConferenceCommand() :
	DaemonCommand("conference", "conference add|rm|leave|enter <call_id>",
				  "Create and manage an audio conference.\n"
				  "You can:\n"
				  "- add   : join the call with id 'call id' into the audio conference. Creates new one if none exists.\n"
				  "- rm    : remove the call with id 'call id' from the audio conference.\n"
				  "- leave : temporarily leave the current conference.\n"
				  "- enter : re-join the conference after leaving it.")
{
	addExample(new DaemonCommandExample("conference add 1",
										"Status: Ok\n\n"
										"Call Id: 1\n"
										"Conference: add OK"));
	addExample(new DaemonCommandExample("conference leave 1",
										"Status: Ok\n\n"
										"Call Id: 1\n"
										"Conference: leave OK"));
	addExample(new DaemonCommandExample("conference azerty 1",
										"Status: Error\n\n"
										"Reason: Invalid command format"));
	addExample(new DaemonCommandExample("conference leave 2",
										"Status: Error\n\n"
										"Reason: No call with such id."));
}

void ConferenceCommand::exec(Daemon* app, const string& args) {
	LinphoneCore* lc = app->getCore();
	int id;
	string subcommand;
	int ret;
	istringstream ist(args);
	ist >> subcommand;
	ist >> id;
	ostringstream ost;

	if (subcommand.compare("list") == 0) {
	    LinphoneConference *conference = linphone_core_get_conference(lc);
	    if (conference == NULL) {
	        ost << "No conference in progress. Can't list conference.";
	        app->sendResponse(Response(COMMANDNAME_CONFERENCE, ost.str(), Response::Error));
            return;
	    } else {
	        const MSList *elem;
	        elem = linphone_conference_get_participant_list(conference);
	        string conferenceString;
	        conferenceString += "{ \"isAll\": true, \"conference\": { \"participants\": [ ";
	        conferenceString += "\"";
	        for (int index = 0; index < ms_list_size(elem); index++) {
	            LinphoneParticipant* lLinphoneParticipant = (LinphoneParticipant*) bctbx_list_nth_data(elem,index);
	            char * address;
	            const LinphoneAddress *lLinphoneAddress = linphone_participant_get_address(lLinphoneParticipant);
	            address = linphone_address_as_string(lLinphoneAddress);
	            conferenceString += address;
	            conferenceString += "\"";

	            if(index < ms_list_size(elem)-1) {
	                conferenceString += ",";
	            }
	        }

	        //conferenceString += " ] }";
	        conferenceString += " ],";
	        conferenceString += app->getJsonForConference(conference);
	        conferenceString += " }";
	        app->sendResponse(Response(COMMANDNAME_CONFERENCE, conferenceString, Response::Ok));
            return;
	    }
	}

	LinphoneCall *call=app->findCall(id);
	if (call == NULL) {
	    ost << "No call with such id.";
	    app->sendResponse(Response(COMMANDNAME_CONFERENCE, ost.str(), Response::Error));
		return;
	}

	if (subcommand.compare("add") == 0) {
	    bool_t in_conference;
	    in_conference = (linphone_call_get_conference(call) != NULL);
	    if(!in_conference) {
	        ret = linphone_core_add_to_conference(lc, call);
	    }
	    else {
	        ost << "Call ID: " << id << " is already in conference, can't add the same call";
	        app->sendResponse(Response(COMMANDNAME_CONFERENCE, ost.str(), Response::Error));
            return;
	    }
	} else if (subcommand.compare("rm") == 0) {
		ret = linphone_core_remove_from_conference(lc, call);
	} else if (subcommand.compare("enter") == 0) {
		ret = linphone_core_enter_conference(lc);
	} else if (subcommand.compare("leave") == 0) {
		ret = linphone_core_leave_conference(lc);
	} else {
	    ost << "Invalid command format.";
	    app->sendResponse(Response(COMMANDNAME_CONFERENCE, ost.str(), Response::Error));
		return;
	}

	if (ret == 0 || ret == 1) {
		//ost << "Call ID: " << id << "\n";
		//ost << "Conference: " << subcommand << " OK" <<;
		ost << "{ \"Call ID\": " << id << ", \"Conference\": "  << "\"" << subcommand.c_str() << " OK" << "\"" << " } ";
		app->sendResponse(Response(COMMANDNAME_CONFERENCE, ost.str(), Response::Ok));
		return;
	}
	ost << "Command failed";
	app->sendResponse(Response(COMMANDNAME_CONFERENCE, ost.str(), Response::Error));
}
