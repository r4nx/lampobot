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

bool Statement::bind_by_index(std::size_t param_index, int param) noexcept
{
    return sqlite3_bind_int(stmt_.get(), param_index, param) == SQLITE_OK;
}

bool Statement::bind_by_index(std::size_t param_index, double param) noexcept
{
    return sqlite3_bind_double(stmt_.get(), param_index, param) == SQLITE_OK;
}

bool Statement::step()
{
    const int res = sqlite3_step(stmt_.get());

    if (res != SQLITE_ROW && res != SQLITE_DONE)
        throw std::runtime_error{
            "Failed to execute the step: "s + sqlite3_errstr(res)};

    return res == SQLITE_ROW;
}

} // namespace lampobot::sqlite
