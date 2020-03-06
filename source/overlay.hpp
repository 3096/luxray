#pragma once

#include <memory>

#include <switch.h>

#include "config.h"
#include "layer_info.h"
#include "screens/main_screen.hpp"

extern lv_indev_t* gp_keyIn;
extern lv_indev_t* gp_touchIn;

class Overlay {
   private:
    Overlay();
    Overlay(const Overlay&) = delete;
    ~Overlay();
    static Overlay s_instance;

    struct LayerInfo {
        const uint16_t WIDTH;
        const uint16_t HEIGHT;
        const uint16_t POS_X;
        const uint16_t POS_Y;
    };

    static constexpr LayerInfo DOCKED_LAYER_INFO = {LAYER_INFO_DOCKED_WIDTH, LAYER_INFO_DOCKED_HEIGHT, OVERLAY_POS_X_DOCKED,
                                                    OVERLAY_POS_Y_DOCKED};
    static constexpr LayerInfo HANDHELD_LAYER_INFO = {LAYER_INFO_HANDHELD_WIDTH, LAYER_INFO_HANDHELD_HEIGHT, OVERLAY_POS_X_HANDHELD,
                                                      OVERLAY_POS_Y_HANDHELD};

    ViDisplay m_viDisplay;
    ViLayer m_viLayer;
    NWindow m_nWindow;
    Framebuffer m_frameBufferInfo;

    lv_disp_drv_t m_dispDrv;
    lv_disp_buf_t m_dispBufferInfo;
    lv_color_t mp_renderBuf[LAYER_BUF_LENGTH];
    void* mp_frameBuffers[2];
    bool m_doRender;

    lv_indev_drv_t m_touchDrv;
    lv_indev_drv_t m_keyDrv;

    lv_theme_t* mp_theme;

    inline Framebuffer* getFbInfo_();
    inline void copyPrivFb_();
    inline const LayerInfo* getCurLayerInfo_() {
        // temp
        return isDocked_() ? &DOCKED_LAYER_INFO : &HANDHELD_LAYER_INFO;
    }

    static void flushBuffer_(lv_disp_drv_t* p_disp, const lv_area_t* p_area, lv_color_t* p_lvBuffer);
    static bool touchRead_(lv_indev_drv_t* indev_driver, lv_indev_data_t* data);
    static bool keysRead_(lv_indev_drv_t* indev_driver, lv_indev_data_t* data);
    static bool isDocked_();  // TODO: move all render related function to screen, including this one

   public:
    static void run();

    static inline void pauseRendering() { s_instance.m_doRender = false; }
    static inline void resumeRendering() { s_instance.m_doRender = true; }
    // static inline void toggleRendering() { s_instance.m_doRender = !m_doRender; }  // unused
    // static inline bool isRendering() { s_instance.return m_doRender; }  // unused
    static void flushEmptyFb();
};
