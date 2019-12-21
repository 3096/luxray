#pragma once

#include <switch.h>
#include <stdexcept>

#include "color.hpp"
#include "debug.hpp"

class Overlay {
   private:
    // Handheld and docked
    const int SCREEN_WIDTH = 1920;
    const int SCREEN_HEIGHT = 1080;

    // width * height page-aligned
    const int FB_BYTES_PER_PIXEL = sizeof(rgba4444_t);
    const int FB_WIDTH = 256;   // Aligned (* bpp) to 64
    const int FB_HEIGHT = 128;  // Aligned to 128

    const int LAYER_X = 0.0f;
    const int LAYER_Y = 0.0f;
    const int LAYER_WIDTH = FB_WIDTH;  // Can be different from fb values (will be up/downscaled)
    const int LAYER_HEIGHT = FB_HEIGHT;

    ViDisplay m_viDisplay;
    ViLayer m_viLayer;
    NWindow m_nWindow;
    Framebuffer m_frameBuffer;

   public:
    Overlay();
    ~Overlay();

    void* getCurFramebuf();

    inline void testFrameBuf() {
        rgba4444_t testColor = make_color<rgba4444_t>(0, -1, -1, 0b1000);
        framebufferBegin(&m_frameBuffer, NULL);
        rgba4444_t* curFramebuf = (rgba4444_t*)getCurFramebuf();
        std::fill((rgba4444_t*)curFramebuf, (rgba4444_t*)curFramebuf + m_frameBuffer.fb_size, testColor);
        framebufferEnd(&m_frameBuffer);
    }
};
