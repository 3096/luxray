#pragma once

// #define HANDHELD

#define OVERLAY_DPI_BASE            100

#ifdef HANDHELD
#    define OVERLAY_SCALE           2

#    define OVERLAY_WIDTH_BASE      336
#    define OVERLAY_HEIGHT_BASE     384

#    define OVERLAY_WIDTH           OVERLAY_WIDTH_BASE * OVERLAY_SCALE * 2 / 3
#    define OVERLAY_HEIGHT          OVERLAY_HEIGHT_BASE * OVERLAY_SCALE * 2 / 3
#    define OVERLAY_DPI             OVERLAY_DPI_BASE * OVERLAY_SCALE * 2 / 3
#    define OVERLAY_POS_X           0
#    define OVERLAY_POS_Y           0

#    define OVERLAY_FONT            lv_font_roboto_28

#    define INNER_HEAP_SIZE         0x300000
#else
#    define OVERLAY_SCALE           1

// aligned (* bpp) to 64
#    define OVERLAY_WIDTH_BASE      320
// aligned to 128
#    define OVERLAY_HEIGHT_BASE     384

#    define OVERLAY_WIDTH           OVERLAY_WIDTH_BASE * OVERLAY_SCALE
#    define OVERLAY_HEIGHT          OVERLAY_HEIGHT_BASE * OVERLAY_SCALE
#    define OVERLAY_DPI             OVERLAY_DPI_BASE * OVERLAY_SCALE
#    define OVERLAY_POS_X           0
#    define OVERLAY_POS_Y           OVERLAY_HEIGHT_BASE

#    define OVERLAY_FONT            lv_font_roboto_22

#    define INNER_HEAP_SIZE         0x200000
#endif

#define USE_LINEAR_BUF              0
