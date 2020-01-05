#pragma once

#include "screen.hpp"
#include "time_error_screen.hpp"

class MainScreen : public Screen {
   private:
    std::unique_ptr<TimeErrorScreen> mp_timeErrorScreen;
    
    bool m_timeScreenEnabled;

    virtual bool procFrame_();

   public:
    MainScreen(Screen* prevScreen);
    ~MainScreen();
};
