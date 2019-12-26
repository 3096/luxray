#pragma once

#include <switch.h>
#include "../lvgl/lvgl.h"

class Screen {
   private:
    lv_obj_t* mp_prevScreen;

   protected:
    lv_obj_t* mp_thisScreen;
    virtual bool procFrame();

   public:
    Screen(lv_obj_t* prevScreen);
    ~Screen();

    void show();
};
