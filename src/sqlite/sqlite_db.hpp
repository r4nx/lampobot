#ifndef LAMPOBOT_SQLITE_DB_HPP_
#define LAMPOBOT_SQLITE_DB_HPP_

#include <sqlite3.h>

#include <filesystem>
#include <memory>

namespace lampobot::sqlite {

class Database {
    friend class Statement;

    using db_ptr_t = std::unique_ptr<sqlite3, decltype(&sqlite3_close_v2)>;

public:
    Database(
        const std::filesystem::path &db_file,
        int                          flags = SQLITE_OPEN_READWRITE);

protected:
    db_ptr_t db_{nullptr, &sqlite3_close_v2};
};

} // namespace lampobot::sqlite

#endif // !LAMPOBOT_SQLITE_DB_HPP_
