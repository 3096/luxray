#include <switch.h>

#include "../overlay.hpp"

#include "screen.hpp"

extern Overlay* gp_overlay;

Screen::Screen(Screen* prevScreen)
    : mp_prevScreen(prevScreen),
      m_ScreenIsJustToggled(false),
      m_screenIsOn(true),
      mp_screenObj(lv_obj_create(nullptr, nullptr)) {}

Screen::~Screen() { lv_obj_del(mp_screenObj); }

void Screen::show() {
    if (mp_prevScreen) {
        mp_prevScreen->unmount_();
    }
    mount_();

    while (true) {
        hidScanInput();
        m_keysDown = hidKeysDown(CONTROLLER_P1_AUTO);
        m_keysHeld = hidKeysHeld(CONTROLLER_P1_AUTO);
        bool screenToggleKeysPressed = (m_keysHeld | KEY_DUP | KEY_RSTICK_UP) == m_keysHeld;

        if (not m_ScreenIsJustToggled and screenToggleKeysPressed) {
            m_screenIsOn = !m_screenIsOn;
            m_ScreenIsJustToggled = true;
            if (m_screenIsOn) {
                lv_obj_invalidate(mp_screenObj);  // Re-render screen
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

    unmount_();
    if (mp_prevScreen) {
        mp_prevScreen->mount_();
    }
}

bool Screen::procFrame_() { return not((m_keysDown & KEY_L) and m_screenIsOn); }

void Screen::mount_() { lv_scr_load(mp_screenObj); }

void Screen::unmount_() {}
