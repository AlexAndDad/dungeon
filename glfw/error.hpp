//
// Created by Richard Hodges on 16/02/2018.
//

#pragma once

#include "config.hpp"

namespace glfw {
    enum class error : int
    {
        NOT_INITIALIZED = GLFW_NOT_INITIALIZED,
        NO_CURRENT_CONTEXT = GLFW_NO_CURRENT_CONTEXT,
        INVALID_ENUM = GLFW_INVALID_ENUM,
        INVALID_VALUE = GLFW_INVALID_VALUE,
        OUT_OF_MEMORY = GLFW_OUT_OF_MEMORY,
        API_UNAVAILABLE = GLFW_API_UNAVAILABLE,
        VERSION_UNAVAILABLE = GLFW_VERSION_UNAVAILABLE,
        PLATFORM_ERROR = GLFW_PLATFORM_ERROR,
        FORMAT_UNAVAILABLE = GLFW_FORMAT_UNAVAILABLE,
        NO_WINDOW_CONTEXT = GLFW_NO_WINDOW_CONTEXT
    };

}


