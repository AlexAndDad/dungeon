cmake_minimum_required(VERSION 3.9)
include(external_packages/Init.cmake)
include(sugar_files)
include(sugar_include)

project(dungeon)

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
set(CURSES_NEED_NCURSES TRUE)
find_package(Curses REQUIRED)
target_include_directories(${my_target} SYSTEM PUBLIC ${CURSES_INCLUDE_DIRS})
target_link_libraries(${my_target} PUBLIC ${CURSES_LIBRARIES})
