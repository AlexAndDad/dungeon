//
// Created by rhodges on 13/02/18.
//

#ifndef DUNGEON_IBRARY_HPP
#define DUNGEON_IBRARY_HPP

#include "config.hpp"
#include <boost/noncopyable.hpp>
#include <vector>
#include <string>

#include "desktop.hpp"

namespace glfw
{

    struct monitor {};
    struct window;

    struct library : boost::noncopyable
    {
        library(int major = 2, int minor = 1);

        ~library();

        void set_context_version(int major, int minor);

        static void error_callback(int error, const char *description);

        [[noreturn]]
        static void report_errors();

        static int count;
        static thread_local std::vector<std::string> recent_errors;
    };


}

#endif //DUNGEON_IBRARY_HPP
