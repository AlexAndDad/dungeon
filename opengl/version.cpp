#include "version.hpp"
#include <sstream>

namespace opengl
{
    auto get_version() -> version
    {
        version v;
        glGetIntegerv(GL_MAJOR_VERSION, &v.major);
        glGetIntegerv(GL_MINOR_VERSION, &v.minor);
        return v;
    }


    auto operator<<(std::ostream& os, version const& arg) -> std::ostream&
    {
        return os << arg.major << '.' << arg.minor;
    }

    std::string to_string(version const& arg)
    {
        std::ostringstream ss;
        ss << arg;
        return std::move(ss).str();
    }

}