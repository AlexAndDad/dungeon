//
// Created by Richard Hodges on 28/01/2018.
//

#pragma once

#include "opengl/config.hpp"
#include "opengl/error.hpp"
#include "basic_resource_object.hpp"
#include <vector>
#include <initializer_list>
#include <boost/iterator/zip_iterator.hpp>
#include "buffers_service.hpp"
#include <notstd/handle.hpp>

namespace opengl {


    /// @todo misunderstanding of bound buffers
    struct buffers : notstd::unique_handle<buffers_service>
    {
        buffers(std::size_t N);

        buffers(std::initializer_list<buffer_init> il);
        buffers(buffer_init const& bi);


        void bind() const;
        void unbind() const;

        std::size_t size() const;
    };

}


