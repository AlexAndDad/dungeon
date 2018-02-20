//
// Created by rhodges on 13/02/18.
//

#ifndef DUNGEON_WINDOW_OBSERVER_HPP
#define DUNGEON_WINDOW_OBSERVER_HPP

#include "config.hpp"
#include "window_service.hpp"
#include "opengl/context.hpp"

namespace glfw {

    /// Define the operations on a window handle, whether it's an observing handle or an owner.
    /// This class defers all operations to the service object, which for this class is stateless. However the general
    /// design allows for a stateful reference to a service to be maintained if necesssary.
    /// @tparam Derived is designed to be any class whihc can deliver a reference to the underlying handle
    /// through a call to Derived::get_implementation() or Derived::get_implementation() const
    /// @see window_service
    template<class Derived>
        struct basic_window_handle
        {
            /// The class which provides window services. i.e. performs actions against the underlying handle.
            using service_type = window_service;

            /// The type of the underlying handle object
            using implementation_type = service_type::implementation_type;

            /// Request whether window has the close flag set
            /// @pre not empty()
            /// @see window_service::empty
            /// @returns true if window has been flagged for closure, otherwise false
            auto should_close() const -> bool
            {
                return get_service().should_close(self().get_implementation());
            }

            /// Ask whether the handle is invalid.
            /// @see window_service::empty()
            auto empty() const -> bool
            {
                return get_service().empty(self().get_implementation());
            }

            void set_should_close(bool yn = true)
            {
                get_service().set_shold_close(self().get_implementation(), yn);
            }

            window_opengl_context &opengl_context()
            {
                return get_service().opengl_context(self().get_implementation());
            };

            operator GLFWwindow *() const noexcept
            {
                return self().get_implementation();
            }

        protected:

            constexpr static auto get_service() -> service_type
            {
                return service_type();
            }

        private:

            auto self() const -> Derived const &
            {
                return *static_cast<Derived const *>(this);
            }

            auto self() -> Derived &
            {
                return *static_cast<Derived *>(this);
            }
        };

    /// The abstraction of observing a GLFW window.
    struct window_observer : basic_window_handle<window_observer>
    {
        friend basic_window_handle<window_observer>;

        window_observer(implementation_type);


    protected:

        auto get_implementation() const noexcept -> GLFWwindow *const &
        {
            return impl_;
        }

        auto get_implementation() noexcept -> GLFWwindow *&
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
