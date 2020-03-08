#pragma once

#include <switch.h>

#include "../lvgl/lvgl.h"
#include "../ui/controller.hpp"

class BasicScreen {
   private:
    lv_obj_t* mp_screenObj;
    lv_group_t* mp_inputGroup;

   public:
    BasicScreen();
    ~BasicScreen();

    inline lv_obj_t* getLvScreenObj() { return mp_screenObj; }
    inline lv_group_t* getLvInputGroup() { return mp_inputGroup; }

    inline bool returnButtonPressed() {
        return ui::Controller::getKeysDown() & KEY_L;  // TODO: make configurable
    }
};
