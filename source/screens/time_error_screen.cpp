#include "../ui/controller.hpp"
#include "main_screen.hpp"

#include "time_error_screen.hpp"

TimeErrorScreen TimeErrorScreen::s_instance;

TimeErrorScreen::TimeErrorScreen() : IScreen() {
    lv_obj_t* p_errorMsgLabel = lv_label_create(getLvScreenObj(), NULL);
    lv_label_set_text(p_errorMsgLabel,
                      "To use the Date Advance\n"
                      "feature, turn on\n"
                      "\"Synchronize Clock via\n"
                      "Internet\" in System Settings.");
    lv_obj_align(p_errorMsgLabel, NULL, LV_ALIGN_CENTER, 0, 0);
}

TimeErrorScreen::~TimeErrorScreen() {}

void TimeErrorScreen::procFrame() {
    if (m_basicScreen.returnButtonPressed()) {
        ui::Controller::show(MainScreen::getInstance());
    }
}
