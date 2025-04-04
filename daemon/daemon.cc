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

#include <cstdio>
#ifndef _WIN32
#include <sys/ioctl.h>
#endif

#include <signal.h>

#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <math.h>
#include <sstream>
#include <vector>
//#include "core/core.h"

#ifdef HAVE_READLINE
#include <readline/history.h>
#include <readline/readline.h>
#endif

#ifndef _WIN32
#include <poll.h>
#endif

#include <bctoolbox/defs.h>

#include "account/account.h"
#include "commands/adaptive-jitter-compensation.h"
#include "commands/answer.h"
#include "commands/audio-codec-get.h"
#include "commands/audio-codec-move.h"
#include "commands/audio-codec-set.h"
#include "commands/audio-codec-toggle.h"
#include "commands/audio-stream-start.h"
#include "commands/audio-stream-stats.h"
#include "commands/audio-stream-stop.h"
#include "commands/auth-infos-clear.h"
#include "commands/call-mute.h"
#include "commands/call-pause.h"
#include "commands/call-resume.h"
#include "commands/call-stats.h"
#include "commands/call-status.h"
#include "commands/call-transfer.h"
#include "commands/call.h"
#include "commands/cn.h"
#include "commands/conference-mute.h"
#include "commands/conference.h"
#include "commands/configcommand.h"
#include "commands/contact.h"
#include "commands/defaultProxy.h"
#include "commands/dtmf.h"
#include "commands/echo.h"
#include "commands/firewall-policy.h"
#include "commands/friends.h"
#include "commands/get-sound-card.h"
#include "commands/help.h"
#include "commands/ipv6.h"
#include "commands/jitterbuffer.h"
#include "commands/maxCall.h"
#include "commands/media-encryption.h"
#include "commands/message.h"
#include "commands/msfilter-add-fmtp.h"
#include "commands/netsim.h"
#include "commands/ping.h"
#include "commands/play-wav.h"
#include "commands/play.h"
#include "commands/pop-event.h"
#include "commands/port.h"
#include "commands/ptime.h"
#include "commands/quit.h"
#include "commands/register-info.h"
#include "commands/register-status.h"
#include "commands/register.h"
#include "commands/set-sound-card.h"
#include "commands/soundcards.h"
#include "commands/terminate.h"
#include "commands/unregister.h"
#include "commands/version.h"
#include "commands/video.h"
#include "commands/volume.h"
#include "daemon.h"

#include "private.h"

using namespace std;

#define INT_TO_VOIDPTR(i) ((void *)(intptr_t)(i))
#define VOIDPTR_TO_INT(p) ((int)(intptr_t)(p))

#ifndef WIN32
#else
#include <windows.h>
void usleep(int waitTime) {
	Sleep(waitTime / 1000);
}
#endif

#ifdef HAVE_READLINE
#define LICENCE_GPL
#else
#define LICENCE_COMMERCIAL
#endif

#ifdef __linux__
Timer *Timer::timer_instance = nullptr;
#endif

bool idleTimeout = false;

const char *const ice_state_str[] = {
    "Not activated",        /* LinphoneIceStateNotActivated */
    "Failed",               /* LinphoneIceStateFailed */
    "In progress",          /* LinphoneIceStateInProgress */
    "Host connection",      /* LinphoneIceStateHostConnection */
    "Reflexive connection", /* LinphoneIceStateReflexiveConnection */
    "Relayed connection"    /* LinphoneIceStateRelayConnection */
};

void *Daemon::iterateThread(void *arg) {
	Daemon *daemon = (Daemon *)arg;
	while (daemon->mRunning) {
		ms_mutex_lock(&daemon->mMutex);
		daemon->iterate();
		ms_mutex_unlock(&daemon->mMutex);
		usleep(20000);
	}
	return 0;
}

float Daemon::linearToDb(float volume) {
	return static_cast<float>(round(volume / 100));
}

std::string Daemon::replaceAll(std::string str, const std::string &from, const std::string &to) {
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}
	return str;
}

std::string Daemon::replaceEscapeChar(std::string replaceStr) {
	std::string returnStr;
	returnStr = replaceAll(replaceStr, "\\", "\\\\");
	returnStr = replaceAll(returnStr, "\"", "\\\"");
	return returnStr;
}

std::string Daemon::getJsonForConferenceParticipant(LinphoneParticipant *linphoneParticipant) {
	string linphoneAddress;
	const LinphoneAddress *lLinphoneAddress = linphone_participant_get_address(linphoneParticipant);
	LinphoneCall *call = linphone_core_get_call_by_remote_address2(this->getCore(), lLinphoneAddress);
	linphoneAddress += "{ \"address\": \"";
	linphoneAddress += linphone_address_as_string(lLinphoneAddress);
	linphoneAddress += "\", \"callId\": ";
	linphoneAddress += std::to_string(updateCallId(call));
	linphoneAddress += " }";
	return linphoneAddress;
}

std::string Daemon::linphone_conference_state_to_string(LinphoneConferenceState state) {
	switch (state) {
		case LinphoneConferenceStateNone:
			return "LinphoneConferenceStateNone";
			break;
		case LinphoneConferenceStateInstantiated:
			return "LinphoneConferenceStateInstantiated";
			break;
		case LinphoneConferenceStateCreationPending:
			return "LinphoneConferenceStateCreationPending";
			break;
		case LinphoneConferenceStateCreated:
			return "LinphoneConferenceStateCreated";
			break;
		case LinphoneConferenceStateCreationFailed:
			return "LinphoneConferenceStateCreationFailed";
			break;
		case LinphoneConferenceStateTerminationPending:
			return "LinphoneConferenceStateTerminationPending";
			break;
		case LinphoneConferenceStateTerminated:
			return "LinphoneConferenceStateTerminated";
			break;
		case LinphoneConferenceStateTerminationFailed:
			return "LinphoneConferenceStateTerminationFailed";
			break;
		case LinphoneConferenceStateDeleted:
			return "LinphoneConferenceStateDeleted";
			break;
	}
	return "";
}

std::string Daemon::getJsonForConference(LinphoneConference *conference) {
	float outputVolumeFloat = -1;
	float inputVolumeFloat = -1;
	std::string output_Device_Str = "";
	std::string input_Device_Str = "";

	LinphoneConferenceState conferenceState = LinphoneConferenceStateInstantiated;
	conferenceState = linphone_conference_get_state(conference);

	const LinphoneAudioDevice *output_device = nullptr;
	const LinphoneAudioDevice *input_device = nullptr;

	string micMutedStrForConference = "";
	string speakerMutedStrForConference = "";

	if (conferenceState != LinphoneConferenceState::LinphoneConferenceStateCreated) {
		micMutedStrForConference = "\"\"";
		speakerMutedStrForConference = "\"\"";
	} else {
		output_device = linphone_conference_get_output_audio_device(conference);
		input_device = linphone_conference_get_input_audio_device(conference);

		outputVolumeFloat = linphone_conference_get_output_volume_gain(conference);
		inputVolumeFloat = linphone_conference_get_input_volume_gain(conference);

		micMutedStrForConference = linphone_conference_get_microphone_muted(conference) ? "true" : "false";
		speakerMutedStrForConference = linphone_conference_get_speaker_muted(conference) ? "true" : "false";
	}

	if (output_device == nullptr) {
		output_Device_Str = "\"\"";
	} else {
		output_Device_Str = getJsonForAudioDevice(output_device);
	}
	if (input_device == nullptr) {
		input_Device_Str = "\"\"";
	} else {
		input_Device_Str = getJsonForAudioDevice(input_device);
	}
	if (micMutedStrForConference.empty()) {
		micMutedStrForConference = "{ }";
	}
	if (speakerMutedStrForConference.empty()) {
		speakerMutedStrForConference = "{ }";
	}

	string participants_string;
	const bctbx_list_t *elem;
	elem = linphone_conference_get_participant_list(conference);
	for (int index = 0; index < (int)bctbx_list_size(elem); index++) {
		LinphoneParticipant *lLinphoneParticipant = (LinphoneParticipant *)bctbx_list_nth_data(elem, index);
		participants_string += this->getJsonForConferenceParticipant(lLinphoneParticipant);
		if (index < (int)bctbx_list_size(elem) - 1) {
			participants_string += ",";
		}
	}

	ostringstream ost;
	ost << "{ \"conference\": { \"muted\": { "
	    << "\"input\": " << micMutedStrForConference << ", \"output\": " << speakerMutedStrForConference << " }"
	    << ", \"participants\": [ " << participants_string << " ] "
	    << ", \"state\": "
	    << "\"" << linphone_conference_state_to_string(conferenceState) << "\""
	    << ", \"soundcards\": { \"output\": " << output_Device_Str << ", \"input\": " << input_Device_Str << " }"
	    << ", \"volumes\": { \"output\": " << outputVolumeFloat << ", \"input\": " << inputVolumeFloat << " } } }";
	return ost.str();
}

std::string Daemon::getJsonForCall(LinphoneCall *call) {
	LinphoneCallState call_state = LinphoneCallIdle;
	call_state = linphone_call_get_state(call);

	LinphoneCallLog *callLog = linphone_call_get_call_log(call);
	const LinphoneAddress *fromAddr = linphone_call_log_get_from_address(callLog);
	const LinphoneAddress *toAddr = linphone_call_log_get_to_address(callLog);
	std::string toStr = "";
	std::string fromStr = "";
	toStr = linphone_address_as_string(toAddr);
	fromStr = linphone_address_as_string(fromAddr);
	toStr = replaceEscapeChar(toStr);
	fromStr = replaceEscapeChar(fromStr);

	const char *errorMessage;
	if (call_state != LinphoneCallState::LinphoneCallStateError) {
		errorMessage = "";
	} else {
		errorMessage = linphone_reason_to_string(linphone_call_get_reason(call));
	}
	replaceEscapeChar(errorMessage);

	const char *flag;
	bool_t in_conference;
	in_conference = (linphone_call_get_conference(call) != NULL);
	flag = in_conference ? "true" : "false";
	string direction = ((linphone_call_get_dir(call) == LinphoneCallOutgoing) ? "out" : "in");
	float outputVolumeFloat = -1;
	float inputVolumeFloat = -1;
	std::string output_Device_Str = "";
	std::string input_Device_Str = "";
	const LinphoneAudioDevice *output_device = linphone_call_get_output_audio_device(call);
	if (output_device == nullptr) {
		output_Device_Str = "\"\"";
	} else {
		output_Device_Str = getJsonForAudioDevice(output_device);
	}
	const LinphoneAudioDevice *input_device = linphone_call_get_input_audio_device(call);
	if (input_device == nullptr) {
		input_Device_Str = "\"\"";
	} else {
		input_Device_Str = getJsonForAudioDevice(input_device);
	}
	if (call_state == LinphoneCallState::LinphoneCallStateStreamsRunning) {
		outputVolumeFloat = linphone_call_get_speaker_volume_gain(call);
		inputVolumeFloat = linphone_call_get_microphone_volume_gain(call);
	}
	string micMutedStrForCall = linphone_call_get_microphone_muted(call) ? "true" : "false";
	string speakerMutedStrForCall = linphone_call_get_speaker_muted(call) ? "true" : "false";

	ostringstream ost;
	ost << "{ \"id\": " << updateCallId(call) << ", \"state\": "
	    << "\"" << linphone_call_state_to_string(call_state) << "\""
	    << ", \"addressFrom\": "
	    << "\"" << fromStr.c_str() << "\""
	    << ", \"addressTo\": "
	    << "\"" << toStr.c_str() << "\""
	    << ", \"direction\": "
	    << "\"" << direction << "\""
	    << ", \"duration\":" << linphone_call_get_duration(call) << ", \"inConference\": " << flag << ", \"muted\": { "
	    << "\"input\": " << micMutedStrForCall << ", \"output\": " << speakerMutedStrForCall << " }"
	    << ", \"errorMessage\": "
	    << "\"" << errorMessage << "\""
	    << ", \"volumes\": { \"output\": " << outputVolumeFloat << ", \"input\": " << inputVolumeFloat << " }"
	    << ", \"soundcards\": { \"output\": " << output_Device_Str << ", \"input\": " << input_Device_Str << " } }";
	return ost.str();
}

CallEvent::CallEvent(Daemon *daemon, LinphoneCall *call, LinphoneCallState state) : Event("call-state-changed") {
	if (state == LinphoneCallState::LinphoneCallStateEnd || state == LinphoneCallState::LinphoneCallStateError ||
	    state == LinphoneCallState::LinphoneCallStateReleased) {
		bool_t in_conference;
		in_conference = (linphone_call_get_conference(call) != NULL);
		if (in_conference) {
			linphone_core_remove_from_conference(daemon->getCore(), call);
		}
	}
	string callStr;
	callStr = "{ \"calls\": [ ";
	callStr += daemon->getJsonForCall(call);
	callStr += " ] }";
	setBody(callStr);
}
#ifndef _MSC_VER
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif // _MSC_VER
ConferenceEvent::ConferenceEvent(Daemon *daemon, LinphoneConference *conference, LinphoneConferenceState state)
    : Event("conference-state-changed") {
	setBody(daemon->getJsonForConference(conference));
}
#ifndef _MSC_VER
#pragma GCC diagnostic pop
#endif // _MSC_VER

DtmfEvent::DtmfEvent(Daemon *daemon, LinphoneCall *call, int dtmf) : Event("receiving-tone") {
	ostringstream ostr;
	const LinphoneAddress *address = linphone_call_get_remote_address(call);
	char *remote = linphone_address_as_string(address);
	ostr << "CallId: " << daemon->updateCallId(call) << "\n";
	ostr << "Tone: " << (char)dtmf << "\n";
	ostr << "From: " << remote;
	setBody(ostr.str());
	ms_free(remote);
}

#ifndef _MSC_VER
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif // _MSC_VER
AccountRegistrationChangedEvent::AccountRegistrationChangedEvent(Daemon *daemon,
                                                                 LinphoneAccount *account,
                                                                 LinphoneRegistrationState cstate,
                                                                 const char *message)
    : Event("proxy-registration-state-changed") {
	const LinphoneAccountParams *params = linphone_account_get_params(account);
	string accountStr;
	accountStr = "{ \"proxies\": [ ";
	accountStr += daemon->getJsonForAccountParams(params, account);
	accountStr += " ] }";

	setBody(accountStr);
	if (linphone_account_get_state(account) == LinphoneRegistrationCleared) {
		linphone_core_clear_all_auth_info(daemon->getCore());
	}
}
#ifndef _MSC_VER
#pragma GCC diagnostic pop
#endif // _MSC_VER

FriendPresenceStateChangedEvent::FriendPresenceStateChangedEvent(Daemon *daemon, LinphoneFriend *_friend)
    : Event("friend-presence-state-changed") {
	if (!daemon->getJsonForFriend(_friend).empty()) {
		string friendStr;
		friendStr = "{ \"friend\": ";
		friendStr += daemon->getJsonForFriend(_friend);
		friendStr += " }";

		setBody(friendStr);
	}
}

static ostream &printCallStatsHelper(ostream &ostr, const LinphoneCallStats *stats, const string &prefix) {
	ostr << prefix << "ICE state: " << ice_state_str[linphone_call_stats_get_ice_state(stats)] << "\n";
	ostr << prefix << "RoundTripDelay: " << linphone_call_stats_get_round_trip_delay(stats) << "\n";
	//	ostr << prefix << "Jitter: " << stats->jitter_stats.jitter << "\n";
	//	ostr << prefix << "MaxJitter: " << stats->jitter_stats.max_jitter << "\n";
	//	ostr << prefix << "SumJitter: " << stats->jitter_stats.sum_jitter << "\n";
	//	ostr << prefix << "MaxJitterTs: " << stats->jitter_stats.max_jitter_ts << "\n";
	ostr << prefix << "JitterBufferSizeMs: " << linphone_call_stats_get_jitter_buffer_size_ms(stats) << "\n";

	ostr << prefix << "Received-InterarrivalJitter: " << linphone_call_stats_get_receiver_interarrival_jitter(stats)
	     << "\n";
	ostr << prefix << "Received-FractionLost: " << linphone_call_stats_get_receiver_loss_rate(stats) << "\n";

	ostr << prefix << "Sent-InterarrivalJitter: " << linphone_call_stats_get_sender_interarrival_jitter(stats) << "\n";
	ostr << prefix << "Sent-FractionLost: " << linphone_call_stats_get_sender_loss_rate(stats) << "\n";
	return ostr;
}

CallStatsEvent::CallStatsEvent(Daemon *daemon, LinphoneCall *call, const LinphoneCallStats *stats)
    : Event("call-stats") {
	const LinphoneCallParams *callParams = linphone_call_get_current_params(call);
	const char *prefix = "";

	ostringstream ostr;
	ostr << "CallId: " << daemon->updateCallId(call) << "\n";
	ostr << "Type: ";
	if (linphone_call_stats_get_type(stats) == LINPHONE_CALL_STATS_AUDIO) {
		ostr << "Audio";
	} else {
		ostr << "Video";
	}
	ostr << "\n";

	printCallStatsHelper(ostr, stats, prefix);

	if (linphone_call_stats_get_type(stats) == LINPHONE_CALL_STATS_AUDIO) {
		const LinphonePayloadType *audioCodec = linphone_call_params_get_used_audio_payload_type(callParams);
		ostr << PayloadTypeResponse(linphone_call_get_core(call), audioCodec, -1, prefix, false).getBody() << "\n";
	} else {
		const LinphonePayloadType *videoCodec = linphone_call_params_get_used_video_payload_type(callParams);
		ostr << PayloadTypeResponse(linphone_call_get_core(call), videoCodec, -1, prefix, false).getBody() << "\n";
	}

	setBody(ostr.str());
}

AudioStreamStatsEvent::AudioStreamStatsEvent(Daemon *daemon, AudioStream *stream, const LinphoneCallStats *stats)
    : Event("audio-stream-stats") {
	const char *prefix = "";

	ostringstream ostr;
	ostr << "AudioStreamId: " << daemon->updateAudioStreamId(stream) << "\n";
	ostr << "Type: ";
	if (linphone_call_stats_get_type(stats) == LINPHONE_CALL_STATS_AUDIO) {
		ostr << "Audio";
	} else {
		ostr << "Video";
	}
	ostr << "\n";

	printCallStatsHelper(ostr, stats, prefix);

	setBody(ostr.str());
}

CallPlayingStatsEvent::CallPlayingStatsEvent(BCTBX_UNUSED(Daemon *daemon), int id) : Event("call-playing-complete") {
	ostringstream ostr;

	ostr << "Id: " << id << "\n";

	setBody(ostr.str());
}

PayloadTypeResponse::PayloadTypeResponse(BCTBX_UNUSED(LinphoneCore *core),
                                         const LinphonePayloadType *payloadType,
                                         int index,
                                         const string &prefix,
                                         bool enabled_status) {
	ostringstream ostr;
	if (payloadType != NULL) {
		if (index >= 0) ostr << prefix << "Index: " << index << "\n";
		const char *recv_fmtp = linphone_payload_type_get_recv_fmtp(payloadType);
		const char *send_fmtp = linphone_payload_type_get_send_fmtp(payloadType);
		ostr << prefix << "Payload-type-number: " << linphone_payload_type_get_number(payloadType) << "\n";
		ostr << prefix << "Clock-rate: " << linphone_payload_type_get_clock_rate(payloadType) << "\n";
		ostr << prefix << "Bitrate: " << linphone_payload_type_get_normal_bitrate(payloadType) << "\n";
		ostr << prefix << "Mime: " << linphone_payload_type_get_mime_type(payloadType) << "\n";
		ostr << prefix << "Channels: " << linphone_payload_type_get_channels(payloadType) << "\n";
		ostr << prefix << "Recv-fmtp: " << (recv_fmtp ? recv_fmtp : "") << "\n";
		ostr << prefix << "Send-fmtp: " << (send_fmtp ? send_fmtp : "") << "\n";
		if (enabled_status)
			ostr << prefix << "Enabled: " << (linphone_payload_type_enabled(payloadType) == TRUE ? "true" : "false")
			     << "\n";
		setBody(ostr.str().c_str());
	}
}

PayloadTypeParser::PayloadTypeParser(LinphoneCore *core, const string &mime_type, bool accept_all)
    : mAll(false), mSuccesful(true), mPayloadType(NULL), mPosition(-1) {
	int number = -1;
	if (accept_all && (mime_type.compare("ALL") == 0)) {
		mAll = true;
		return;
	}
	istringstream ist(mime_type);
	ist >> number;
	if (ist.fail()) {
		char type[64] = {0};
		int rate, channels;
		if (sscanf(mime_type.c_str(), "%63[^/]/%u/%u", type, &rate, &channels) != 3) {
			mSuccesful = false;
			return;
		}
		mPayloadType = linphone_core_get_payload_type(core, type, rate, channels);
		if (mPayloadType) {
			bctbx_list_t *codecs = linphone_core_get_audio_payload_types(core);
			bctbx_list_t *elem;
			int index = 0;
			for (elem = codecs; elem != NULL; elem = elem->next, ++index) {
				if (linphone_payload_type_weak_equals((LinphonePayloadType *)elem->data, mPayloadType)) {
					mPosition = index;
					break;
				}
			}
			bctbx_list_free_with_data(codecs, (bctbx_list_free_func)linphone_payload_type_unref);
		}
	} else if (number != -1) {
		bctbx_list_t *codecs = linphone_core_get_audio_payload_types(core);
		bctbx_list_t *elem;
		for (elem = codecs; elem != NULL; elem = elem->next) {
			if (number == linphone_payload_type_get_number((LinphonePayloadType *)elem->data)) {
				mPayloadType = linphone_payload_type_ref((LinphonePayloadType *)elem->data);
				break;
			}
		}
	}
}

DaemonCommandExample::DaemonCommandExample(const string &command, const string &output)
    : mCommand(command), mOutput(output) {
}

DaemonCommand::DaemonCommand(const string &name, const string &proto, const string &description)
    : mName(name), mProto(proto), mDescription(description) {
}

void DaemonCommand::addExample(std::unique_ptr<const DaemonCommandExample> &&example) {
	mExamples.emplace_back(std::move(example));
}

const string DaemonCommand::getHelp() const {
	ostringstream ost;
	ost << getProto() << endl << endl;
	ost << "Description:" << endl << getDescription() << endl << endl;
	int c = 1;
	for (const auto &example : getExamples()) {
		ost << "Example " << c++ << ":" << endl;
		ost << ">" << example->getCommand() << endl;
		ost << example->getOutput() << endl;
		ost << endl;
	}
	return ost.str();
}

bool DaemonCommand::matches(const string &name) const {
	return mName.compare(name) == 0;
}

Daemon::Daemon(const char *config_path,
               const char *factory_config_path,
               const char *log_file,
               const char *pipe_name,
               bool display_video,
               bool capture_video)
    : mLSD(0), mLogFile(NULL), mAutoVideo(0), mCallIds(0), mProxyIds(0), mAudioStreamIds(0) {
	ms_mutex_init(&mMutex, NULL);
	mServerFd = (bctbx_pipe_t)-1;
	mChildFd = (bctbx_pipe_t)-1;
	if (pipe_name == NULL) {
#ifdef HAVE_READLINE
		const char *homedir = getenv("HOME");
		rl_readline_name = (char *)"daemon";
		if (homedir == NULL) homedir = ".";
		mHistfile = string(homedir) + string("/.linphone_history");
		read_history(mHistfile.c_str());
		setlinebuf(stdout);
#endif
	} else {
		mServerFd = bctbx_server_pipe_create(pipe_name);
#ifndef _WIN32
		listen(mServerFd, 2);
		fprintf(stdout, "Server unix socket created, name=%s fd=%i\n", pipe_name, (int)mServerFd);
#else
		fprintf(stdout, "Named pipe  created, name=%s fd=%p\n", pipe_name, mServerFd);
#endif
	}

	if (log_file != NULL) {
		mLogFile = fopen(log_file, "a+");
		linphone_core_enable_logs(mLogFile);
	} else {
		linphone_core_disable_logs();
	}

	LinphoneCoreVTable vtable;
	memset(&vtable, 0, sizeof(vtable));
	vtable.account_registration_state_changed = accountRegistrationChanged;
	vtable.call_state_changed = callStateChanged;
	// vtable.call_stats_updated = callStatsUpdated;
	vtable.dtmf_received = dtmfReceived;
	vtable.message_received = messageReceived;
	vtable.notify_presence_received = friendPresenceStateChanged;
	vtable.conference_state_changed = conference_state_changed;
	mLc = linphone_core_new(&vtable, config_path, factory_config_path, this);
	linphone_core_set_user_data(mLc, this);
	linphone_core_enable_video_capture(mLc, capture_video);
	linphone_core_enable_video_display(mLc, display_video);

	for (const bctbx_list_t *account = linphone_core_get_account_list(mLc); account != NULL;
	     account = bctbx_list_next(account)) {
		updateProxyId((LinphoneAccount *)bctbx_list_get_data(account));
	}

	initCommands();
	mUseStatsEvents = true;

#ifdef HAVE_JABRA
	jabra = new JabraSdk(this);
#endif
}

const list<DaemonCommand *> &Daemon::getCommandList() const {
	return mCommands;
}

LinphoneCore *Daemon::getCore() {
	return mLc;
}

LinphoneSoundDaemon *Daemon::getLSD() {
	return mLSD;
}

int Daemon::updateCallId(LinphoneCall *call) {
	int val = VOIDPTR_TO_INT(linphone_call_get_user_data(call));
	if (val == 0) {
		linphone_call_set_user_data(call, INT_TO_VOIDPTR(++mCallIds));
		return mCallIds;
	}
	return val;
}

LinphoneAudioDevice *Daemon::findAudioDevice(std::string idString) {
	bctbx_list_t *deviceList = linphone_core_get_extended_audio_devices(getCore());
	LinphoneAudioDevice *pDevice = NULL;
	while (deviceList != NULL) {
		pDevice = (LinphoneAudioDevice *)deviceList->data;
		if (linphone_audio_device_get_id(pDevice) == idString) {
			bctbx_list_free_with_data(deviceList, (void (*)(void *))linphone_audio_device_unref);
			return pDevice;
		}

		deviceList = deviceList->next;
	}
	bctbx_list_free_with_data(deviceList, (void (*)(void *))linphone_audio_device_unref);
	return pDevice;
}

std::string Daemon::linphoneAudioDeviceTypeToString(LinphoneAudioDeviceType linphoneAudioDeviceType) const {
	std::string type;
	switch (linphoneAudioDeviceType) {
		case LinphoneAudioDeviceType::LinphoneAudioDeviceTypeMicrophone:
			type = "Microphone";
			break;
		case LinphoneAudioDeviceType::LinphoneAudioDeviceTypeEarpiece:
			type = "Earpiece";
			break;
		case LinphoneAudioDeviceType::LinphoneAudioDeviceTypeSpeaker:
			type = "Speaker";
			break;
		case LinphoneAudioDeviceType::LinphoneAudioDeviceTypeBluetooth:
			type = "Bluetooth";
			break;
		case LinphoneAudioDeviceType::LinphoneAudioDeviceTypeBluetoothA2DP:
			type = "BluetoothA2DP";
			break;
		case LinphoneAudioDeviceType::LinphoneAudioDeviceTypeTelephony:
			type = "Telephony";
			break;
		case LinphoneAudioDeviceType::LinphoneAudioDeviceTypeAuxLine:
			type = "AuxLine";
			break;
		case LinphoneAudioDeviceType::LinphoneAudioDeviceTypeGenericUsb:
			type = "Generic USB";
			break;
		case LinphoneAudioDeviceType::LinphoneAudioDeviceTypeHeadset:
			type = "Headset";
			break;
		case LinphoneAudioDeviceType::LinphoneAudioDeviceTypeHeadphones:
			type = "Headphones";
			break;
		case LinphoneAudioDeviceType::LinphoneAudioDeviceTypeUnknown:
		default:
			type = "Unknown";
			break;
	}
	return type;
}

std::string Daemon::getJsonForPresenceActivities(const LinphonePresenceModel *model) {
	const char *activityStr = "";
	std::string vectorStr = "";
	vector<std::string> v;

	unsigned int nbActivities = linphone_presence_model_get_nb_activities(model);
	for (unsigned int i = 0; i < nbActivities; i++) {
		LinphonePresenceActivity *activity = linphone_presence_model_get_nth_activity(model, i);
		if (activity == NULL) {
			activityStr = linphone_presence_activity_to_string(activity);
			vectorStr = activityStr;
			v.push_back(vectorStr);
		}
	}

	ostringstream ost;
	std::string activities = this->join(v, ", ");
	if (activities.empty()) {
		activities = " ";
	}

	ost << "\"activities\": [" << activities.c_str() << "] ";

	return ost.str();
}

std::string Daemon::getJsonForPresenceService(const LinphonePresenceModel *model) {
	vector<std::string> v;

	unsigned int nbServices = linphone_presence_model_get_nb_services(model);
	for (unsigned int i = 0; i < nbServices; i++) {
		LinphonePresenceService *service = linphone_presence_model_get_nth_service(model, i);
		if (!service) continue;

		bctbx_list_t *services_descriptions = linphone_presence_service_get_service_descriptions(service);
		while (services_descriptions) {
			char *description = (char *)bctbx_list_get_data(services_descriptions);
			std::string vectorStr = "";
			vectorStr = description;
			v.push_back(vectorStr);
		}
	}
	ostringstream ost;
	std::string descriptions = this->join(v, ", ");
	if (descriptions.empty()) {
		descriptions = " ";
	}

	ost << "\"descriptions\": [" << descriptions.c_str() << "] ";

	return ost.str();
}

std::string Daemon::getJsonForFriend(LinphoneFriend *_friend) {
	ostringstream ost;

	const LinphoneAddress *address = linphone_friend_get_address(_friend);
	char *friendAddress = linphone_address_as_string(address);
	const LinphonePresenceModel *model = linphone_friend_get_presence_model(_friend);

	ost << "{ \"address\": "
	    << "\"" << friendAddress << "\", \"presenceModel\": { ";

	if (model == NULL) {
		return NULL;
	} else if (model != NULL) {
		const char *note_content = "";
		const char *isOnlineStr = "";

		// Notes
		LinphonePresenceNote *note = linphone_presence_model_get_note(model, NULL);
		if (note) {
			note_content = linphone_presence_note_get_content(note);
		}

		// IsOnline
		switch (linphone_presence_model_is_online(model)) {
			case true:
				isOnlineStr = "true";
				break;
			case false:
				isOnlineStr = "false";
				break;
		}

		ost << "\"note\": "
		    << "\"" << note_content << "\""
		    << ", " << this->getJsonForPresenceActivities(model) << ", " << this->getJsonForPresenceService(model)
		    << ", \"isOnline\": " << isOnlineStr;
	}

	ost << " } }";

	return ost.str();
}

std::string Daemon::getJsonForAudioDevice(const LinphoneAudioDevice *device) {
	ostringstream ost;
	std::string deviceId(linphone_audio_device_get_id(device));
	std::string deviceType(linphoneAudioDeviceTypeToString(linphone_audio_device_get_type(device)));
	std::string deviceName(linphone_audio_device_get_device_name(device));
	std::string driverName(linphone_audio_device_get_driver_name(device));
	std::string canPlay;
	std::string canRecord;
	/*std::string isDefaultOutputString;
	std::string isDefaultInputString;
	std::string isDefaultRingerString;*/

	bool isDefaultOutput = false;
	bool isDefaultInput = false;
	bool isDefaultRinger = false;

	const LinphoneAudioDevice *output_device = linphone_core_get_default_output_audio_device(getCore());
	isDefaultOutput = device == output_device;

	const LinphoneAudioDevice *input_device = linphone_core_get_default_input_audio_device(getCore());
	isDefaultInput = device == input_device;

	const LinphoneAudioDevice *ringer_device;
	const std::string &ringer_Device_Str = linphone_core_get_ringer_device(getCore());
	ringer_device = findAudioDevice(ringer_Device_Str);
	isDefaultRinger = device == ringer_device;

	switch (linphone_audio_device_get_capabilities(device)) {
		case LinphoneAudioDeviceCapabilityAll:
			canPlay = "true";
			canRecord = "true";
			break;
		case LinphoneAudioDeviceCapabilityPlay:
			canPlay = "true";
			canRecord = "false";
			break;
		case LinphoneAudioDeviceCapabilityRecord:
			canPlay = "false";
			canRecord = "true";
			break;
	}
	std::string canPlayStrTrue = "true";
	ost << "{ \"id\": "
	    << "\"" << deviceId << "\""
	    << ", \"driver\": "
	    << "\"" << driverName.c_str() << "\""
	    << ", \"type\": "
	    << "\"" << deviceType << "\""
	    << ", \"name\": "
	    << "\"" << deviceName.c_str() << "\""
	    << ", \"canRecord\": " << canRecord << ", \"canPlay\": " << canPlay
	    << ", \"isDefaultOutput\": " << (isDefaultOutput ? "true" : "false")
	    << ", \"isDefaultInput\": " << (isDefaultInput ? "true" : "false")
	    << ", \"isDefaultRinger\": " << (isDefaultRinger ? "true" : "false") << " }";
	return ost.str();
}

LinphoneCall *Daemon::findCall(int id) {
	const bctbx_list_t *elem = linphone_core_get_calls(mLc);
	for (; elem != NULL; elem = elem->next) {
		LinphoneCall *call = (LinphoneCall *)elem->data;
		if (VOIDPTR_TO_INT(linphone_call_get_user_data(call)) == id) return call;
	}
	return NULL;
}

int Daemon::updateProxyId(LinphoneAccount *account) {
	int val = VOIDPTR_TO_INT(linphone_account_get_user_data(account));
	if (val == 0) {
		linphone_account_set_user_data(account, INT_TO_VOIDPTR(++mProxyIds));
		return mProxyIds;
	}
	return val;
}

LinphoneAccount *Daemon::findProxy(int id) {
	const bctbx_list_t *elem = linphone_core_get_account_list(mLc);
	for (; elem != NULL; elem = elem->next) {
		LinphoneAccount *account = (LinphoneAccount *)elem->data;
		if (VOIDPTR_TO_INT(linphone_account_get_user_data(account)) == id) return account;
	}
	return NULL;
}

LinphoneAuthInfo *Daemon::findAuthInfo(int id) {
	const bctbx_list_t *elem = linphone_core_get_auth_info_list(mLc);
	if (elem == NULL || id < 1 || (unsigned int)id > bctbx_list_size(elem)) {
		return NULL;
	}
	while (id > 1) {
		elem = elem->next;
		--id;
	}
	return (LinphoneAuthInfo *)elem->data;
}

int Daemon::updateAudioStreamId(AudioStream *audio_stream) {
	for (map<int, AudioStreamAndOther *>::iterator it = mAudioStreams.begin(); it != mAudioStreams.end(); ++it) {
		if (it->second->stream == audio_stream) return it->first;
	}

	++mAudioStreamIds;
	mAudioStreams.insert(make_pair(mAudioStreamIds, new AudioStreamAndOther(audio_stream)));
	return mAudioStreamIds;
}

AudioStreamAndOther *Daemon::findAudioStreamAndOther(int id) {
	map<int, AudioStreamAndOther *>::iterator it = mAudioStreams.find(id);
	if (it != mAudioStreams.end()) return it->second;
	return NULL;
}

AudioStream *Daemon::findAudioStream(int id) {
	map<int, AudioStreamAndOther *>::iterator it = mAudioStreams.find(id);
	if (it != mAudioStreams.end()) return it->second->stream;
	return NULL;
}

void Daemon::removeAudioStream(int id) {
	map<int, AudioStreamAndOther *>::iterator it = mAudioStreams.find(id);
	if (it != mAudioStreams.end()) {
		mAudioStreams.erase(it);
		delete (it->second);
	}
}

static bool compareCommands(const DaemonCommand *command1, const DaemonCommand *command2) {
	return (command1->getProto() < command2->getProto());
}

void Daemon::initCommands() {
	mCommands.push_back(new RegisterCommand());
	mCommands.push_back(new ContactCommand());
	mCommands.push_back(new RegisterStatusCommand());
	mCommands.push_back(new RegisterInfoCommand());
	mCommands.push_back(new UnregisterCommand());
	mCommands.push_back(new AuthInfosClearCommand());
	mCommands.push_back(new CallCommand());
	mCommands.push_back(new TerminateCommand());
	mCommands.push_back(new DtmfCommand());
	mCommands.push_back(new PlayWavCommand());
	mCommands.push_back(new PopEventCommand());
	mCommands.push_back(new AnswerCommand());
	mCommands.push_back(new SoundcardCommand());
	mCommands.push_back(new GetSoundCard());
	mCommands.push_back(new SetSoundCard());
	mCommands.push_back(new DefaultProxy());
	mCommands.push_back(new VolumeCommand());
	mCommands.push_back(new MaxCallsCommand());
	mCommands.push_back(new Friends());
	mCommands.push_back(new PingCommand());
	mCommands.push_back(new CallStatusCommand());
	mCommands.push_back(new CallStatsCommand());
	mCommands.push_back(new CallPauseCommand());
	mCommands.push_back(new CallMuteCommand());
	mCommands.push_back(new CallResumeCommand());
	mCommands.push_back(new CallTransferCommand());
	mCommands.push_back(new ConferenceMuteCommand());
	mCommands.push_back(new Video());
	mCommands.push_back(new Video::Preview());
	mCommands.push_back(new VideoSource());
	mCommands.push_back(new VideoSourceGet());
	mCommands.push_back(new VideoSourceList());
	mCommands.push_back(new VideoSourceSet());
	mCommands.push_back(new VideoSourceReload());
	mCommands.push_back(new VideoDisplayGet());
	mCommands.push_back(new VideoDisplaySet());
	mCommands.push_back(new AutoVideo());
	mCommands.push_back(new ConferenceCommand());
	mCommands.push_back(new AudioCodecGetCommand());
	mCommands.push_back(new AudioCodecEnableCommand());
	mCommands.push_back(new AudioCodecDisableCommand());
	mCommands.push_back(new AudioCodecMoveCommand());
	mCommands.push_back(new AudioCodecSetCommand());
	mCommands.push_back(new AudioStreamStartCommand());
	mCommands.push_back(new AudioStreamStopCommand());
	mCommands.push_back(new AudioStreamStatsCommand());
	mCommands.push_back(new MSFilterAddFmtpCommand());
	mCommands.push_back(new PtimeCommand());
	mCommands.push_back(new IPv6Command());
	mCommands.push_back(new FirewallPolicyCommand());
	mCommands.push_back(new MediaEncryptionCommand());
	mCommands.push_back(new PortCommand());
	mCommands.push_back(new AdaptiveBufferCompensationCommand());
	mCommands.push_back(new JitterBufferCommand());
	mCommands.push_back(new JitterBufferResetCommand());
	mCommands.push_back(new VersionCommand());
	mCommands.push_back(new QuitCommand());
	mCommands.push_back(new HelpCommand());
	mCommands.push_back(new ConfigGetCommand());
	mCommands.push_back(new ConfigSetCommand());
	mCommands.push_back(new NetsimCommand());
	mCommands.push_back(new CNCommand());
	mCommands.push_back(new IncallPlayerStartCommand());
	mCommands.push_back(new IncallPlayerStopCommand());
	mCommands.push_back(new IncallPlayerPauseCommand());
	mCommands.push_back(new IncallPlayerResumeCommand());
	mCommands.push_back(new EchoCalibrationCommand());
	mCommands.push_back(new MessageCommand());
	mCommands.sort(compareCommands);
}

void Daemon::uninitCommands() {
	while (!mCommands.empty()) {
		delete mCommands.front();
		mCommands.pop_front();
	}
}

bool Daemon::pullEvent() {
	bool status = false;
	ostringstream ostr;
	size_t size = mEventQueue.size();

	if (size != 0) size--;

	ostr << "Size: " << size << "\n"; // size is the number items remaining in the queue after popping the event.

	if (!mEventQueue.empty()) {
		Event *e = mEventQueue.front();
		mEventQueue.pop();
		ostr << e->toBuf() << "\n";
		delete e;
		status = true;
	}

	sendResponse(Response(COMMANDNAME_POP_EVENT, ostr.str().c_str(), Response::Ok));
	return status;
}

void Daemon::callStateChanged(LinphoneCall *call, LinphoneCallState state, BCTBX_UNUSED(const char *msg)) {
	queueEvent(new CallEvent(this, call, state));

	if (state == LinphoneCallIncomingReceived && mAutoAnswer) {
		linphone_call_accept(call);
	}

#ifdef HAVE_JABRA
	if (jabra) {
		jabra->OnCallStateChanged(call, state);
	}
#endif
}

void Daemon::conference_state_changed(LinphoneConference *conference, LinphoneConferenceState state) {
	queueEvent(new ConferenceEvent(this, conference, state));
}

void Daemon::messageReceived(BCTBX_UNUSED(LinphoneChatRoom *cr), LinphoneChatMessage *msg) {
	queueEvent(new IncomingMessageEvent(msg));
}

void Daemon::callStatsUpdated(LinphoneCall *call, const LinphoneCallStats *stats) {
	if (mUseStatsEvents) {
		/* don't queue periodical updates (3 per seconds for just bandwidth updates) */
		if (!(_linphone_call_stats_get_updated(stats) & LINPHONE_CALL_STATS_PERIODICAL_UPDATE)) {
			queueEvent(new CallStatsEvent(this, call, stats));
		}
	}
}

void Daemon::callPlayingComplete(int id) {
	queueEvent(new CallPlayingStatsEvent(this, id));
}

void Daemon::dtmfReceived(LinphoneCall *call, int dtmf) {
	queueEvent(new DtmfEvent(this, call, dtmf));
}

void Daemon::accountRegistrationChanged(LinphoneAccount *account,
                                        LinphoneRegistrationState cstate,
                                        const char *message) {
	queueEvent(new AccountRegistrationChangedEvent(this, account, cstate, message));
}

void Daemon::friendPresenceStateChanged(LinphoneFriend *_friend) {
	queueEvent(new FriendPresenceStateChangedEvent(this, _friend));
}

void Daemon::callStateChanged(LinphoneCore *lc, LinphoneCall *call, LinphoneCallState state, const char *msg) {
	Daemon *app = (Daemon *)linphone_core_get_user_data(lc);
	app->callStateChanged(call, state, msg);
}

void Daemon::conference_state_changed(LinphoneCore *lc, LinphoneConference *conference, LinphoneConferenceState state) {
	Daemon *app = (Daemon *)linphone_core_get_user_data(lc);
	app->conference_state_changed(conference, state);
}

void Daemon::callStatsUpdated(LinphoneCore *lc, LinphoneCall *call, const LinphoneCallStats *stats) {
	Daemon *app = (Daemon *)linphone_core_get_user_data(lc);
	app->callStatsUpdated(call, stats);
}
void Daemon::dtmfReceived(LinphoneCore *lc, LinphoneCall *call, int dtmf) {
	Daemon *app = (Daemon *)linphone_core_get_user_data(lc);
	app->dtmfReceived(call, dtmf);
}

void Daemon::accountRegistrationChanged(LinphoneCore *lc,
                                        LinphoneAccount *account,
                                        LinphoneRegistrationState cstate,
                                        const char *message) {
	Daemon *app = (Daemon *)linphone_core_get_user_data(lc);
	app->accountRegistrationChanged(account, cstate, message);
}

void Daemon::friendPresenceStateChanged(LinphoneCore *lc, LinphoneFriend *_friend) {
	Daemon *app = (Daemon *)linphone_core_get_user_data(lc);
	app->friendPresenceStateChanged(_friend);
}

void Daemon::messageReceived(LinphoneCore *lc, LinphoneChatRoom *cr, LinphoneChatMessage *msg) {
	Daemon *app = (Daemon *)linphone_core_get_user_data(lc);
	app->messageReceived(cr, msg);
}

void Daemon::iterateStreamStats() {
	for (map<int, AudioStreamAndOther *>::iterator it = mAudioStreams.begin(); it != mAudioStreams.end(); ++it) {
		OrtpEvent *ev;
		while (it->second->queue && (NULL != (ev = ortp_ev_queue_get(it->second->queue)))) {
			OrtpEventType evt = ortp_event_get_type(ev);
			if (evt == ORTP_EVENT_RTCP_PACKET_RECEIVED || evt == ORTP_EVENT_RTCP_PACKET_EMITTED) {
				linphone_call_stats_fill(it->second->stats, &it->second->stream->ms, ev);
				if (mUseStatsEvents)
					mEventQueue.push(new AudioStreamStatsEvent(this, it->second->stream, it->second->stats));
			}
			ortp_event_destroy(ev);
		}
	}
}

void Daemon::iterate() {
	linphone_core_iterate(mLc);
	iterateStreamStats();
	if (mChildFd == (bctbx_pipe_t)-1) {
		if (!mEventQueue.empty()) {
			Event *r = mEventQueue.front();
			mEventQueue.pop();
			fprintf(stdout, "\n%s\n", r->toBuf().c_str());
			fflush(stdout);
			delete r;
		}
	} else {
		if (!mEventQueue.empty()) {
			Event *r = mEventQueue.front();
			mEventQueue.pop();
			string buf = r->toBuf();
			if (mChildFd != (bctbx_pipe_t)-1) {
				if (bctbx_pipe_write(mChildFd, (uint8_t *)buf.c_str(), (int)buf.size()) == -1) {
					ms_error("Fail to write to pipe: %s", strerror(errno));
				}
			} else {
				cout << buf << flush;
			}
			fflush(stdout);
			delete r;
		}
	}
}

void Daemon::execCommand(const string &command) {
	size_t findpos = 0;
	size_t strleng = 0;
	string execCommand;
	string ergCommand;
	execCommand = command.substr(0, command.length());
	while (strleng <= execCommand.length()) {
		findpos = execCommand.find("\n", 0);
		ergCommand = execCommand.substr(0, findpos + 1);
		execCommand = execCommand.substr(ergCommand.length(), execCommand.length());
		strleng++;
		// stringstream ist(execCommand);
		istringstream ist(ergCommand);
		string name;
		ist >> name;
		stringbuf argsbuf;
		ist.get(argsbuf);
		string args = argsbuf.str();
		if (!args.empty() && (args[0] == ' ')) args.erase(0, 1);
		list<DaemonCommand *>::iterator it =
		    find_if(mCommands.begin(), mCommands.end(), [&name](const DaemonCommand *dc) { return dc->matches(name); });
		if (it != mCommands.end()) {
			ms_mutex_lock(&mMutex);
			(*it)->exec(this, args);
			ms_mutex_unlock(&mMutex);
		} else {
			sendResponse(Response(name, "Unknown command.", Response::Error));
		}
	}
}

void Daemon::sendResponse(const Response &resp) {
	string buf = resp.toBuf();
	if (mChildFd != (bctbx_pipe_t)-1) {
		if (bctbx_pipe_write(mChildFd, (uint8_t *)buf.c_str(), (int)buf.size()) == -1) {
			ms_error("Fail to write to pipe: %s", strerror(errno));
		}
	} else {
		cout << buf << flush;
	}
}

std::string Daemon::join(const vector<string> &values, string delimiter) {
	string result;

	for (vector<string>::const_iterator value = values.begin(); value != values.end(); ++value) {
		result += *value;
		if (value != values.end() - 1) result += delimiter;
	}
	return result;
}

std::string Daemon::getJsonForAccountParams(const LinphoneAccountParams *params, LinphoneAccount *account) {
	ostringstream ost;
	std::string serverAddr = linphone_account_params_get_server_addr(params);
	std::string identity = linphone_account_params_get_identity(params);
	bool isDefaulProxy = false;

	const char *errorMessage = linphone_error_info_get_phrase(linphone_account_get_error_info(account));

	if (LinphonePrivate::Account::toCpp(account)->getConfig() ==
	    linphone_core_get_default_proxy_config(this->getCore())) {
		isDefaulProxy = true;
	} else {
		isDefaulProxy = false;
	}

	ost << "{ \"id\": " << updateProxyId(account) << ", \"address\": "
	    << "\"" << serverAddr.c_str() << "\""
	    << ", \"identity\": "
	    << "\"" << identity.c_str() << "\""
	    << ", \"state\": "
	    << "\"" << linphone_registration_state_to_string(linphone_account_get_state(account)) << "\""
	    << ", \"defaultProxy\": "
	    << "\"" << (isDefaulProxy ? "true" : "false") << "\""
	    << ", \"errorMessage\": ";

	if (errorMessage != nullptr) {
		ost << "\"" << errorMessage << "\"";
	} else {
		ost << "\"\"";
	}

	ost << " }";
	return ost.str();
}

void Daemon::queueEvent(Event *ev) {
	mEventQueue.push(ev);
}

string Daemon::readPipe() {
	char buffer[32768];
	memset(buffer, '\0', sizeof(buffer));
#ifdef _WIN32
	if (mChildFd == (bctbx_pipe_t)-1) {
		mChildFd = bctbx_server_pipe_accept_client(mServerFd);
		ms_message("Client accepted");
	}
	if (mChildFd != (bctbx_pipe_t)-1) {
		int ret = bctbx_pipe_read(mChildFd, (uint8_t *)buffer, sizeof(buffer));
		if (ret == -1) {
			ms_error("Fail to read from pipe: %s", strerror(errno));
			mChildFd = (bctbx_pipe_t)-1;
		} else {
			if (ret == 0) {
				ms_message("Client disconnected");
				mChildFd = (bctbx_pipe_t)-1;
				return "";
			}
			buffer[ret] = '\0';
			return buffer;
		}
	}
#else
	struct pollfd pfd[2];
	int nfds = 1;
	memset(&pfd[0], 0, sizeof(pfd));
	if (mServerFd != (bctbx_pipe_t)-1) {
		pfd[0].events = POLLIN;
		pfd[0].fd = mServerFd;
	}
	if (mChildFd != (bctbx_pipe_t)-1) {
		pfd[1].events = POLLIN;
		pfd[1].fd = mChildFd;
		nfds++;
	}
	int err = poll(pfd, (nfds_t)nfds, 50);
	if (err > 0) {
		if (mServerFd != (bctbx_pipe_t)-1 && (pfd[0].revents & POLLIN)) {
			struct sockaddr_storage addr;
			socklen_t addrlen = sizeof(addr);
			int childfd = accept(mServerFd, (struct sockaddr *)&addr, &addrlen);
			if (childfd != -1) {
				if (mChildFd != (bctbx_pipe_t)-1) {
					ms_error("Cannot accept two client at the same time");
					close(childfd);
				} else {
					mChildFd = (bctbx_pipe_t)childfd;
					return "";
				}
			}
		}
		if (mChildFd != (bctbx_pipe_t)-1 && (pfd[1].revents & POLLIN)) {
			int ret;
			if ((ret = bctbx_pipe_read(mChildFd, (uint8_t *)buffer, sizeof(buffer))) == -1) {
				ms_error("Fail to read from pipe: %s", strerror(errno));
			} else {
				if (ret == 0) {
					ms_message("Client disconnected");
					bctbx_server_pipe_close_client(mChildFd);
					mChildFd = (bctbx_pipe_t)-1;
					return "";
				}
				buffer[ret] = '\0';
				return buffer;
			}
		}
	}
#endif
	return "";
}

void Daemon::dumpCommandsHelp() {
	int cols = 80;
#ifdef TIOCGSIZE
	struct ttysize ts;
	ioctl(STDIN_FILENO, TIOCGSIZE, &ts);
	cols = ts.ts_cols;
#elif defined(TIOCGWINSZ)
	struct winsize ts;
	ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
	cols = ts.ws_col;
#endif

	cout << endl;
	for (list<DaemonCommand *>::iterator it = mCommands.begin(); it != mCommands.end(); ++it) {
		cout << setfill('-') << setw(cols) << "-" << endl << endl;
		cout << (*it)->getHelp();
	}
}

static string htmlEscape(const string &orig) {
	string ret = orig;
	size_t pos;

	while (1) {
		pos = ret.find('<');
		if (pos != string::npos) {
			ret.erase(pos, 1);
			ret.insert(pos, "&lt");
			continue;
		}
		pos = ret.find('>');
		if (pos != string::npos) {
			ret.erase(pos, 1);
			ret.insert(pos, "&gt");
			continue;
		}
		break;
	}
	while (1) {
		pos = ret.find('\n');
		if (pos != string::npos) {
			ret.erase(pos, 1);
			ret.insert(pos, "<br>");
			continue;
		}
		break;
	}
	return ret;
}

void Daemon::dumpCommandsHelpHtml() {
	cout << endl;
	cout << "<!DOCTYPE html><html><body>" << endl;
	cout << "<h1>List of linphone-daemon commands.</h1>" << endl;
	for (const auto &cmd : mCommands) {
		cout << "<h2>" << htmlEscape(cmd->getProto()) << "</h2>" << endl;
		cout << "<h3>"
		     << "Description"
		     << "</h3>" << endl;
		cout << "<p>" << htmlEscape(cmd->getDescription()) << "</p>" << endl;
		cout << "<h3>"
		     << "Examples"
		     << "</h3>" << endl;
		const auto &examples = cmd->getExamples();
		cout << "<p><i>";
		for (const auto &example : examples) {
			cout << "<b>" << htmlEscape("Linphone-daemon>") << htmlEscape(example->getCommand()) << "</b><br>" << endl;
			cout << htmlEscape(example->getOutput()) << "<br>" << endl;
			cout << "<br><br>";
		}
		cout << "</i></p>" << endl;
	}

	cout << "</body></html>" << endl;
}

static void printHelp() {
	cout << "daemon-linphone [<options>]" << endl
	     <<
#if defined(LICENCE_GPL) || defined(LICENCE_COMMERCIAL)
	    "Licence: "
#ifdef LICENCE_GPL
	    "GPL"
#endif
#ifdef LICENCE_COMMERCIAL
	    "Commercial"
#endif
	     << endl
	     <<
#endif

	    "where options are :" << endl
	     << "\t--help                     Print this notice." << endl
	     << "\t--dump-commands-help       Dump the help of every available commands." << endl
	     << "\t--dump-commands-html-help  Dump the help of every available commands." << endl
	     << "\t--pipe <pipename>          Create an unix server socket in /tmp to receive commands from." << endl
	     << "\t--log <path>               Supply a file where the log will be saved." << endl
	     << "\t--factory-config <path>    Supply a readonly linphonerc style config file to start with." << endl
	     << "\t--config <path>            Supply a linphonerc style config file to start with." << endl
	     << "\t--disable-stats-events     Do not automatically raise RTP statistics events." << endl
             << "\t--disable-idle-timeout     Do not automatically clear proxy config after 10 seconds of inactivity." << endl
	     << "\t--enable-lsd               Use the linphone sound daemon." << endl
	     << "\t-C                         Enable video capture." << endl
	     << "\t-D                         Enable video display." << endl
	     << "\t--auto-answer              Automatically answer incoming calls." << endl
	     << "\t--list-soundcards          List all soundcards" << endl
	     << "\t--version                  Prints the daemon version number" << endl
	     << "\t--api-version              Prints the api version number" << endl;
}

void Daemon::startThread() {
	ms_thread_create(&this->mThread, NULL, Daemon::iterateThread, this);
}

#ifdef max
#undef max
#endif

string Daemon::readLine(const string &prompt, bool *eof) {
	*eof = false;
#ifdef HAVE_READLINE
	return readline(prompt.c_str());
#else
	if (cin.eof()) {
		*eof = true;
		return "";
	}
	cout << prompt;
	stringbuf outbuf;
	cin.get(outbuf);
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	return outbuf.str();
#endif
}

void Daemon::onTimerEvent(void *data) {
	Daemon *daemon = (Daemon *)data;
	linphone_core_clear_proxy_config(daemon->getCore());
}

void Daemon::resetTimer() {
	if (!idleTimeout) {
		if (mTimer != NULL) {
			mTimer->Stop();
		}
		mTimer->SetCallback(onTimerEvent, this);
		mTimer->Start(10000);
	}
}

int Daemon::run() {
	const string prompt("daemon-linphone>");
	mRunning = true;
	startThread();

	mTimer = new Timer();
	this->resetTimer();

	while (mRunning) {
		bool eof = false;
		if (mServerFd == (bctbx_pipe_t)-1) {
			// Read from console
			string line = readLine(prompt, &eof);
			if (!line.empty()) {
				this->resetTimer();
#ifdef HAVE_READLINE
				add_history(line.c_str());
#endif
				execCommand(line + "\n");
			}
		} else {
			// Read from pipe and split lines on \n
			string lines = readPipe();
			if (!lines.empty()) {
				this->resetTimer();
				lines = replaceAll(lines, "\r\n", "\n");

				size_t pos = 0;
				string delimiter = "\n";
				while ((pos = lines.find(delimiter)) != std::string::npos) {
					string line = lines.substr(0, pos);
					lines.erase(0, pos + delimiter.length());

					execCommand(line + "\n");
				}
			}
		}

		if (eof && mRunning) {
			mRunning = false; // ctrl+d
			cout << "Quitting..." << endl;
		}
	}
	stopThread();
	this->mTimer->Stop();
	return 0;
}

void Daemon::stopThread() {
	void *ret;
	ms_thread_join(mThread, &ret);
}

void Daemon::quit() {
	mRunning = false;
}

void Daemon::enableStatsEvents(bool enabled) {
	mUseStatsEvents = enabled;
}

void Daemon::enableAutoAnswer(bool enabled) {
	mAutoAnswer = enabled;
}

void Daemon::enableLSD(bool enabled) {
	if (mLSD) linphone_sound_daemon_destroy(mLSD);
	linphone_core_use_sound_daemon(mLc, NULL);
	if (enabled) {
		mLSD = linphone_sound_daemon_new(mLc->factory, NULL, 44100, 1);
		linphone_core_use_sound_daemon(mLc, mLSD);
	}
}

Daemon::~Daemon() {
	uninitCommands();

#ifdef HAVE_JABRA
	if (jabra) {
		delete jabra;
	}
#endif

	for (map<int, AudioStreamAndOther *>::iterator it = mAudioStreams.begin(); it != mAudioStreams.end(); ++it) {
		audio_stream_stop(it->second->stream);
	}

	enableLSD(false);
	linphone_core_unref(mLc);
	if (mChildFd != (bctbx_pipe_t)-1) {
		bctbx_server_pipe_close_client(mChildFd);
	}
	if (mServerFd != (bctbx_pipe_t)-1) {
		bctbx_server_pipe_close(mServerFd);
	}
	if (mLogFile != NULL) {
		linphone_core_enable_logs(NULL);
		fclose(mLogFile);
	}

	ms_mutex_destroy(&mMutex);

#ifdef HAVE_READLINE
	stifle_history(30);
	write_history(mHistfile.c_str());
#endif
}

static Daemon *the_app = NULL;

static void sighandler(BCTBX_UNUSED(int signum)) {
	if (the_app) {
		the_app->quit();
		the_app = NULL;
	}
}

static void listSoundcards() {
	const char **devices;
	size_t ndev;
	const char *msplugins_dir;
	const char *image_resources_dir;

	LinphoneFactory *lfactory = linphone_factory_get();
	msplugins_dir = linphone_factory_get_msplugins_dir(lfactory);
	image_resources_dir = linphone_factory_get_image_resources_dir(lfactory);
	MSFactory *factory = ms_factory_new_with_voip_and_directories(msplugins_dir, image_resources_dir);

	const bctbx_list_t *elem = ms_snd_card_manager_get_list(ms_factory_get_snd_card_manager(factory));
	ndev = bctbx_list_size(elem);
	devices = reinterpret_cast<const char **>(ms_malloc((ndev + 1) * sizeof(const char *)));
	for (int i = 0; elem != NULL; elem = elem->next, i++) {
		devices[i] = ms_snd_card_get_string_id((MSSndCard *)elem->data);
	}
	devices[ndev] = NULL;

	for (int i = 0; devices[i] != NULL; ++i) {
		cout << devices[i] << "\n";
	}
}

#if defined(__APPLE__)
extern "C" int apple_main(int argc, char **argv) {
#else
int main(int argc, char *argv[]) {
#endif
	const char *config_path = NULL;
	const char *factory_config_path = NULL;
	const char *pipe_name = NULL;
	const char *log_file = NULL;
	bool capture_video = false;
	bool display_video = false;
	bool stats_enabled = true;
	bool lsd_enabled = false;
	bool auto_answer = false;
	int i;

	for (i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "--help") == 0) {
			printHelp();
			return 0;
		} else if (strcmp(argv[i], "--dump-commands-help") == 0) {
			Daemon app(NULL, NULL, NULL, NULL, false, false);
			app.dumpCommandsHelp();
			return 0;
		} else if (strcmp(argv[i], "--dump-commands-html-help") == 0) {
			Daemon app(NULL, NULL, NULL, NULL, false, false);
			app.dumpCommandsHelpHtml();
			return 0;
		} else if (strcmp(argv[i], "--pipe") == 0) {
			if (i + 1 >= argc) {
				fprintf(stderr, "no pipe name specified after --pipe\n");
				return -1;
			}
			pipe_name = argv[++i];
			stats_enabled = false;
		} else if (strcmp(argv[i], "--factory-config") == 0) {
			if (i + 1 >= argc) {
				fprintf(stderr, "no file specify after --factory-config\n");
				return -1;
			}
			factory_config_path = argv[i + 1];
			i++;
		} else if (strcmp(argv[i], "--config") == 0) {
			if (i + 1 >= argc) {
				fprintf(stderr, "no file specify after --config\n");
				return -1;
			}
			config_path = argv[i + 1];
			i++;
		} else if (strcmp(argv[i], "--log") == 0) {
			if (i + 1 >= argc) {
				fprintf(stderr, "no file specify after --log\n");
				return -1;
			}
			log_file = argv[i + 1];
			i++;
		} else if (strcmp(argv[i], "-C") == 0) {
			capture_video = true;
		} else if (strcmp(argv[i], "-D") == 0) {
			display_video = true;
		} else if (strcmp(argv[i], "--disable-stats-events") == 0) {
			stats_enabled = false;
		} else if (strcmp(argv[i], "--enable-lsd") == 0) {
			lsd_enabled = true;
		} else if (strcmp(argv[i], "--auto-answer") == 0) {
			auto_answer = true;
		} else if (strcmp(argv[i], "--list-soundcards") == 0) {
			listSoundcards();
			return 0;
		} else if (strcmp(argv[i], "--disable-idle-timeout") == 0) {
			idleTimeout = true;
		} else if (strcmp(argv[i], "--version") == 0) {
			printf("Version: %s", linphone_core_get_version());
			return 0;
		} else if (strcmp(argv[i], "--api-version") == 0) {
			printf("ApiVersion: %d", API_VERSION);
			return 0;
		} else {
			fprintf(stderr, "Unrecognized option : %s", argv[i]);
		}
	}
	Daemon app(config_path, factory_config_path, log_file, pipe_name, display_video, capture_video);

	the_app = &app;
	signal(SIGINT, sighandler);
	app.enableStatsEvents(stats_enabled);
	app.enableLSD(lsd_enabled);
	app.enableAutoAnswer(auto_answer);
	return app.run();
}
