#pragma once

#include "ui/lv_helper.hpp"

namespace theme {

void initialize();

auto createWindow(lv_obj_t* p_parent) -> lv_obj_t*;
auto createBtnmatrix(lv_obj_t* p_parent) -> lv_obj_t*;

}  // namespace theme
