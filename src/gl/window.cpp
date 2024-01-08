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
        ImGui_ImplGlfw_Shutdown();
    }
    auto Window::mainloop() const -> void
    {
        bool show_demo_window = true;

        glfwMakeContextCurrent(ctx.handle());
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(ctx.handle(), true);

        ImGui_ImplOpenGL3_Init();
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        GLuint texture;
        glCreateTextures(GL_TEXTURE_2D, 1, &texture);
        uint8_t *image = new uint8_t[width * height * 3];
        for (int i = 0; i < width; i++)
            for (int j = 0; j < height; j++)
            {
                image[(j * width + i) * 3 + 0] = uint8_t(255);
                image[(j * width + i) * 3 + 1] = 0;
                image[(j * width + i) * 3 + 2] = 0;
            }
        glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureStorage2D(texture, 1, GL_RGB8, width, height);
        glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, image);
        // glGenerateTextureMipmap(texture);

        while (!glfwWindowShouldClose(ctx.handle()))
        {
            glfwPollEvents();

            int display_w, display_h;
            glfwGetFramebufferSize(ctx.handle(), &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT);

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
            if (show_demo_window)
                ImGui::ShowDemoWindow(&show_demo_window);

            static bool renderWindowIsOpen = true;
            ImGui::Begin("Render", &renderWindowIsOpen, ImGuiWindowFlags_AlwaysAutoResize);
            {
                ImGui::Image(ImTextureID(texture), {float(display_w), float(display_h)}, ImVec2(0, 1), ImVec2(1, 0));
            }
            ImGui::End();
            ImGui::EndFrame();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(ctx.handle());
        }
    }
} // namespace cc
