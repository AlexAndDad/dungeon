//
// Created by rhodges on 21/02/18.
//

#pragma once


#include <ft2build.h>
#include FT_FREETYPE_H

#include "service_base.hpp"
#include <notstd/handle.hpp>
#include <boost/noncopyable.hpp>
#include <typeindex>
#include <memory>
#include <unordered_map>
#include <boost/filesystem.hpp>

namespace freetype {

    struct face;


    struct library_service : notstd::stateless_service<library_service>
    {
        using native_handle_type = FT_Library;

        static native_handle_type construct();

        static void destroy(native_handle_type &impl) noexcept;

        static bool empty(native_handle_type &impl)  noexcept;

        static void invalidate(native_handle_type &impl) noexcept;
    };

    struct library : notstd::unique_handle<library_service>
    {
        using path = boost::filesystem::path;

        library();

        auto acquire(path const &pathname) -> face;

        template<class Service>
            Service& use()
        {
            auto i = services_.find(typeid(Service));
            if (i == std::end(services_))
            {
                auto ib = services_.emplace(typeid(Service), std::make_unique<Service>(*this));
                i = ib.first;
            }
            return static_cast<Service&>(*(i->second));
        }

    private:
        using inherited = notstd::unique_handle<library_service>;
        using service_map = std::unordered_map<std::type_index, std::unique_ptr<service_base>>;
        service_map services_;
    };

}


