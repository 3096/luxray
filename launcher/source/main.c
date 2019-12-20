#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <switch.h>

const u64 LUXRAY_SYSMODULE_TITLEID = 0x0100000000000195;

Result startBackgroundService() {
    // already running
    u64 pid = 0;
    pmdmntGetProcessId(&pid, LUXRAY_SYSMODULE_TITLEID);
    if (pid != 0) return 0;

    NcmProgramLocation location = {LUXRAY_SYSMODULE_TITLEID, NcmStorageId_None};
    return pmshellLaunchProgram(0, &location, &pid);
}

Result stopBackgroundService() {
    u64 pid = 0;
    pmdmntGetProcessId(&pid, LUXRAY_SYSMODULE_TITLEID);
    if (pid == 0) return -1;

    return pmshellTerminateProcess(pid);
}

int main(int argc, char* argv[]) {
    consoleInit(NULL);

    printf("pmshellInitialize(): %x\n", pmshellInitialize());

    while (appletMainLoop()) {
        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_PLUS) break;  // break in order to return to hbmenu
        
        if (kDown & KEY_A) {
            printf("stopBackgroundService(): %x\n", stopBackgroundService());
            printf("startBackgroundService(): %x\n", startBackgroundService());
        }

        consoleUpdate(NULL);
    }

    pmshellExit();
    consoleExit(NULL);
    return 0;
}
