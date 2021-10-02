#ifndef LAMPOBOT_SQLITE_EXCEPTION_HPP_
#define LAMPOBOT_SQLITE_EXCEPTION_HPP_

#include <stdexcept>
#include <string>

namespace lampobot::sqlite {

class SQLiteException : public std::runtime_error {
public:
    SQLiteException(const std::string &error_msg, int ret_code);
};

} // namespace lampobot::sqlite

#endif // !LAMPOBOT_SQLITE_EXCEPTION_HPP_
