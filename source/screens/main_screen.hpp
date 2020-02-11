#pragma once

#include "screen.hpp"
#include "time_error_screen.hpp"

class MainScreen : public Screen {
   private:
    std::unique_ptr<TimeErrorScreen> mp_timeErrorScreen;

    lv_obj_t* mp_screenListObj;

    enum SubScreen { TIME_SCREEN, DEN_LIST_SCREEN, POKE_SCREEN, NO_SUB_SCREEN };

    SubScreen m_screenToShow;

    bool m_shouldContinue;

    virtual bool procFrame_();

    void handleShowScreen_(lv_event_t event, SubScreen screenToShow);
    static void handleShowTimeScreen_(lv_obj_t* obj, lv_event_t event);
    static void handleShowDenListScreen_(lv_obj_t* obj, lv_event_t event);
    static void handleShowPokeScreen_(lv_obj_t* obj, lv_event_t event);

    void handleExitImpl_();
    static void handleExit_(lv_obj_t* obj, lv_event_t event);

   public:
    MainScreen();
    ~MainScreen();
};
