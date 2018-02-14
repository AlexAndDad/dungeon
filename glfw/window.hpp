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

    struct window : window_observer
    {
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
