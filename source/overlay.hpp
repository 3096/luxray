#pragma once

#include <switch.h>
#include <cstring>
#include <memory>
#include <stdexcept>

#include "config.h"
#include "debug.hpp"
#include "lvgl/lvgl.h"
#include "screens/main_screen.hpp"
#include "util.hpp"

extern lv_indev_t* gp_keyIn;
extern lv_indev_t* gp_touchIn;

class Overlay {
   public:
    static constexpr u_int32_t SCREEN_WIDTH = 1920;
    static constexpr u_int32_t SCREEN_HEIGHT = 1080;
    static constexpr u_int32_t SCREEN_WIDTH_HANDHELD = 1280;
    static constexpr u_int32_t SCREEN_HEIGHT_HANDHELD = 720;

    static constexpr size_t OVERLAY_BUF_LENGTH = OVERLAY_WIDTH * OVERLAY_HEIGHT;

    static constexpr float OVERLAY_POS_X = 0;
    static constexpr float OVERLAY_POS_Y = OVERLAY_HEIGHT_BASE * 2 - OVERLAY_HEIGHT_BASE * OVERLAY_SCALE;

    ViDisplay m_viDisplay;
    ViLayer m_viLayer;
    NWindow m_nWindow;
    Framebuffer m_frameBufferInfo;

    lv_disp_drv_t m_dispDrv;
    lv_disp_buf_t m_dispBufferInfo;
    lv_color_t mp_renderBuf[OVERLAY_BUF_LENGTH];
    void* mp_frameBuffers[2];

    lv_indev_drv_t m_touchDrv;
    lv_indev_drv_t m_keyDrv;

    lv_theme_t* mp_theme;

    std::unique_ptr<MainScreen> mp_mainScreen;

    inline Framebuffer* getFbInfo_();

   public:
    Overlay();
    ~Overlay();

    void run();

    inline void copyPrivFb();

    static void flushBuffer(lv_disp_drv_t* p_disp, const lv_area_t* p_area, lv_color_t* p_lvBuffer);
    static bool touchRead(lv_indev_drv_t* indev_driver, lv_indev_data_t* data);
    static bool keysRead(lv_indev_drv_t* indev_driver, lv_indev_data_t* data);
};
