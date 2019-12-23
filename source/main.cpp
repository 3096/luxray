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

#define INNER_HEAP_SIZE 0x300000 // temp for testing
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
    // Initialization
    LOG("Main start");

    // lv_examples hello world
    LOG("Hello world example");
    lv_obj_t* scr = lv_disp_get_scr_act(NULL); /*Get the current screen*/
    /*Create a Label on the currently active screen*/
    lv_obj_t* label1 = lv_label_create(scr, NULL);
    /*Modify the Label's text*/
    lv_label_set_text(label1, "Hello world!");
    /* Align the Label to the center
     * NULL means align on parent (which is the screen now)
     * 0, 0 at the end means an x, y offset after alignment*/
    lv_obj_align(label1, NULL, LV_ALIGN_CENTER, 0, 0);

    // main loop
    LOG("Enter loop");
    while (true) {
        lv_task_handler();
        svcSleepThread(10'000'000);
    }

    // Deinitialization and resources clean up
    LOG("Main exit");
    return 0;
}
