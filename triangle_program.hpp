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

struct triangle_program
{
    using mat4 = glm::mat4;
    using vec3 = glm::vec3;

    triangle_program();

    void set_z_rotation(float val)
    {
        z_angle_ = val;
    }

    void run(mat4 const& view_matrix);

    float z_angle_ = 0.0;

    opengl::program shader_program_;
    GLint mvp_location_, vpos_location_, vcol_location_;
};


