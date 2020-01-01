#pragma once

#include <memory>

#include "../core/time.hpp"
#include "screen.hpp"

class TimeScreen : public Screen {
   private:
    // consts
    static constexpr const char* STRING_CUR_DATE = "Current Date:";
    // static constexpr const char* STRING_TARGET_DATE = "Target Date:";
    static constexpr const char* STRING_TARGET_CHANGE = "Target Change:";
    static constexpr const char* STRING_STEPPING = "Step Remaining:";
    static constexpr const char* STRING_DAYS = " days";

    static constexpr const char* BUTTON_MAP_LAYOUT[] = {
        "1", "2", "3", "R/S", "+1", "\n",
        "4", "5", "6", "Set", "+3", "\n",
        "7", "8", "9", "Step", "-1", "\n",
        "-", "0", "<-", "NTP", " ", ""
    };
    /*  Button IDs:
        "1"= 0,  "2"= 1,  "3"= 2,   "R/S"= 3,   "+1"= 4,
        "4"= 5,  "5"= 6,  "6"= 7,   "Set"= 8,   "+3"= 9,
        "7"= 10, "8"= 11, "9"= 12,  "Step"= 13, "-1"= 14,
        "-"= 15, "0"= 16, "<-"= 17, "NTP"= 18,  "Stop"= 19
    */

    enum Button {
        BUTTON_RESET = 3,
        BUTTON_PLUS_ONE = 4,
        BUTTON_SET = 8,
        BUTTON_PLUS_THREE = 9,
        BUTTON_STEP = 13,
        BUTTON_MINUS_ONE = 14,
        BUTTON_NEGATIVE = 15,
        BUTTON_BACKSPACE = 17,
        BUTTON_NTP = 18,
        BUTTON_UNUSED = 19  // TODO: make step button do stop task
    };

    static constexpr int MAX_TARGET_CHANGE = 99999;

    // members
    std::unique_ptr<TimeTaskHandler> mp_timeTaskHandler;
    lv_obj_t* mp_promptLabel;
    lv_obj_t* mp_valueLabel;
    lv_obj_t* mp_buttonMatrix;

    bool m_doButtonClick;
    bool m_doNTP;
    bool m_isInStepDays;
    bool m_internetIsConnected;

    int m_curTargetChange;
    int8_t m_curTargetSign;
    std::string m_promptLabelStr;
    std::string m_valueLabelStr;

    // helpers
    void handleButtonEventImpl_();
    static void handleButtonEvent_(lv_obj_t* obj, lv_event_t event);
    void handleStepDaysStart_(int8_t stepDirection, int daysToStep);
    void handleStepDaysEnd_();

    // poly
    virtual bool procFrame_();

   public:
    TimeScreen(lv_obj_t* prevScreen);
    ~TimeScreen();
};
