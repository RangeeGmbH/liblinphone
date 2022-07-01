//
// Created by stefanh on 23.06.22.
//

#include "conference-mute.h"

using namespace std;

ConferenceMuteCommand::ConferenceMuteCommand() :
DaemonCommand("conference-mute", "conference-mute [get|set] [input|output] ([0|1])", "Mute/unmute the microphone or speaker in conference (1 to mute, 0 to unmute).")
{
    addExample(new DaemonCommandExample("conference-mute 1",
                                        "Status: Ok\n\n"
                                        "Microphone Muted"));
    addExample(new DaemonCommandExample("conference-mute",
                                        "Status: Ok\n\n"
                                        "Microphone Muted"));
    addExample(new DaemonCommandExample("conference-mute 0",
                                        "Status: Ok\n\n"
                                        "Microphone Unmuted"));
    addExample(new DaemonCommandExample("conference-mute 1",
                                        "Status: Error\n\n"
                                        "Reason: No confernce in progress. Can't mute."));
}

void ConferenceMuteCommand::exec(Daemon* app, const string& args)
{
    LinphoneCore *lc = app->getCore();
    int muted;
    bool getConference;
    ostringstream ost;
    LinphoneConference *conference = linphone_core_get_conference(lc);
    string param;
    istringstream ist(args);
    istringstream paramStringStream(args);
    paramStringStream >> param;
    ist >> muted;

    if (ist.fail()) {
        app->sendResponse(Response(COMMANDNAME_CONFERENCE_MUTE, "Missing parameter", Response::Error));
    }
    if (param == "set") {
        ist >> param;
        ist >> muted;
        if (conference == NULL) {
            getConference = false;
        }
        else{
            getConference = true;
            if (param == "input") {
                ////set
                linphone_conference_mute_microphone(conference, !muted);;
            }
            if (param == "output") {
                ////set
                linphone_conference_mute_speaker(conference, (bool_t)muted);
            }
        }
        if(getConference) {
            ////get
            std::string conferenceJSON;
            conferenceJSON = "{ \"conference\": ";
            conferenceJSON += app->getJsonForConference(conference);
            conferenceJSON += " }";
            app->sendResponse(Response(COMMANDNAME_CONFERENCE_MUTE, conferenceJSON, Response::Ok));
        }
        else {
            ost << "No conference in progress. Can't mute.";
            app->sendResponse(Response(COMMANDNAME_CONFERENCE_MUTE, ost.str(), Response::Error));
            return;
        }
    }
    if (param == "get") {
        std::string conferenceJSON;
        conferenceJSON = "{ \"conference\": ";
        conferenceJSON += app->getJsonForConference(conference);
        conferenceJSON += " }";
        app->sendResponse(Response(COMMANDNAME_CONFERENCE_MUTE, conferenceJSON, Response::Ok));
    }
}