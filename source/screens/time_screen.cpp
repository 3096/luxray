#include "time_screen.hpp"

static void event_handler(lv_obj_t* obj, lv_event_t event) { LOG("%d", event); }

TimeScreen::TimeScreen(lv_obj_t* prevScreen) : Screen(prevScreen) {
    lv_obj_t* window = lv_win_create(mp_thisScreen, nullptr);
    lv_win_set_title(window, "  Date Advance");

    mp_promptLabel = lv_label_create(window, nullptr);
    lv_obj_align(mp_promptLabel, nullptr, LV_ALIGN_IN_TOP_LEFT, 18, 18);

    mp_valueLabel = lv_label_create(window, nullptr);
    lv_obj_align(mp_valueLabel, nullptr, LV_ALIGN_IN_TOP_RIGHT, -18, 18);

    mp_buttonMap = lv_btnm_create(window, NULL);
    lv_obj_align(mp_buttonMap, nullptr, LV_ALIGN_IN_BOTTOM_MID, 0, -18);
    lv_btnm_set_map(mp_buttonMap, (const char**)BUTTON_MAP_LAYOUT);
    lv_obj_set_event_cb(mp_buttonMap, event_handler);
}

TimeScreen::~TimeScreen() {}

bool TimeScreen::procFrame() {
    // TODO
    return Screen::procFrame();
}
