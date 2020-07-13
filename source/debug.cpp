#include "debug.hpp"

#include <switch.h>

#ifdef NDEBUG
// debugging is disabled, stub em
void __libnx_exception_handler(ThreadExceptionDump* ctx) {}
void debugInit() {}
void debugExit() {}

#else
#    include <cstdio>
#    include <string>

#    include "lvgl/lvgl.h"

void lvglLog(lv_log_level_t level, const char* filePath, uint32_t lineNum, const char* funcName,
             const char* description) {
    LOG("level %d - %s:%d: %s %s\n", level, filePath, lineNum, funcName, description);
    if (level >= 3) {
        *(int*)nullptr = 0xB;  // force a crash
    }
}

#    ifdef DEBUG_LOG_FILE
FILE* g_debug_file;
extern "C" {
void __libnx_init_time(void);
}
#    endif

void debugInit() {
    TRY_FATAL(smInitialize());

#    ifdef DEBUG_LOG_FILE
    TRY_FATAL(timeInitialize());
    __libnx_init_time();
    time_t currentTime;
    TRY_FATAL(timeGetCurrentTime(TimeType_Default, (u64*)&currentTime));
    timeExit();

    std::string logName = "sdmc:/luxray" + std::to_string(currentTime) + ".log";  // TODO: better log location
    g_debug_file = fopen(logName.c_str(), "w");
    if (g_debug_file == NULL) fatalThrow(0xf);
#    endif

#    ifdef DEBUG_NX_LINK
    nxlinkStdio();
#    endif

    smExit();

    lv_log_register_print_cb(lvglLog);
}

void debugExit() {
#    ifdef DEBUG_LOG_FILE
    fclose(g_debug_file);
#    endif
}

void debugLogFormat(const char* prettyFunction, const char* fmt, ...) {
    va_list args;

#    ifdef DEBUG_LOG_FILE
    fprintf(g_debug_file, "%s: ", prettyFunction);
    fprintf(g_debug_file, fmt, args);
    fprintf(g_debug_file, "\n");
    fflush(g_debug_file);
#    endif

#    ifdef DEBUG_NX_LINK
    printf("%s: ", prettyFunction);
    printf(fmt, args);
    printf("\n");
#    endif
}

void __libnx_exception_handler(ThreadExceptionDump* ctx) {
    MemoryInfo mem_info;
    u32 page_info;
    svcQueryMemory(&mem_info, &page_info, ctx->pc.x);
    LOG("%#x exception with pc=%#lx", ctx->error_desc, ctx->pc.x - mem_info.addr);
    debugExit();
}

#endif
