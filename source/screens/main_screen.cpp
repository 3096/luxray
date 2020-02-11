#include <switch.h>

#include "../core/system.hpp"
#include "../debug.hpp"
#include "den_list_screen.hpp"
#include "poke_screen.hpp"
#include "time_screen.hpp"

#include "main_screen.hpp"

// TODO: redo how screens are managed

MainScreen* gp_mainScreen;
TimeScreen* gp_timeScreen;
DenListScreen* gp_denListScreen;
PokeScreen* gp_pokeScreen;

MainScreen::MainScreen() : Screen(nullptr), m_screenToShow(NO_SUB_SCREEN), m_shouldContinue(true) {
    gp_timeScreen = new TimeScreen(this);
    gp_denListScreen = new DenListScreen(this);
    gp_pokeScreen = new PokeScreen(this);
    mp_timeErrorScreen = std::make_unique<TimeErrorScreen>(this);

    // list of buttons to different screens
    mp_screenListObj = lv_list_create(mp_screenObj, nullptr);
    lv_obj_align(mp_screenListObj, nullptr, LV_ALIGN_CENTER, 0, 0);

    // TODO: refactor the strings here
    lv_obj_set_event_cb(lv_list_add_btn(mp_screenListObj, nullptr, "Date Advance"), handleShowTimeScreen_);
    lv_obj_set_event_cb(lv_list_add_btn(mp_screenListObj, nullptr, "Den List"), handleShowDenListScreen_);
    lv_obj_set_event_cb(lv_list_add_btn(mp_screenListObj, nullptr, "Pokemon Stats"), handleShowDenListScreen_);
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
        case DEN_LIST_SCREEN:
            gp_denListScreen->show();
            break;
        case POKE_SCREEN:
            gp_pokeScreen->show();
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

void MainScreen::handleShowTimeScreen_(lv_obj_t* obj, lv_event_t event) {
    gp_mainScreen->handleShowScreen_(event, TIME_SCREEN);
}

void MainScreen::handleShowDenListScreen_(lv_obj_t* obj, lv_event_t event) {
    gp_mainScreen->handleShowScreen_(event, DEN_LIST_SCREEN);
}

void MainScreen::handleShowPokeScreen_(lv_obj_t* obj, lv_event_t event) {
    gp_mainScreen->handleShowScreen_(event, DEN_LIST_SCREEN);
}

void MainScreen::handleExitImpl_() { m_shouldContinue = false; }

void MainScreen::handleExit_(lv_obj_t* obj, lv_event_t event) {
    if (event == LV_EVENT_CLICKED) {
        gp_mainScreen->handleExitImpl_();
    }
}
