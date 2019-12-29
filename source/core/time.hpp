#pragma once

#include <switch.h>
#include <stdexcept>

#include "../debug.hpp"

class TimeTaskHandler
{
private:
    time_t m_curTime;
public:
    TimeTaskHandler();
    ~TimeTaskHandler();

    void handleTask();
    std::string getCurDateStr();
    void setDayChange(int dayChange);
};
