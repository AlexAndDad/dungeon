cmake_minimum_required(VERSION 3.9)
project(dungeon)

if (NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 14)
endif ()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(dungeon main.cpp
        monster.hpp monster.cpp
        monster_types/lich.cpp monster_types/lich.hpp
        monster_types/goblin.cpp monster_types/goblin.hpp
        monster_types/all.hpp
        )
