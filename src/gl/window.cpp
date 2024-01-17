#include "window.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

namespace cc
{
    Window::Window(std::string const &title, Application *app, int w, int h) : width(w), height(h), ctx(w, h, title), app(app)
    {
        app->init();
    }
    Window::~Window()
    {
        ImGui_ImplGlfw_Shutdown();
    }
    auto Window::mainloop() -> void
    {
        bool show_demo_window = true;

        glfwMakeContextCurrent(ctx.handle());
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(ctx.handle(), true);

        ImGui_ImplOpenGL3_Init();
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        // glGenerateTextureMipmap(texture);
        uint8_t cnt = 0;
        while (!glfwWindowShouldClose(ctx.handle()))
        {
            glfwPollEvents();

            int display_w, display_h;
            glfwGetFramebufferSize(ctx.handle(), &display_w, &display_h);
            app->set_viewport_height(display_h);
            app->set_viewport_width(display_w);

            glViewport(0, 0, display_w, display_h);
            glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT);

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            app->render();

            ImGui::EndFrame();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(ctx.handle());
        }
    }
} // namespace cc
