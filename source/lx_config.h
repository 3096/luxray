#pragma once

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
#define DEFAULT_LV_KEY_MAP {            \
    {KEY_DUP, LV_KEY_UP},               \
    {KEY_DDOWN, LV_KEY_DOWN},           \
    {KEY_DRIGHT, LV_KEY_RIGHT},         \
    {KEY_DLEFT, LV_KEY_LEFT},           \
    {KEY_L, LV_KEY_ESC},                \
    {KEY_R, LV_KEY_ENTER},              \
}

#define DEFAULT_ACTION_KEY_MAP          { .goBack = KEY_L, .toggleOverlay = KEY_DUP | KEY_RSTICK_UP }

// debug options
// #define DEBUG_LOG_FILE
#define DEBUG_NX_LINK
