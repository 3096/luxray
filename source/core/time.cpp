#include "time.hpp"

TimeTaskHandler::TimeTaskHandler()
    : STEP_TICK_INTERVAL(STEP_INTERVAL * armGetSystemTickFreq()), m_curDaysLeftToStep(0), m_lastStepInterval(0) {
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
        u64 curStepInterval = armGetSystemTick() / STEP_TICK_INTERVAL;
        if (curStepInterval > m_lastStepInterval) {
            setDayChange(m_curStepDirection);
            m_curDaysLeftToStep--;
            m_lastStepInterval = curStepInterval;
        }
    }
}

void TimeTaskHandler::startStepDaysTask(int8_t stepDirection, int daysToStep) {
    m_curStepDirection = stepDirection;
    m_curDaysLeftToStep = daysToStep;
    m_lastStepInterval = armGetSystemTick() / STEP_TICK_INTERVAL;
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

void TimeTaskHandler::setDayChange(int dayChange) { setTime_(m_curTime + dayChange * 60 * 60 * 24); }

void TimeTaskHandler::setTimeNTP() { setTime_(ntpGetTime()); }
