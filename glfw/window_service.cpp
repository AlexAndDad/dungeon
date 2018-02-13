//
// Created by rhodges on 13/02/18.
//

#include "window_service.hpp"
#include "library.hpp"
#include <memory>

namespace glfw
{
    auto window_service::construct_on_desktop(int width, int height, const char *title) -> implementation_type
    {
        auto window_data = std::make_unique<per_window_data>();

        auto pwindow = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (not pwindow) {
            library::report_errors();
        }
        glfwSetWindowUserPointer(pwindow, window_data.release());
        return pwindow;
    }

    void window_service::destroy(implementation_type& impl) noexcept
    {
        if (impl)
        {
            if(auto pv = glfwGetWindowUserPointer(impl))
            {
                auto user_ptr = static_cast<per_window_data*>(pv);
                delete user_ptr;
            }
            glfwDestroyWindow(impl);
            impl = nullptr;
        }
    }

}