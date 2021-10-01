#ifndef LAMPOBOT_SQLITE_STATEMENT_HPP_
#define LAMPOBOT_SQLITE_STATEMENT_HPP_

#include "sqlite_db.hpp"

#include <sqlite3.h>

#include <algorithm> // std::all_of
#include <cstddef>
#include <memory>
#include <string>
#include <utility> // std::forward

namespace lampobot::sqlite {

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

    bool bind_by_index(std::size_t param_index, int param);
    bool bind_by_index(std::size_t param_index, double param);

protected:
    stmt_ptr_t stmt_{nullptr, &sqlite3_finalize};
};

} // namespace lampobot::sqlite

#endif // !LAMPOBOT_SQLITE_STATEMENT_HPP_
