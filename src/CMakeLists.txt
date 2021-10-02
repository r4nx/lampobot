add_executable(lampobot
    "main.cpp"
    "abstract_script.hpp"
    "lua_script.hpp"
    "lua_script.cpp"
    "sqlite/sqlite_exception.hpp"
    "sqlite/sqlite_exception.cpp"
    "sqlite/sqlite_db.hpp"
    "sqlite/sqlite_db.cpp"
    "sqlite/sqlite_statement.hpp"
    "sqlite/sqlite_statement.cpp"
)

target_compile_features(lampobot PRIVATE cxx_std_20)

if (CURL_FOUND)
    target_compile_definitions(lampobot PRIVATE HAVE_CURL)
endif()

if (LUA_FOUND)
    target_include_directories(lampobot PRIVATE ${LUA_INCLUDE_DIR})
endif()

target_link_libraries(lampobot
    Threads::Threads
    OpenSSL::SSL
    OpenSSL::Crypto
    Boost::boost
    Boost::system
    CURL::libcurl
    TgBot
    sol2::sol2
    SQLite::SQLite3
    ${LUA_LIBRARIES}
)
