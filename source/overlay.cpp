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
    TRY_GOTO(viSetLayerSize(&m_viLayer, LAYER_WIDTH, LAYER_HEIGHT), close_managed_layer);
    TRY_GOTO(viSetLayerPosition(&m_viLayer, LAYER_X, LAYER_Y), close_managed_layer);
    TRY_GOTO(nwindowCreateFromLayer(&this->m_nWindow, &this->m_viLayer), close_managed_layer);
    TRY_GOTO(framebufferCreate(&this->m_frameBuffer, &this->m_nWindow, FB_WIDTH, FB_HEIGHT, PIXEL_FORMAT_BGRA_8888, 1),
             close_window);

    lv_init();
    lv_disp_drv_init(&m_dispDrv);
    lv_disp_buf_init(&m_dispBuf, &m_renderBuf, NULL, LV_HOR_RES_MAX * LV_VER_RES_MAX);
    m_dispDrv.buffer = &m_dispBuf;
    m_dispDrv.flush_cb = Overlay::flushBuffer;
    lv_disp_drv_register(&m_dispDrv);

    return;

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
    LOG("Finalize Overlay");

    framebufferClose(&m_frameBuffer);
    nwindowClose(&m_nWindow);
    viDestroyManagedLayer(&m_viLayer);
    viCloseDisplay(&m_viDisplay);
    viExit();
}

Framebuffer* Overlay::getFb_() { return &m_frameBuffer; }

void Overlay::flushBuffer(lv_disp_drv_t* p_disp, const lv_area_t* p_area, lv_color_t* p_lvColor) {
    Framebuffer* p_fb = gp_overlay->getFb_();
    lv_color_t* p_frameBuf = (lv_color_t*)framebufferBegin(p_fb, nullptr);

    // https://github.com/switchbrew/libnx/blob/v1.6.0/nx/include/switch/display/gfx.h#L106-L119
    for (int y = p_area->y1; y <= p_area->y2; y++) {
        for (int x = p_area->x1; x <= p_area->x2; x++) {
            u32 swizzledPos;
            swizzledPos = ((y & 127) / 16) + (x / 16 * 8) + ((y / 16 / 8) * (FB_WIDTH / 16 * 8));
            swizzledPos *= 16 * 16 * 4;
            swizzledPos += ((y % 16) / 8) * 512 + ((x % 16) / 8) * 256 + ((y % 8) / 2) * 64 + ((x % 8) / 4) * 32 +
                           (y % 2) * 16 + (x % 4) * 4;
            swizzledPos /= 4;
            p_frameBuf[swizzledPos] = *p_lvColor;
            p_lvColor++;
        }
    }

    framebufferEnd(p_fb);
    lv_disp_flush_ready(p_disp);
}
