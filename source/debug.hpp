#pragma once

#include "config.h"
#include "util.hpp"

#ifdef NDEBUG
#    define LOGL(...) ({})
#    define LOGLNPF(...) ({})

#else
void* debugLogFormat(const char* prettyFunction, const char* fmt, const char* lineEnd, ...);
#    define LOGL(fmt, ln, ...) debugLogFormat(__PRETTY_FUNCTION__, fmt, ln __VA_OPT__(, ) __VA_ARGS__)
#    define LOGLNPF(fmt, ln, ...) debugLogFormat(nullptr, fmt, ln __VA_OPT__(, ) __VA_ARGS__)

#endif

#define LOG(fmt, ...) LOGL(fmt, "\n" __VA_OPT__(, ) __VA_ARGS__)       // LOG FULL LINE
#define LOGSL(fmt, ...) LOGL(fmt, "" __VA_OPT__(, ) __VA_ARGS__)       // LOG START LINE
#define LOGML(fmt, ...) LOGLNPF(fmt, "" __VA_OPT__(, ) __VA_ARGS__)    // LOG MID LINE
#define LOGEL(fmt, ...) LOGLNPF(fmt, "\n" __VA_OPT__(, ) __VA_ARGS__)  // LOG END LINE

#define LOG32(v) LOG(STRINGIFY(v) " = %d", v)
#define LOG64(v) LOG(STRINGIFY(v) " = %ld", v)
#define LOG32X(v) LOG(STRINGIFY(v) " = 0x%x", v)
#define LOG64X(v) LOG(STRINGIFY(v) " = 0x%lx", v)

#ifdef NDEBUG
#    define LOGCONSTRUCTM
#    define LOGCONSTRUCT
#    define LOGCONSTRUCTSL

#else
#    define LOGCONSTRUCTM void* __logInitMember
#    define LOGCONSTRUCT __logInitMember(LOG("constructing...")),
#    define LOGCONSTRUCTSL __logInitMember(LOGSL("constructing... ")),
#endif

void debugInit();
void debugExit();
