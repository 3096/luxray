#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <string>

#include <switch.h>

#include "debug.hpp"
#include "lvgl/lvgl.h"
#include "overlay.hpp"
#include "screens/time_screen.hpp"
#include "util.hpp"

extern "C" {
u32 __nx_applet_type = AppletType_None;
TimeServiceType __nx_time_service_type = TimeServiceType_System;
char nx_inner_heap[INNER_HEAP_SIZE];

u32 __nx_nv_transfermem_size = 0x15000;

void __libnx_initheap(void);
void __libnx_init_time(void);
void __appInit(void);
void __appExit(void);
}

Overlay* gp_overlay;
TimeScreen* gp_timeScreen;

void __libnx_initheap(void) {
    extern char* fake_heap_start;
    extern char* fake_heap_end;
    fake_heap_start = nx_inner_heap;
    fake_heap_end = nx_inner_heap + sizeof(nx_inner_heap);
}

void __appInit(void) {
    // Init/exit services
    Result rc;
    if (R_FAILED(smInitialize())) fatalThrow(MAKERESULT(Module_Libnx, LibnxError_InitFail_SM));
    if (R_FAILED(hidInitialize())) fatalThrow(MAKERESULT(Module_Libnx, LibnxError_InitFail_HID));
    if (R_FAILED(fsInitialize())) fatalThrow(MAKERESULT(Module_Libnx, LibnxError_InitFail_FS));
    if (R_FAILED(rc = setsysInitialize())) fatalThrow(rc);
    if (R_FAILED(rc = nifmInitialize(NifmServiceType_User))) fatalThrow(rc);

    fsdevMountSdmc();
}

void __appExit(void) {
    // Cleanup services.
    fsdevUnmountAll();
    nifmExit();
    setsysExit();
    timeExit();
    fsExit();
    hidExit();
    smExit();
}

int main(int argc, char* argv[]) {
    if (R_FAILED(timeInitialize())) {
        return MAKERESULT(Module_Libnx, LibnxError_InitFail_Time);
    }
    __libnx_init_time();
    debugInit();
    LOG("Main start");

    try {
        gp_overlay = new Overlay();
        gp_overlay->run();
    } catch (std::runtime_error& e) {
        LOG("runtime_error: %s", e.what());
    }

    delete gp_overlay;

#ifdef TESLA
    envSetNextLoad("sdmc:/switch/.overlays/ovlmenu.ovl", "--initial-load");
#endif

    LOG("Main exit");
    debugExit();
}
