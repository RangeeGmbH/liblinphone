//
// Created by stefanh on 12.04.22.
//

#include "friends.h"

Friends::Friends() :
DaemonCommand("friends", "friends",
              "set or get the maximum number of simultaneous calls Linphone core can manage at a time.") {
    addExample(new DaemonCommandExample("maxcalls",
                                        "Status: OK\n"));
    addExample(new DaemonCommandExample("maxcalls 1",
                                        "Status: OK\n"));
}

void Friends::exec(Daemon *app, const string &args) {
    istringstream ist(args);
    string ost;
    ostringstream ostStr;
    std::string param;
    ist >> param;
    if (ist.fail()) {
        app->sendResponse(Response(COMMANDNAME_FRIENDS, "Missing parameter", Response::Error));
    }
    if (param == "delete") {
        string friendsName;
        ist >> param;

        LinphoneAddress *addr = linphone_address_new(param.c_str());
        if(addr != NULL) {
            LinphoneFriend *_friend = linphone_friend_list_find_friend_by_address(linphone_core_get_default_friend_list(app->getCore()), addr);
            if(_friend != NULL) {
                LinphoneFriendListStatus friendStatus;
                friendStatus = linphone_friend_list_remove_friend(linphone_core_get_default_friend_list(app->getCore()), _friend);
                switch (friendStatus) {
                    case LinphoneFriendListStatus::LinphoneFriendListOK:
                        ost = "\"OK\"";
                        app->sendResponse(Response(COMMANDNAME_FRIENDS, ost, Response::Ok));
                        break;
                    case LinphoneFriendListStatus::LinphoneFriendListNonExistentFriend:
                        ost = "\"friend does not exists\"";
                        app->sendResponse(Response(COMMANDNAME_FRIENDS, ost, Response::Error));
                        break;
                    case LinphoneFriendListStatus::LinphoneFriendListInvalidFriend:
                        ost = "\"friend is invalid\"";
                        app->sendResponse(Response(COMMANDNAME_FRIENDS, ost, Response::Error));
                        break;
                }
            }
            else {
                ost = "\"can't find LinphoneFriend witch address: " + param + "\"";
                app->sendResponse(Response(COMMANDNAME_FRIENDS, ost, Response::Error));
            }
        }
        else {
            ost = "\"can't create LinphoneAddress: " + param + "\"";
            app->sendResponse(Response(COMMANDNAME_FRIENDS, ost, Response::Error));
        }
    }
    if (param == "clear") {
        // remove existing "default" list
        linphone_core_remove_friend_list(app->getCore(), linphone_core_get_default_friend_list(app->getCore()));

        // create new "default" list
        LinphoneFriendList *lfl = linphone_core_create_friend_list(app->getCore());
        ////////////////////////////////////////////////////////////

        // add new friend "default" list to friend lists
        linphone_core_add_friend_list(app->getCore(), lfl);
    }
    if (param == "add") {
        string friendsName;
        ist >> param;

        LinphoneAddress *addr = linphone_address_new(param.c_str());

        if(addr != NULL) {
            LinphoneFriend *_friend = linphone_friend_list_find_friend_by_address(linphone_core_get_default_friend_list(app->getCore()), addr);
            if(_friend == NULL)
            {
                LinphoneFriend *new_friend;
                new_friend = linphone_core_create_friend_with_address(app->getCore(), param.c_str());
                if(new_friend == NULL) {
                    ost = "\"create friend with address: " + param + " failed. \"";
                    app->sendResponse(Response(COMMANDNAME_FRIENDS, ost, Response::Error));
                }
                else {
                    linphone_friend_enable_subscribes(new_friend,TRUE); /*configure this friend to emit SUBSCRIBE message after being added to LinphoneCore*/
                    linphone_friend_set_inc_subscribe_policy(new_friend,LinphoneSPAccept); /* Accept incoming subscription request for this friend*/
                    // linphone_core_add_friend(app->getCore(),my_friend); /* add my friend to the buddy list, initiate SUBSCRIBE message*/

                    linphone_friend_list_add_friend(linphone_core_get_default_friend_list(app->getCore()), new_friend);

                    linphone_core_add_friend_list(app->getCore(), linphone_core_get_default_friend_list(app->getCore()));
                }
            }
            else {
                ost = "\"can't find LinphoneFriend witch address: " + param + "\"";
                app->sendResponse(Response(COMMANDNAME_FRIENDS, ost, Response::Error));
            }
        }
        else {
            ost = "\"can't create LinphoneAddress: " + param + "\"";
            app->sendResponse(Response(COMMANDNAME_FRIENDS, ost, Response::Error));
        }
    }
}