#include "controller.hpp"

#include <cassert>
#include <stdexcept>

#include "../core/system.hpp"
#include "../overlay.hpp"

namespace ui {

Controller::Controller()
    : LOGCONSTRUCTSL mp_curScreen(nullptr),
      mp_nextScreen(nullptr),
      m_screenIsJustToggled(false),
      m_screenIsOn(true),
      m_shouldRerender(true),
      m_shouldExit(false) {
    lv_style_init(&m_fontStyleNormal);
    lv_style_init(&m_fontStyleSmall);
    updateFontStyles_();

    LOGEL("done");
}

Controller::~Controller() {}

void Controller::mountScreen_(IScreen* screenToMount) {
    screenToMount->mount();
    lv_indev_set_group(gp_keyIn, screenToMount->getLvInputGroup());
    lv_scr_load(screenToMount->getLvScreenObj());
    m_shouldRerender = true;
}

void Controller::updateFontStyles_() {
    // TODO: write a custom shared font renderer
    if (Overlay::getIsDockedStatus()) {
        lv_style_set_text_font(&m_fontStyleNormal, LV_STATE_DEFAULT, DOCKED_FONT.normal);
        lv_style_set_text_font(&m_fontStyleSmall, LV_STATE_DEFAULT, DOCKED_FONT.small);
    } else {
        lv_style_set_text_font(&m_fontStyleNormal, LV_STATE_DEFAULT, HANDHELD_FONT.normal);
        lv_style_set_text_font(&m_fontStyleSmall, LV_STATE_DEFAULT, HANDHELD_FONT.small);
    }
}

void Controller::threadMain_() {
    // mount inital screen
    // DEBUG_ASSERT(mp_curScreen);  // TODO: implement an assert
    mountScreen_(mp_curScreen);

    // main loop
    while (true) {
        // check if exit is requested
        if (m_shouldExit) {
            mp_curScreen->unmount();
            break;
        }

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
            if (mp_nextScreen) {  // check if next screen is requested
                mp_curScreen->unmount();
                mountScreen_(mp_nextScreen);

                mp_curScreen = mp_nextScreen;
                mp_nextScreen = nullptr;
            }

            if (Overlay::getIsDockedStatusChanged()) {
                updateFontStyles_();
                m_shouldRerender = true;
            }

            if (m_shouldRerender) {
                lv_obj_refresh_style(mp_curScreen->getLvScreenObj(), LV_STYLE_PROP_ALL);
                lv_obj_invalidate(mp_curScreen->getLvScreenObj());
                mp_curScreen->renderScreen();
                m_shouldRerender = false;
            }

            // curScreen process frame
            mp_curScreen->procFrame();
            lv_task_handler();
        }

        Overlay::waitForVSync();
    }
}

void Controller::show(IScreen& screenToShow) {
    auto& s_instance = getInstance();
    if (s_instance.mp_curScreen == nullptr) {
        s_instance.mp_curScreen = &screenToShow;
        s_instance.threadMain_();  // TODO: create ui thread instead of just calling
    } else {
        s_instance.mp_nextScreen = &screenToShow;
    }
}

}  // namespace ui
