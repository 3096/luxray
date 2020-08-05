#include "main_screen.hpp"

#include <switch.h>

#include "../core/system.hpp"
#include "error_screen.hpp"
#include "lx/ui/controller.hpp"
#include "time_screen.hpp"

MainScreen MainScreen::s_instance;

MainScreen::MainScreen() : LOGCONSTRUCTSL m_basicScreen(*this), m_screenToShow(NO_SUB_SCREEN), m_shouldExit(false) {
    // list of buttons to different screens
    mp_screenListObj = lv_list_create(getLvScreenObj(), nullptr);
    lv_obj_align(mp_screenListObj, nullptr, LV_ALIGN_CENTER, 0, 0);
    m_basicScreen.addLvObjPositionUpdater(mp_screenListObj, lv_obj_realign);

    // TODO: refactor the strings here
    lv_obj_set_event_cb(lv_list_add_btn(mp_screenListObj, nullptr, "Date Advance"), handleShowTimeScreen_);
    lv_obj_set_event_cb(lv_list_add_btn(mp_screenListObj, nullptr, "Exit"), handleExit_);

    lv_group_add_obj(getLvInputGroup(), mp_screenListObj);

    LOGEL("done");
}

MainScreen::~MainScreen() {}

void MainScreen::renderScreen() { m_basicScreen.renderScreen(); }

void MainScreen::procFrame() {
    switch (m_screenToShow) {
        case TIME_SCREEN:
            if (os::setsysInternetTimeSyncIsOn()) {
                showScreen_(TimeScreen::getInstance());
            } else {
                showScreen_(ErrorScreen::getInstance(TIME_SETTING_ERROR_MSG));
            }
            break;
        default:
            break;
    }

    if (m_shouldExit) {
        lx::ui::Controller::stop();
    }
}

void MainScreen::showScreen_(lx::ui::IScreen& screenToShow) {
    lx::ui::Controller::show(screenToShow);
    // remove m_screenToShow to show main screen again
    m_screenToShow = NO_SUB_SCREEN;
}

void MainScreen::handleShowScreen_(lv_event_t event, SubScreen screenToShow) {
    if (event == LV_EVENT_CLICKED) {
        m_screenToShow = screenToShow;
    }
}

void MainScreen::handleExit_(lv_obj_t* obj, lv_event_t event) {
    if (event == LV_EVENT_CLICKED) {
        s_instance.m_shouldExit = true;
    }
}
