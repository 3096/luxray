#pragma once

#include "../lvgl/lvgl.h"

class Screen {
   private:
    Screen* mp_prevScreen;

    bool m_ScreenIsJustToggled;
    bool m_screenIsOn;

   protected:
    lv_obj_t* mp_screenObj;
    lv_group_t* mp_inputGroup;

    uint64_t m_keysDown;
    uint64_t m_keysHeld;

    virtual bool procFrame_();
    virtual void mount_();
    virtual void unmount_();

   public:
    Screen(Screen* prevScreen);
    virtual ~Screen();

    void show();
};
