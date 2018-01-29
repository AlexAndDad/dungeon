//
// Created by Richard Hodges on 27/01/2018.
//

#pragma once

#include "opengl/program.hpp"
#include "shaders/fragment_shader.glsl.hpp"
#include "shaders/vertex_shader.glsl.hpp"
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "opengl/buffers.hpp"

struct xy_colour
{
    struct
    {
        float x, y;
    } vPos;

    struct
    {
        float r, g, b;
    } vCol;
};

struct triangle_buffers
{
    template<std::size_t N>
    triangle_buffers(const xy_colour (&src)[N])
        : buffers_(opengl::array_buffer(src, opengl::buffer_usage::static_draw))
        , size_(N)
    {}

    std::size_t size() const
    {
        return size_;
    }

    void bind() const
    {
        buffers_.bind();
    }

private:
    opengl::buffers buffers_;
    std::size_t size_;
};

struct triangle_program
{
    using mat4 = glm::mat4;
    using vec3 = glm::vec3;

    triangle_program();

    void set_z_rotation(float val)
    {
        z_angle_ = val;
    }

    void run(mat4 const &view_matrix, const triangle_buffers &buffers);

    float z_angle_ = 0.0;

    opengl::program shader_program_;
    GLint mvp_location_, vpos_location_, vcol_location_;
};


