if ("${CMAKE_SOURCE_DIR}/external_packages" STREQUAL "${CMAKE_CURRENT_LIST_DIR}")
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
else ()
    message(FATAL "configuring as a submodule\nexpected: ${CMAKE_SOURCE_DIR}/external_packages/\nactual: ${CMAKE_CURRENT_LIST_DIR}")
endif ()
