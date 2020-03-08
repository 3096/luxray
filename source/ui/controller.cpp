#include <cassert>
#include <stdexcept>

#include "../core/system.hpp"
#include "../overlay.hpp"
#include "../util.hpp"

#include "controller.hpp"

namespace ui {

Controller Controller::s_instance;

Controller::Controller()
    : mp_curScreen(nullptr),
      mp_nextScreen(nullptr),
      m_ScreenIsJustToggled(false),
      m_screenIsOn(true),
      m_shouldExit(false) {
    LOG("ui::Controller initialized");
}

Controller::~Controller() {}

void Controller::mountScreen_(IScreen* screenToMount) {
    lv_indev_set_group(gp_keyIn, screenToMount->getLvInputGroup());
    lv_scr_load(screenToMount->getLvScreenObj());
    screenToMount->mount();
}

void Controller::threadMain_() {
    // mount inital screen
    assert(mp_curScreen);
    mountScreen_(mp_curScreen);

    // main loop
    while (true) {
        // check dock status
        if (os::apmConsoleIsDocked() != m_isDocked) {
            // TODO: update overlay driver
            // TODO: re-render cur screen
            m_isDocked = !m_isDocked;
        }

        // update hid
        hidScanInput();
        m_keysDown = hidKeysDown(CONTROLLER_P1_AUTO);
        m_keysHeld = hidKeysHeld(CONTROLLER_P1_AUTO);
        // TODO: parameterize button combo
        bool screenToggleKeysPressed = (m_keysHeld | KEY_DUP | KEY_RSTICK_UP) == m_keysHeld;

        // update screen toggle
        if (not m_ScreenIsJustToggled and screenToggleKeysPressed) {
            m_screenIsOn = !m_screenIsOn;
            m_ScreenIsJustToggled = true;
            if (m_screenIsOn) {
                lv_obj_invalidate(mp_curScreen->getLvScreenObj());  // Re-render screen
            } else {
                Overlay::flushEmptyFb();  // Turn off screen
            }
        } else if (m_ScreenIsJustToggled and not screenToggleKeysPressed) {
            m_ScreenIsJustToggled = false;
        }

        if (m_screenIsOn) {
            // check if next screen is requested
            if (mp_nextScreen) {
                mp_curScreen->unmount();

                mountScreen_(mp_nextScreen);

                mp_curScreen = mp_nextScreen;
                mp_nextScreen = nullptr;
            }

            // curScreen process frame
            lv_task_handler();
            mp_curScreen->procFrame();
        }

        // check if exit is requested
        if (m_shouldExit) {
            mp_curScreen->unmount();
            break;
        }

        svcSleepThread(10'000'000);
    }
}

void Controller::show(IScreen& screenToShow) {
    if (s_instance.mp_curScreen == nullptr) {
        s_instance.mp_curScreen = &screenToShow;
        s_instance.threadMain_();  // TODO: create ui thread instead of just calling
    } else {
        s_instance.mp_nextScreen = &screenToShow;
    }
}

}  // namespace ui
