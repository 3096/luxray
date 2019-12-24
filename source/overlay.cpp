#include "overlay.hpp"

extern "C" u64 __nx_vi_layer_id;

extern Overlay* gp_overlay;

Overlay::Overlay() {
    LOG("Initialize Overlay");

    TRY_GOTO(viInitialize(ViServiceType_Manager), end);
    TRY_GOTO(viOpenDefaultDisplay(&m_viDisplay), close_serv);
    TRY_GOTO(viCreateManagedLayer(&m_viDisplay, (ViLayerFlags)0, 0, &__nx_vi_layer_id),
             close_display);  // flag 0 allows non-fullscreen layer
    TRY_GOTO(viCreateLayer(&m_viDisplay, &m_viLayer), close_managed_layer);
    TRY_GOTO(viSetLayerScalingMode(&m_viLayer, ViScalingMode_FitToLayer), close_managed_layer);
    TRY_GOTO(viSetLayerZ(&m_viLayer, 100), close_managed_layer);  // Arbitrary z index
    TRY_GOTO(viSetLayerSize(&m_viLayer, OVERLAY_WIDTH, OVERLAY_HEIGHT), close_managed_layer);
    TRY_GOTO(viSetLayerPosition(&m_viLayer, OVERLAY_POS_X, OVERLAY_POS_Y), close_managed_layer);
    TRY_GOTO(nwindowCreateFromLayer(&m_nWindow, &m_viLayer), close_managed_layer);
    TRY_GOTO(
        framebufferCreate(&m_frameBufferInfo, &m_nWindow, OVERLAY_WIDTH, OVERLAY_HEIGHT, PIXEL_FORMAT_BGRA_8888, 2),
        close_window);
#if USE_LINEAR_BUF
    TRY_GOTO(framebufferMakeLinear(&m_frameBufferInfo), close_fb);
#endif
    mp_frameBuffers[0] = m_frameBufferInfo.buf;
    mp_frameBuffers[1] = (lv_color_t*)m_frameBufferInfo.buf + OVERLAY_BUF_LENGTH;

    LOG("libnx initialized");

    lv_init();
    lv_disp_drv_init(&m_dispDrv);
    lv_disp_buf_init(&m_dispBufferInfo, mp_renderBuf, nullptr, OVERLAY_BUF_LENGTH);
    m_dispDrv.buffer = &m_dispBufferInfo;
    m_dispDrv.flush_cb = flushBuffer;
    lv_disp_drv_register(&m_dispDrv);

    lv_indev_drv_init(&m_keyDrv);
    m_keyDrv.type = LV_INDEV_TYPE_KEYPAD;
    m_keyDrv.read_cb = keysRead;
    mp_keyIn = lv_indev_drv_register(&m_keyDrv);

    lv_indev_drv_init(&m_touchDrv);
    m_touchDrv.type = LV_INDEV_TYPE_POINTER;
    m_touchDrv.read_cb = touchRead;
    lv_indev_drv_register(&m_touchDrv);

    LOG("lv initialized");

    return;

#if USE_LINEAR_BUF
close_fb:
    framebufferClose(&m_frameBufferInfo);
#endif
close_window:
    nwindowClose(&m_nWindow);
close_managed_layer:
    viDestroyManagedLayer(&m_viLayer);
close_display:
    viCloseDisplay(&m_viDisplay);
close_serv:
    viExit();
end:
    throw std::runtime_error("Overlay init failed");
}

Overlay::~Overlay() {
    LOG("Exit Overlay");

    framebufferClose(&m_frameBufferInfo);
    nwindowClose(&m_nWindow);
    viDestroyManagedLayer(&m_viLayer);
    viCloseDisplay(&m_viDisplay);
    viExit();
}

Framebuffer* Overlay::getFbInfo_() { return &m_frameBufferInfo; }

void Overlay::copyPrivFb() {
    std::memcpy(mp_frameBuffers[m_nWindow.cur_slot], mp_frameBuffers[m_nWindow.cur_slot ^ 1],
                m_frameBufferInfo.fb_size);
}

void Overlay::flushBuffer(lv_disp_drv_t* p_disp, const lv_area_t* p_area, lv_color_t* p_lvColor) {
    Framebuffer* p_fbInfo = gp_overlay->getFbInfo_();
    lv_color_t* p_frameBuf = (lv_color_t*)framebufferBegin(p_fbInfo, nullptr);

#if USE_LINEAR_BUF
    int renderWidth = p_area->x2 - p_area->x1 + 1;
    for (int y = p_area->y1; y <= p_area->y2; y++) {
        std::memcpy(&p_frameBuf[y * OVERLAY_WIDTH + p_area->x1], p_lvColor, renderWidth * sizeof(lv_color_t));
        p_lvColor += renderWidth;
    }
#else
    gp_overlay->copyPrivFb();

    // https://github.com/switchbrew/libnx/blob/v1.6.0/nx/include/switch/display/gfx.h#L106-L119
    for (int y = p_area->y1; y <= p_area->y2; y++) {
        for (int x = p_area->x1; x <= p_area->x2; x++) {
            u32 swizzledPos = ((y & 127) / 16) + (x / 16 * 8) + ((y / 16 / 8) * (OVERLAY_WIDTH / 16 * 8));
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

bool Overlay::touchRead(lv_indev_drv_t* indev_driver, lv_indev_data_t* data) {
    /*Save the state and save the pressed coordinate*/
    data->state = hidTouchCount() > 0 ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
    if (data->state == LV_INDEV_STATE_PR) {
        touchPosition touch;
        hidTouchRead(&touch, 0);
        data->point.x = touch.px * SCREEN_WIDTH / SCREEN_WIDTH_HANDHELD - OVERLAY_POS_X;
        data->point.y = touch.py * SCREEN_HEIGHT / SCREEN_HEIGHT_HANDHELD - OVERLAY_POS_Y;
    }

    return false; /*Return `false` because we are not buffering and no more data to read*/
}

bool Overlay::keysRead(lv_indev_drv_t* indev_driver, lv_indev_data_t* data) {
    u64 keysDown = hidKeysHeld(CONTROLLER_P1_AUTO);
    data->state = keysDown ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
    if (data->state == LV_INDEV_STATE_PR) {
        if (keysDown & KEY_UP)
            data->key = LV_KEY_UP;
        else if (keysDown & KEY_DOWN)
            data->key = LV_KEY_DOWN;
        else if (keysDown & KEY_RIGHT)
            data->key = LV_KEY_RIGHT;
        else if (keysDown & KEY_LEFT)
            data->key = LV_KEY_LEFT;
        else if (keysDown & KEY_A)
            data->key = LV_KEY_ENTER;
        else if (keysDown & KEY_B)
            data->key = LV_KEY_ESC;
        else if (keysDown & KEY_L)
            data->key = LV_KEY_PREV;
        else if (keysDown & KEY_R)
            data->key = LV_KEY_NEXT;
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
