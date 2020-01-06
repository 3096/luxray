#pragma once

#include <time.h>
#include <cstdint>
#include <string>

class TimeTaskHandler {
   private:
    // TODO: STEP_INTERVAL should be configurable since apparently only some switches are laggier than others
#ifdef HANDHELD
    static constexpr float STEP_INTERVAL = 0.36;
#else
    static constexpr float STEP_INTERVAL = 0.45;
#endif
    const uint64_t STEP_INTERVAL_TICKS;

    time_t m_curTime;
    time_t m_curTimeChange;

    int m_curDaysLeftToStep;
    int m_curStepDirection;
    bool m_resetAfterStep;
    uint64_t m_lastStepTick;

    void setTime_(time_t time);

   public:
    TimeTaskHandler();
    ~TimeTaskHandler();

    void handleTask();

    void startStepDaysTask(int8_t sign, int daysToStep, bool resetAfterStep);
    inline void stopStepDaysTask() { m_curDaysLeftToStep = 0; }
    inline int daysLeftToStep() { return m_curDaysLeftToStep; }

    std::string getCurDateStr();
    void setDayChange(int dayChange);
    void setTimeNTP();
    void resetTime();
};
