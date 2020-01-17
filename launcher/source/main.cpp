#include <stdio.h>
#include <unistd.h>
#include <string>

#include <switch.h>

#include "error.hpp"

const uint64_t LUXRAY_DOCKED_TITLE_ID = 0x0100000000000195;
const uint64_t LUXRAY_HANDHELD_TITLE_ID = 0x0100000000000405;

const char* LUXRAY_DOCKED_EXEFS_PATH = "sdmc:/atmosphere/contents/0100000000000195/exefs.nsp";
const char* LUXRAY_HANDHELD_EXEFS_PATH = "sdmc:/atmosphere/contents/0100000000000405/exefs.nsp";

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
    printf(
        "       A - Launch docked mode\n"
        "       Y - Launch handheld mode\n");

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
    printf(
        "       A - Relaunch docked mode\n"
        "       Y - Relaunch handheld mode\n"
        "       X - Stop Luxray\n");

    while (appletMainLoop()) {
        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_PLUS) {
            return false;
        }

        if (kDown & KEY_A) {
            stopLuxray(pid);
            pid = startLuxray(LUXRAY_DOCKED_TITLE_ID);
        }

        if (kDown & KEY_Y) {
            stopLuxray(pid);
            pid = startLuxray(LUXRAY_HANDHELD_TITLE_ID);
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
    Result res;

    res = pmshellInitialize();
    if (R_FAILED(res)) {
        throw ResultError("pmshellInitialize", res);
    }

    res = pmdmntInitialize();
    if (R_FAILED(res)) {
        throw ResultError("pmdmntInitialize", res);
    }

    while (appletMainLoop()) {
        printf(CONSOLE_ESC(1;1H) CONSOLE_ESC(2J)  // clears console
               CONSOLE_ESC(36;1m) "Luxray Launcher\n\n" CONSOLE_ESC(m));

        // check files
        if (access(LUXRAY_DOCKED_EXEFS_PATH, F_OK) != 0) {
            printf(CONSOLE_ESC(31;1m) "WARNING: did not find %s\n\n" CONSOLE_ESC(m), LUXRAY_DOCKED_EXEFS_PATH);
        }
        if (access(LUXRAY_HANDHELD_EXEFS_PATH, F_OK) != 0) {
            printf(CONSOLE_ESC(31;1m) "WARNING: did not find %s\n\n" CONSOLE_ESC(m), LUXRAY_HANDHELD_EXEFS_PATH);
        }

        printf("Press: + - Exit launcher\n");

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
        printf(CONSOLE_ESC(31;1m) "\nResult error %s: 0x%x\n" CONSOLE_ESC(m), resultError.what(),
               resultError.getResult());
        printf("\nPress + to exit...\n");
        while (appletMainLoop()) {
            consoleUpdate(NULL);
            hidScanInput();
            u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
            if (kDown & KEY_PLUS) break;
        }
    }

    consoleExit(NULL);
    return 0;
}
