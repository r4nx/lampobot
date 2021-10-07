#include "sqlite_statement.hpp"
#include "sqlite_exception.hpp"

#include <sqlite3.h>

#include <cstddef>
#include <cstdint>
#include <string>
#include <utility> // std::pair

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

int Statement::bind_by_index(int param_index, int param) noexcept
{
    return sqlite3_bind_int(stmt_.get(), param_index, param);
}

int Statement::bind_by_index(int param_index, std::int64_t param) noexcept
{
    return sqlite3_bind_int64(stmt_.get(), param_index, param);
}

int Statement::bind_by_index(int param_index, double param) noexcept
{
    return sqlite3_bind_double(stmt_.get(), param_index, param);
}

int Statement::bind_by_index(int param_index, const char *param) noexcept
{
    return sqlite3_bind_text(
        stmt_.get(),
        param_index,
        param,
        -1,
        SQLITE_STATIC);
}

int Statement::bind_by_index(
    int                                         param_index,
    const std::pair<const void *, std::size_t> &param) noexcept
{
    return sqlite3_bind_blob64(
        stmt_.get(),
        param_index,
        param.first,
        param.second,
        SQLITE_STATIC);
}

bool Statement::step()
{
    const int res = sqlite3_step(stmt_.get());

    if (res != SQLITE_ROW && res != SQLITE_DONE)
        throw SQLiteException{"Failed to execute the step", res};

    return res == SQLITE_ROW;
}

Column Statement::column_by_index(std::size_t column_index) noexcept
{
    return Column{*this, column_index};
}

int Statement::get_columns_count() const noexcept
{
    return sqlite3_column_count(stmt_.get());
}

int Statement::reset() noexcept
{
    return sqlite3_reset(stmt_.get());
}

int Statement::clear_bindings() noexcept
{
    return sqlite3_clear_bindings(stmt_.get());
}

} // namespace lampobot::sqlite
