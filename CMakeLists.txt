cmake_minimum_required(VERSION 3.16.3)

project(lampobot VERSION 0.1.0
               DESCRIPTION "Scripting-powered chat bot"
               LANGUAGES CXX)

set(Boost_USE_MULTITHREADED ON)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)
find_package(CURL)
find_package(Lua REQUIRED)
find_package(SQLite3 REQUIRED)

include(FetchContent)

FetchContent_Declare(
    sol2
    GIT_REPOSITORY https://github.com/ThePhD/sol2.git
    GIT_TAG 6283d3c2be81ef222318f1d463b69b5f90d6dea5
)
FetchContent_MakeAvailable(sol2)

add_subdirectory(external/tgbot-cpp)
add_subdirectory(src)
