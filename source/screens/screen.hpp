#pragma once

#include <switch.h>

#include "../debug.hpp"
#include "../lvgl/lvgl.h"

class Screen {
   private:
    lv_obj_t* mp_prevScreen;

    bool m_ScreenIsJustToggled;

   protected:
    lv_obj_t* mp_thisScreen;
    bool m_screenIsOn;

    u64 m_keysDown;
    u64 m_keysHeld;

    virtual bool procFrame_();

   public:
    Screen(lv_obj_t* prevScreen);
    virtual ~Screen();

    void show();
};
