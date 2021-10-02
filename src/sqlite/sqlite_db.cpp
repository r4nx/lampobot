#include "sqlite_db.hpp"
#include "sqlite_exception.hpp"

#include <sqlite3.h>

#include <filesystem>
#include <string>

using namespace std::string_literals;
namespace fs = std::filesystem;

namespace lampobot::sqlite {

Database::Database(const fs::path &db_file, int flags)
{
    sqlite3 *raw_db = nullptr;

    if (int res =
            sqlite3_open_v2(db_file.string().c_str(), &raw_db, flags, nullptr);
        res != SQLITE_OK)
    {
        sqlite3_close_v2(raw_db);

        throw SQLiteException{"Failed to open the database", res};
    }

    db_.reset(raw_db);
}

sqlite3 *Database::get_ptr() const noexcept
{
    return db_.get();
}

int Database::get_error_code() const noexcept
{
    return sqlite3_extended_errcode(db_.get());
}

} // namespace lampobot::sqlite
