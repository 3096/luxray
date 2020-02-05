#include <switch.h>

#include "../core/system.hpp"
#include "../debug.hpp"
#include "time_screen.hpp"

#include "main_screen.hpp"

MainScreen* gp_mainScreen;
TimeScreen* gp_timeScreen;

MainScreen::MainScreen() : Screen(nullptr), m_screenToShow(NO_SUB_SCREEN), m_shouldContinue(true) {
    gp_timeScreen = new TimeScreen(this);
    mp_timeErrorScreen = std::make_unique<TimeErrorScreen>(this);

    // list of buttons to different screens
    mp_screenListObj = lv_list_create(mp_screenObj, nullptr);
    lv_obj_align(mp_screenListObj, nullptr, LV_ALIGN_CENTER, 0, 0);

    // TODO: refactor the strings here
    lv_obj_set_event_cb(lv_list_add_btn(mp_screenListObj, nullptr, "Date Advance"), handleShowTimeScreen_);
    lv_obj_set_event_cb(lv_list_add_btn(mp_screenListObj, nullptr, "Exit"), handleExit_);

    lv_group_add_obj(mp_inputGroup, mp_screenListObj);
}

MainScreen::~MainScreen() { delete gp_timeScreen; }

bool MainScreen::procFrame_() {
    switch (m_screenToShow) {
        case TIME_SCREEN:
            if (os::setsysInternetTimeSyncIsOn()) {
                gp_timeScreen->show();
            } else {
                mp_timeErrorScreen->show();
            }
            break;
        default:
            return m_shouldContinue;
    }

    // remove m_screenToShow to show main screen again
    m_screenToShow = NO_SUB_SCREEN;
    return m_shouldContinue;
}

void MainScreen::handleShowScreen_(lv_event_t event, SubScreen screenToShow) {
    if (event == LV_EVENT_CLICKED) {
        m_screenToShow = screenToShow;
    }
}

void MainScreen::handleExitImpl_() {
    m_shouldContinue = false;  // TODO: add confirm message box or something
}

void MainScreen::handleExit_(lv_obj_t* obj, lv_event_t event) {
    if (event == LV_EVENT_CLICKED) {
        gp_mainScreen->handleExitImpl_();
    }
}
