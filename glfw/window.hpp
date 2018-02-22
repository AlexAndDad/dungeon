//
// Created by rhodges on 13/02/18.
//

#ifndef DUNGEON_INDOW_HPP
#define DUNGEON_INDOW_HPP

#include "window_observer.hpp"

namespace glfw
{
    struct library;
    struct desktop;
    struct monitor;

    /// A handle which owns the lifetime of a GLFWwindow
    struct window : window_observer
    {
        /// Construct a window on the desktop
        /// @param dt is a desktop object. It is used as a signal that the window should be on the desktop.
        /// @param width is an integer representing the pixel width of the window
        /// @param height is an integer representing the pixel height of the window
        /// @param title is a c-string which will be used as the window title. The title is copied into the window
        ///        and may be destroyed once this call returns
        ///
        window(desktop const& dt, int width, int height, const char* title);

        window(window&& other) noexcept
            : window_observer(std::move(other))
        {
            other.impl_ = nullptr;
        }

        window& operator=(window&& other) noexcept
        {
            auto tmp = std::move(other);
            swap(tmp);
            return *this;
        }

        void reset(implementation_type newimpl = nullptr) noexcept
        {
            if (impl_)
                glfwDestroyWindow(impl_);
            impl_ = newimpl;

        }

        ~window()
        {
            reset();
        }

    private:

        friend library;

    };

}


#endif //DUNGEON_INDOW_HPP
