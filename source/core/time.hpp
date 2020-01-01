#pragma once

#include <time.h>
#include <cstdint>
#include <string>

class TimeTaskHandler {
   private:
    static constexpr float STEP_INTERVAL = 0.5;
    const u_int64_t STEP_TICK_INTERVAL;

    time_t m_curTime;
    time_t m_curTimeChange;

    int m_curDaysLeftToStep;
    int m_curStepDirection;
    u_int64_t m_lastStepIntervalNum;

    void setTime_(time_t time);

   public:
    TimeTaskHandler();
    ~TimeTaskHandler();

    void handleTask();

    void startStepDaysTask(int8_t sign, int daysToStep);
    inline void stopStepDaysTask() { m_curDaysLeftToStep = 0; }
    inline int daysLeftToStep() { return m_curDaysLeftToStep; }

    std::string getCurDateStr();
    void setDayChange(int dayChange);
    void setTimeNTP();
    void resetTime();
};
