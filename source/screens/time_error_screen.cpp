#include "time_error_screen.hpp"

TimeErrorScreen::TimeErrorScreen(Screen* prevScreen) : Screen(prevScreen) {
    lv_obj_t* errorMsg = lv_label_create(mp_screenObj, NULL);
    lv_label_set_text(errorMsg,
                      "To use the Date Advance\n"
                      "feature, turn on\n"
                      "\"Synchronize Clock via\n"
                      "Internet\" in System Settings.");
    lv_obj_align(errorMsg, NULL, LV_ALIGN_CENTER, 0, 0);
}

TimeErrorScreen::~TimeErrorScreen() {}
