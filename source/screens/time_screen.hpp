#include "screen.hpp"

#include "../debug.hpp"

class TimeScreen : public Screen {
   private:
    static constexpr const char* BUTTON_MAP_LAYOUT[] = {"+1", "Set", "Step", "NTP", "\n", "1", "2", "3", "<-", "\n",
                                                        "4",  "5",   "6",    "0",   "\n", "7", "8", "9", "-",  ""};

    lv_obj_t* mp_promptLabel;
    lv_obj_t* mp_valueLabel;

    lv_obj_t* mp_buttonMap;

    virtual bool procFrame();

   public:
    TimeScreen(lv_obj_t* prevScreen);
    ~TimeScreen();
};
