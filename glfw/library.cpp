//
// Created by rhodges on 13/02/18.
//

#include "library.hpp"
#include "window_service.hpp"
#include <boost/date_time/posix_time/posix_time.hpp>

namespace glfw
{
    int library::count = 0;
    thread_local std::vector<std::string> library::recent_errors;


    library::library(int major, int minor)
    {
        if (count)
            throw std::logic_error("gflw is a singleton");
        glfwSetErrorCallback(&library::error_callback);
        if (not glfwInit())
            throw std::runtime_error("failed to intialise glfw");
        ++count;
        set_context_version(major, minor);
    }

    library::~library()
    {
        if (count) {
            glfwTerminate();
            --count;
        }
    }

    void library::set_context_version(int major, int minor)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    }

    void library::error_callback(int error, const char *description)
    {
        auto ts = boost::posix_time::microsec_clock::local_time();
        std::ostringstream ss;
        ss << boost::posix_time::to_iso_extended_string(ts) << " : glfw : " << description;
        auto s = std::move(ss).str();
        std::clog << s << std::endl;
        recent_errors.push_back(std::move(s));
    }

    void library::report_errors()
    {
        std::ostringstream ss;
        ss << "a glfw operation failed.";
        if (recent_errors.empty()) {
            ss << " No recent errors have been reported.";
        } else {
            ss << " Recent errors are:";
            for (auto &&err : recent_errors) {
                ss << '\n' << err;
            }
            recent_errors.clear();
        }
        throw std::runtime_error(std::move(ss).str());
    }


}