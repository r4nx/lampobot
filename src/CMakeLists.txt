add_executable(lampobot
    "main.cpp"
    "abstract_script.hpp"
    "lua_script.hpp"
    "lua_script.cpp"
)

target_compile_features(lampobot PRIVATE cxx_std_20)

target_include_directories(lampobot PRIVATE
    ${OPENSSL_INCLUDE_DIR}
    ${Boost_INCLUDE_DIR}
)

if (CURL_FOUND)
    target_include_directories(lampobot PRIVATE ${CURL_INCLUDE_DIRS})
    target_compile_definitions(lampobot PRIVATE HAVE_CURL)
endif()

if (LUA_FOUND)
    target_include_directories(lampobot PRIVATE ${LUA_INCLUDE_DIR})
endif()

target_link_libraries(lampobot
    ${CMAKE_THREAD_LIBS_INIT}
    ${OPENSSL_LIBRARIES}
    ${Boost_LIBRARIES}
    ${CURL_LIBRARIES}
    ${LUA_LIBRARIES}
    TgBot
    sol2::sol2
    SQLite::SQLite3
)
