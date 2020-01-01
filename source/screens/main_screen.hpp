#pragma once

#include "screen.hpp"

class MainScreen : public Screen {
   private:
    virtual bool procFrame_();

   public:
    MainScreen(lv_obj_t* prevScreen);
    ~MainScreen();
};
