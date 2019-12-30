#include "screen.hpp"
#include "../overlay.hpp"

extern Overlay* gp_overlay;

Screen::Screen(lv_obj_t* prevScreen)
    : mp_prevScreen(prevScreen),
      m_ScreenIsJustToggled(false),
      mp_thisScreen(lv_obj_create(nullptr, nullptr)),
      m_screenIsOn(true) {}

Screen::~Screen() { lv_obj_del(mp_thisScreen); }

void Screen::show() {
    lv_scr_load(mp_thisScreen);

    while (true) {
        hidScanInput();
        m_keysDown = hidKeysDown(CONTROLLER_P1_AUTO);
        m_keysHeld = hidKeysHeld(CONTROLLER_P1_AUTO);
        bool screenToggleKeysPressed = (m_keysHeld | KEY_DUP | KEY_RSTICK_UP) == m_keysHeld;

        if (not m_ScreenIsJustToggled and screenToggleKeysPressed) {
            m_screenIsOn = !m_screenIsOn;
            m_ScreenIsJustToggled = true;
            if (m_screenIsOn) {
                lv_obj_invalidate(mp_thisScreen);  // Re-render screen
            } else {
                gp_overlay->flushEmptyFb();  // Turn off screen
            }
        } else if (m_ScreenIsJustToggled and not screenToggleKeysPressed) {
            m_ScreenIsJustToggled = false;
        }

        svcSleepThread(10'000'000);
        if (m_screenIsOn) {
            lv_task_handler();
            if (not procFrame_()) {
                break;
            }
        }
    }

    if (mp_prevScreen) {
        lv_scr_load(mp_prevScreen);
    }
}

bool Screen::procFrame_() { return not((m_keysDown & KEY_L) and m_screenIsOn); }
