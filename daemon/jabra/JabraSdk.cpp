#include "JabraSdk.h"

#include <JabraNativeHid.h>

Daemon *JabraSdk::daemon = nullptr;

bool JabraSdk::initialized = false;
map<unsigned short, Jabra_DeviceInfo> JabraSdk::devices = {};
Jabra_DeviceInfo *JabraSdk::mainDevice = nullptr;

JabraSdk::JabraSdk(Daemon *app) {
    daemon = app;

    Jabra_SetAppID(JABRA_APPLICATION_ID);

    /* Initialize Jabra library with callbacks for attach, remove and input events */
    if (Jabra_InitializeV2(nullptr, JabraDeviceAttachedFunc, JabraDeviceRemovedFunc, nullptr,
                           JabraButtonInDataTranslatedFunc, false, nullptr)) {
        initialized = true;

        Jabra_ConnectToJabraApplication(JABRA_APPLICATION_GUID, JABRA_APPLICATION_NAME);
        Jabra_SetSoftphoneReady(true);
    }
}

JabraSdk::~JabraSdk() {
    if (initialized) {
        Jabra_DisconnectFromJabraApplication();
        Jabra_Uninitialize();
    }
}

void JabraSdk::JabraDeviceAttachedFunc(Jabra_DeviceInfo deviceInfo) {
    if (!initialized) return;

    string message = "Jabra device attached: " + string(deviceInfo.deviceName);
    linphone_logging_service_message(linphone_logging_service_get(), message.c_str());

    devices[deviceInfo.deviceID] = deviceInfo;

    if (deviceInfo.isDongle) {
        mainDevice = &devices[deviceInfo.deviceID];
    }

    // Set the HID state to GN_HID if the device supports it
    if (Jabra_IsGnHidStdHidSupported(deviceInfo.deviceID)) {
        Jabra_SetHidWorkingState(deviceInfo.deviceID, Jabra_HidState::GN_HID);
    }
}

void JabraSdk::JabraDeviceRemovedFunc(unsigned short deviceID) {
    if (!initialized) return;

    if (mainDevice && mainDevice->deviceID == deviceID) {
        mainDevice = nullptr;
    }

    Jabra_FreeDeviceInfo(devices[deviceID]);
    devices.erase(deviceID);
}

void
JabraSdk::JabraButtonInDataTranslatedFunc(unsigned short deviceID, Jabra_HidInput translatedInData, bool buttonInData) {
    if (!initialized) return;

    // Make sure we know about the device
    if (devices.count(deviceID) < 1) {
        return;
    }

    // Update main device to the device that sent the (latest) event
    // Just in case there is more than one device connected, but not ideal
    mainDevice = &devices[deviceID];

    LinphoneCall *call = linphone_core_get_current_call(daemon->getCore());

    // Get device lock, if we don't have it already
    if (!Jabra_IsLocked(mainDevice->deviceID)) {
        if (!Jabra_GetLock(mainDevice->deviceID)) {
            // Unable to get the lock. There is nothing we can do about it.
            return;
        }
    }

    switch (translatedInData) {
        case OffHook:
            // Acknowledge button press
            Jabra_SetOffHook(mainDevice->deviceID, buttonInData);

            if (call) {
                LinphoneCallState state = linphone_call_get_state(call);
                string state_string = linphone_call_state_to_string(state);

                if (buttonInData && isRingingState(state)) {
                    // Accept incoming call
                    linphone_call_accept(call);
                } else if (!buttonInData && (isOffHookState(state) || isHoldState(state))) {
                    // Hang up call
                    linphone_call_terminate(call);
                }
            } else {
                // No call, so just ensure the off-hook state is off
                Jabra_SetOffHook(mainDevice->deviceID, false);
            }
            break;
        case Mute:
            // Acknowledge button press
            Jabra_SetMute(mainDevice->deviceID, buttonInData);

            if (call) {
                // Apply mute state to call
                linphone_call_set_microphone_muted(call, buttonInData);
                daemon->queueEvent(new CallEvent(daemon, call, linphone_call_get_state(call)));
            } else {
                // No call, counteract the headsets mute state
                Jabra_SetMute(mainDevice->deviceID, !buttonInData);
            }
            break;
        case RejectCall:
            if (call) {
                // Reject incoming call
                linphone_call_decline(call, LinphoneReasonBusy);
            }
            break;
        case PseudoOffHook:
            // User pressed the off-hook button without a call, blocking the headset from further use
            // Acknowledge button press
            Jabra_SetOffHook(mainDevice->deviceID, buttonInData);

            if (!call) {
                // No call, so just ensure the off-hook state is off
                Jabra_SetOffHook(mainDevice->deviceID, false);
            }
            break;
        default:
            break;
    }

    if (!call) {
        if (Jabra_IsLocked(mainDevice->deviceID)) {
            Jabra_ReleaseLock(mainDevice->deviceID);
        }
    }
}

void JabraSdk::OnCallStateChanged(LinphoneCall *call, LinphoneCallState state) {
    if (!initialized) return;

    if (mainDevice == nullptr) {
        return;
    }

    string state_string = linphone_call_state_to_string(state);

    // Get device lock, if we don't have it already
    if (!Jabra_IsLocked(mainDevice->deviceID)) {
        Jabra_GetLock(mainDevice->deviceID);
    }

    if (Jabra_IsRingerSupported(mainDevice->deviceID)) {
        Jabra_SetRinger(mainDevice->deviceID, isRingingState(state));
    }

    if (Jabra_IsHoldSupported(mainDevice->deviceID)) {
        Jabra_SetHold(mainDevice->deviceID, isHoldState(state));
    }

    if (Jabra_IsOffHookSupported(mainDevice->deviceID)) {
        Jabra_SetOffHook(mainDevice->deviceID, isOffHookState(state));
    }

    if (Jabra_IsMuteSupported(mainDevice->deviceID)) {
        Jabra_SetMute(mainDevice->deviceID, linphone_call_get_microphone_muted(call));
    }

    if (isEndState(state)) {
        // Call ended, release the lock
        if (Jabra_IsLocked(mainDevice->deviceID)) {
            Jabra_ReleaseLock(mainDevice->deviceID);
        }
    }
}

bool JabraSdk::isRingingState(LinphoneCallState state) {
    return state == LinphoneCallState::LinphoneCallStateIncomingReceived;
}

bool JabraSdk::isHoldState(LinphoneCallState state) {
    return state == LinphoneCallState::LinphoneCallStatePausing ||
           state == LinphoneCallState::LinphoneCallStatePaused ||
           state == LinphoneCallState::LinphoneCallStateResuming;
}

bool JabraSdk::isOffHookState(LinphoneCallState state) {
    return state == LinphoneCallState::LinphoneCallStateOutgoingInit ||
           state == LinphoneCallState::LinphoneCallStateOutgoingProgress ||
           state == LinphoneCallState::LinphoneCallStateOutgoingRinging ||
           state == LinphoneCallState::LinphoneCallStateOutgoingEarlyMedia ||
           state == LinphoneCallState::LinphoneCallStateConnected ||
           state == LinphoneCallState::LinphoneCallStateStreamsRunning;
}

bool JabraSdk::isEndState(LinphoneCallState state) {
    return state == LinphoneCallState::LinphoneCallStateEnd ||
           state == LinphoneCallState::LinphoneCallStateReleased ||
           state == LinphoneCallState::LinphoneCallStateError;
}
