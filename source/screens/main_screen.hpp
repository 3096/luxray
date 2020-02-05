#pragma once

#include "screen.hpp"
#include "time_error_screen.hpp"

class MainScreen;
extern MainScreen* gp_mainScreen;

class MainScreen : public Screen {
   private:
    std::unique_ptr<TimeErrorScreen> mp_timeErrorScreen;

    lv_obj_t* mp_screenListObj;

    enum SubScreen { TIME_SCREEN = 0, NO_SUB_SCREEN };

    SubScreen m_screenToShow;

    bool m_shouldContinue;

    virtual bool procFrame_();

    void handleShowScreen_(lv_event_t event, SubScreen screenToShow);
    static inline void handleShowTimeScreen_(lv_obj_t* obj, lv_event_t event) {
#ifndef __INTELLISENSE__  // IntelliSense strikes again, doesn't recognize forward definition properly
        gp_mainScreen->handleShowScreen_(event, TIME_SCREEN);
#endif
    }

    void handleExitImpl_();
    static void handleExit_(lv_obj_t* obj, lv_event_t event);

   public:
    MainScreen();
    ~MainScreen();
};
