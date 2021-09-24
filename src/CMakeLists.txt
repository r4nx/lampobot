add_executable(lampobot
    "main.cpp"
)

target_compile_features(lampobot PRIVATE cxx_std_20)
target_link_libraries(lampobot
    TgBot
    ${CMAKE_THREAD_LIBS_INIT}
    ${OPENSSL_LIBRARIES}
    ${Boost_LIBRARIES}
    ${CURL_LIBRARIES}
    ${LUA_LIBRARIES}
    sol2::sol2
    SQLite::SQLite3
)
