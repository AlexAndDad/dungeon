//
// Created by Richard Hodges on 28/01/2018.
//

#include "buffers.hpp"
#include <boost/iterator/zip_iterator.hpp>

namespace opengl {

    buffers::buffers(std::size_t N)
        : resource_object<buffers_service>(std::piecewise_construct, N)
    {}


    buffers::buffers(std::initializer_list<buffer_init> il)
    : buffers(il.size())
    {
        auto& impl = get_implementation();
        if (impl.empty())
            return;

        auto first = boost::make_zip_iterator(boost::make_tuple(il.begin(), impl.begin()));
        auto last = boost::make_zip_iterator(boost::make_tuple(il.end(), impl.end()));
        for ( ; first != last ; ++first)
        {
            auto&& init = boost::get<0>(*first);
            auto&& ident = boost::get<1>(*first);
            glBindBuffer(static_cast<GLenum>(init.target), ident);
            glBufferData(static_cast<GLenum>(init.target), init.size, init.data, static_cast<GLenum>(init.usage));
        }
    }

}