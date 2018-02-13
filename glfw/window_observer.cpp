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

    void window_observer::make_context_current() const
    {
        glfwMakeContextCurrent(impl_);
    }

}