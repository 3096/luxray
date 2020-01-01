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
