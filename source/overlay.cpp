#include "overlay.hpp"

extern "C" u64 __nx_vi_layer_id;

Overlay::Overlay(/* args */) {
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
    TRY_GOTO(framebufferCreate(&this->m_frameBuffer, &this->m_nWindow, FB_WIDTH, FB_HEIGHT, PIXEL_FORMAT_RGBA_4444, 1),
             close_window);

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

void* Overlay::getCurFramebuf() { return (rgba4444_t*)m_frameBuffer.buf + m_frameBuffer.fb_size * m_nWindow.cur_slot; }
