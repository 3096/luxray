#pragma once

#include "../debug.hpp"
#include "../lvgl/lvgl.h"
#include "../overlay.hpp"

namespace ui {

namespace size {

static inline auto MARGIN() { return Overlay::getScaledRenderCoord(18); }

}  // namespace size

namespace lv {

inline void updateFitParent(lv_obj_t* p_lvObj) {
    auto* p_parent = lv_obj_get_parent(p_lvObj);
    lv_obj_set_size(p_lvObj, lv_obj_get_width_fit(p_parent), lv_obj_get_height_fit(p_parent));
}

}  // namespace lv

namespace lv_win {

static inline auto HEADER_HEIGHT() { return Overlay::getScaledRenderCoord(50); }

inline void updateHeader(lv_obj_t* p_lvWin) { lv_win_set_header_height(p_lvWin, HEADER_HEIGHT()); }

inline void updateFitParent(lv_obj_t* p_lvWin) {
    updateHeader(p_lvWin);
    lv::updateFitParent(p_lvWin);
}

}  // namespace lv_win

}  // namespace ui
