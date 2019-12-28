#include "screen.hpp"

#include "../debug.hpp"

class TimeScreen : public Screen {
   private:
    static constexpr const char* BUTTON_MAP_LAYOUT[] = {
        "1", "2", "3", "R/S", "+1", "\n",
        "4", "5", "6", "Set", "+3", "\n",
        "7", "8", "9", "Step", " ", "\n",
        "-", "0", "<-", "NTP", " ", ""
    };

    lv_obj_t* mp_promptLabel;
    lv_obj_t* mp_valueLabel;

    virtual bool procFrame();

   public:
    TimeScreen(lv_obj_t* prevScreen);
    ~TimeScreen();
};
