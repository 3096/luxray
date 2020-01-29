#include <stdexcept>

#include <switch.h>

#include "../debug.hpp"
#include "ntp.hpp"

#include "time.hpp"

TimeTaskHandler::TimeTaskHandler()
    : STEP_INTERVAL_TICKS(STEP_INTERVAL * armGetSystemTickFreq()),
      m_curDaysLeftToStep(-1),
      m_isInStep(false),
      m_isAbortingStep(false),
      m_lastStepTick(0) {
    handleTask();
    m_resetTargetTm = *localtime(&m_curTime);
}

TimeTaskHandler::~TimeTaskHandler() {}

void TimeTaskHandler::handleTask() {
    Result rs = timeGetCurrentTime(TimeType_UserSystemClock, (u64*)&m_curTime);
    if (R_FAILED(rs)) {
        std::string err_msg = "timeGetCurrentTime failed with " + rs;
        throw std::runtime_error(err_msg);
    }

    if (m_isInStep) {
        u64 curSystemTick = armGetSystemTick();
        if (curSystemTick - m_lastStepTick > STEP_INTERVAL_TICKS) {
            if (m_isAbortingStep or m_curDaysLeftToStep <= 0) {
                if (m_resetAfterStep) {
                    resetTime();
                }
                m_isAbortingStep = m_isInStep = false;
            } else {
                setTime_(m_curTime + m_curStepDirection * 60 * 60 * 24);
                m_curDaysLeftToStep--;
            }
            m_lastStepTick = curSystemTick;
        }
    }
}

void TimeTaskHandler::startStepDaysTask(int8_t stepDirection, int daysToStep, bool resetAfterStep) {
    m_curStepDirection = stepDirection;
    m_curDaysLeftToStep = daysToStep;
    m_resetAfterStep = resetAfterStep;
    m_isInStep = true;
}

std::string TimeTaskHandler::getCurDateStr() {
    struct tm* p_tm = localtime(&m_curTime);
    char dateStr[11];
    strftime(dateStr, sizeof(dateStr), "%F", p_tm);
    return std::string(dateStr);
}

void TimeTaskHandler::setTime_(time_t time) {
    Result rs = timeSetCurrentTime(TimeType_NetworkSystemClock, (uint64_t)time);
    if (R_FAILED(rs)) {
        std::string err_msg = "timeSetCurrentTime failed with " + rs;
        throw std::runtime_error(err_msg);
    }
}

void TimeTaskHandler::setDayChange(int dayChange) {
    time_t timeToSet = m_curTime + (time_t)dayChange * 60 * 60 * 24;
    setTime_(timeToSet);
    m_resetTargetTm = *localtime(&timeToSet);
}

void TimeTaskHandler::setTimeNTP() {
    try {
        setTime_(ntpGetTime());
    } catch (std::runtime_error& e) {
        LOG("runtime_error: %s", e.what());
    }
}

void TimeTaskHandler::resetTime() {
    // use current HMS so only the date is reset
    struct tm curTimeTm = *localtime(&m_curTime);
    struct tm timeToSetTm = m_resetTargetTm;
    timeToSetTm.tm_hour = curTimeTm.tm_hour;
    timeToSetTm.tm_min = curTimeTm.tm_min;
    timeToSetTm.tm_sec = curTimeTm.tm_sec;

    setTime_(mktime(&timeToSetTm));
}
