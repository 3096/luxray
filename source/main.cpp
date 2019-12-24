#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <string>

#include <switch.h>

#include "debug.hpp"
#include "overlay.hpp"
#include "util.hpp"

#include "lvgl/lvgl.h"

extern "C" {
u32 __nx_applet_type = AppletType_None;

#define INNER_HEAP_SIZE 0x1000000  // temp for testing
char nx_inner_heap[INNER_HEAP_SIZE];

u32 __nx_nv_transfermem_size = 0x15000;

void __libnx_initheap(void);
void __appInit(void);
void __appExit(void);
}

Overlay* gp_overlay;

void __libnx_initheap(void) {
    extern char* fake_heap_start;
    extern char* fake_heap_end;
    fake_heap_start = nx_inner_heap;
    fake_heap_end = nx_inner_heap + sizeof(nx_inner_heap);
}

void __appInit(void) {
    // Init/exit services
    TRY(smInitialize(), fatalThrow(MAKERESULT(Module_Libnx, LibnxError_InitFail_SM)));
    TRY(hidInitialize(), fatalThrow(MAKERESULT(Module_Libnx, LibnxError_InitFail_HID)));
    TRY(fsInitialize(), fatalThrow(MAKERESULT(Module_Libnx, LibnxError_InitFail_FS)));
    TRY(timeInitialize(), fatalThrow(MAKERESULT(Module_Libnx, LibnxError_InitFail_Time)));
    // __libnx_init_time();

    fsdevMountSdmc();
    debugInit();

    LOG("Service init");

    try {
        gp_overlay = new Overlay();
    } catch (std::runtime_error& e) {
        LOG("runtime_error: %s", e.what());
        fatalThrow(MAKERESULT(405, 0));
    }
}

void __appExit(void) {
    // Cleanup services.
    LOG("Service cleanup");
    delete gp_overlay;
    debugExit();
    fsdevUnmountAll();
    fsExit();
    timeExit();
    hidExit();
    smExit();
}

int main(int argc, char* argv[]) {
    LOG("Main start");

    gp_overlay->run();
    
    LOG("Main exit");
    __appExit();
    return 0;
}
