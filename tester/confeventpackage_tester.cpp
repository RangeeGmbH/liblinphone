/*
    liblinphone_tester - liblinphone test suite
    Copyright (C) 2013  Belledonne Communications SARL

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <map>
#include <string>

#include "linphone/core.h"
#include "private.h"
#include "liblinphone_tester.h"
#include "conference/conference-listener.h"
#include "conference/remote-conference-event-handler.h"
#include "conference/local-conference-event-handler.h"

using namespace LinphonePrivate;
using namespace std;

static const char *first_notify = \
"<?xml version=\"1.0\" encoding=\"UTF-8\"?> "\
"   <conference-info"\
"    xmlns=\"urn:ietf:params:xml:ns:conference-info\""\
"    entity=\"%s\""\
"    state=\"full\" version=\"1\">"\
"   <!--"\
"     CONFERENCE INFO"\
"   -->"\
"    <conference-description>"\
"     <subject>Agenda: This month's goals</subject>"\
"      <service-uris>"\
"       <entry>"\
"        <uri>http://sharepoint/salesgroup/</uri>"\
"        <purpose>web-page</purpose>"\
"       </entry>"\
"      </service-uris>"\
"     </conference-description>"\
"   <!--"\
"      CONFERENCE STATE"\
"   -->"\
"    <conference-state>"\
"     <user-count>33</user-count>"\
"    </conference-state>"\
"   <!--"\
"     USERS"\
"   -->"\
"    <users>"\
"     <user entity=\"sip:bob@example.com\" state=\"full\">"\
"      <display-text>Bob Hoskins</display-text>"\
"   <!--"\
"     ENDPOINTS"\
"   -->"\
"      <endpoint entity=\"sip:bob@pc33.example.com\">"\
"       <display-text>Bob's Laptop</display-text>"\
"       <status>disconnected</status>"\
"       <disconnection-method>departed</disconnection-method>"\
"       <disconnection-info>"\
"        <when>2005-03-04T20:00:00Z</when>"\
"        <reason>bad voice quality</reason>"\
"        <by>sip:mike@example.com</by>"\
"       </disconnection-info>"\
"   <!--"\
"     MEDIA"\
"   -->"\
"       <media id=\"1\">"\
"        <display-text>main audio</display-text>"\
"        <type>audio</type>"\
"        <label>34567</label>"\
"        <src-id>432424</src-id>"\
"        <status>sendrecv</status>"\
"       </media>"\
"      </endpoint>"\
"     </user>"\
"   <!--"\
"     USER"\
"   -->"\
"     <user entity=\"sip:alice@example.com\" state=\"full\">"\
"      <display-text>Alice</display-text>"\
"      <roles>"\
"      	<entry>admin</entry>"\
"      	<entry>participant</entry>"\
"      </roles>"\
"   <!--"\
"     ENDPOINTS"\
"   -->"\
"      <endpoint entity=\"sip:4kfk4j392jsu@example.com;grid=433kj4j3u\">"\
"       <status>connected</status>"\
"       <joining-method>dialed-out</joining-method>"\
"       <joining-info>"\
"        <when>2005-03-04T20:00:00Z</when>"\
"        <by>sip:mike@example.com</by>"\
"       </joining-info>"\
"   <!--"\
"     MEDIA"\
"   -->"\
"       <media id=\"1\">"\
"        <display-text>main audio</display-text>"\
"        <type>audio</type>"\
"        <label>34567</label>"\
"        <src-id>534232</src-id>"\
"        <status>sendrecv</status>"\
"       </media>"\
"      </endpoint>"\
"     </user>"\
"    </users>"\
"   </conference-info>";

static const char *participant_added_notify = \
"<?xml version=\"1.0\" encoding=\"UTF-8\"?> "\
"   <conference-info"\
"    xmlns=\"urn:ietf:params:xml:ns:conference-info\""\
"    entity=\"%s\""\
"    state=\"full\" version=\"1\">"\
"   <!--"\
"     CONFERENCE INFO"\
"   -->"\
"    <conference-description>"\
"     <subject>Agenda: This month's goals</subject>"\
"      <service-uris>"\
"       <entry>"\
"        <uri>http://sharepoint/salesgroup/</uri>"\
"        <purpose>web-page</purpose>"\
"       </entry>"\
"      </service-uris>"\
"     </conference-description>"\
"   <!--"\
"      CONFERENCE STATE"\
"   -->"\
"    <conference-state>"\
"     <user-count>33</user-count>"\
"    </conference-state>"\
"   <!--"\
"     USERS"\
"   -->"\
"    <users>"\
"     <user entity=\"sip:frank@example.com\" state=\"full\">"\
"      <display-text>Bob Hoskins</display-text>"\
"   <!--"\
"     ENDPOINTS"\
"   -->"\
"      <endpoint entity=\"sip:frank@pc33.example.com\">"\
"       <display-text>Frank's Laptop</display-text>"\
"       <status>disconnected</status>"\
"       <disconnection-method>departed</disconnection-method>"\
"       <disconnection-info>"\
"        <when>2005-03-04T20:00:00Z</when>"\
"        <reason>bad voice quality</reason>"\
"        <by>sip:mike@example.com</by>"\
"       </disconnection-info>"\
"   <!--"\
"     MEDIA"\
"   -->"\
"       <media id=\"1\">"\
"        <display-text>main audio</display-text>"\
"        <type>audio</type>"\
"        <label>34567</label>"\
"        <src-id>432424</src-id>"\
"        <status>sendrecv</status>"\
"       </media>"\
"      </endpoint>"\
"     </user>"\
"    </users>"\
"   </conference-info>";

static const char *participant_not_added_notify = \
"<?xml version=\"1.0\" encoding=\"UTF-8\"?> "\
"   <conference-info"\
"    xmlns=\"urn:ietf:params:xml:ns:conference-info\""\
"    entity=\"%s\""\
"    state=\"full\" version=\"1\">"\
"   <!--"\
"     CONFERENCE INFO"\
"   -->"\
"    <conference-description>"\
"     <subject>Agenda: This month's goals</subject>"\
"      <service-uris>"\
"       <entry>"\
"        <uri>http://sharepoint/salesgroup/</uri>"\
"        <purpose>web-page</purpose>"\
"       </entry>"\
"      </service-uris>"\
"     </conference-description>"\
"   <!--"\
"      CONFERENCE STATE"\
"   -->"\
"    <conference-state>"\
"     <user-count>33</user-count>"\
"    </conference-state>"\
"   <!--"\
"     USERS"\
"   -->"\
"    <users>"\
"     <user entity=\"sip:frank@example.com\" state=\"partial\">"\
"      <display-text>Bob Hoskins</display-text>"\
"   <!--"\
"     ENDPOINTS"\
"   -->"\
"      <endpoint entity=\"sip:frank@pc33.example.com\">"\
"       <display-text>Frank's Laptop</display-text>"\
"       <status>disconnected</status>"\
"       <disconnection-method>departed</disconnection-method>"\
"       <disconnection-info>"\
"        <when>2005-03-04T20:00:00Z</when>"\
"        <reason>bad voice quality</reason>"\
"        <by>sip:mike@example.com</by>"\
"       </disconnection-info>"\
"   <!--"\
"     MEDIA"\
"   -->"\
"       <media id=\"1\">"\
"        <display-text>main audio</display-text>"\
"        <type>audio</type>"\
"        <label>34567</label>"\
"        <src-id>432424</src-id>"\
"        <status>sendrecv</status>"\
"       </media>"\
"      </endpoint>"\
"     </user>"\
"    </users>"\
"   </conference-info>";

static const char *participant_deleted_notify = \
"<?xml version=\"1.0\" encoding=\"UTF-8\"?> "\
"   <conference-info"\
"    xmlns=\"urn:ietf:params:xml:ns:conference-info\""\
"    entity=\"%s\""\
"    state=\"full\" version=\"1\">"\
"   <!--"\
"     CONFERENCE INFO"\
"   -->"\
"    <conference-description>"\
"     <subject>Agenda: This month's goals</subject>"\
"      <service-uris>"\
"       <entry>"\
"        <uri>http://sharepoint/salesgroup/</uri>"\
"        <purpose>web-page</purpose>"\
"       </entry>"\
"      </service-uris>"\
"     </conference-description>"\
"   <!--"\
"      CONFERENCE STATE"\
"   -->"\
"    <conference-state>"\
"     <user-count>33</user-count>"\
"    </conference-state>"\
"   <!--"\
"     USERS"\
"   -->"\
"    <users>"\
"     <user entity=\"sip:bob@example.com\" state=\"deleted\">"\
"      <display-text>Bob Hoskins</display-text>"\
"   <!--"\
"     ENDPOINTS"\
"   -->"\
"      <endpoint entity=\"sip:bob@pc33.example.com\">"\
"       <display-text>Bob's Laptop</display-text>"\
"       <status>disconnected</status>"\
"       <disconnection-method>departed</disconnection-method>"\
"       <disconnection-info>"\
"        <when>2005-03-04T20:00:00Z</when>"\
"        <reason>bad voice quality</reason>"\
"        <by>sip:mike@example.com</by>"\
"       </disconnection-info>"\
"   <!--"\
"     MEDIA"\
"   -->"\
"       <media id=\"1\">"\
"        <display-text>main audio</display-text>"\
"        <type>audio</type>"\
"        <label>34567</label>"\
"        <src-id>432424</src-id>"\
"        <status>sendrecv</status>"\
"       </media>"\
"      </endpoint>"\
"     </user>"\
"    </users>"\
"   </conference-info>";

static const char *participant_admined_notify = \
"<?xml version=\"1.0\" encoding=\"UTF-8\"?> "\
"   <conference-info"\
"    xmlns=\"urn:ietf:params:xml:ns:conference-info\""\
"    entity=\"%s\""\
"    state=\"full\" version=\"1\">"\
"   <!--"\
"     CONFERENCE INFO"\
"   -->"\
"    <conference-description>"\
"     <subject>Agenda: This month's goals</subject>"\
"      <service-uris>"\
"       <entry>"\
"        <uri>http://sharepoint/salesgroup/</uri>"\
"        <purpose>web-page</purpose>"\
"       </entry>"\
"      </service-uris>"\
"     </conference-description>"\
"   <!--"\
"      CONFERENCE STATE"\
"   -->"\
"    <conference-state>"\
"     <user-count>33</user-count>"\
"    </conference-state>"\
"   <!--"\
"     USERS"\
"   -->"\
"    <users>"\
"     <user entity=\"sip:bob@example.com\" state=\"partial\">"\
"      <display-text>Bob Hoskins</display-text>"\
"      <roles>"\
"      	<entry>participant</entry>"\
"      	<entry>admin</entry>"\
"      </roles>"\
"   <!--"\
"     ENDPOINTS"\
"   -->"\
"      <endpoint entity=\"sip:bob@pc33.example.com\">"\
"       <display-text>Bob's Laptop</display-text>"\
"       <status>disconnected</status>"\
"       <disconnection-method>departed</disconnection-method>"\
"       <disconnection-info>"\
"        <when>2005-03-04T20:00:00Z</when>"\
"        <reason>bad voice quality</reason>"\
"        <by>sip:mike@example.com</by>"\
"       </disconnection-info>"\
"   <!--"\
"     MEDIA"\
"   -->"\
"       <media id=\"1\">"\
"        <display-text>main audio</display-text>"\
"        <type>audio</type>"\
"        <label>34567</label>"\
"        <src-id>432424</src-id>"\
"        <status>sendrecv</status>"\
"       </media>"\
"      </endpoint>"\
"     </user>"\
"    </users>"\
"   </conference-info>";

static const char *participant_unadmined_notify = \
"<?xml version=\"1.0\" encoding=\"UTF-8\"?> "\
"   <conference-info"\
"    xmlns=\"urn:ietf:params:xml:ns:conference-info\""\
"    entity=\"%s\""\
"    state=\"full\" version=\"1\">"\
"   <!--"\
"     CONFERENCE INFO"\
"   -->"\
"    <conference-description>"\
"     <subject>Agenda: This month's goals</subject>"\
"      <service-uris>"\
"       <entry>"\
"        <uri>http://sharepoint/salesgroup/</uri>"\
"        <purpose>web-page</purpose>"\
"       </entry>"\
"      </service-uris>"\
"     </conference-description>"\
"   <!--"\
"      CONFERENCE STATE"\
"   -->"\
"    <conference-state>"\
"     <user-count>33</user-count>"\
"    </conference-state>"\
"   <!--"\
"     USERS"\
"   -->"\
"    <users>"\
"     <user entity=\"sip:alice@example.com\" state=\"partial\">"\
"      <display-text>Alice Hoskins</display-text>"\
"      <roles>"\
"      	<entry>participant</entry>"\
"      </roles>"\
"   <!--"\
"     ENDPOINTS"\
"   -->"\
"      <endpoint entity=\"sip:alice@pc33.example.com\">"\
"       <display-text>Alice's Laptop</display-text>"\
"       <status>disconnected</status>"\
"       <disconnection-method>departed</disconnection-method>"\
"       <disconnection-info>"\
"        <when>2005-03-04T20:00:00Z</when>"\
"        <reason>bad voice quality</reason>"\
"        <by>sip:mike@example.com</by>"\
"       </disconnection-info>"\
"   <!--"\
"     MEDIA"\
"   -->"\
"       <media id=\"1\">"\
"        <display-text>main audio</display-text>"\
"        <type>audio</type>"\
"        <label>34567</label>"\
"        <src-id>432424</src-id>"\
"        <status>sendrecv</status>"\
"       </media>"\
"      </endpoint>"\
"     </user>"\
"    </users>"\
"   </conference-info>";


static const char *bobUri = "sip:bob@example.com";
static const char *aliceUri = "sip:alice@example.com";
static const char *frankUri = "sip:frank@example.com";
static const char *confUri = "sips:conf233@example.com";



class ConferenceEventTester : public Conference::ConferenceListener{
public:
  shared_ptr<Conference::RemoteConferenceEventHandler> mHandler;
  map<string, int> mParticipants;

  ConferenceEventTester(LinphoneCore *lc, LinphoneAddress *confAddr);

  void conferenceCreated(LinphoneAddress *confAddress);
  void conferenceTerminated(LinphoneAddress *confAddress);
  void participantAdded(LinphoneAddress *addr);
  void participantRemoved(LinphoneAddress *addr);
  void participantSetAdmin(LinphoneAddress *addr, bool isAdmin);
};

ConferenceEventTester::ConferenceEventTester(LinphoneCore *lc, LinphoneAddress *confAddr){
  mHandler = make_shared<Conference::RemoteConferenceEventHandler>(lc, this, confAddr);
}

void ConferenceEventTester::conferenceCreated(LinphoneAddress *confAddress){}
void ConferenceEventTester::conferenceTerminated(LinphoneAddress *confAddress){}
void ConferenceEventTester::participantAdded(LinphoneAddress *addr){
	mParticipants.insert(pair<string, int>(linphone_address_as_string(addr),0));
}
void ConferenceEventTester::participantRemoved(LinphoneAddress *addr){
  mParticipants.erase(linphone_address_as_string(addr));
}
void ConferenceEventTester::participantSetAdmin(LinphoneAddress *addr, bool isAdmin){
const char *addrAsString = linphone_address_as_string(addr);
  if(mParticipants.find(addrAsString) != mParticipants.end()){
    mParticipants.erase(addrAsString);
    mParticipants.insert(pair<string, int>(addrAsString, isAdmin ? 1 : 0));
  }
}

void first_notify_parsing(void){
  LinphoneCoreManager* marie = linphone_core_manager_new("marie_rc");
  LinphoneAddress *confAddress = linphone_core_interpret_url(marie->lc, confUri);
  ConferenceEventTester tester(marie->lc, confAddress);
  LinphoneAddress *bobAddr = linphone_core_interpret_url(marie->lc, bobUri);
  LinphoneAddress *aliceAddr = linphone_core_interpret_url(marie->lc, aliceUri);
  char notify[strlen(first_notify) + strlen(confUri)];

  snprintf(notify, sizeof(notify), first_notify, confUri);
  tester.mHandler->notifyReceived(notify);

  BC_ASSERT_EQUAL(tester.mParticipants.size(), 2, int, "%d");
  BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(bobAddr)) != tester.mParticipants.end());
  BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(aliceAddr)) != tester.mParticipants.end());
  BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(bobAddr))->second == 0);
  BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(aliceAddr))->second == 1);

  linphone_address_unref(bobAddr);
  linphone_address_unref(aliceAddr);
  linphone_address_unref(confAddress);
  linphone_core_manager_destroy(marie);
}

void first_notify_parsing_wrong_conf(void){
	LinphoneCoreManager* marie = linphone_core_manager_new("marie_rc");
	LinphoneAddress *confAddress = linphone_core_interpret_url(marie->lc, "sips:conf322@example.com");
  ConferenceEventTester tester(marie->lc, confAddress);
	LinphoneAddress *bobAddr = linphone_core_interpret_url(marie->lc, bobUri);
	LinphoneAddress *aliceAddr = linphone_core_interpret_url(marie->lc, aliceUri);
	char notify[strlen(first_notify) + strlen(confUri)];

	snprintf(notify, sizeof(notify), first_notify, confUri);
	tester.mHandler->notifyReceived(notify);

	BC_ASSERT_EQUAL(tester.mParticipants.size(), 0, int, "%d");
	BC_ASSERT_FALSE(tester.mParticipants.find(linphone_address_as_string(bobAddr)) != tester.mParticipants.end());
	BC_ASSERT_FALSE(tester.mParticipants.find(linphone_address_as_string(aliceAddr)) != tester.mParticipants.end());

	linphone_address_unref(bobAddr);
	linphone_address_unref(aliceAddr);
	linphone_address_unref(confAddress);
	linphone_core_manager_destroy(marie);
}

void participant_added_parsing(void){
	LinphoneCoreManager* marie = linphone_core_manager_new("marie_rc");
	LinphoneAddress *confAddress = linphone_core_interpret_url(marie->lc, confUri);
  ConferenceEventTester tester(marie->lc, confAddress);
	LinphoneAddress *bobAddr = linphone_core_interpret_url(marie->lc, bobUri);
	LinphoneAddress *aliceAddr = linphone_core_interpret_url(marie->lc, aliceUri);
	LinphoneAddress *frankAddr = linphone_core_interpret_url(marie->lc, frankUri);
	char notify[strlen(first_notify) + strlen(confUri)];
	char notify_added[strlen(participant_added_notify) + strlen(confUri)];

	snprintf(notify, sizeof(notify), first_notify, confUri);
	tester.mHandler->notifyReceived(notify);

	BC_ASSERT_EQUAL(tester.mParticipants.size(), 2, int, "%d");
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(bobAddr)) != tester.mParticipants.end());
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(aliceAddr)) != tester.mParticipants.end());
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(bobAddr))->second == 0);
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(aliceAddr))->second == 1);

	snprintf(notify_added, sizeof(notify_added), participant_added_notify, confUri);
	tester.mHandler->notifyReceived(notify_added);

	BC_ASSERT_EQUAL(tester.mParticipants.size(), 3, int, "%d");
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(frankAddr)) != tester.mParticipants.end());
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(frankAddr))->second == 0);

	linphone_address_unref(bobAddr);
	linphone_address_unref(aliceAddr);
	linphone_address_unref(frankAddr);
	linphone_address_unref(confAddress);
	linphone_core_manager_destroy(marie);
}

void participant_not_added_parsing(void){
	LinphoneCoreManager* marie = linphone_core_manager_new("marie_rc");
	LinphoneAddress *confAddress = linphone_core_interpret_url(marie->lc, confUri);
  ConferenceEventTester tester(marie->lc, confAddress);
	LinphoneAddress *bobAddr = linphone_core_interpret_url(marie->lc, bobUri);
	LinphoneAddress *aliceAddr = linphone_core_interpret_url(marie->lc, aliceUri);
	LinphoneAddress *frankAddr = linphone_core_interpret_url(marie->lc, frankUri);
	char notify[strlen(first_notify) + strlen(confUri)];
	char notify_not_added[strlen(participant_not_added_notify) + strlen(confUri)];

	snprintf(notify, sizeof(notify), first_notify, confUri);
	tester.mHandler->notifyReceived(notify);

	BC_ASSERT_EQUAL(tester.mParticipants.size(), 2, int, "%d");
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(bobAddr)) != tester.mParticipants.end());
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(aliceAddr)) != tester.mParticipants.end());
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(bobAddr))->second == 0);
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(aliceAddr))->second == 1);

	snprintf(notify_not_added, sizeof(notify_not_added), participant_not_added_notify, confUri);
	tester.mHandler->notifyReceived(notify_not_added);

	BC_ASSERT_EQUAL(tester.mParticipants.size(), 2, int, "%d");
	BC_ASSERT_FALSE(tester.mParticipants.find(linphone_address_as_string(frankAddr)) != tester.mParticipants.end());

	linphone_address_unref(bobAddr);
	linphone_address_unref(aliceAddr);
	linphone_address_unref(frankAddr);
	linphone_address_unref(confAddress);
	linphone_core_manager_destroy(marie);
}

void participant_deleted_parsing(void){
	LinphoneCoreManager* marie = linphone_core_manager_new("marie_rc");
	LinphoneAddress *confAddress = linphone_core_interpret_url(marie->lc, confUri);
  ConferenceEventTester tester(marie->lc, confAddress);
	LinphoneAddress *bobAddr = linphone_core_interpret_url(marie->lc, bobUri);
	LinphoneAddress *aliceAddr = linphone_core_interpret_url(marie->lc, aliceUri);
	char notify[strlen(first_notify) + strlen(confUri)];
	char notify_deleted[strlen(participant_deleted_notify) + strlen(confUri)];

	snprintf(notify, sizeof(notify), first_notify, confUri);
	tester.mHandler->notifyReceived(notify);

	BC_ASSERT_EQUAL(tester.mParticipants.size(), 2, int, "%d");
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(bobAddr)) != tester.mParticipants.end());
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(aliceAddr)) != tester.mParticipants.end());
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(bobAddr))->second == 0);
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(aliceAddr))->second == 1);

	snprintf(notify_deleted, sizeof(notify_deleted), participant_deleted_notify, confUri);
	tester.mHandler->notifyReceived(notify_deleted);

	BC_ASSERT_EQUAL(tester.mParticipants.size(), 1, int, "%d");
	BC_ASSERT_FALSE(tester.mParticipants.find(linphone_address_as_string(bobAddr)) != tester.mParticipants.end());

	linphone_address_unref(bobAddr);
	linphone_address_unref(aliceAddr);
	linphone_address_unref(confAddress);
	linphone_core_manager_destroy(marie);
}

void participant_admined_parsing(void){
	LinphoneCoreManager* marie = linphone_core_manager_new("marie_rc");
	LinphoneAddress *confAddress = linphone_core_interpret_url(marie->lc, confUri);
  ConferenceEventTester tester(marie->lc, confAddress);
	LinphoneAddress *bobAddr = linphone_core_interpret_url(marie->lc, bobUri);
	LinphoneAddress *aliceAddr = linphone_core_interpret_url(marie->lc, aliceUri);
	char notify[strlen(first_notify) + strlen(confUri)];
	char notify_admined[strlen(participant_admined_notify) + strlen(confUri)];

	snprintf(notify, sizeof(notify), first_notify, confUri);
	tester.mHandler->notifyReceived(notify);

	BC_ASSERT_EQUAL(tester.mParticipants.size(), 2, int, "%d");
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(bobAddr)) != tester.mParticipants.end());
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(aliceAddr)) != tester.mParticipants.end());
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(bobAddr))->second == 0);
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(aliceAddr))->second == 1);

	snprintf(notify_admined, sizeof(notify_admined), participant_admined_notify, confUri);
	tester.mHandler->notifyReceived(notify_admined);

	BC_ASSERT_EQUAL(tester.mParticipants.size(), 2, int, "%d");
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(bobAddr)) != tester.mParticipants.end());
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(bobAddr))->second == 1);

	linphone_address_unref(bobAddr);
	linphone_address_unref(aliceAddr);
	linphone_address_unref(confAddress);
	linphone_core_manager_destroy(marie);
}

void participant_unadmined_parsing(void){
	LinphoneCoreManager* marie = linphone_core_manager_new("marie_rc");
	LinphoneAddress *confAddress = linphone_core_interpret_url(marie->lc, confUri);
	ConferenceEventTester tester(marie->lc, confAddress);
	LinphoneAddress *bobAddr = linphone_core_interpret_url(marie->lc, bobUri);
	LinphoneAddress *aliceAddr = linphone_core_interpret_url(marie->lc, aliceUri);
	char notify[strlen(first_notify) + strlen(confUri)];
	char notify_unadmined[strlen(participant_unadmined_notify) + strlen(confUri)];

	snprintf(notify, sizeof(notify), first_notify, confUri);
	tester.mHandler->notifyReceived(notify);

	BC_ASSERT_EQUAL(tester.mParticipants.size(), 2, int, "%d");
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(bobAddr)) != tester.mParticipants.end());
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(aliceAddr)) != tester.mParticipants.end());
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(bobAddr))->second == 0);
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(aliceAddr))->second == 1);

	snprintf(notify_unadmined, sizeof(notify_unadmined), participant_unadmined_notify, confUri);
	tester.mHandler->notifyReceived(notify_unadmined);

	BC_ASSERT_EQUAL(tester.mParticipants.size(), 2, int, "%d");
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(aliceAddr)) != tester.mParticipants.end());
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(aliceAddr))->second == 0);

	linphone_address_unref(bobAddr);
	linphone_address_unref(aliceAddr);
	linphone_address_unref(confAddress);
	linphone_core_manager_destroy(marie);
}

void send_first_notify(void) {
  LinphoneCoreManager* marie = linphone_core_manager_new("marie_rc");
  LinphoneCoreManager* pauline = linphone_core_manager_new(transport_supported(LinphoneTransportTls) ? "pauline_rc" : "pauline_tcp_rc");
  ConferenceEventTester tester(marie->lc, pauline->identity);
  Conference::LocalConference localConf(pauline->lc, pauline->identity);
  LinphoneAddress *bobAddr = linphone_core_interpret_url(marie->lc, bobUri);
  LinphoneAddress *aliceAddr = linphone_core_interpret_url(marie->lc, aliceUri);
  Conference::Participant p1(bobAddr, false);
  Conference::Participant p2(aliceAddr, true);
  LinphoneEvent *lev = linphone_core_create_notify(pauline->lc, marie->identity, "Conference");
  string notify;

  localConf.addParticipant(p1);
  localConf.addParticipant(p2);
  notify = localConf.getHandler()->subscribeReceived(lev);
  tester.mHandler->notifyReceived(notify.c_str());

  BC_ASSERT_EQUAL(tester.mParticipants.size(), 2, int, "%d");
  BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(bobAddr)) != tester.mParticipants.end());
  BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(aliceAddr)) != tester.mParticipants.end());
  BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(bobAddr))->second == 0);
  BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(aliceAddr))->second == 1);

  linphone_event_unref(lev);
  linphone_address_unref(bobAddr);
  linphone_address_unref(aliceAddr);
  linphone_core_manager_destroy(marie);
  linphone_core_manager_destroy(pauline);
}

void send_added_notify(void) {
	LinphoneCoreManager* marie = linphone_core_manager_new("marie_rc");
	LinphoneCoreManager* pauline = linphone_core_manager_new(transport_supported(LinphoneTransportTls) ? "pauline_rc" : "pauline_tcp_rc");
	ConferenceEventTester tester(marie->lc, pauline->identity);
	Conference::LocalConference localConf(pauline->lc, pauline->identity);
	LinphoneAddress *bobAddr = linphone_core_interpret_url(marie->lc, bobUri);
	LinphoneAddress *aliceAddr = linphone_core_interpret_url(marie->lc, aliceUri);
	LinphoneAddress *frankAddr = linphone_core_interpret_url(marie->lc, frankUri);
	Conference::Participant p1(bobAddr, false);
	Conference::Participant p2(aliceAddr, true);
	LinphoneEvent *lev = linphone_core_create_notify(pauline->lc, marie->identity, "Conference");
	string notify;

	localConf.addParticipant(p1);
	localConf.addParticipant(p2);
	notify = localConf.getHandler()->subscribeReceived(lev);
	tester.mHandler->notifyReceived(notify.c_str());

	BC_ASSERT_EQUAL(tester.mParticipants.size(), 2, int, "%d");
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(bobAddr)) != tester.mParticipants.end());
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(aliceAddr)) != tester.mParticipants.end());
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(bobAddr))->second == 0);
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(aliceAddr))->second == 1);

	notify = localConf.getHandler()->notifyParticipantAdded(frankAddr);
	tester.mHandler->notifyReceived(notify.c_str());

	BC_ASSERT_EQUAL(tester.mParticipants.size(), 3, int, "%d");
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(bobAddr)) != tester.mParticipants.end());
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(aliceAddr)) != tester.mParticipants.end());
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(frankAddr)) != tester.mParticipants.end());
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(bobAddr))->second == 0);
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(aliceAddr))->second == 1);
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(frankAddr))->second == 0);

	linphone_event_unref(lev);
	linphone_address_unref(bobAddr);
	linphone_address_unref(aliceAddr);
	linphone_address_unref(frankAddr);
	linphone_core_manager_destroy(marie);
	linphone_core_manager_destroy(pauline);
}

void send_removed_notify(void) {
	LinphoneCoreManager* marie = linphone_core_manager_new("marie_rc");
	LinphoneCoreManager* pauline = linphone_core_manager_new(transport_supported(LinphoneTransportTls) ? "pauline_rc" : "pauline_tcp_rc");
	ConferenceEventTester tester(marie->lc, pauline->identity);
	Conference::LocalConference localConf(pauline->lc, pauline->identity);
	LinphoneAddress *bobAddr = linphone_core_interpret_url(marie->lc, bobUri);
	LinphoneAddress *aliceAddr = linphone_core_interpret_url(marie->lc, aliceUri);
	Conference::Participant p1(bobAddr, false);
	Conference::Participant p2(aliceAddr, true);
	LinphoneEvent *lev = linphone_core_create_notify(pauline->lc, marie->identity, "Conference");
	string notify;

	localConf.addParticipant(p1);
	localConf.addParticipant(p2);
	notify = localConf.getHandler()->subscribeReceived(lev);
	tester.mHandler->notifyReceived(notify.c_str());

	BC_ASSERT_EQUAL(tester.mParticipants.size(), 2, int, "%d");
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(bobAddr)) != tester.mParticipants.end());
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(aliceAddr)) != tester.mParticipants.end());
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(bobAddr))->second == 0);
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(aliceAddr))->second == 1);

	notify = localConf.getHandler()->notifyParticipantRemoved(bobAddr);
	tester.mHandler->notifyReceived(notify.c_str());

	BC_ASSERT_EQUAL(tester.mParticipants.size(), 1, int, "%d");
	BC_ASSERT_FALSE(tester.mParticipants.find(linphone_address_as_string(bobAddr)) != tester.mParticipants.end());
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(aliceAddr)) != tester.mParticipants.end());
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(aliceAddr))->second == 1);

	linphone_event_unref(lev);
	linphone_address_unref(bobAddr);
	linphone_address_unref(aliceAddr);
	linphone_core_manager_destroy(marie);
	linphone_core_manager_destroy(pauline);
}

void send_admined_notify(void) {
	LinphoneCoreManager* marie = linphone_core_manager_new("marie_rc");
	LinphoneCoreManager* pauline = linphone_core_manager_new(transport_supported(LinphoneTransportTls) ? "pauline_rc" : "pauline_tcp_rc");
	ConferenceEventTester tester(marie->lc, pauline->identity);
	Conference::LocalConference localConf(pauline->lc, pauline->identity);
	LinphoneAddress *bobAddr = linphone_core_interpret_url(marie->lc, bobUri);
	LinphoneAddress *aliceAddr = linphone_core_interpret_url(marie->lc, aliceUri);
	Conference::Participant p1(bobAddr, false);
	Conference::Participant p2(aliceAddr, true);
	LinphoneEvent *lev = linphone_core_create_notify(pauline->lc, marie->identity, "Conference");
	string notify;

	localConf.addParticipant(p1);
	localConf.addParticipant(p2);
	notify = localConf.getHandler()->subscribeReceived(lev);
	tester.mHandler->notifyReceived(notify.c_str());

	BC_ASSERT_EQUAL(tester.mParticipants.size(), 2, int, "%d");
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(bobAddr)) != tester.mParticipants.end());
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(aliceAddr)) != tester.mParticipants.end());
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(bobAddr))->second == 0);
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(aliceAddr))->second == 1);

	notify = localConf.getHandler()->notifyParticipantSetAdmin(bobAddr, true);
	tester.mHandler->notifyReceived(notify.c_str());

	BC_ASSERT_EQUAL(tester.mParticipants.size(), 2, int, "%d");
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(bobAddr)) != tester.mParticipants.end());
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(aliceAddr)) != tester.mParticipants.end());
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(aliceAddr))->second == 1);
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(bobAddr))->second == 1);

	linphone_event_unref(lev);
	linphone_address_unref(bobAddr);
	linphone_address_unref(aliceAddr);
	linphone_core_manager_destroy(marie);
	linphone_core_manager_destroy(pauline);
}

void send_unadmined_notify(void) {
	LinphoneCoreManager* marie = linphone_core_manager_new("marie_rc");
	LinphoneCoreManager* pauline = linphone_core_manager_new(transport_supported(LinphoneTransportTls) ? "pauline_rc" : "pauline_tcp_rc");
	ConferenceEventTester tester(marie->lc, pauline->identity);
	Conference::LocalConference localConf(pauline->lc, pauline->identity);
	LinphoneAddress *bobAddr = linphone_core_interpret_url(marie->lc, bobUri);
	LinphoneAddress *aliceAddr = linphone_core_interpret_url(marie->lc, aliceUri);
	Conference::Participant p1(bobAddr, false);
	Conference::Participant p2(aliceAddr, true);
	LinphoneEvent *lev = linphone_core_create_notify(pauline->lc, marie->identity, "Conference");
	string notify;

	localConf.addParticipant(p1);
	localConf.addParticipant(p2);
	notify = localConf.getHandler()->subscribeReceived(lev);
	tester.mHandler->notifyReceived(notify.c_str());

	BC_ASSERT_EQUAL(tester.mParticipants.size(), 2, int, "%d");
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(bobAddr)) != tester.mParticipants.end());
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(aliceAddr)) != tester.mParticipants.end());
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(bobAddr))->second == 0);
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(aliceAddr))->second == 1);

	notify = localConf.getHandler()->notifyParticipantSetAdmin(aliceAddr, false);
	tester.mHandler->notifyReceived(notify.c_str());

	BC_ASSERT_EQUAL(tester.mParticipants.size(), 2, int, "%d");
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(bobAddr)) != tester.mParticipants.end());
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(aliceAddr)) != tester.mParticipants.end());
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(aliceAddr))->second == 0);
	BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(bobAddr))->second == 0);

	linphone_event_unref(lev);
	linphone_address_unref(bobAddr);
	linphone_address_unref(aliceAddr);
	linphone_core_manager_destroy(marie);
	linphone_core_manager_destroy(pauline);
}

void send_subscribe_receive_first_notify(void){
  LinphoneCoreManager* marie = linphone_core_manager_new("marie_rc");
  LinphoneCoreManager* pauline = linphone_core_manager_new(transport_supported(LinphoneTransportTls) ? "pauline_rc" : "pauline_tcp_rc");
  ConferenceEventTester tester(marie->lc, pauline->identity);
  LinphoneAddress *bobAddr = linphone_core_interpret_url(marie->lc, bobUri);
  LinphoneAddress *aliceAddr = linphone_core_interpret_url(marie->lc, aliceUri);
  string confId("conf233");

  BC_ASSERT_TRUE(wait_for_until(marie->lc,pauline->lc,&marie->stat.number_of_LinphoneRegistrationOk,1,1000));
  BC_ASSERT_TRUE(wait_for_until(marie->lc,pauline->lc,&pauline->stat.number_of_LinphoneRegistrationOk,1,1000));

  tester.mHandler->subscribe(confId);

  BC_ASSERT_TRUE(wait_for_until(marie->lc,pauline->lc,&pauline->stat.number_of_LinphoneSubscriptionIncomingReceived,1,1000));
  BC_ASSERT_TRUE(wait_for_until(marie->lc,pauline->lc,&pauline->stat.number_of_LinphoneSubscriptionActive,1,3000));
  wait_for_until(marie->lc,pauline->lc,&marie->stat.number_of_NotifyReceived,1,3000);

  BC_ASSERT_EQUAL(tester.mParticipants.size(), 2, int, "%d");
  BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(bobAddr)) != tester.mParticipants.end());
  BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(aliceAddr)) != tester.mParticipants.end());
  BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(bobAddr))->second == 0);
  BC_ASSERT_TRUE(tester.mParticipants.find(linphone_address_as_string(aliceAddr))->second == 1);

  tester.mHandler->unsubscribe();

  BC_ASSERT_TRUE(wait_for_until(marie->lc,pauline->lc,&pauline->stat.number_of_LinphoneSubscriptionTerminated,1,1000));

  linphone_address_unref(bobAddr);
  linphone_address_unref(aliceAddr);
  linphone_core_manager_destroy(marie);
  linphone_core_manager_destroy(pauline);
}

test_t conf_event_tests[] = {
	TEST_NO_TAG("First notify parsing", first_notify_parsing),
	TEST_NO_TAG("First notify parsing wrong conf", first_notify_parsing_wrong_conf),
	TEST_NO_TAG("Participant added", participant_added_parsing),
	TEST_NO_TAG("Participant not added", participant_not_added_parsing),
	TEST_NO_TAG("Participant deleted", participant_deleted_parsing),
	TEST_NO_TAG("Participant admined", participant_admined_parsing),
	TEST_NO_TAG("Participant unadmined", participant_unadmined_parsing),
	TEST_NO_TAG("Send first notify", send_first_notify),
	TEST_NO_TAG("Send participant added notify", send_added_notify),
	TEST_NO_TAG("Send participant removed notify", send_removed_notify),
	TEST_NO_TAG("Send participant admined notify", send_admined_notify),
	TEST_NO_TAG("Send participant unadmined notify", send_unadmined_notify)
  //TEST_NO_TAG("Send subscribe receive first notify", send_subscribe_receive_first_notify)
};

test_suite_t conf_event_test_suite = {"Conf event package", NULL, NULL, liblinphone_tester_before_each, liblinphone_tester_after_each,
								 sizeof(conf_event_tests) / sizeof(conf_event_tests[0]), conf_event_tests};
