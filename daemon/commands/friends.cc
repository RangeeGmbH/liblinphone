//
// Created by stefanh on 12.04.22.
//

#include "friends.h"

Friends::Friends() :
DaemonCommand("friends", "friends add|delete <sip address> | clear",
              "add or delete a friend with an sip address to an subscriber list ( used for blf ).\n Also the List can be clear") {
    addExample(make_unique<DaemonCommandExample>("friends add <sip address>",
                                        "Status: OK\n"));
    addExample(make_unique<DaemonCommandExample>("friends delete <sip address>",
                                        "Status: OK\n"));
    addExample(make_unique<DaemonCommandExample>("friends clear",
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
            string friendAddress = linphone_address_as_string(addr);
            LinphoneFriend *_friend = linphone_friend_list_find_friend_by_address(linphone_core_get_default_friend_list(app->getCore()), addr);
            if(_friend != NULL) {
                LinphoneFriendListStatus friendStatus;
                friendStatus = linphone_friend_list_remove_friend(linphone_core_get_default_friend_list(app->getCore()), _friend);
                switch (friendStatus) {
                    case LinphoneFriendListStatus::LinphoneFriendListOK:
                        ost = "\"delete friend with address: " + friendAddress + "is Ok\"";
                        app->sendResponse(Response(COMMANDNAME_FRIENDS, ost, Response::Ok));
                        break;
                    case LinphoneFriendListStatus::LinphoneFriendListNonExistentFriend:
                        ost = "friend with address: " + friendAddress + " does not exists";
                        app->sendResponse(Response(COMMANDNAME_FRIENDS, ost, Response::Error));
                        break;
                    case LinphoneFriendListStatus::LinphoneFriendListInvalidFriend:
                        ost = "friend with address: " +friendAddress + " is invalid";
                        app->sendResponse(Response(COMMANDNAME_FRIENDS, ost, Response::Error));
                        break;
                }
            }
            else {
                ost = "can't find LinphoneFriend witch address: " + friendAddress;
                app->sendResponse(Response(COMMANDNAME_FRIENDS, ost, Response::Error));
            }
        }
        else {
            ost = "can't create LinphoneAddress: " + param;
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

        //OK
        ost = "\"clear friend list Ok ( this will stop listen on notify )\"";
        app->sendResponse(Response(COMMANDNAME_FRIENDS, ost, Response::Ok));
    }
    if (param == "add") {
        string friendsName;
        ist >> param;

        LinphoneAddress *addr = linphone_address_new(param.c_str());

        if(addr != NULL) {
            string friendAddress = linphone_address_as_string(addr);
            LinphoneFriend *_friend = linphone_friend_list_find_friend_by_address(linphone_core_get_default_friend_list(app->getCore()), addr);
            if(_friend == NULL)
            {
                LinphoneFriend *new_friend;
                new_friend = linphone_core_create_friend_with_address(app->getCore(), param.c_str());
                if(new_friend == NULL) {
                    ost = "create friend with address: " + friendAddress + " failed.";
                    app->sendResponse(Response(COMMANDNAME_FRIENDS, ost, Response::Error));
                }
                else {
                    linphone_friend_enable_subscribes(new_friend,TRUE); /*configure this friend to emit SUBSCRIBE message after being added to LinphoneCore*/
                    linphone_friend_set_inc_subscribe_policy(new_friend,LinphoneSPAccept); /* Accept incoming subscription request for this friend*/
                    // linphone_core_add_friend(app->getCore(),my_friend); /* add my friend to the buddy list, initiate SUBSCRIBE message*/

                    linphone_friend_list_add_friend(linphone_core_get_default_friend_list(app->getCore()), new_friend);

                    //OK
                    ost = "\"add friend with address: " + friendAddress + " to friend list is Ok\"";
                    app->sendResponse(Response(COMMANDNAME_FRIENDS, ost, Response::Ok));
                }
            }
            else {
                ost = "LinphoneFriend witch address: " + friendAddress + " is already in list";
                app->sendResponse(Response(COMMANDNAME_FRIENDS, ost, Response::Error));
            }
        }
        else {
            ost = "can't create LinphoneAddress: " + param;
            app->sendResponse(Response(COMMANDNAME_FRIENDS, ost, Response::Error));
        }
    }
}