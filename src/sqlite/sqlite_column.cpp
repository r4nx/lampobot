#include "sqlite_column.hpp"
#include "sqlite_statement.hpp"

#include <cstddef>
#include <cstdint>
#include <string>
#include <utility> // std::pair

#include <sqlite3.h>

namespace lampobot::sqlite {

Column::Column(Statement &stmt, std::size_t index) noexcept
    : stmt_{stmt},
      index_{index}
{}

Column::Type Column::get_type() const noexcept
{
    const int column_type = sqlite3_column_type(stmt_.get_ptr(), index_);

    switch (column_type)
    {
        case SQLITE_INTEGER:
            return Type::sql_integer;
        case SQLITE_FLOAT:
            return Type::sql_float;
        case SQLITE_TEXT:
            return Type::sql_text;
        case SQLITE_BLOB:
            return Type::sql_blob;
        case SQLITE_NULL:
            return Type::sql_null;
        default:
            return Type::sql_null;
    }
}

Column::operator int() const noexcept
{
    return sqlite3_column_int(stmt_.get_ptr(), index_);
}

Column::operator std::int64_t() const noexcept
{
    return sqlite3_column_int64(stmt_.get_ptr(), index_);
}

Column::operator double() const noexcept
{
    return sqlite3_column_double(stmt_.get_ptr(), index_);
}

Column::operator const char *() const
{
    return reinterpret_cast<const char *>(
        sqlite3_column_text(stmt_.get_ptr(), index_));
}

Column::operator std::pair<const void *, std::size_t>() const noexcept
{
    return {
        sqlite3_column_blob(stmt_.get_ptr(), index_),
        sqlite3_column_bytes(stmt_.get_ptr(), index_)};
}

} // namespace lampobot::sqlite
