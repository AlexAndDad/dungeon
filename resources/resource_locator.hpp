//
// Created by rhodges on 22/02/18.
//

#ifndef DUNGEON_RESOURCE_BASE_HPP
#define DUNGEON_RESOURCE_BASE_HPP

#include <boost/filesystem.hpp>

struct resource_locator
{
    using path = boost::filesystem::path;

    static void initialise(path const& exe_path);
    static path const& root();
    static path const& fonts();

    struct static_data;

};


#endif //DUNGEON_RESOURCE_BASE_HPP
