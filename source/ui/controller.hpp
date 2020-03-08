#pragma once

#include "i_screen.hpp"

namespace ui {

class Controller {
   private:
    Controller();
    Controller(const Controller&) = delete;
    ~Controller();
    static Controller s_instance;

    // state members
    IScreen* mp_curScreen;
    IScreen* mp_nextScreen;

    bool m_ScreenIsJustToggled;
    bool m_screenIsOn;
    bool m_shouldExit;
    bool m_isDocked;

    uint64_t m_keysDown;
    uint64_t m_keysHeld;

    // helpers
    void mountScreen_(IScreen* screenToMount);
    void threadMain_();

   public:
    static void show(IScreen& screenToShow);
    inline static void stop() { s_instance.m_shouldExit = true; }

    inline static uint64_t getKeysDown() { return s_instance.m_keysDown; }
    inline static uint64_t getKeysHeld() { return s_instance.m_keysHeld; }
};

}  // namespace ui
