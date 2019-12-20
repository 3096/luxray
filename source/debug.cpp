#include "debug.hpp"

#ifdef DEBUG
FILE* g_debug_file;

extern "C" {
void __libnx_exception_handler(ThreadExceptionDump* ctx) {
    MemoryInfo mem_info;
    u32 page_info;
    svcQueryMemory(&mem_info, &page_info, ctx->pc.x);
    LOG("%#x exception with pc=%#lx\n", ctx->error_desc, ctx->pc.x - mem_info.addr);
}
}
#endif

void debugInit() {
#ifdef DEBUG
    time_t currentTime;
    TRY_FATAL(timeGetCurrentTime(TimeType_Default, (u64*)&currentTime));
    std::string logName = "sdmc:/luxray" + std::to_string(currentTime) + ".log";  // TODO: better log location
    g_debug_file = fopen(logName.c_str(), "w");
    if (g_debug_file == NULL) fatalThrow(0x00f);
#endif
}

void debugExit() {
#ifdef DEBUG
    fclose(g_debug_file);
#endif
}
