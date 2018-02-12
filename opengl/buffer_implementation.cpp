//
// Created by rhodges on 12/02/18.
//

#include "buffer_implementation.hpp"

namespace opengl
{

    buffer_implementation::buffer_implementation(std::size_t N)
    : idents(N)
    , targets(N)
    {}


    void clear(buffer_implementation& impl)
    {
        impl.idents.clear();
        impl.targets.clear();
    }

    void swap(buffer_implementation& l, buffer_implementation& r) noexcept
    {
        using std::swap;
        swap(l.idents, r.idents);
        swap(l.targets, r.targets);
    }

}
