#ifndef LAMPOBOT_SQLITE_COLUMN_HPP_
#define LAMPOBOT_SQLITE_COLUMN_HPP_

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <utility> // std::pair

namespace lampobot::sqlite {

class Statement;

class Column {
    enum class Type { sql_integer, sql_float, sql_text, sql_blob, sql_null };

public:
    Column(Statement &stmt, std::size_t index) noexcept;

    /**
     * @return Column type; returns Type::sql_null in case of no mapping of
     * SQLite type to wrapper type was found.
     */
    Type get_type() const noexcept;

    /**
     * @brief SQLite 32-bit INTEGER type analogue.
     */
    explicit operator int() const noexcept;

    /**
     * @brief SQLite 64-bit INTEGER type analogue.
     */
    explicit operator std::int64_t() const noexcept;

    /**
     * @brief SQLite REAL type analogue.
     */
    explicit operator double() const noexcept;

    /**
     * @brief SQLite TEXT type analogue. May be std::nullopt in case if column
     * contains NULL (as it's forbidden and meaningless to construct std::string
     * from a nullptr).
     */
    explicit operator std::optional<std::string>() const;

    /**
     * @brief SQLite BLOB type analogue. Returned pointer may be invalidated in
     * case if conversion occurs, or Statement::step(), Statement::reset() or
     * Statement::~Statement() is called.
     */
    explicit operator std::pair<const void *, std::size_t>() const noexcept;

protected:
    Statement & stmt_;
    std::size_t index_ = 0;
};

} // namespace lampobot::sqlite

#endif // !LAMPOBOT_SQLITE_COLUMN_HPP_
