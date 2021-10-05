#ifndef LAMPOBOT_SQLITE_DB_HPP_
#define LAMPOBOT_SQLITE_DB_HPP_

#include <sqlite3.h>

#include <filesystem>
#include <memory>

namespace lampobot::sqlite {

class Database {
    using db_ptr_t = std::unique_ptr<sqlite3, decltype(&sqlite3_close_v2)>;

public:
    /**
     * @param db_file Path to database file.
     * @param flags SQLite flags to be passed to sqlite3_open_v2
     *
     * @exception SQLiteException If something went wrong with opening the
     * database
     */
    explicit Database(
        const std::filesystem::path &db_file,
        int                          flags = SQLITE_OPEN_READWRITE);

    /**
     * @return Pointer to the underlying SQLite database structure
     */
    sqlite3 *get_ptr() const noexcept;

    /**
     * @return Result code of the last SQLite API call
     */
    int get_error_code() const noexcept;

protected:
    db_ptr_t db_{nullptr, &sqlite3_close_v2};
};

} // namespace lampobot::sqlite

#endif // !LAMPOBOT_SQLITE_DB_HPP_
