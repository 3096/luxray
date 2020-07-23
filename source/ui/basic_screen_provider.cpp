#include "basic_screen_provider.hpp"

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

}  // namespace ui
