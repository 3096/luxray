#pragma once

#include <memory>

#include "screen.hpp"
#include "time_screen.hpp"

class MainScreen : public Screen {
   private:
    std::unique_ptr<TimeScreen> mp_timeScreen;

    virtual bool procFrame();

   public:
    MainScreen(lv_obj_t* prevScreen);
    ~MainScreen();
};
