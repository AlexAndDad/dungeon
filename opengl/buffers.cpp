//
// Created by Richard Hodges on 28/01/2018.
//

#include "buffers.hpp"
#include <boost/iterator/zip_iterator.hpp>

namespace opengl {

    namespace {
        void init_and_bind(buffer_init const &init,
                           GLuint &ident,
                           buffer_target &target)
        try
        {
            target = init.target;
            glBindBuffer(static_cast<GLenum>(target), ident);
            glBufferData(static_cast<GLenum>(target), init.size, init.data, static_cast<GLenum>(init.usage));
            check_errors("glBufferData");
        }
        catch(...)
        {
            std::throw_with_nested(std::runtime_error("init_and_bind"));
        }

    }

    buffers::buffers(std::size_t N)
        : notstd::unique_handle<buffers_service>(std::make_tuple(N))
    {}

    buffers::buffers(buffer_init const &bi)
    try
        : buffers(1)
    {
        auto &impl = mutable_native_handle();
        init_and_bind(bi, impl.idents.front(), impl.targets.front());
    }
    catch (...) {
        std::throw_with_nested(std::runtime_error("buffers::buffers"));
    }


    buffers::buffers(std::initializer_list<buffer_init> il)
        : buffers(il.size())
    {
        auto &impl = mutable_native_handle();
        if (impl.empty())
            return;

        auto first = boost::make_zip_iterator(boost::make_tuple(il.begin(), impl.idents.begin(), impl.targets.begin()));
        auto last = boost::make_zip_iterator(boost::make_tuple(il.end(), impl.idents.end(), impl.targets.end()));
        for (; first != last; ++first) {
            auto &&init = boost::get<0>(*first);
            auto &&ident = boost::get<1>(*first);
            auto &&target = boost::get<2>(*first);
            init_and_bind(init, ident, target);
        }
    }

    void buffers::bind() const
    {
        auto &&impl = native_handle();
        auto first = boost::make_zip_iterator(boost::make_tuple(impl.idents.begin(), impl.targets.begin()));
        auto last = boost::make_zip_iterator(boost::make_tuple(impl.idents.end(), impl.targets.end()));
        for (; first != last; ++first) {
            auto &&ident = boost::get<0>(*first);
            auto &&target = boost::get<1>(*first);
            glBindBuffer(static_cast<GLenum>(target), ident);
            check_errors("glBindBuffer");
        }
    }

    void buffers::unbind() const
    {
        for (auto &&target : native_handle().targets) {
            glBindBuffer(static_cast<GLenum>(target), 0);
        }
    }

    std::size_t buffers::size() const
    {
        return native_handle().targets.size();
    }


}