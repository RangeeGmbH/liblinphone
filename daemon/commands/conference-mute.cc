//
// Created by stefanh on 23.06.22.
//

#include "conference-mute.h"

using namespace std;

ConferenceMuteCommand::ConferenceMuteCommand() :
DaemonCommand("conference-mute", "conference-mute 0|1", "Mute/unmute the microphone in conference (1 to mute, 0 to unmute). No argument means MUTE.")
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
    int get;
    ostringstream ost;
    LinphoneConference *conference =linphone_core_get_conference(lc);
    string param;
    istringstream ist(args);
    istringstream paramStringStream(args);
    paramStringStream >> param;
    ist >> muted;
    if (param != "get"){
        get = false;
        if (ist.fail() || (muted != 0)) {
            muted = TRUE;
            if (conference == NULL) {
                ost << "No conference in progress. Can't mute.";
                app->sendResponse(Response(COMMANDNAME_CONFERENCE_MUTE, ost.str(), Response::Error));
                return;
            }
        } else {
            muted = FALSE;
            if (conference == NULL) {
                ost << "No conference in progress. Can't unmute.";
                app->sendResponse(Response(COMMANDNAME_CONFERENCE_MUTE, ost.str(), Response::Error));
                return;
            }
        }
        linphone_core_enable_mic(lc, !muted);
        std::ostringstream buf;
        string mutedStr = "\"Muted\": \"yes\"";
        string unmutedStr = "\"Muted\": \"no\"";
        if(muted == TRUE) {
            buf << mutedStr;
        }
        else {
            buf << unmutedStr;
        }
        ost << "{ " <<  buf.str().c_str() << " }";
        app->sendResponse(Response(COMMANDNAME_CONFERENCE_MUTE, ost.str(), Response::Ok));
    }

    if (param == "get") {
        get = true;
        string mutedStr = linphone_conference_microphone_is_muted(conference) ? "\"Muted\": \"no\"" : "\"Muted\": \"yes\"";
        ost << "{ " << mutedStr << " }";
        app->sendResponse(Response(COMMANDNAME_CONFERENCE_MUTE, ost.str(), Response::Ok));
    }
    if(param != "get" && get == true){
        ost << "\"Wrong parameter\"";
        app->sendResponse(Response(COMMANDNAME_CONFERENCE_MUTE, ost.str(), Response::Ok));
    }
}