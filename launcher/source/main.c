#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <switch.h>

const u64 LUXRAY_SYSMODULE_TITLEID = 0x0100000000000195;

Result getPID(u64* pid) {
    *pid = 0;
    Result res = pmdmntGetProcessId(pid, LUXRAY_SYSMODULE_TITLEID);
    if (R_FAILED(res)) {
        printf("pmdmntGetProcessId failed with %x\n", res);
        return res;
    }

    if (*pid == 0) {
        printf("PID not found?\n");
        return -1;
    }

    printf("PID found: %lx\n", *pid);
    return res;
}

Result startBackgroundService() {
    // already running
    u64 pid = 0;
    if (R_SUCCEEDED(getPID(&pid))) {
        printf("Already running?\n");
        return -1;
    }

    NcmProgramLocation location = {LUXRAY_SYSMODULE_TITLEID, NcmStorageId_None};
    return pmshellLaunchProgram(0, &location, &pid);
}

Result stopBackgroundService() {
    u64 pid = 0;
    Result res = getPID(&pid);
    if (R_SUCCEEDED(res)) {
        res = pmshellTerminateProcess(pid);
    }

    return res;
}

int main(int argc, char* argv[]) {
    consoleInit(NULL);

    printf("pmshellInitialize(): %x\n", pmshellInitialize());
    printf("pmdmntInitialize(): %x\n", pmdmntInitialize());

    while (appletMainLoop()) {
        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_PLUS) break;  // break in order to return to hbmenu

        if (kDown & KEY_A) {
            printf("stopBackgroundService(): %x\n", stopBackgroundService());
            printf("startBackgroundService(): %x\n", startBackgroundService());
        }

        if (kDown & KEY_B) {
            printf("stopBackgroundService(): %x\n", stopBackgroundService());
        }

        if (kDown & KEY_Y) {
            u64 pid;
            printf("getPID(): %x\n", getPID(&pid));
        }

        consoleUpdate(NULL);
    }

    pmshellExit();
    pmdmntExit();
    consoleExit(NULL);
    return 0;
}
