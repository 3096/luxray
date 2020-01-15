#include <stdio.h>
#include <string>

#include <switch.h>

#include "error.hpp"

const uint64_t LUXRAY_DOCKED_TITLE_ID = 0x0100000000000195;
const uint64_t LUXRAY_HANDHELD_TITLE_ID = 0x0100000000000405;

uint64_t getPID(uint64_t& outPID) {
    if (R_SUCCEEDED(pmdmntGetProcessId(&outPID, LUXRAY_DOCKED_TITLE_ID)) or
        R_SUCCEEDED(pmdmntGetProcessId(&outPID, LUXRAY_HANDHELD_TITLE_ID))) {
        return true;
    }
    return false;
}

uint64_t startLuxray(uint64_t titleID) {
    uint64_t pid = 0;
    NcmProgramLocation location = {titleID, NcmStorageId_None};
    Result res = pmshellLaunchProgram(0, &location, &pid);
    if (R_FAILED(res)) {
        throw ResultError("pmshellLaunchProgram", res);
    }
    return pid;
}

void stopLuxray(uint64_t pid) {
    Result res = pmshellTerminateProcess(pid);
    if (R_FAILED(res)) {
        throw ResultError("pmshellTerminateProcess", res);
    }
}

bool showStartOptions() {
    printf("Starting Message\n");

    while (appletMainLoop()) {
        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_PLUS) {
            return false;
        }

        if (kDown & KEY_A) {
            startLuxray(LUXRAY_DOCKED_TITLE_ID);
            return true;
        }

        if (kDown & KEY_Y) {
            startLuxray(LUXRAY_HANDHELD_TITLE_ID);
            return true;
        }

        consoleUpdate(NULL);
    }
    return false;
}

bool showRunningOptions(uint64_t pid) {
    printf("Running Message\n");

    while (appletMainLoop()) {
        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_PLUS) {
            return false;
        }

        if (kDown & KEY_A) {
            stopLuxray(pid);
            startLuxray(LUXRAY_DOCKED_TITLE_ID);
            return true;
        }

        if (kDown & KEY_Y) {
            stopLuxray(pid);
            startLuxray(LUXRAY_HANDHELD_TITLE_ID);
            return true;
        }

        if (kDown & KEY_X) {
            stopLuxray(pid);
            return true;
        }

        consoleUpdate(NULL);
    }
    return false;
}

void launcherMain() {
    // TODO check luxray files

    Result res;

    res = pmshellInitialize();
    if (R_FAILED(res)) {
        throw ResultError("pmshellInitialize", res);
    }

    res = pmdmntInitialize();
    if (R_FAILED(res)) {
        throw ResultError("pmdmntInitialize", res);
    }

    printf("Welcome Message\n");

    while (appletMainLoop()) {
        printf("Usage Message\n");

        uint64_t pid;
        if (getPID(pid)) {
            if (not showRunningOptions(pid)) {
                break;
            }
        } else {
            if (not showStartOptions()) {
                break;
            }
        }
    }

    pmshellExit();
    pmdmntExit();
}

int main(int argc, char* argv[]) {
    consoleInit(NULL);

    try {
        launcherMain();
    } catch (ResultError& resultError) {
        printf("Result error %s: 0x%x\n", resultError.what(), resultError.getResult());
        printf("Press + to exit...\n");
        while (appletMainLoop()) {
            hidScanInput();
            u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
            if (kDown & KEY_PLUS) break;
        }
    }

    consoleExit(NULL);
    return 0;
}
