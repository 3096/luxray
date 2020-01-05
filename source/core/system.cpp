#include <stdexcept>
#include <string>

#include <switch.h>

#include "../debug.hpp"

#include "system.hpp"

bool os::nifmInternetIsConnected() {
    NifmInternetConnectionStatus nifmICS;
    Result rs = nifmGetInternetConnectionStatus(NULL, NULL, &nifmICS);
    if (R_FAILED(rs)) {
        // LOG("nifmGetInternetConnectionStatus failed with error code %x", rs);
        return false;
    }

    return nifmICS == NifmInternetConnectionStatus_Connected;
}

bool os::setsysInternetTimeSyncIsOn() {
    bool internetTimeSyncIsOn;
    Result rs = setsysIsUserSystemClockAutomaticCorrectionEnabled(&internetTimeSyncIsOn);
    if (R_FAILED(rs)) {
        std::string msg = "Unable to detect if Internet time sync is enabled: " + std::to_string(rs);
        throw std::runtime_error(msg);
    }

    return internetTimeSyncIsOn;
}
