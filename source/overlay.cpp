#include "overlay.hpp"

#include <cstring>
#include <stdexcept>

#include "core/system.hpp"
#include "debug.hpp"
#include "theme.hpp"
#include "util.hpp"

extern "C" u64 __nx_vi_layer_id;

Overlay Overlay::s_instance;

lv_indev_t* gp_keyIn;
lv_indev_t* gp_touchIn;

Overlay::Overlay() {
    LOGSL("constructing... ");

    TRY_GOTO(viOpenDefaultDisplay(&m_viDisplay), end);
    TRY_GOTO(viGetDisplayVsyncEvent(&m_viDisplay, &m_viDisplayVsyncEvent), close_display);
    TRY_GOTO(viCreateManagedLayer(&m_viDisplay, (ViLayerFlags)0, 0, &__nx_vi_layer_id),
             close_display);  // flag 0 allows non-fullscreen layer
    TRY_GOTO(viCreateLayer(&m_viDisplay, &m_viLayer), close_managed_layer);
    TRY_GOTO(viSetLayerScalingMode(&m_viLayer, ViScalingMode_FitToLayer), close_managed_layer);
    TRY_GOTO(viSetLayerZ(&m_viLayer, 100), close_managed_layer);  // Arbitrary z index
    TRY_GOTO(nwindowCreateFromLayer(&m_nWindow, &m_viLayer), close_managed_layer);
    TRY_GOTO(framebufferCreate(&m_frameBufferInfo, &m_nWindow, LAYER_BUFFER_WIDTH, LAYER_BUFFER_HEIGHT,
                               PIXEL_FORMAT_BGRA_8888, 2),
             close_window);
#if USE_LINEAR_BUF
    TRY_GOTO(framebufferMakeLinear(&m_frameBufferInfo), close_fb);
#endif
    mp_frameBuffers[0] = m_frameBufferInfo.buf;
    mp_frameBuffers[1] = (lv_color_t*)m_frameBufferInfo.buf + LAYER_BUFFER_SIZE;

    LOGML("framebuffer initialized... ");

    lv_init();

    lv_disp_drv_init(&m_dispDrv);
    lv_disp_buf_init(&m_dispBufferInfo, mp_renderBuf, nullptr, LAYER_BUFFER_SIZE);
    m_dispDrv.buffer = &m_dispBufferInfo;
    m_dispDrv.flush_cb = flushBuffer_;
    m_isDocked = os::apmConsoleIsDocked();
    try {
        setLayerSizeAndPosition_();
    } catch (std::runtime_error& e) {
        LOGEL("%s", e.what());
        goto close_fb;
    }
    mp_disp = lv_disp_drv_register(&m_dispDrv);

    lv_indev_drv_init(&m_touchDrv);
    m_touchDrv.type = LV_INDEV_TYPE_POINTER;
    m_touchDrv.read_cb = touchRead_;
    gp_touchIn = lv_indev_drv_register(&m_touchDrv);

    lv_indev_drv_init(&m_keyDrv);
    m_keyDrv.type = LV_INDEV_TYPE_KEYPAD;
    m_keyDrv.read_cb = keysRead_;
    gp_keyIn = lv_indev_drv_register(&m_keyDrv);

    LOGEL("lv initialized... all done");

    m_doRender = true;
    return;

close_fb:
    framebufferClose(&m_frameBufferInfo);
close_window:
    nwindowClose(&m_nWindow);
close_managed_layer:
    viDestroyManagedLayer(&m_viLayer);
close_display:
    viCloseDisplay(&m_viDisplay);
end:
    throw std::runtime_error("Overlay init failed");
}

Overlay::~Overlay() {
    LOGSL("Exit Overlay...");

    eventClose(&m_viDisplayVsyncEvent);
    framebufferClose(&m_frameBufferInfo);
    nwindowClose(&m_nWindow);
    viDestroyManagedLayer(&m_viLayer);
    viCloseDisplay(&m_viDisplay);
    viExit();

    LOGEL("done");
}

void Overlay::copyPrivFb_() {
    std::memcpy(mp_frameBuffers[m_nWindow.cur_slot], mp_frameBuffers[m_nWindow.cur_slot ^ 1],
                m_frameBufferInfo.fb_size);
}

void Overlay::flushBuffer_(lv_disp_drv_t* p_disp, const lv_area_t* p_area, lv_color_t* p_lvColor) {
    if (not s_instance.m_doRender) {
        return;
    }
    Framebuffer* p_fbInfo = &s_instance.m_frameBufferInfo;
    lv_color_t* p_frameBuf = (lv_color_t*)framebufferBegin(p_fbInfo, nullptr);

#if USE_LINEAR_BUF
    int renderWidth = p_area->x2 - p_area->x1 + 1;
    for (int y = p_area->y1; y <= p_area->y2; y++) {
        std::memcpy(&p_frameBuf[y * OVERLAY_WIDTH + p_area->x1], p_lvColor, renderWidth * sizeof(lv_color_t));
        p_lvColor += renderWidth;
    }
#else
    s_instance.copyPrivFb_();

    // https://github.com/switchbrew/libnx/blob/v1.6.0/nx/include/switch/display/gfx.h#L106-L119
    for (int y = p_area->y1; y <= p_area->y2; y++) {
        for (int x = p_area->x1; x <= p_area->x2; x++) {
            u32 swizzledPos = ((y & 127) / 16) + (x / 16 * 8) + ((y / 16 / 8) * (LAYER_BUFFER_WIDTH / 16 * 8));
            swizzledPos *= 16 * 16 * 4;
            swizzledPos += ((y % 16) / 8) * 512 + ((x % 16) / 8) * 256 + ((y % 8) / 2) * 64 + ((x % 8) / 4) * 32 +
                           (y % 2) * 16 + (x % 4) * 4;
            swizzledPos /= 4;
            p_frameBuf[swizzledPos] = *p_lvColor;
            p_lvColor++;
        }
    }
#endif

    framebufferEnd(p_fbInfo);
    lv_disp_flush_ready(p_disp);
}

void Overlay::flushEmptyFb() {
    void* p_frameBuf = framebufferBegin(&s_instance.m_frameBufferInfo, nullptr);
    std::memset(p_frameBuf, 0, s_instance.m_frameBufferInfo.fb_size);
    framebufferEnd(&s_instance.m_frameBufferInfo);
}

bool Overlay::touchRead_(lv_indev_drv_t* indev_driver, lv_indev_data_t* data) {
    if (hidTouchCount() > 0) {
        data->state = LV_INDEV_STATE_PR;
        touchPosition touch;
        hidTouchRead(&touch, 0);
        auto curLayerInfo = s_instance.getCurLayerInfo_();
        if (Overlay::getIsDockedStatus()) {
            data->point.x = touch.px * DOCK_HANDHELD_PIXEL_RATIO - curLayerInfo.POS_X;
            data->point.y = touch.py * DOCK_HANDHELD_PIXEL_RATIO - curLayerInfo.POS_Y;
        } else {
            data->point.x = touch.px - curLayerInfo.POS_X * DOCK_HANDHELD_PIXEL_RATIO;
            data->point.y = touch.py - curLayerInfo.POS_Y * DOCK_HANDHELD_PIXEL_RATIO;
        }
    } else {
        data->state = LV_INDEV_STATE_REL;
    }
    return false; /*Return `false` because we are not buffering and no more data to read*/
}

bool Overlay::keysRead_(lv_indev_drv_t* indev_driver, lv_indev_data_t* data) {
    u64 keysDown = hidKeysHeld(CONTROLLER_P1_AUTO);
    if (keysDown) {
        data->state = LV_INDEV_STATE_PR;
        // TODO: refactor this to a button map
        if (keysDown & KEY_DUP)
            data->key = LV_KEY_UP;
        else if (keysDown & KEY_DDOWN)
            data->key = LV_KEY_DOWN;
        else if (keysDown & KEY_DRIGHT)
            data->key = LV_KEY_RIGHT;
        else if (keysDown & KEY_DLEFT)
            data->key = LV_KEY_LEFT;
        else if (keysDown & KEY_L)
            data->key = LV_KEY_ESC;
        else if (keysDown & KEY_R)
            data->key = LV_KEY_ENTER;
        else
            data->state = LV_INDEV_STATE_REL;
    } else {
        data->state = LV_INDEV_STATE_REL;
    }

    //     LV_KEY_UP = 17,       /*0x11*/
    //     LV_KEY_DOWN = 18,     /*0x12*/
    //     LV_KEY_RIGHT = 19,    /*0x13*/
    //     LV_KEY_LEFT = 20,     /*0x14*/
    //     LV_KEY_ESC = 27,      /*0x1B*/
    //     LV_KEY_DEL = 127,     /*0x7F*/
    //     LV_KEY_BACKSPACE = 8, /*0x08*/
    //     LV_KEY_ENTER = 10,    /*0x0A, '\n'*/
    //     LV_KEY_NEXT = 9,      /*0x09, '\t'*/
    //     LV_KEY_PREV = 11,     /*0x0B, '*/
    //     LV_KEY_HOME = 2,      /*0x02, STX*/
    //     LV_KEY_END = 3,       /*0x03, ETX*/

    return false;
}

void Overlay::waitForVSync() { TRY_THROW(eventWait(&s_instance.m_viDisplayVsyncEvent, UINT64_MAX)); }

bool Overlay::getIsDockedStatusChanged() {
    auto curIsDocked = os::apmConsoleIsDocked();
    if (curIsDocked != s_instance.m_isDocked) {
        s_instance.m_isDocked = curIsDocked;
        s_instance.setLayerSizeAndPosition_();
        lv_disp_drv_update(s_instance.mp_disp, &s_instance.m_dispDrv);
        return true;
    }
    return false;
}

void Overlay::setLayerSizeAndPosition_() {
    auto curLayerInfo = getCurLayerInfo_();
    TRY_THROW(nwindowSetCrop(&m_nWindow, 0, 0, curLayerInfo.WIDTH, curLayerInfo.HEIGHT));
    // TODO: check clipping here, for vi error 4 (2114-0004 / 0x872)
    // TODO: control position another way
    if (m_isDocked) {
        TRY_THROW(viSetLayerSize(&m_viLayer, curLayerInfo.WIDTH, curLayerInfo.HEIGHT));
        TRY_THROW(viSetLayerPosition(&m_viLayer, curLayerInfo.POS_X, curLayerInfo.POS_Y));
    } else {
        TRY_THROW(viSetLayerPosition(&m_viLayer, curLayerInfo.POS_X, curLayerInfo.POS_Y));
        TRY_THROW(viSetLayerSize(&m_viLayer, curLayerInfo.WIDTH * DOCK_HANDHELD_PIXEL_RATIO,
                                 curLayerInfo.HEIGHT * DOCK_HANDHELD_PIXEL_RATIO));
    }
    m_dispDrv.hor_res = curLayerInfo.WIDTH;
    m_dispDrv.ver_res = curLayerInfo.HEIGHT;
}
