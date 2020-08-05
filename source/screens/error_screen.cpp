#include "error_screen.hpp"

#include "lx/debug.hpp"
#include "lx/ui/controller.hpp"
#include "lx/ui/lv_helper.hpp"
#include "main_screen.hpp"

ErrorScreen ErrorScreen::s_instance;

ErrorScreen::ErrorScreen() : m_basicScreen(*this) {
    LOGSL("constructing... ");

    mp_errorMsgLabel = lx::ui::lv_label::create(getLvScreenObj());
    lv_obj_align(mp_errorMsgLabel, nullptr, LV_ALIGN_CENTER, 0, 0);
    m_basicScreen.addLvObjPositionUpdater(
        mp_errorMsgLabel, [this](lv_obj_t* p_errorMsgLabel) { lv_label_set_text(p_errorMsgLabel, m_errorMsgStr); });
    m_basicScreen.addLvObjPositionUpdater(mp_errorMsgLabel, lv_obj_realign);

    LOGEL("done");
}

ErrorScreen::~ErrorScreen() {}
