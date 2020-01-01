#include <stdexcept>

#include <switch.h>

#include "../debug.hpp"
#include "ntp.hpp"

#include "time.hpp"

TimeTaskHandler::TimeTaskHandler()
    : STEP_TICK_INTERVAL(STEP_INTERVAL * armGetSystemTickFreq()),
      m_curTimeChange(0),
      m_curDaysLeftToStep(0),
      m_lastStepIntervalNum(0) {
    handleTask();
}

TimeTaskHandler::~TimeTaskHandler() {}

void TimeTaskHandler::handleTask() {
    Result rs = timeGetCurrentTime(TimeType_UserSystemClock, (u64*)&m_curTime);
    if (R_FAILED(rs)) {
        std::string err_msg = "timeGetCurrentTime failed with " + rs;
        throw std::runtime_error(err_msg);
    }

    if (m_curDaysLeftToStep > 0) {
        u64 curStepIntervalNum = armGetSystemTick() / STEP_TICK_INTERVAL;
        if (curStepIntervalNum > m_lastStepIntervalNum) {
            setDayChange(m_curStepDirection);
            m_curDaysLeftToStep--;
            m_lastStepIntervalNum = curStepIntervalNum;
        }
    }
}

void TimeTaskHandler::startStepDaysTask(int8_t stepDirection, int daysToStep) {
    m_curStepDirection = stepDirection;
    m_curDaysLeftToStep = daysToStep;
    m_lastStepIntervalNum = armGetSystemTick() / STEP_TICK_INTERVAL;
}

std::string TimeTaskHandler::getCurDateStr() {
    struct tm* p_tm = localtime(&m_curTime);
    char dateStr[11];
    strftime(dateStr, sizeof(dateStr), "%F", p_tm);
    return dateStr;
}

void TimeTaskHandler::setTime_(time_t time) {
    Result rs = timeSetCurrentTime(TimeType_NetworkSystemClock, (uint64_t)time);
    if (R_FAILED(rs)) {
        std::string err_msg = "timeSetCurrentTime failed with " + rs;
        throw std::runtime_error(err_msg);
    }
}

void TimeTaskHandler::setDayChange(int dayChange) {
    time_t change = (time_t)dayChange * 60 * 60 * 24;
    setTime_(m_curTime + change);
    m_curTimeChange += change;
}

void TimeTaskHandler::setTimeNTP() {
    setTime_(ntpGetTime());
    m_curTimeChange = 0;
}

void TimeTaskHandler::resetTime() {
    setTime_(m_curTime - m_curTimeChange);
    m_curTimeChange = 0;
}
