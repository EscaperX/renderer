#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace opengl_wrapper
{
    auto create_quad_vao() -> GLuint;
    auto create_quad_program() -> GLuint;
}