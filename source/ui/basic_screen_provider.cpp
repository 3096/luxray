#include "basic_screen_provider.hpp"

#include <stdexcept>

namespace ui {

BasicScreenProvider::BasicScreenProvider()
    : mp_screenObj(lv_obj_create(nullptr, nullptr)), mp_inputGroup(lv_group_create()) {
    // TODO: fix theme
    // lv_group_set_style_mod_cb(mp_inputGroup, theme::focusStyleMod);
}

BasicScreenProvider::~BasicScreenProvider() {
    lv_obj_del(mp_screenObj);
    lv_group_del(mp_inputGroup);
}

void BasicScreenProvider::renderScreen() {
    for (auto& screenUpdater : m_lvObjUpdaterList) {
        screenUpdater.updateCb(screenUpdater.p_lvObj);
    }
}

// void BasicScreenProvider::UPDATE_FIT_PARENT(lv_obj_t* p_lvObj) {
//     auto* p_parent = lv_obj_get_parent(p_lvObj);
//     if (p_parent) {
//         lv_obj_set_size(p_lvObj, lv_obj_get_width_fit(p_parent), lv_obj_get_height_fit(p_parent));

//     } else {
//         throw std::runtime_error("lv obj must have a parent");
//     }
// }

}  // namespace ui
