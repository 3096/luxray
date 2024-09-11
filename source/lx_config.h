#pragma once
// clang-format off

#define INNER_HEAP_SIZE                 0x300000

// docked params
#define OVERLAY_BASE_WIDTH_DOCKED       320
#define OVERLAY_BASE_HEIGHT_DOCKED      384
#define OVERLAY_UPSCALE_DOCKED          1
#define OVERLAY_POS_X_DOCKED            0
#define OVERLAY_POS_Y_DOCKED            OVERLAY_BASE_HEIGHT_DOCKED

#define OVERLAY_FONT_DOCKED             { .normal = &lv_font_montserrat_20, .small = &lv_font_montserrat_14 }

// handheld params
#define OVERLAY_BASE_WIDTH_HANDHELD     336
#define OVERLAY_BASE_HEIGHT_HANDHELD    384
#define OVERLAY_UPSCALE_HANDHELD        2
#define OVERLAY_POS_X_HANDHELD          0
#define OVERLAY_POS_Y_HANDHELD          0

#define OVERLAY_FONT_HANDHELD           { .normal = &lv_font_montserrat_24, .small = &lv_font_montserrat_20 }

// default kep map options
#include <switch/services/hid.h>
#define DEFAULT_LV_KEY_MAP {                        \
    {HidNpadButton_Up, LV_KEY_UP},                  \
    {HidNpadButton_Down, LV_KEY_DOWN},              \
    {HidNpadButton_Right, LV_KEY_RIGHT},            \
    {HidNpadButton_Left, LV_KEY_LEFT},              \
    {HidNpadButton_L, LV_KEY_ESC},                  \
    {HidNpadButton_R, LV_KEY_ENTER},                \
}

#define DEFAULT_ACTION_KEY_MAP {                                            \
    .goBack = HidNpadButton_L,                                              \
    .toggleOverlay = HidNpadButton_Up | HidNpadButton_StickRUp              \
}

// debug options
// #define DEBUG_LOG_FILE
#define DEBUG_NX_LINK
