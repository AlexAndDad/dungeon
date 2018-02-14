//
// Created by Richard Hodges on 27/01/2018.
//

#include "triangle_program.hpp"

triangle_program::triangle_program()
    : shader_program_(opengl::vertex_shader(shaders::vertex_shader_glsl),
                      opengl::fragment_shader(shaders::fragment_shader_glsl))
    , mvp_location_{glGetUniformLocation(shader_program_.native_handle(), "MVP")}
    , vpos_location_{glGetAttribLocation(shader_program_.native_handle(), "vPos")}
    , vcol_location_{glGetAttribLocation(shader_program_.native_handle(), "vCol")}
{
    opengl::check_errors("get locations");

}


void triangle_program::run(mat4 const& view_matrix, const triangle_buffers& buffers)
{
    shader_program_.use();

    buffers.bind();

    glVertexAttribPointer(vpos_location_, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 5, (void *) 0);
    opengl::check_errors("vpos - glVertexAttribPointer");

    glEnableVertexAttribArray(vpos_location_);
    opengl::check_errors("vpos - glEnableVertexAttribArray");

    glVertexAttribPointer(vcol_location_, 3, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 5, (void *) (sizeof(float) * 2));
    glEnableVertexAttribArray(vcol_location_);
    opengl::check_errors("vcol array setup");

    auto angle = z_angle_;
    auto m = glm::rotate(mat4(1.0), angle, vec3(0.0, 0.0, -1.0));
    auto mvp = mat4(1.0) * view_matrix * m;
    glUniformMatrix4fv(mvp_location_, 1, GL_FALSE, (const GLfloat *) glm::value_ptr(mvp));
    glDrawArrays(GL_TRIANGLES, 0, buffers.size());

}
