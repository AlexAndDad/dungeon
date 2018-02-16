cmake_minimum_required(VERSION 3.0)
project(NotStd VERSION 0.1.0 LANGUAGES CXX)
set(component_name handle)
set(component notstd-${component_name})

if (HUNTER_ENABLED)
    hunter_add_package(sugar)
endif ()
find_package(sugar CONFIG REQUIRED)

set(INCLUDE_FILES)
set(SOURCE_FILES)
sugar_include(include)
sugar_include(src)

add_library(${component} ${INCLUDE_FILES} ${SOURCE_FILES})
target_include_directories(${component} SYSTEM PUBLIC
        ${PROJECT_BINARY_DIR}/include
        ${PROJECT_SOURCE_DIR}/include)

add_library(${PROJECT_NAME}::${component_name} ALIAS ${component})

set(compiler_is_clang 0)
if (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    set(compiler_is_clang 1)
endif ()

set(compiler_like_gcc 0)
if (${CMAKE_CXX_COMPILER_ID} MATCHES "GNU" OR compiler_is_clang)
    set(compiler_like_gcc 1)
    message(STATUS "Compiler is ${CMAKE_CXX_COMPILER_ID} - like gcc")
endif ()

if (compiler_like_gcc)
    target_compile_definitions(${component} PUBLIC _DEFAULT_SOURCE=1)
endif ()

if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
    if (compiler_lke_gcc)
        target_compile_options(${component} PRIVATE -Wall -Wextra)
    endif ()
endif ()


# documentation
find_package(Doxygen QUIET)
if (DOXYGEN_FOUND)
    include(sugar_status_print)
    include(sugar_doxygen_generate)
    sugar_doxygen_generate(DEVELOPER
            TARGET ${component}
            DOXYTARGET "${component}-doc"
            DOXYFILE "${PROJECT_SOURCE_DIR}/doc/Doxyfile.in")
    option("${PROJECT_NAME}_BUILD_DOCUMENTATION" "Build the documentation" ON)
    if ("${${PROJECT_NAME}_BUILD_DOCUMENTATION}")
        add_custom_target(${component}-build-docs ALL DEPENDS "${component}-doc")
    endif ()
else ()
    message(STATUS "Doxygen not found. Skipping documentation targets")
    message(STATUS "Consider $ sudo dnf install doxygen")
endif ()

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug" AND "${DOXYGEN_FOUND}" AND "${compiler_is_clang}")
    target_compile_options(${component} PRIVATE -Wdocumentation)
endif ()
