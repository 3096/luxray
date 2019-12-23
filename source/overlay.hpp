#pragma once

#include <switch.h>
#include <cstring>
#include <memory>
#include <stdexcept>

#include "color.hpp"
#include "debug.hpp"

#include "lvgl/lvgl.h"

class Overlay {
   private:
    // Handheld and docked
    static const int SCREEN_WIDTH = 1920;
    static const int SCREEN_HEIGHT = 1080;

    // width * height page-aligned
    static const int FB_BYTES_PER_PIXEL = sizeof(lv_color_t);
    static const int FB_WIDTH = 320;   // Aligned (* bpp) to 64
    static const int FB_HEIGHT = 384;  // Aligned to 128

    static constexpr float LAYER_X = 0;
    static constexpr float LAYER_Y = 384;
    static const int LAYER_WIDTH = FB_WIDTH;  // Can be different from fb values (will be up/downscaled)
    static const int LAYER_HEIGHT = FB_HEIGHT;

    ViDisplay m_viDisplay;
    ViLayer m_viLayer;
    NWindow m_nWindow;
    Framebuffer m_frameBuffer;

    lv_disp_drv_t m_dispDrv;
    lv_disp_buf_t m_dispBuf;
    lv_color_t m_renderBuf[LV_HOR_RES_MAX * LV_VER_RES_MAX];  // TODO: make this FB_WIDTH * FB_HEIGHT

    Framebuffer* getFb_();

   public:
    Overlay();
    ~Overlay();

    // temp
    inline void testFrameBuf() {
        lv_color_t testColor = {0xFF, 0xFF, 0, 0b00100000};
        lv_color_t* fb = (lv_color_t*)framebufferBegin(&m_frameBuffer, NULL);
        std::fill(fb, fb + m_frameBuffer.fb_size, testColor);
        framebufferEnd(&m_frameBuffer);
    }

    static void flushBuffer(lv_disp_drv_t* p_disp, const lv_area_t* p_area, lv_color_t* p_lvBuffer);
};
