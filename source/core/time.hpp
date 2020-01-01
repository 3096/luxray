#pragma once

#include <switch.h>
#include <stdexcept>

#include "../debug.hpp"
#include "ntp.hpp"

class TimeTaskHandler {
   private:
    static constexpr float STEP_INTERVAL = 0.5;
    const u64 STEP_TICK_INTERVAL;

    time_t m_curTime;

    int m_curDaysLeftToStep;
    int m_curStepDirection;
    u64 m_lastStepInterval;

    void setTime_(time_t time);

   public:
    TimeTaskHandler();
    ~TimeTaskHandler();

    void handleTask();

    void startStepDaysTask(int8_t sign, int daysToStep);
    int daysLeftToStep();

    std::string getCurDateStr();
    void setDayChange(int dayChange);
    void setTimeNTP();
};
