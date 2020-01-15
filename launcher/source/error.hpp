#pragma once

#include <stdexcept>

#include <switch.h>

class ResultError : public std::runtime_error {
   private:
    Result m_res;

   public:
    ResultError(const char* what, Result res);
    ~ResultError();

    inline Result getResult() { return m_res; }
};
