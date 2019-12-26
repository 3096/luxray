#include "main_screen.hpp"

MainScreen::MainScreen(lv_obj_t* prevScreen) : Screen(prevScreen) {
    mp_timeScreen = std::make_unique<TimeScreen>(mp_thisScreen);

    // temp for testing
    lv_obj_t* titleLabel = lv_label_create(mp_thisScreen, NULL);
    lv_label_set_text(titleLabel, "Luxray\n\nPreview Test");
    lv_obj_align(titleLabel, NULL, LV_ALIGN_CENTER, 0, 0);
}

MainScreen::~MainScreen() {}

bool MainScreen::procFrame() {
    // temp for testing
    if (hidKeysDown(CONTROLLER_P1_AUTO) & KEY_A) {
        mp_timeScreen->show();
    }
    Screen::procFrame();
    return true; // TODO: MainScreen may exit another way
}
