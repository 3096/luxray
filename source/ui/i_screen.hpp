#pragma once

#include "../lvgl/lvgl.h"

class IScreen {
   public:
    virtual void mount() {}
    virtual void unmount() {}
    virtual void procFrame() = 0;

    virtual lv_obj_t* getLvScreenObj() = 0;
    virtual lv_group_t* getLvInputGroup() = 0;
};
