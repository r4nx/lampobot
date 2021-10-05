#ifndef LAMPOBOT_SQLITE_EXCEPTION_HPP_
#define LAMPOBOT_SQLITE_EXCEPTION_HPP_

#include <stdexcept>
#include <string>

namespace lampobot::sqlite {

class SQLiteException : public std::runtime_error {
public:
    /**
     * @param error_msg Message describing the error context.
     * @param ret_code Result code to get the description of error via
     * sqlite3_errstr() function.
     */
    SQLiteException(const std::string &error_msg, int ret_code);
};

} // namespace lampobot::sqlite

#endif // !LAMPOBOT_SQLITE_EXCEPTION_HPP_
