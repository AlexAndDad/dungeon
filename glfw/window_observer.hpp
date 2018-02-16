//
// Created by rhodges on 13/02/18.
//

#ifndef DUNGEON_WINDOW_OBSERVER_HPP
#define DUNGEON_WINDOW_OBSERVER_HPP

#include "config.hpp"
#include "window_service.hpp"
#include "opengl/context.hpp"

namespace glfw {

    struct window_observer
    {
        using service_type = window_service;
        using implementation_type = service_type::implementation_type;

        window_observer(implementation_type);

        operator GLFWwindow *() const noexcept
        { return impl_; }

        void set_should_close(bool yn)
        {
            glfwSetWindowShouldClose(impl_, yn ? GLFW_TRUE : GLFW_FALSE);
        }

        auto get_implementation() -> implementation_type &
        {
            return impl_;
        }

        window_opengl_context &opengl_context()
        {
            return get_service().opengl_context(get_implementation());

        };

        constexpr static auto get_service() -> service_type
        {
            return service_type();
        }

    protected:
        auto swap(window_observer &other) noexcept
        {
            using std::swap;
            swap(impl_, other.impl_);
        }

    protected:
        implementation_type impl_;
    };

    window_opengl_context &opengl_context(window_observer &win);

}


#endif //DUNGEON_WINDOW_OBSERVER_HPP
