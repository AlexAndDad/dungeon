//
// Created by rhodges on 13/02/18.
//

#ifndef DUNGEON_WINDOW_OBSERVER_HPP
#define DUNGEON_WINDOW_OBSERVER_HPP

#include "config.hpp"
#include "window_service.hpp"
#include "opengl/context.hpp"

namespace glfw {

    /// The abstraction of observing a GLFW window.
    struct window_observer
    {
        using service_type = window_service;
        using implementation_type = service_type::implementation_type;

        window_observer(implementation_type);

        operator GLFWwindow *() const noexcept
        { return impl_; }

        auto should_close() const -> bool
        {
            return get_service().should_close(get_implementation());
        }

        auto empty() const -> bool
        {
            return get_service().empty(impl_);
        }

        void set_should_close(bool yn)
        {
            get_service().set_shold_close(get_implementation(), yn);
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

        auto get_implementation() const -> GLFWwindow* const&
        {
            return impl_;
        }

        auto get_implementation() -> GLFWwindow* &
        {
            return impl_;
        }

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
