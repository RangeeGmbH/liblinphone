#ifndef LINPHONE_SDK_JABRASDK_H
#define LINPHONE_SDK_JABRASDK_H

#include <sstream>
#include <map>

#include <linphone/core.h>

#include <Common.h>
#include <JabraDeviceConfig.h>

#include "daemon.h"

using namespace std;

class Daemon;

class JabraSdk {
public:
    virtual ~JabraSdk();

    explicit JabraSdk(Daemon *app);

    static void OnCallStateChanged(LinphoneCall *call, LinphoneCallState state);

protected:
    static Daemon *daemon;

    static bool initialized;
    static map<unsigned short, Jabra_DeviceInfo> devices;
    static Jabra_DeviceInfo *mainDevice;

    static bool isRingingState(LinphoneCallState state);

    static bool isHoldState(LinphoneCallState state);

    static bool isOffHookState(LinphoneCallState state);

    static bool isEndState(LinphoneCallState state);

    /**
     * @brief Static callback called when a Jabra device is attached.
     * @param[in] deviceInfo structure containing misc. information about the device attached.
     */
    static void JabraDeviceAttachedFunc(Jabra_DeviceInfo deviceInfo);

    /**
     * @brief Static callback called when a Jabra device is removed.
     * @param[in] deviceID API device ID used for API calls, used to distinguish the Jabra devices from each other.
     */
    static void JabraDeviceRemovedFunc(unsigned short deviceID);

    /**
     * Static callback called when button data is sent by the Jabra device.
     * @brief JabraButtonInDataTranslatedFunc
     * @param[in] deviceID API device ID used for API calls, used to distinguish the Jabra devices from each other.
     * @param[in] translatedInData the HID input that caused the callback to be called.
     * @param[in] buttonInData the value of the HID input.
     */
    static void
    JabraButtonInDataTranslatedFunc(unsigned short deviceID, Jabra_HidInput translatedInData, bool buttonInData);
};

#endif //LINPHONE_SDK_JABRASDK_H
