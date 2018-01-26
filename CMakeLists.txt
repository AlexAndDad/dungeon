cmake_minimum_required(VERSION 3.6)
include(external_packages/Init.cmake)
option(HUNTER_STATUS_DEBUG "Hunter debug info" ON)

HunterGate(
        URL "https://github.com/ruslo/hunter/archive/v0.19.234.tar.gz"
        SHA1 "3deec1041bd01c91e78269522b901fbab3a765e5"
)
project(dungeon)

hunter_add_package(Sugar)
find_package(Sugar CONFIG REQUIRED)

set(my_target dungeon)

if (NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 14)
endif ()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

sugar_files(SOURCE_FILES main.cpp monster.cpp monster.hpp text_vanisher.cpp text_vanisher.hpp)
sugar_include(monster_types)
sugar_include(game_engine)
add_executable(${my_target} ${SOURCE_FILES})


# add ncurses
#
#set(CURSES_NEED_NCURSES TRUE)
#find_package(Curses REQUIRED)
#target_include_directories(${my_target} SYSTEM PUBLIC ${CURSES_INCLUDE_DIRS})
#target_link_libraries(${my_target} PUBLIC ${CURSES_LIBRARIES})

#
#
#
hunter_add_package(Boost)
find_package(Boost CONFIG REQUIRED)
target_link_libraries(${my_target} PUBLIC Boost::boost)
