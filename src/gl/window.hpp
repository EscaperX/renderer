#pragma once
#include "gl-context.hpp"
#include "application.hpp"

namespace cc
{
    class Window final
    {
    public:
        Window(std::string const &title = "Test", Application *app = nullptr, int w = 1024, int h = 1024);
        ~Window();
        auto mainloop() -> void;

    private:
        int width, height;
        GLContext ctx;
        Application *app;
    };
}