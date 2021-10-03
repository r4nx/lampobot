#ifndef LAMPOBOT_SQLITE_COLUMN_HPP_
#define LAMPOBOT_SQLITE_COLUMN_HPP_

#include <cstddef>
#include <cstdint>
#include <string>
#include <utility> // std::pair

namespace lampobot::sqlite {

class Statement;

class Column {
    enum class Type { sql_integer, sql_float, sql_text, sql_blob, sql_null };

public:
    Column(Statement &stmt, std::size_t index) noexcept;

    Type get_type() const noexcept;

    explicit operator int() const noexcept;
    explicit operator std::int64_t() const noexcept;
    explicit operator double() const noexcept;
    explicit operator std::string() const;
    explicit operator std::pair<const void *, std::size_t>() const noexcept;

protected:
    Statement & stmt_;
    std::size_t index_ = 0;
};

} // namespace lampobot::sqlite

#endif // !LAMPOBOT_SQLITE_COLUMN_HPP_
