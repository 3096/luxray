#include "screen.hpp"

Screen::Screen(lv_obj_t* prevScreen) : mp_prevScreen(prevScreen) { mp_thisScreen = lv_obj_create(nullptr, nullptr); }

Screen::~Screen() { lv_obj_del(mp_thisScreen); }

void Screen::show() {
    lv_scr_load(mp_thisScreen);
    while (true) {
        if (not procFrame_()) {
            break;
        }
    }
    if (mp_prevScreen) lv_scr_load(mp_prevScreen);
}

bool Screen::procFrame_() {
    hidScanInput();
    u64 keysDown = hidKeysDown(CONTROLLER_P1_AUTO);
    lv_task_handler();
    svcSleepThread(10'000'000);
    return !(keysDown & KEY_B);
}
