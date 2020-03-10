#include "debug.hpp"
#include "lvgl/lvgl.h"
#include "ui/controller.hpp"
#include "screens/main_screen.hpp"
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

void __libnx_initheap(void) {
    extern char* fake_heap_start;
    extern char* fake_heap_end;
    fake_heap_start = nx_inner_heap;
    fake_heap_end = nx_inner_heap + sizeof(nx_inner_heap);
}

bool g_appInitSuccessful;
void __appInit(void) {
    g_appInitSuccessful = true;

    // Init services
    Result rc;
    if (R_FAILED(smInitialize())) fatalThrow(MAKERESULT(Module_Libnx, LibnxError_InitFail_SM));
    if (R_FAILED(hidInitialize())) fatalThrow(MAKERESULT(Module_Libnx, LibnxError_InitFail_HID));
    if (R_FAILED(fsInitialize())) fatalThrow(MAKERESULT(Module_Libnx, LibnxError_InitFail_FS));
    if (R_FAILED(timeInitialize())) {
        g_appInitSuccessful = false;  // timeInitialize TimeServiceType_System has failed in the past, so just in case
        __nx_time_service_type = TimeServiceType_User;
        if (R_FAILED(rc = timeInitialize())) fatalThrow(MAKERESULT(Module_Libnx, LibnxError_InitFail_Time));
    }
    if (R_FAILED(rc = apmInitialize())) fatalThrow(rc);
    if (R_FAILED(rc = setsysInitialize())) fatalThrow(rc);
    if (R_FAILED(rc = nifmInitialize(NifmServiceType_User))) fatalThrow(rc);
    if (R_FAILED(rc = viInitialize(ViServiceType_Manager))) fatalThrow(rc);

    __libnx_init_time();
    fsdevMountSdmc();
    debugInit();

    LOG("service init");
}

void __appExit(void) {
    // Cleanup services.
    fsdevUnmountAll();
    viExit();
    nifmExit();
    setsysExit();
    apmExit();
    timeExit();
    fsExit();
    hidExit();
    smExit();

#ifdef TESLA
    envSetNextLoad("sdmc:/switch/.overlays/ovlmenu.ovl", "--skipCombo");
#endif
}

int main(int argc, char* argv[]) {
    if (not g_appInitSuccessful) {
        LOG("__appInit failed");
        return -1;
    }

    LOG("Main start");

    try {
        ui::Controller::show(MainScreen::getInstance());
        } catch (std::runtime_error& e) {
        LOG("runtime_error: %s", e.what());
    }

    LOG("Main exit");
    debugExit();
}
