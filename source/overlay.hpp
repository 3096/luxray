#pragma once

#include <switch.h>
#include <cstring>
#include <memory>
#include <stdexcept>

#include "color.hpp"
#include "config.h"
#include "debug.hpp"
#include "lvgl/lvgl.h"

class Overlay {
   private:
    // Handheld and docked
    // static constexpr u_int32_t SCREEN_WIDTH = 1920;
    // static constexpr u_int32_t SCREEN_HEIGHT = 1080;

    static constexpr size_t OVERLAY_BUF_LENGTH = OVERLAY_WIDTH * OVERLAY_HEIGHT;

    static constexpr float OVERLAY_POS_X = 0;
    static constexpr float OVERLAY_POS_Y = OVERLAY_HEIGHT_BASE * 2 - OVERLAY_HEIGHT_BASE * OVERLAY_SCALE;

    ViDisplay m_viDisplay;
    ViLayer m_viLayer;
    NWindow m_nWindow;
    Framebuffer m_frameBuffer;

    lv_disp_drv_t m_dispDrv;
    lv_disp_buf_t m_dispBuf;
    lv_color_t m_renderBuf[OVERLAY_BUF_LENGTH];

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
