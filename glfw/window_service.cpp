//
// Created by rhodges on 13/02/18.
//

#include "window_service.hpp"
#include "library.hpp"
#include <memory>

namespace glfw
{
    void window_service::deleter::operator()(GLFWwindow* pwin) const noexcept
    {
        if (pwin)
        {
            delete static_cast<per_window_data*>(glfwGetWindowUserPointer(pwin));
            glfwDestroyWindow(pwin);
        }
    }

    auto window_service::construct_on_desktop(int width, int height, const char *title) -> implementation_type
    {
        auto ptr = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (not ptr)
        {
            library::report_errors();
        }

        return ptr;
    }

    void window_service::destroy(implementation_type& impl) noexcept
    {
        auto del = deleter();
        del(impl);
        impl = nullptr;
    }

}