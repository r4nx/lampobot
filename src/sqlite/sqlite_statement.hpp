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
    Statement(const Database &db, const std::string &sql);

    template <Bindable... Params>
    Statement(const Database &db, const std::string &sql, Params &&...params)
        : Statement{db, sql}
    {
        bind(std::forward<Params>(params)...);
    }

    sqlite3_stmt *get_ptr() const noexcept;

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

    bool step();

    template <typename... Params>
    std::optional<std::tuple<Params...>> get_columns()
    {
        if (get_columns_count() < sizeof...(Params))
            return {};

        std::size_t column_index = 0;
        return {static_cast<Params>(column_by_index(column_index++))...};
    }

    Column column_by_index(std::size_t column_index) noexcept;
    int    get_columns_count() const noexcept;

    int reset() noexcept;
    int clear_bindings() noexcept;

protected:
    stmt_ptr_t stmt_{nullptr, &sqlite3_finalize};
};

} // namespace lampobot::sqlite

#endif // !LAMPOBOT_SQLITE_STATEMENT_HPP_
