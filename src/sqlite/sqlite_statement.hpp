#ifndef LAMPOBOT_SQLITE_STATEMENT_HPP_
#define LAMPOBOT_SQLITE_STATEMENT_HPP_

#include "sqlite_column.hpp"
#include "sqlite_db.hpp"

#include <sqlite3.h>

#include <algorithm> // std::all_of
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <utility> // std::forward, std::pair

namespace lampobot::sqlite {

class Statement;

template <typename T>
concept Bindable = requires(Statement &stmt, std::size_t param_index, T &&param)
{
    stmt.bind_by_index(param_index, std::forward<T>(param));
};

class Statement {
    using stmt_ptr_t =
        std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)>;

public:
    /**
     * @param db Target database.
     * @param sql Query to execute.
     *
     * @exception SQLiteException If the statement is failed to prepare
     */
    Statement(const Database &db, const std::string &sql);

    /**
     * @param db Target database.
     * @param sql Query to execute.
     * @param params Parameters to bind.
     */
    template <Bindable... Params>
    Statement(const Database &db, const std::string &sql, Params &&...params)
        : Statement{db, sql}
    {
        bind(std::forward<Params>(params)...);
    }

    /**
     * @return Pointer to the underlying SQLite statement structure
     */
    sqlite3_stmt *get_ptr() const noexcept;

    /**
     * @brief Bind parameters to the statement.
     *
     * @param params Parameters to bind.
     * @return bool, indicating the success in case if each call to
     * sqlite3_bind_* returns SQLITE_OK
     */
    template <Bindable... Params>
    bool bind(Params &&...params)
    {
        static_assert(
            sizeof...(Params) > 0,
            "You have to bind at least one parameter");

        std::size_t param_index  = 1;
        const auto  bind_results = {
            bind_by_index(param_index++, std::forward<Params>(params))...};

        return std::ranges::all_of(bind_results, [](const bool x) {
            return x;
        });
    }

    bool bind_by_index(std::size_t param_index, int param) noexcept;
    bool bind_by_index(std::size_t param_index, std::int64_t param) noexcept;
    bool bind_by_index(std::size_t param_index, double param) noexcept;
    bool bind_by_index(std::size_t param_index, const char *param) noexcept;
    bool bind_by_index(
        std::size_t                                 param_index,
        const std::pair<const void *, std::size_t> &param) noexcept;

    /**
     * @brief Evaluate the statement via iterating through rows.
     *
     * If the statement is going to be reused, it must be reset with a
     * Statement::reset() call after this function returned false.
     *
     * @exception SQLiteException If error code, returned by sqlite3_step,
     * doesn't equal SQLITE_ROW or SQLITE_DONE
     * @return If any rows are left to iterate
     */
    bool step();

    /**
     * @brief Access the columns of the query result.
     *
     * See sqlite_column.hpp for the list of available types to specify here as
     * template parameters.
     *
     * Beware: wrapper doesn't check if the statement was actually executed and
     * there are rows returned, so the returned values may be a complete garbage
     * and it's up to you to ensure that successful call to Statement::step()
     * precede the call to this function.
     *
     * The only thing that is done by the wrapper is comparing the count of
     * passed parameters with the actual columns count and returning a
     * std::nullopt in case if the latter is fewer than the former.
     */
    template <typename... Params>
    std::optional<std::tuple<Params...>> get_columns()
    {
        if (get_columns_count() < sizeof...(Params))
            return {};

        std::size_t column_index = 0;
        return {static_cast<Params>(column_by_index(column_index++))...};
    }

    /**
     * @return Instance of wrapper type, that may be casted to C++ analogues of
     * SQLite types.
     */
    Column column_by_index(std::size_t column_index) noexcept;
    int    get_columns_count() const noexcept;

    /**
     * @brief Reset the statement to be able to be reused. Doesn't reset
     * bindings.
     *
     * @return Raw SQLite return code.
     */
    int reset() noexcept;

    /**
     * @brief Reset the statement bindings.
     *
     * @return Raw SQLite return code.
     */
    int clear_bindings() noexcept;

protected:
    stmt_ptr_t stmt_{nullptr, &sqlite3_finalize};
};

} // namespace lampobot::sqlite

#endif // !LAMPOBOT_SQLITE_STATEMENT_HPP_
