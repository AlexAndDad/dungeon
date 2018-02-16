//
// Created by rhodges on 13/02/18.
//

#include "buffers_service.hpp"
#include "error.hpp"

namespace opengl {

    auto buffers_service::construct(std::size_t N) -> implementation_type
    {
        auto result = implementation_type(N);
        if (not result.empty()) {
            glGenBuffers(N, result.idents.data());
            check_errors("glGenBuffers");
        }
        return result;
    }

    void buffers_service::destroy(implementation_type &impl) noexcept
    {
        if (not empty(impl)) {
            glDeleteBuffers(impl.idents.size(), impl.idents.data());
        }
    }

    bool buffers_service::empty(implementation_type const &impl)
    {
        return impl.empty();
    }

    void buffers_service::invalidate(implementation_type &impl) noexcept
    {
        clear(impl);
    }

}
