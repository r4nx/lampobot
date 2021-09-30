#include "sqlite_db.hpp"

#include <sqlite3.h>

#include <filesystem>
#include <stdexcept>
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

        throw std::runtime_error{
            "Failed to open the database: "s + sqlite3_errstr(res)};
    }

    db_.reset(raw_db);
}

} // namespace lampobot::sqlite
