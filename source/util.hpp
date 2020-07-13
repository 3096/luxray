#pragma once

#include <cstdio>
#include <iomanip>
#include <sstream>
#include <stdexcept>

#define _STRINGIFY(x) #x
#define STRINGIFY(x) _STRINGIFY(x)
#ifndef CONCATENATE
#    define _CONCATENATE(x1, x2) x1##x2
#    define CONCATENATE(x1, x2) _CONCATENATE(x1, x2)
#endif

#define TRY(x, cb)                           \
    ({                                       \
        if (Result rc = (x); R_FAILED(rc)) { \
            ({ cb; });                       \
        }                                    \
    })
#define TRY_GOTO(x, l) TRY(x, goto l)  // TODO: find a better way and don't use this
#define TRY_FATAL(x) TRY(x, fatalThrow(rc))
#define TRY_THROW(x)                                                                             \
    ({                                                                                           \
        if (Result rc = (x); R_FAILED(rc)) {                                                     \
            std::stringstream errMsgSs;                                                          \
            errMsgSs << __PRETTY_FUNCTION__ << " " STRINGIFY(x) " failed: 0x" << std::hex << rc; \
            throw std::runtime_error(errMsgSs.str());                                            \
        }                                                                                        \
    })
