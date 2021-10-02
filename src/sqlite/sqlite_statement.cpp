#include "sqlite_statement.hpp"
#include "sqlite_exception.hpp"

#include <sqlite3.h>

#include <cstddef>
#include <string>

using namespace std::string_literals;

namespace lampobot::sqlite {

Statement::Statement(const Database &db, const std::string &sql)
{
    sqlite3_stmt *raw_stmt = nullptr;

    if (int res = sqlite3_prepare_v2(
            db.get_ptr(),
            sql.c_str(),
            -1,
            &raw_stmt,
            nullptr);
        res != SQLITE_OK)
    {
        sqlite3_finalize(raw_stmt);

        throw SQLiteException{"Failed to prepare the statement", res};
    }

    stmt_.reset(raw_stmt);
}

sqlite3_stmt *Statement::get_ptr() const noexcept
{
    return stmt_.get();
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
        throw SQLiteException{"Failed to execute the step", res};

    return res == SQLITE_ROW;
}

} // namespace lampobot::sqlite
