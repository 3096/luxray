#include <stdexcept>

#include <switch.h>

#include "../debug.hpp"
#include "ntp.hpp"

#include "time.hpp"

TimeTaskHandler::TimeTaskHandler()
    : STEP_INTERVAL_TICKS(STEP_INTERVAL * armGetSystemTickFreq()),
      m_curTimeChange(0),
      m_curDaysLeftToStep(-1),
      m_lastStepTick(0) {
    handleTask();
}

TimeTaskHandler::~TimeTaskHandler() {}

void TimeTaskHandler::handleTask() {
    Result rs = timeGetCurrentTime(TimeType_UserSystemClock, (u64*)&m_curTime);
    if (R_FAILED(rs)) {
        std::string err_msg = "timeGetCurrentTime failed with " + rs;
        throw std::runtime_error(err_msg);
    }

    if (m_curDaysLeftToStep >= 0) {
        u64 curSystemTick = armGetSystemTick();
        if (curSystemTick - m_lastStepTick > STEP_INTERVAL_TICKS) {
            if (m_curDaysLeftToStep > 0) {
                setDayChange(m_curStepDirection);
            } else if (m_resetAfterStep) {
                resetTime();
            }
            m_lastStepTick = curSystemTick;
            m_curDaysLeftToStep--;
        }
    }
}

void TimeTaskHandler::startStepDaysTask(int8_t stepDirection, int daysToStep, bool resetAfterStep) {
    m_curStepDirection = stepDirection;
    m_curDaysLeftToStep = daysToStep;
    m_resetAfterStep = resetAfterStep;
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
    try {
        setTime_(ntpGetTime());
        m_curTimeChange = 0;
    } catch (std::runtime_error& e) {
        LOG("runtime_error: %s", e.what());
    }
}

void TimeTaskHandler::resetTime() {
    setTime_(m_curTime - m_curTimeChange);
    m_curTimeChange = 0;
}
