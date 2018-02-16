//
// Created by rhodges on 13/02/18.
//

#include "window_observer.hpp"
#include "library.hpp"

namespace glfw
{
    window_observer::window_observer(implementation_type impl)
    : impl_(impl)
    {
    }

    window_opengl_context& opengl_context(window_observer& win)
    {
        return win.opengl_context();
    }


}