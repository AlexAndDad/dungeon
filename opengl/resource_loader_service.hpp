//
// Created by rhodges on 23/02/18.
//

#pragma once

#include "context_service.hpp"
#include <boost/filesystem.hpp>
#include <vector>

namespace opengl {

    struct resource_loader_service : implement_context_service<resource_loader_service>
    {
        using path = boost::filesystem::path;

        resource_loader_service(context& owner);

        void add_search_path(path path_root);

        auto load_as_string(path const& leaf) -> std::string;


    private:

        std::vector<path> search_paths_;
    };


}

