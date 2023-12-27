#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

namespace cc
{
    class GLContext final
    {
    public:
        GLContext(int w, int h, const std::string &title);
        ~GLContext();

        GLFWwindow *handle() const;

    private:
        GLFWwindow *m_window{};
    };
}