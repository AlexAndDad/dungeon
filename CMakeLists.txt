cmake_minimum_required(VERSION 3.6)
include(external_packages/Init.cmake)
project(dungeon)

hunter_add_package(sugar)
find_package(sugar CONFIG REQUIRED)

set(my_target dungeon)

if (NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 14)
endif ()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

sugar_files(SOURCE_FILES main.cpp monster.cpp monster.hpp triangle_program.cpp triangle_program.hpp)
sugar_include(environment)
sugar_include(opengl)
sugar_include(monster_types)
sugar_include(game_engine)
sugar_include(shaders)
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
include(AddBoost)
AddBoost(TARGETS ${my_target} PRIVATE COMPONENTS system)

if (NOT DEFINED glm_ROOT)
    hunter_add_package(glm)
endif ()
find_package(glm CONFIG REQUIRED)
target_link_libraries(${my_target} PUBLIC glm)

hunter_add_package(glew)
find_package(glew CONFIG REQUIRED)
target_link_libraries(${my_target} PUBLIC glew::glew)

hunter_add_package(glfw)
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${my_target} PUBLIC glfw)


set(compiler_like_gcc 0)
if (${CMAKE_CXX_COMPILER_ID} MATCHES "GNU" OR ${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    set(compiler_like_gcc 1)
    message(STATUS "Compiler is ${CMAKE_CXX_COMPILER_ID} - like gcc")
endif ()

if (compiler_like_gcc)
    target_compile_definitions(${my_target} PUBLIC _DEFAULT_SOURCE=1)
endif ()

if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
    if (compiler_lke_gcc)
        target_compile_options(${my_target} PRIVATE -Wall -Wextra)
    endif ()
endif()
