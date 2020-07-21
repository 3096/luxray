#pragma once

#include <cstdlib>

#include "../debug.hpp"
#include "i_screen.hpp"

namespace ui {

class Controller {
    LOGCONSTRUCTM;

   private:
    Controller();
    Controller(const Controller&) = delete;
    ~Controller();
    static inline auto& getInstance() {
        static Controller s_instance;
        return s_instance;
    }

    // state members
    IScreen* mp_curScreen;
    IScreen* mp_nextScreen;

    bool m_screenIsJustToggled;
    bool m_screenIsOn;
    bool m_shouldRerender;
    bool m_shouldExit;

    uint64_t m_keysDown;
    uint64_t m_keysHeld;

    lv_style_t m_globalStyle;

    // helpers
    inline void mountScreen_(IScreen* screenToMount);
    void threadMain_();

   public:
    static void show(IScreen& screenToShow);
    inline static void stop() { getInstance().m_shouldExit = true; }

    inline static uint64_t getKeysDown() { return getInstance().m_keysDown; }
    inline static uint64_t getKeysHeld() { return getInstance().m_keysHeld; }

    inline static lv_style_t* globalStylePtr() { return &getInstance().m_globalStyle; }
};

}  // namespace ui
