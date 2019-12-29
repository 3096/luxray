#include "time.hpp"

TimeTaskHandler::TimeTaskHandler() {
    handleTask();
}

TimeTaskHandler::~TimeTaskHandler() {}

void TimeTaskHandler::handleTask() {
    Result rs = timeGetCurrentTime(TimeType_UserSystemClock, (u64*)&m_curTime);
    if (R_FAILED(rs)) {
        std::string err_msg = "timeGetCurrentTime failed with " + rs;
        LOG("%s", err_msg.c_str());
        throw std::runtime_error(err_msg);
    }
}

std::string TimeTaskHandler::getCurDateStr() {
    struct tm* p_tm = localtime(&m_curTime);
    char dateStr[11];
    strftime(dateStr, sizeof(dateStr), "%F", p_tm);
    return dateStr;
}

void TimeTaskHandler::setDayChange(int dayChange) {
    struct tm* p_tmTimeToSet = localtime(&m_curTime);
    p_tmTimeToSet->tm_mday += dayChange;
    time_t timeToSet = mktime(p_tmTimeToSet);
    Result rs = timeSetCurrentTime(TimeType_NetworkSystemClock, (uint64_t)timeToSet);
    if (R_FAILED(rs)) {
        std::string err_msg = "timeSetCurrentTime failed with " + rs;
        LOG("%s", err_msg.c_str());
        throw std::runtime_error(err_msg);
    }
}
