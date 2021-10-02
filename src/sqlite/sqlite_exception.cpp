#include "sqlite_exception.hpp"

#include <stdexcept>
#include <string>

#include <sqlite3.h>

namespace lampobot::sqlite {

SQLiteException::SQLiteException(const std::string &error_msg, int ret_code)
    : std::runtime_error{error_msg + ": " + sqlite3_errstr(ret_code)}
{}

} // namespace lampobot::sqlite
