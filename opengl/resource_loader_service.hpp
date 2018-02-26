//
// Created by rhodges on 23/02/18.
//

#pragma once

#include "context_service.hpp"
#include <boost/filesystem.hpp>
#include <vector>

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/utility/string_view.hpp>

namespace opengl {

    struct mapped_file
    {
        using file_mapping = boost::interprocess::file_mapping;
        using mapped_region = boost::interprocess::mapped_region;
        using path = boost::filesystem::path;

        mapped_file(path const& pathname);

        auto size() const -> std::size_t
        {
            return mr_.get_size();
        }

        auto data() const -> const char *
        {
            return reinterpret_cast<const char*>(mr_.get_address());
        }

        auto as_string_view() -> boost::string_view
        {
            return { data(), size() };
        }

    private:

        file_mapping mapping_;
        mapped_region mr_;
    };

    namespace detail
    {
        inline const GLchar *to_gl_char(mapped_file const& mf) {
            return mf.data();
        }

        inline GLint get_gl_string_length(mapped_file const& mf) {
            return GLint(mf.size());
        }
    }

    struct resource_loader_service : implement_context_service<resource_loader_service>
    {
        using path = boost::filesystem::path;

        resource_loader_service(context &owner);

        void add_search_path(path path_root);

        auto load_as_string(path const &leaf) -> std::string;

        auto load_as_mapped_file(path const& leaf) -> mapped_file;


    private:

        std::vector<path> search_paths_;
    };


}

