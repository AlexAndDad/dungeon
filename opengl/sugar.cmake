sugar_files(SOURCE_FILES
        config.hpp
        vertex_array_object.cpp vertex_array_object.hpp
        buffer_implementation.cpp buffer_implementation.hpp
        buffers_service.cpp buffers_service.hpp
        buffers.cpp buffers.hpp
        context.cpp context.hpp
        context_service.cpp context_service.hpp
        error.cpp
        error.hpp
        glew_initialisation.cpp glew_initialisation.hpp
        glyph_render_impl.cpp glyph_render_impl.hpp glyph_render_service.cpp glyph_render_service.hpp glyph_renderer.cpp glyph_renderer.hpp
        opengl.hpp
        program.cpp
        program.hpp
        program_service.cpp program_service.hpp
        basic_resource_object.hpp
        resource_loader_service.cpp resource_loader_service.hpp
        shader.cpp
        shader.hpp
        shader_service.cpp shader_service.hpp
        shader_type.cpp shader_type.hpp
        texture.hpp texture.cpp
        version.cpp version.hpp)

sugar_include(detail)
