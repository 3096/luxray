#pragma once

#include "config.h"
#include "util.hpp"

#ifdef NDEBUG
#    define LOG(...) ({})

#else
void debugLogFormat(const char* prettyFunction, const char* fmt, ...);
#    define LOG(fmt, ...) debugLogFormat(__PRETTY_FUNCTION__, fmt __VA_OPT__(, ) __VA_ARGS__);

#endif

#define LOG32(v) LOG(STRINGIFY(v) " = %d", v)
#define LOG64(v) LOG(STRINGIFY(v) " = %ld", v)
#define LOG32X(v) LOG(STRINGIFY(v) " = 0x%x", v)
#define LOG64X(v) LOG(STRINGIFY(v) " = 0x%lx", v)

void debugInit();
void debugExit();
