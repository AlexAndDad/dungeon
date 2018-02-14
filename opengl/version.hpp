#pragma once

#include "config.hpp"
#include <tuple>

namespace opengl
{
    struct version
    {
        int major, minor;

    private:
        auto as_tuple() const { return std::tie(major, minor); }
        friend auto operator<<(std::ostream& os, version const& arg) -> std::ostream&;
        friend std::string to_string(version const& arg);
    };

    auto get_version() -> version;
}