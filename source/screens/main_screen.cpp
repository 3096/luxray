#include <switch.h>

#include "../core/system.hpp"
#include "time_screen.hpp"

#include "main_screen.hpp"

extern TimeScreen* gp_timeScreen;

MainScreen::MainScreen(Screen* prevScreen) : Screen(prevScreen) {
    gp_timeScreen = new TimeScreen(this);
    mp_timeErrorScreen = std::make_unique<TimeErrorScreen>(this);

    // TODO: redo main screen when adding more features
    lv_obj_t* p_titleLabel = lv_label_create(mp_screenObj, NULL);
    lv_label_set_text(p_titleLabel, "Luxray\n\nBy 3096");
    lv_obj_align(p_titleLabel, NULL, LV_ALIGN_CENTER, 0, 0);
}

MainScreen::~MainScreen() { delete gp_timeScreen; }

bool MainScreen::procFrame_() {
    m_timeScreenEnabled = os::setsysInternetTimeSyncIsOn();

    // temp
    if (m_keysDown & (KEY_R)) {
        if (m_timeScreenEnabled) {
            gp_timeScreen->show();
        } else {
            mp_timeErrorScreen->show();
        }
    }
    return true;  // TODO: MainScreen may exit another way
}
