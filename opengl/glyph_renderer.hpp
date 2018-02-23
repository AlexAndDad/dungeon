//
// Created by rhodges on 23/02/18.
//

#pragma once

#include "glyph_render_service.hpp"

namespace opengl {
    struct glyph_renderer_observer
    {
        using service_type = glyph_render_service;
        using implementation_type = service_type::implementation_type;

        glyph_renderer_observer(context &owner, char character, std::size_t pixel_height);

        void render(glm::vec2 position, glm::vec2 scale);

    protected:

        auto get_service() const -> service_type&
        {
            return *service_;
        }

        auto get_implementation() const -> implementation_type const &
        {
            return impl_;
        }

        auto get_implementation() -> implementation_type &
        {
            return impl_;
        }

    private:
        implementation_type impl_;
        service_type* service_;
    };

    struct glyph_renderer : glyph_renderer_observer
    {
        using glyph_renderer_observer::glyph_renderer_observer;
    };

}