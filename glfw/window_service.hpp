//
// Created by rhodges on 13/02/18.
//

#ifndef DUNGEON_WINDOW_SERVICE_HPP
#define DUNGEON_WINDOW_SERVICE_HPP

#include <memory>
#include "config.hpp"
#include "opengl/context.hpp"
#include "library.hpp"

namespace glfw {

    struct window_opengl_context : opengl::context
    {
        window_opengl_context(GLFWwindow* pwin)
            : window_ptr_(pwin)
        {}


        bool is_equal(opengl::context const& other) const override
        {
            if(auto pr = dynamic_cast<window_opengl_context const*>(&other))
            {
                return this->window_ptr_ == pr->window_ptr_;
            }
            else
            {
                return false;
            }
        }

        void handle_select() override
        {
            glfwMakeContextCurrent(window_ptr_);
            library::check_errors();
        }

    private:

        GLFWwindow* window_ptr_;
    };

    struct per_window_data
    {
        per_window_data(GLFWwindow* winptr)
            : opengl_context_(winptr)
        {}

        window_opengl_context& opengl_context()
        {
            return opengl_context_;
        }

    private:
        window_opengl_context opengl_context_;
    };

    /// Provides operations against a GLFWwindow* handle
    struct window_service
    {
        /// Declares the type of the handle
        using implementation_type = GLFWwindow *;

        /// constrcts a window handle on the current desktop
        static auto construct_on_desktop(int width, int height, const char *title) -> implementation_type ;

        /// Destroys a window handle
        /// @pre not empty(impl)
        /// @post The underlying window is destroyed.
        /// @post impl == nullptr
        /// @exceptions guaranteed no exceptions
        ///
        static void destroy(implementation_type& impl) noexcept ;

        static per_window_data& user_data(const implementation_type& impl)
        {
            auto p_user_data = static_cast<per_window_data*>(glfwGetWindowUserPointer(impl));
            if (not p_user_data)
            {
                glfwMakeContextCurrent(impl);
                auto puser = std::make_unique<per_window_data>(impl);
                p_user_data = puser.get();
                glfwSetWindowUserPointer(impl, puser.release());

            }
            return *p_user_data;
        }

        static window_opengl_context& opengl_context(implementation_type const& impl)
        {
            return user_data(impl).opengl_context();
        }

        struct deleter
        {
            void operator()(GLFWwindow* pwin) const noexcept;
        };

        static auto empty(implementation_type const& impl) -> bool
        {
            return impl == nullptr;
        }

        static auto should_close(implementation_type const& impl) -> bool
        {
            assert(not empty(impl));
            return glfwWindowShouldClose(impl);
        }

        static auto set_shold_close(implementation_type& impl, bool should) -> void
        {
            assert(not empty(impl));
            glfwSetWindowShouldClose(impl, should ? GLFW_TRUE : GLFW_FALSE);
        }

    };

}
#endif //DUNGEON_WINDOW_SERVICE_HPP
