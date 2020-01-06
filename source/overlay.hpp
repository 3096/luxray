#pragma once

#include <memory>

#include <switch.h>

#include "config.h"
#include "screens/main_screen.hpp"

extern lv_indev_t* gp_keyIn;
extern lv_indev_t* gp_touchIn;

class Overlay {
   public:
    static constexpr const uint16_t SCREEN_WIDTH = 1920;
    static constexpr const uint16_t SCREEN_HEIGHT = 1080;
    static constexpr const uint16_t SCREEN_WIDTH_HANDHELD = 1280;
    static constexpr const uint16_t SCREEN_HEIGHT_HANDHELD = 720;

    static constexpr const size_t OVERLAY_BUF_LENGTH = OVERLAY_WIDTH * OVERLAY_HEIGHT;

    ViDisplay m_viDisplay;
    ViLayer m_viLayer;
    NWindow m_nWindow;
    Framebuffer m_frameBufferInfo;

    lv_disp_drv_t m_dispDrv;
    lv_disp_buf_t m_dispBufferInfo;
    lv_color_t mp_renderBuf[OVERLAY_BUF_LENGTH];
    void* mp_frameBuffers[2];
    bool m_doRender;

    lv_indev_drv_t m_touchDrv;
    lv_indev_drv_t m_keyDrv;

    lv_theme_t* mp_theme;

    std::unique_ptr<MainScreen> mp_mainScreen;

    inline Framebuffer* getFbInfo_();
    inline void copyPrivFb_();

    static void flushBuffer_(lv_disp_drv_t* p_disp, const lv_area_t* p_area, lv_color_t* p_lvBuffer);
    static bool touchRead_(lv_indev_drv_t* indev_driver, lv_indev_data_t* data);
    static bool keysRead_(lv_indev_drv_t* indev_driver, lv_indev_data_t* data);

   public:
    Overlay();
    ~Overlay();

    void run();
    inline void pauseRendering() { m_doRender = false; }
    inline void resumeRendering() { m_doRender = true; }
    inline void toggleRendering() { m_doRender = !m_doRender; }
    inline bool isRendering() { return m_doRender; }
    void flushEmptyFb();
};
