//
// Created by Richard Hodges on 27/01/2018.
//

#pragma once

#include "environment/config.hpp"
#include <GL/glew.h>

#if __has_include(<OpenGL/gl.h>)

#include <OpenGL/gl.h>

#else
#include "GL/gl.h"
#endif

#include <GLFW/glfw3.h>
#include <cstddef>
