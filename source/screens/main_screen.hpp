#pragma once

#include "screen.hpp"

class MainScreen : public Screen {
   private:
    /* data */
   public:
    MainScreen(lv_obj_t* prevScreen);
    ~MainScreen();
};
