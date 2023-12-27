#include "gl-context.hpp"
#include <stdexcept>

namespace cc
{

    GLContext::GLContext(int w, int h, const std::string &title)
    {
        if (glfwInit() != GLFW_TRUE)
        {
            throw std::runtime_error("init glfw failure");
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        m_window = glfwCreateWindow(w, h, title.c_str(), nullptr, nullptr);
        if (m_window == nullptr)
        {
            throw std::runtime_error("create glfw window failure");
        }
        glfwMakeContextCurrent(m_window);
        glfwSetWindowUserPointer(m_window, this);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            throw std::runtime_error("initialize glad failure");
        }
        // TODO: Add information log for opengl
    }
    GLContext::~GLContext()
    {
        glfwTerminate();
    }

    GLFWwindow *GLContext::handle() const
    {
        return m_window;
    }
}