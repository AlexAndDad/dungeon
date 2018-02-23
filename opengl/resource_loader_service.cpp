//
// Created by rhodges on 23/02/18.
//

#include "resource_loader_service.hpp"
#include <fstream>
#include <stdexcept>

namespace opengl {

    resource_loader_service::resource_loader_service(context &owner)
        : implement_context_service<resource_loader_service>(owner)
    {

    }

    void resource_loader_service::add_search_path(path path_root)
    {
        search_paths_.push_back(std::move(path_root));
    }

    namespace {
        template<typename Container>
        auto load_into_container(Container &&target, resource_loader_service::path const &pathname) -> void
        try
        {
            using ios = std::ios;
            auto ins = std::ifstream(pathname.string(), ios::in | ios::binary | ios::ate);
            ins.exceptions(ios::failbit);

            auto size = ins.tellg();
            target.resize(std::size_t(size));
            ins.seekg(0);
            ins.read(std::addressof(target[0]), size);
        }
        catch (...)
        {
            std::throw_with_nested(
                std::runtime_error("load_into_container : failed to load file [" + pathname.string() + ']'));
        }

        bool exists_and_is_file(resource_loader_service::path const &pathname)
        {
            auto result = false;

            if (exists(pathname))
            {
                auto state = status(pathname);
                result = state.type() != boost::filesystem::file_type::directory_file;
            }

            return result;
        }
    }

    auto resource_loader_service::load_as_string(path const &leaf) -> std::string
    {
        std::string result;
        if (leaf.is_absolute())
        {
            load_into_container(result, leaf);
            goto loaded;
        }
        for (auto &&root : search_paths_)
        {
            auto full_path = root / leaf;
            if (exists_and_is_file(full_path))
            {
                load_into_container(result, full_path);
                goto loaded;
            }
        }
        throw std::runtime_error("load_as_string : not found : [" + leaf.string() + ']');
        loaded:
        return result;
    }

}