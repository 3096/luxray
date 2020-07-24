#include "basic_screen_provider.hpp"

namespace ui {

BasicScreenProvider::BasicScreenProvider()
    : mp_screenObj(lv_obj_create(nullptr, nullptr)), mp_inputGroup(lv_group_create()) {
    lv_obj_add_style(mp_screenObj, LV_OBJ_PART_MAIN, Controller::getScreenStyle());
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
