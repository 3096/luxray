#include "main_screen.hpp"

MainScreen::MainScreen(lv_obj_t* prevScreen) : Screen(prevScreen) {
    lv_obj_t* label1 = lv_label_create(mp_thisScreen, NULL);
    lv_label_set_text(label1, "Hello world!");
    lv_obj_align(label1, NULL, LV_ALIGN_CENTER, 0, 0);
}

MainScreen::~MainScreen() {}
