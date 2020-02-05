#pragma once

#include "lvgl/lvgl.h"

namespace theme {

lv_theme_t* getTheme();
void focusStyleMod(lv_group_t* group, lv_style_t* style);

}  // namespace Theme
