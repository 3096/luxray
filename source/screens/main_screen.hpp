#pragma once

#include <memory>

#include "screen.hpp"
#include "time_screen.hpp"

class MainScreen : public Screen {
   private:
    virtual bool procFrame_();

   public:
    MainScreen(lv_obj_t* prevScreen);
    ~MainScreen();
};
