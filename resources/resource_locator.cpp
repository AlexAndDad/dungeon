//
// Created by rhodges on 22/02/18.
//

#include "resource_locator.hpp"

struct resource_locator::static_data
{
    path font_path;
};

namespace
{
    resource_locator::static_data& statics()
    {
        static resource_locator::static_data _;
        return _;
    }
}

void resource_locator::initialise(path const& exe_path)
{
    auto& data = statics();
    data.font_path = exe_path.parent_path() / "resources" / "fonts";
}

resource_locator::path const& resource_locator::fonts()
{
    return statics().font_path;
}
