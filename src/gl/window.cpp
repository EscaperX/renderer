#include "window.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

namespace cc
{
    Window::Window(std::string const &title, Application *app, int w, int h) : width(w), height(h), ctx(w, h, title), app(app)
    {
    }
    Window::~Window()
    {
    }
    auto Window::mainloop() const -> void
    {
        bool show_demo_window;
        while (!glfwWindowShouldClose(ctx.handle()))
        {
            glfwPollEvents();

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
            if (show_demo_window)
                ImGui::ShowDemoWindow(&show_demo_window);
        }
    }
} // namespace cc
