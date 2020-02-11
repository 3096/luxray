#pragma once

#include "screen.hpp"

class PokeScreen : public Screen {
   private:
    /* data */
   public:
    PokeScreen(Screen* prevScreen);
    ~PokeScreen();
};
