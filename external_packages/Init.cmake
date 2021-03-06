option(PROJECT_STATUS_PRINT "Print working status" OFF)


list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})
include(HunterGate)
option(HUNTER_STATUS_DEBUG "Hunter debug info" OFF)
option(HUNTER_STATUS_PRINT "Print working status" ON)
HunterGate(
        URL "https://github.com/ruslo/hunter/archive/v0.20.5.tar.gz"
        SHA1 "9f5eb722cbd9068f3d21a43b07832f8cc41336a1"
)

if ("${CMAKE_SOURCE_DIR}/external_packages" STREQUAL "${CMAKE_CURRENT_LIST_DIR}")

    if (DEFINED CMAKE_TOOLCHAIN_FILE)
        message(STATUS "Configuring for toolchain [${CMAKE_TOOLCHAIN_FILE}]")
    else (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
        if (NOT DEFINED POLLY_ROOT)
            set(POLLY_ROOT "${CMAKE_CURRENT_LIST_DIR}/polly")
            if (NOT EXISTS "${POLLY_ROOT}/cxx11.cmake")
                message(STATUS "polly dependency not present - fetching")
                execute_process(
                        COMMAND git submodule init
                        COMMAND git submodule update external_packages/polly
                        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                        RESULT_VARIABLE result
                        OUTPUT_VARIABLE output
                        ERROR_VARIABLE error)
                if (NOT result EQUAL 0)
                    message(FATAL_ERROR "error:\n${error}")
                else ()
                    message(STATUS "git output: ${output}")
                endif ()
            endif ()
        endif ()
        set(CMAKE_TOOLCHAIN_FILE "${POLLY_ROOT}/cxx14.cmake")
        message(STATUS "Toolchain file set to ${CMAKE_TOOLCHAIN_FILE}")
    endif ()

else ()
    message(FATAL "configuring as a submodule\nexpected: ${CMAKE_SOURCE_DIR}/external_packages/\nactual: ${CMAKE_CURRENT_LIST_DIR}")
endif ()

include(${CMAKE_CURRENT_LIST_DIR}/HunterGate.cmake)
