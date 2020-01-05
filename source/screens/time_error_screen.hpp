#pragma once

#include "screen.hpp"

class TimeErrorScreen : public Screen {
   public:
    TimeErrorScreen(Screen* prevScreen);
    ~TimeErrorScreen();
};
