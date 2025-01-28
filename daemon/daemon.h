/*
 * Copyright (c) 2010-2022 Belledonne Communications SARL.
 *
 * This file is part of Liblinphone
 * (see https://gitlab.linphone.org/BC/public/liblinphone).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DAEMON_H_
#define DAEMON_H_

#include <bctoolbox/list.h>
#include <linphone/core.h>
#include <linphone/core_utils.h>
#include <mediastreamer2/mediastream.h>
#include <mediastreamer2/mscommon.h>

#include <linphone/core.h>
#include <linphone/core_utils.h>


#include <string>
#include "conference_private.h"

#include <list>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <timer.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_READLINE
#ifdef HAVE_READLINE_H
#include <readline.h>
#else
#ifdef HAVE_READLINE_READLINE_H
#include <readline/readline.h>
#endif
#endif
#endif
#ifdef HAVE_HISTORY_H
#include <history.h>
#else
#ifdef HAVE_READLINE_HISTORY_H
#include <readline/history.h>
#endif
#endif

#define API_VERSION 7

#define COMMANDNAME_POP_EVENT "pop-event"

class Daemon;

class DaemonCommandExample {
public:
	DaemonCommandExample(const std::string &command, const std::string &output);
	~DaemonCommandExample() {
	}
	const std::string &getCommand() const {
		return mCommand;
	}
	const std::string &getOutput() const {
		return mOutput;
	}

private:
	const std::string mCommand;
	const std::string mOutput;
};

class DaemonCommand {
public:
	virtual ~DaemonCommand() = default;
	virtual void exec(Daemon *app, const std::string &args) = 0;
	bool matches(const std::string &name) const;
	const std::string getHelp() const;
	const std::string &getProto() const {
		return mProto;
	}
	const std::string &getDescription() const {
		return mDescription;
	}
	const std::vector<std::unique_ptr<const DaemonCommandExample>> &getExamples() const {
		return mExamples;
	}
	void addExample(std::unique_ptr<const DaemonCommandExample> &&example);

protected:
	DaemonCommand(const std::string &name, const std::string &proto, const std::string &description);

	const std::string mName;
	const std::string mProto;
	const std::string mDescription;
	std::vector<std::unique_ptr<const DaemonCommandExample>> mExamples;
};

/*Base class for all kind of responses to commands*/
class Response {
public:
	enum Status { Ok = 0, Error = 1 };
	virtual ~Response() {
	}
	Response() : mStatus(Ok) {
	}
	Response(const std::string& commandName, const std::string& msg, Status status = Error):
	    mStatus(status) {
	    if( status == Ok) {
	        this->commandName = commandName;
	        mBody = msg;
	        mReason = "";
	    } else {
	        this->commandName = commandName;
	        mBody = "{ }";
	        mReason = msg;
	    }
	}

	void setStatus(Status st) {
		mStatus = st;
	}
	void setReason(const std::string &reason) {
		mReason = reason;
	}
	void setBody(const std::string &body) {
		mBody = body;
	}
	const std::string &getBody() const {
		return mBody;
	}

	virtual std::string toBuf() const {
	    std::ostringstream stringStream;
	    std::string status = (mStatus == Ok) ? "true" : "false";
	    //QString testFromJSON = "{"
        //                       "\"command\": { "
        //                       "\"description\": \"Version\""
        //                       "}"
        //                       "}";
        stringStream << "{ \"type\": \"command\", \"name\":" << "\"" << this->commandName.c_str() << "\"" << ", \"data\":" <<  this->mBody.c_str() << ", \"success\":" << status.c_str() << ", \"message\":" << "\"" << this->mReason.c_str() << "\"" << " } \n";
	    return stringStream.str();
	}

private:
	Status mStatus;
	std::string mReason;
	std::string mBody;
	std::string commandName;
};

/*Base class for all kind of event poping out of the linphonecore. They are posted to the Daemon's event queue with
 * queueEvent().*/
class Event {
public:
	Event(const std::string &eventType, const std::string &body = "") : mEventType(eventType), mBody(body) {
	}
	const std::string &getBody() const {
		return mBody;
	}
	void setBody(const std::string &body) {
		mBody = body;
	}
	virtual ~Event() {
	}
	virtual std::string toBuf() const {
	    std::string ost;
		std::ostringstream buf;
		buf << "{ \"type\": \"event\", \"name\": " << "\"" << this->mEventType.c_str() << "\"" << ", \"data\": " << this->mBody.c_str() << " }\n";

		/*buf << "Event-type: " << mEventType;
		if (!mBody.empty()) {
			buf << "\n" << mBody << "\n";
		}*/
		return buf.str();
	}

protected:
	const std::string mEventType;
	std::string mBody;
};

class CallEvent : public Event {
public:
	CallEvent(Daemon *daemon, LinphoneCall *call, LinphoneCallState state);
};

class ConferenceEvent : public Event {
public:
    ConferenceEvent(Daemon *daemon, LinphoneConference *conference, LinphoneConferenceState state);
};

class CallStatsEvent : public Event {
public:
	CallStatsEvent(Daemon *daemon, LinphoneCall *call, const LinphoneCallStats *stats);
};

class CallPlayingStatsEvent : public Event {
public:
	CallPlayingStatsEvent(Daemon *daemon, int id);
};

class DtmfEvent : public Event {
public:
	DtmfEvent(Daemon *daemon, LinphoneCall *call, int dtmf);
};

class AudioStreamStatsEvent : public Event {
public:
	AudioStreamStatsEvent(Daemon *daemon, AudioStream *stream, const LinphoneCallStats *stats);
};

class ProxyRegistrationChangedEvent: public Event {
public:
    ProxyRegistrationChangedEvent(Daemon *daemon, LinphoneProxyConfig *cfg, LinphoneRegistrationState cstate, const char *message);
};

class FriendPresenceStateChangedEvent: public Event {
public:
    FriendPresenceStateChangedEvent(Daemon *daemon, LinphoneFriend *_friend);
};

class PayloadTypeResponse : public Response {
public:
	PayloadTypeResponse(LinphoneCore *core,
	                    const LinphonePayloadType *payloadType,
	                    int index = -1,
	                    const std::string &prefix = std::string(),
	                    bool enabled_status = true);
};

class PayloadTypeParser {
public:
	PayloadTypeParser(LinphoneCore *core, const std::string &mime_type, bool accept_all = false);
	~PayloadTypeParser() {
		if (mPayloadType) linphone_payload_type_unref(mPayloadType);
	}
	inline bool all() {
		return mAll;
	}
	inline bool successful() {
		return mSuccesful;
	}
	inline LinphonePayloadType *getPayloadType() const {
		return mPayloadType;
	}
	inline int getPosition() const {
		return mPosition;
	}

private:
	bool mAll;
	bool mSuccesful;
	LinphonePayloadType *mPayloadType;
	int mPosition;
};

struct AudioStreamAndOther {
	AudioStream *stream;
	OrtpEvQueue *queue;
	LinphoneCallStats *stats;
	AudioStreamAndOther(AudioStream *as) : stream(as) {
		queue = ortp_ev_queue_new();
		rtp_session_register_event_queue(as->ms.sessions.rtp_session, queue);
	}
	~AudioStreamAndOther() {
		rtp_session_unregister_event_queue(stream->ms.sessions.rtp_session, queue);
		ortp_ev_queue_destroy(queue);
	}
};

class Daemon {
	friend class DaemonCommand;

public:
	typedef Response::Status Status;
	Daemon(const char *config_path,
	       const char *factory_config_path,
	       const char *log_file,
	       const char *pipe_name,
	       bool display_video,
	       bool capture_video);
	~Daemon();
	int run();
	void quit();
	void sendResponse(const Response &resp);
	std::string join(const std::vector<std::string>& values, std::string delimiter);
	std::string getJsonForCall(LinphoneCall *call);
	std::string getJsonForConference(LinphoneConference *conference);
	std::string getJsonForConferenceParticipant(LinphoneParticipant *linphoneParticipant);
	std::string getJsonForProxys(LinphoneProxyConfig *cfg);
	std::string getJsonForAudioDevice(const LinphoneAudioDevice* device);
	std::string getJsonForFriend(LinphoneFriend *_friend);
	std::string getJsonForPresenceService(const LinphonePresenceModel* model);
	std::string getJsonForPresenceActivities(const LinphonePresenceModel* model);
	std::string replaceAll(std::string str, const std::string& from, const std::string& to);
	std::string replaceEscapeChar(std::string replaceStr);
    std::string linphone_conference_state_to_string(LinphoneConferenceState state);
    float linearToDb(float volume);
	LinphoneAudioDevice * findAudioDevice(std::string idString);
	std::string linphoneAudioDeviceTypeToString(LinphoneAudioDeviceType linphoneAudioDeviceType) const;
	void queueEvent(Event *resp);
	LinphoneCore *getCore();
	LinphoneSoundDaemon *getLSD();
	const std::list<DaemonCommand *> &getCommandList() const;
	LinphoneCall *findCall(int id);
	LinphoneProxyConfig *findProxy(int id);
	LinphoneAuthInfo *findAuthInfo(int id);
	AudioStream *findAudioStream(int id);
	AudioStreamAndOther *findAudioStreamAndOther(int id);
	void removeAudioStream(int id);
	bool pullEvent();
	int updateCallId(LinphoneCall *call);
	int updateProxyId(LinphoneProxyConfig *proxy);
	inline int maxProxyId() {
		return mProxyIds;
	}
	inline int maxAuthInfoId() {
		return (int)bctbx_list_size(linphone_core_get_auth_info_list(mLc));
	}
	int updateAudioStreamId(AudioStream *audio_stream);
	void dumpCommandsHelp();
	void dumpCommandsHelpHtml();
	void enableStatsEvents(bool enabled);
	void enableLSD(bool enabled);
	void enableAutoAnswer(bool enabled);
	void callPlayingComplete(int id);
	void setAutoVideo(bool enabled) {
		mAutoVideo = enabled;
	}
	inline bool autoVideo() {
		return mAutoVideo;
	}

private:
	static void *iterateThread(void *arg);
	static void callStateChanged(LinphoneCore *lc, LinphoneCall *call, LinphoneCallState state, const char *msg);
	static void callStatsUpdated(LinphoneCore *lc, LinphoneCall *call, const LinphoneCallStats *stats);
	static void conference_state_changed(LinphoneCore *lc, LinphoneConference *conference, LinphoneConferenceState state);
	static void dtmfReceived(LinphoneCore *lc, LinphoneCall *call, int dtmf);
	static void messageReceived(LinphoneCore *lc, LinphoneChatRoom *cr, LinphoneChatMessage *msg);
	static void proxyRegistrationChanged(LinphoneCore *lc, LinphoneProxyConfig *cfg, LinphoneRegistrationState cstate, const char *message);
	static void friendPresenceStateChanged(LinphoneCore *lc, LinphoneFriend *_friend);
	void callStateChanged(LinphoneCall *call, LinphoneCallState state, const char *msg);
	void conference_state_changed(LinphoneConference *conference, LinphoneConferenceState state);
	void callStatsUpdated(LinphoneCall *call, const LinphoneCallStats *stats);
	void dtmfReceived(LinphoneCall *call, int dtmf);
	void messageReceived(LinphoneChatRoom *cr, LinphoneChatMessage *msg);
	void proxyRegistrationChanged(LinphoneProxyConfig *cfg, LinphoneRegistrationState cstate, const char *message);
	void friendPresenceStateChanged(LinphoneFriend *_friend);
	void execCommand(const std::string &command);
	std::string readLine(const std::string &, bool *);
	std::string readPipe();
	void iterate();
	void iterateStreamStats();
	void startThread();
	void stopThread();
	void initCommands();
	void uninitCommands();
    static void onTimerEvent(void* data);
    void resetTimer();
	LinphoneCore *mLc;
	LinphoneSoundDaemon *mLSD;
	std::list<DaemonCommand *> mCommands;
	std::queue<Event *> mEventQueue;
	ortp_pipe_t mServerFd;
	ortp_pipe_t mChildFd;
	std::string mHistfile;
	bool mRunning;
	bool mUseStatsEvents;
	bool mAutoAnswer;
	FILE *mLogFile;
	bool mAutoVideo;
	int mCallIds;
	int mProxyIds;
	int mAudioStreamIds;
	ms_thread_t mThread;
	ms_mutex_t mMutex;
    Timer *mTimer = NULL;
	std::map<int, AudioStreamAndOther *> mAudioStreams;
};

#endif // DAEMON_H_
