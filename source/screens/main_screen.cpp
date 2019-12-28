#include "main_screen.hpp"

extern TimeScreen* gp_timeScreen;

MainScreen::MainScreen(lv_obj_t* prevScreen) : Screen(prevScreen) {
    gp_timeScreen = new TimeScreen(mp_thisScreen);

    // temp for testing
    lv_obj_t* titleLabel = lv_label_create(mp_thisScreen, NULL);
    lv_label_set_text(titleLabel, "Luxray\n\nPreview Test");
    lv_obj_align(titleLabel, NULL, LV_ALIGN_CENTER, 0, 0);
}

MainScreen::~MainScreen() {
    delete gp_timeScreen;
}

bool MainScreen::procFrame_() {
    // temp for testing
    if (hidKeysDown(CONTROLLER_P1_AUTO) & KEY_A) {
        gp_timeScreen->show();
    }
    Screen::procFrame_();
    return true; // TODO: MainScreen may exit another way
}
