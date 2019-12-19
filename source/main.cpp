#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <switch.h>

#include "debug.hpp"
#include "util.hpp"

extern "C" {
u32 __nx_applet_type = AppletType_None;

#define INNER_HEAP_SIZE 0x50000
char nx_inner_heap[INNER_HEAP_SIZE];

u32 __nx_nv_transfermem_size = 0x15000;

void __libnx_initheap(void);
void __libnx_exception_handler(ThreadExceptionDump* ctx);
void __appInit(void);
void __appExit(void);
}

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
}

void __appExit(void) {
    // Cleanup services.
    fsdevUnmountAll();
    fsExit();
    timeExit();
    hidExit();
    smExit();
}

int main(int argc, char* argv[]) {
    // Initialization
    debugInit();

    // main loop
    LOG("Test");

    // Deinitialization and resources clean up
    debugExit();
    return 0;
}
