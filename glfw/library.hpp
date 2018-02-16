//
// Created by rhodges on 13/02/18.
//

#ifndef DUNGEON_IBRARY_HPP
#define DUNGEON_IBRARY_HPP

#include "config.hpp"
#include "opengl/version.hpp"

#include <boost/noncopyable.hpp>
#include <vector>
#include <string>

#include "desktop.hpp"

namespace glfw
{

    struct monitor {};
    struct window;

//    constexpr opengl::version default_opengl_version = { 3, 2 };
    constexpr opengl::version default_opengl_version = { 4, 1 };

    struct library : boost::noncopyable
    {
        library(opengl::version version = default_opengl_version);

        ~library();

        void set_context_version(opengl::version context_version);

        static void error_callback(int error, const char *description);

        [[noreturn]]
        static void report_errors();

        /// If there have been any errors, throw
        static void check_errors();

        static int count;
        static thread_local std::vector<std::string> recent_errors;
    };


}

#endif //DUNGEON_IBRARY_HPP
