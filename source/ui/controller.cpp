#include "controller.hpp"

#include <cassert>
#include <stdexcept>

#include "../core/system.hpp"
#include "../debug.hpp"
#include "../overlay.hpp"

namespace ui {

Controller Controller::s_instance;

Controller::Controller()
    : mp_curScreen(nullptr),
      mp_nextScreen(nullptr),
      m_screenIsJustToggled(false),
      m_screenIsOn(true),
      m_shouldRerender(true),
      m_shouldExit(false) {
    LOG("ui::Controller initialized");
}

Controller::~Controller() {}

void Controller::mountScreen_(IScreen* screenToMount) {
    screenToMount->mount();
    lv_indev_set_group(gp_keyIn, screenToMount->getLvInputGroup());
    lv_scr_load(screenToMount->getLvScreenObj());
    m_shouldRerender = true;
}

void Controller::threadMain_() {
    // mount inital screen
    // DEBUG_ASSERT(mp_curScreen);  // TODO: implement an assert
    mountScreen_(mp_curScreen);

    // main loop
    while (true) {
        // update hid
        hidScanInput();
        m_keysDown = hidKeysDown(CONTROLLER_P1_AUTO);
        m_keysHeld = hidKeysHeld(CONTROLLER_P1_AUTO);
        // TODO: parameterize button combo
        bool screenToggleKeysPressed = (m_keysHeld | KEY_DUP | KEY_RSTICK_UP) == m_keysHeld;

        // update screen toggle
        if (not m_screenIsJustToggled and screenToggleKeysPressed) {
            m_screenIsOn = !m_screenIsOn;
            m_screenIsJustToggled = true;
            if (m_screenIsOn) {
                m_shouldRerender = true;
            } else {
                Overlay::flushEmptyFb();  // Turn off screen
            }
        } else if (m_screenIsJustToggled and not screenToggleKeysPressed) {
            m_screenIsJustToggled = false;
        }

        if (m_screenIsOn) {
            // check if next screen is requested
            if (mp_nextScreen) {
                mp_curScreen->unmount();
                mountScreen_(mp_nextScreen);

                mp_curScreen = mp_nextScreen;
                mp_nextScreen = nullptr;
            }

            // check if dock status changed
            // TODO: use an event for this to reduce ipc overhead
            auto curIsDocked = os::apmConsoleIsDocked();
            if (curIsDocked != Overlay::getIsDockedStatus()) {
                Overlay::setIsDockedStatus(curIsDocked);
                m_shouldRerender = true;
                lv_task_handler();  // need to handle the dispplay driver change before rendering
            }

            // if screen needs re-render
            if (m_shouldRerender) {
                Overlay::pauseRendering();
                mp_curScreen->renderScreen();
                Overlay::resumeRendering();
                lv_obj_invalidate(mp_curScreen->getLvScreenObj());
                m_shouldRerender = false;
            }

            // curScreen process frame
            mp_curScreen->procFrame();
            lv_task_handler();
        }

        // check if exit is requested
        if (m_shouldExit) {
            mp_curScreen->unmount();
            break;
        }

        Overlay::waitForVSync();
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
