#pragma once

#include <cstdio>
#include <iomanip>
#include <sstream>
#include <stdexcept>

#include "debug.hpp"

#ifdef DEBUG
extern FILE* g_debug_file;
#endif

#define _STRINGIFY(x) #x
#define STRINGIFY(x) _STRINGIFY(x)
#ifndef CONCATENATE
#    define _CONCATENATE(x1, x2) x1##x2
#    define CONCATENATE(x1, x2) _CONCATENATE(x1, x2)
#endif

#define TRY(x, cb)                                \
    ({                                            \
        if (Result rc = (x); R_FAILED(rc)) {      \
            LOG(STRINGIFY(x) " failed: %#x", rc); \
            ({ cb; });                            \
        }                                         \
    })
#define TRY_GOTO(x, l) TRY(x, goto l)
#define TRY_RETURN(x, v) TRY(x, return v)
#define TRY_FATAL(x) TRY(x, fatalThrow(rc))
#define TRY_THROW(x)                                                                             \
    ({                                                                                           \
        if (Result rc = (x); R_FAILED(rc)) {                                                     \
            std::stringstream errMsgSs;                                                          \
            errMsgSs << __PRETTY_FUNCTION__ << " " STRINGIFY(x) " failed: 0x" << std::hex << rc; \
            /* LOG("%s", errMsgSs.str().c_str()); */                                             \
            throw std::runtime_error(errMsgSs.str());                                            \
        }                                                                                        \
    })

#define ASSERT_SIZE(x, sz) static_assert(sizeof(x) == (sz), "Wrong size in " STRINGIFY(x))
#define ASSERT_STANDARD_LAYOUT(x) static_assert(std::is_standard_layout_v<x>, STRINGIFY(x) " is not standard layout")
