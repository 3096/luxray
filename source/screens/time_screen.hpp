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
    static constexpr const char* INITIAL_BUTTON_MAP[] = {
        "1", "2", "3", "Set", "+1", "\n",
        "4", "5", "6", "Step", "+2", "\n",
        "7", "8", "9", "NTP", "+3", "\n",
        "-", "0", "<-", "Return", "Reset", ""
    };
    static constexpr const uint16_t STEP_MAP_IDX = 9;
    static constexpr const char* STRING_STEP = INITIAL_BUTTON_MAP[STEP_MAP_IDX];
    static constexpr const char* STRING_STEP_CANCEL = "Cancel";

    /*  Button IDs:
        "1"= 0,  "2"= 1,  "3"= 2,   "Set"= 3,   "+1"= 4,
        "4"= 5,  "5"= 6,  "6"= 7,   "Step"= 8,  "+2"= 9,
        "7"= 10, "8"= 11, "9"= 12,  "NTP"= 13,  "+3"= 14,
        "-"= 15, "0"= 16, "<-"= 17, "Auto"= 18, "Reset"= 19
    */
    enum Button {
        BUTTON_SET = 3,
        BUTTON_PLUS_ONE = 4,
        BUTTON_STEP = 8,
        BUTTON_PLUS_TWO = 9,
        BUTTON_NTP = 13,
        BUTTON_PLUS_THREE = 14,
        BUTTON_NEGATIVE = 15,
        BUTTON_BACKSPACE = 17,
        BUTTON_AUTORESET = 18,
        BUTTON_RESET = 19
    };

    static constexpr int MAX_TARGET_CHANGE = 99999;

    // ui
    std::unique_ptr<TimeTaskHandler> mp_timeTaskHandler;
    lv_obj_t* mp_promptLabel;
    lv_obj_t* mp_valueLabel;
    lv_obj_t* mp_buttonMatrix;
    const char* m_buttonMap[24];
    lv_group_t* mp_inputGroup;

    // flags
    bool m_doButtonClick;
    bool m_doAutoReset;
    bool m_isInStepDays;
    bool m_isAlreadyNTP;
    bool m_disableNTP;
    bool m_internetIsConnected;

    // values
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
    virtual void mount_();
    virtual void unmount_();

   public:
    TimeScreen(Screen* prevScreen);
    ~TimeScreen();
};
