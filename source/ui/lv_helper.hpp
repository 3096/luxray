#pragma once

#include "../debug.hpp"
#include "../lvgl/lvgl.h"
#include "../overlay.hpp"
#include "controller.hpp"

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

namespace lv_label {

inline auto create(lv_obj_t* p_parent) {
    auto result = lv_label_create(p_parent, nullptr);
    lv_obj_add_style(result, LV_OBJ_PART_MAIN, ui::Controller::getFontStyleNormal());
    return result;
}

}  // namespace lv_label

namespace lv_win {

inline auto HEADER_HEIGHT() { return Overlay::getScaledRenderCoord(50); }

inline auto create(lv_obj_t* p_parent) {
    auto result = lv_win_create(p_parent, nullptr);
    lv_obj_add_style(result, LV_WIN_PART_HEADER, ui::Controller::getFontStyleNormal());
    return result;
}

inline void updateHeader(lv_obj_t* p_lvWin) { lv_win_set_header_height(p_lvWin, HEADER_HEIGHT()); }

inline void updateFitParent(lv_obj_t* p_lvWin) {
    lv::updateFitParent(p_lvWin);
    updateHeader(p_lvWin);
}

}  // namespace lv_win

namespace lv_btnmatrix {

inline auto create(lv_obj_t* p_parent) {
    auto result = lv_btnmatrix_create(p_parent, nullptr);
    lv_obj_add_style(result, LV_BTNMATRIX_PART_BTN, ui::Controller::getFontStyleSmall());
    return result;
}

}  // namespace lv_btnmatrix

}  // namespace ui
