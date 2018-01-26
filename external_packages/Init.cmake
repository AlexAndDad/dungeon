if ("${CMAKE_SOURCE_DIR}/external_packages" STREQUAL "${CMAKE_CURRENT_LIST_DIR}")
#[[
    if (NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/Sugar/cmake")
        message(STATUS "Sugar dependency not present - fetching")
        execute_process(COMMAND git submodule update --init Sugar
                WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/external_packages"
                RESULT_VARIABLE result
                OUTPUT_VARIABLE output
                ERROR_VARIABLE error)
        if (NOT result EQUAL 0)
            message(FATAL_ERROR "error:\n${error}")
        endif ()
    endif ()
    include("${CMAKE_SOURCE_DIR}/external_packages/Sugar/cmake/Sugar")
]]

    if (DEFINED CMAKE_TOOLCHAIN_FILE)
        message(STATUS "Configuring for toolchain [${CMAKE_TOOLCHAINFILE}]")
    else (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
        if (NOT DEFINED POLLY_ROOT)
            set(POLLY_ROOT "${CMAKE_CURRENT_LIST_DIR}/polly")
            if (NOT EXISTS "${POLLY_ROOT}/cxx11.cmake")
                message(STATUS "polly dependency not present - fetching")
                execute_process(COMMAND git submodule update --init polly
                        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/external_packages"
                        RESULT_VARIABLE result
                        OUTPUT_VARIABLE output
                        ERROR_VARIABLE error)
                if (NOT result EQUAL 0)
                    message(FATAL_ERROR "error:\n${error}")
                endif ()
            endif ()
        endif ()
        set(CMAKE_TOOLCHAIN_FILE ${POLLY_ROOT}/cxx14.cmake)
        message(STATUS "Toolchain file set to ${CMAKE_TOOLCHAIN_FILE}")
    endif ()

else ()
    message(FATAL "configuring as a submodule\nexpected: ${CMAKE_SOURCE_DIR}/external_packages/\nactual: ${CMAKE_CURRENT_LIST_DIR}")
endif ()

include(${CMAKE_CURRENT_LIST_DIR}/HunterGate.cmake)
