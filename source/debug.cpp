#include "debug.hpp"

#ifdef DEBUG
#    include <cstdio>
#    include <string>

#    include "lvgl/lvgl.h"

// namespace ams {
// ncm::ProgramId CurrentProgramId = {0x0100000000000195};
// namespace result {
// bool CallFatalOnResultAssertion = true;
// }
// }  // namespace ams

FILE* g_debug_file;

void lvglLog(lv_log_level_t level, const char* path, uint32_t line, const char* description) {
    fprintf(g_debug_file, "lvgl: level %d - %s:%d: %s\n", level, path, line, description);
    fflush(g_debug_file);
}
#endif

extern "C" {
void __libnx_exception_handler(ThreadExceptionDump* ctx) {
#ifdef DEBUG
    MemoryInfo mem_info;
    u32 page_info;
    svcQueryMemory(&mem_info, &page_info, ctx->pc.x);
    LOG("%#x exception with pc=%#lx", ctx->error_desc, ctx->pc.x - mem_info.addr);
    debugExit();
    // ams::CrashHandler(ctx);
#endif
}
}

void debugInit() {
#ifdef DEBUG
    time_t currentTime;
    TRY_FATAL(timeGetCurrentTime(TimeType_Default, (u64*)&currentTime));
    std::string logName = "sdmc:/luxray" + std::to_string(currentTime) + ".log";  // TODO: better log location
    g_debug_file = fopen(logName.c_str(), "w");
    if (g_debug_file == NULL) fatalThrow(0x00f);
    lv_log_register_print_cb(lvglLog);
#endif
}

void debugExit() {
#ifdef DEBUG
    fclose(g_debug_file);
#endif
}
