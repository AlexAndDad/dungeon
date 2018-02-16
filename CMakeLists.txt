cmake_minimum_required(VERSION 3.6)
include(external_packages/Init.cmake)
project(dungeon)

if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/notstd)
    add_subdirectory(notstd)
endif ()

hunter_add_package(sugar)
find_package(sugar CONFIG REQUIRED)

set(my_target dungeon)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

sugar_files(SOURCE_FILES main.cpp monster.cpp monster.hpp triangle_program.cpp triangle_program.hpp)
sugar_include(environment)
sugar_include(glfw)
sugar_include(opengl)
sugar_include(monster_types)
sugar_include(game_engine)
sugar_include(shaders)
add_executable(${my_target} ${SOURCE_FILES})

if (NOT TARGET NotStd::handle)
    find_package(NotStd REQUIRED)
endif ()
target_link_libraries(${my_target} PUBLIC NotStd::handle)

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
AddBoost(TARGETS ${my_target} PRIVATE COMPONENTS date_time filesystem system program_options)

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
endif ()


# documentation
find_package(Doxygen QUIET)
if (DOXYGEN_FOUND)
    include(sugar_status_print)
    include(sugar_doxygen_generate)
    sugar_doxygen_generate(DEVELOPER
            TARGET ${my_target}
            DOXYTARGET "${my_target}-doc"
            DOXYFILE ${CMAKE_CURRENT_LIST_DIR}/doc/Doxyfile.in)
    add_custom_target(build-docs ALL DEPENDS "${my_target}-doc")
else ()
    message(STATUS "Doxygen not found. Skipping documentation targets")
    message(STATUS "Consider $ sudo dnf install doxygen")
endif ()

set(resource_install_dirs)
foreach (f IN LISTS SHADER_SOURCES)
    string(REGEX REPLACE "^${CMAKE_CURRENT_BINARY_DIR}/" "" frel "${f}")
    if (frel STREQUAL f)
        string(REGEX REPLACE "^${CMAKE_CURRENT_SOURCE_DIR}/" "" frel "${f}")
    endif ()
    get_filename_component(fdir ${frel} DIRECTORY)
    get_filename_component(fleaf ${frel} NAME)
    list(APPEND resource_install_dirs ${fdir})
    list(APPEND "resource_install_files_${fdir}" ${f})
endforeach ()
list(REMOVE_DUPLICATES resource_install_dirs)


foreach (dir IN LISTS resource_install_dirs)
    message(STATUS "DIRECTORY: [${dir}]")
    foreach (f ${resource_install_files_${dir}})
        message(STATUS "\t${f}")
    endforeach ()
endforeach ()