#include "error.hpp"

ResultError::ResultError(const char* what, Result res) : std::runtime_error(what), m_res(res) {}

ResultError::~ResultError() {}
