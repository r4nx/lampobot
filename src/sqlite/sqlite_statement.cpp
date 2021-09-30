#include "sqlite_statement.hpp"

#include <sqlite3.h>

#include <cstddef>
#include <stdexcept>
#include <string>

using namespace std::string_literals;

namespace lampobot::sqlite {

Statement::Statement(const Database &db, const std::string &sql)
{
    sqlite3_stmt *raw_stmt = nullptr;

    if (int res = sqlite3_prepare_v2(
            db.db_.get(),
            sql.c_str(),
            -1,
            &raw_stmt,
            nullptr);
        res != SQLITE_OK)
    {
        sqlite3_finalize(raw_stmt);

        throw std::runtime_error{
            "Failed to prepare the statement: "s + sqlite3_errstr(res)};
    }

    stmt_.reset(raw_stmt);
}

bool Statement::bind_by_index(std::size_t param_index, int param)
{
    return sqlite3_bind_int(stmt_.get(), param_index, param) == SQLITE_OK;
}

bool Statement::bind_by_index(std::size_t param_index, double param)
{
    return sqlite3_bind_double(stmt_.get(), param_index, param) == SQLITE_OK;
}

} // namespace lampobot::sqlite