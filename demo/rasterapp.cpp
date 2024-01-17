#include "rasterapp.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "gl/imgui/imgui.h"
#include "gl/imgui/imgui_impl_glfw.h"
#include "gl/imgui/imgui_impl_opengl3.h"

namespace cc
{
    RasterApp::RasterApp()
    {
    }
    RasterApp::~RasterApp() {}
    auto RasterApp::init() -> void
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &texture);
        glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureStorage2D(texture, 1, GL_RGB8, m_resolution_width, m_resolution_height);
    }
    auto RasterApp::update() -> void {}
    auto RasterApp::render() -> void
    {

        uint8_t *image = new uint8_t[m_resolution_width * m_resolution_height * 3];
        memset(image, 0, sizeof(image));
        for (int i = 10; i < 200; i++)
            for (int j = 10; j < 200; j++)
            {
                image[(j * m_resolution_width + i) * 3 + 0] = uint8_t(255);
                image[(j * m_resolution_width + i) * 3 + 1] = 0;
                image[(j * m_resolution_width + i) * 3 + 2] = 0;
            }

        glTextureSubImage2D(texture, 0, 0, 0, m_resolution_width, m_resolution_height, GL_RGB, GL_UNSIGNED_BYTE, image);
        delete[] image;
        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        static bool show_demo_window = true;
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        static bool renderWindowIsOpen = true;
        ImGui::Begin("Render", &renderWindowIsOpen, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);
        {
            ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Always);
            ImGui::Image(ImTextureID(texture), {float(m_viewport_width), float(m_viewport_height)}, ImVec2(0, 1), ImVec2(1, 0));
        }
        ImGui::End();

        ImGui::SetNextWindowPos({static_cast<float>(m_viewport_width) - 280.0f, 40.0f});
        if (ImGui::Begin("Example: Simple overlay", nullptr,
                         ImGuiWindowFlags_NoMove |
                             ImGuiWindowFlags_NoDecoration |
                             ImGuiWindowFlags_AlwaysAutoResize |
                             ImGuiWindowFlags_NoSavedSettings |
                             ImGuiWindowFlags_NoFocusOnAppearing |
                             ImGuiWindowFlags_NoNav |
                             ImGuiWindowFlags_NoScrollbar |
                             ImGuiWindowFlags_NoScrollWithMouse))
        {

            float cur_timestep = ImGui::GetTime();
            float frame_time = (cur_timestep - m_last_timestep) * 1000.0;
            ImGui::Text("Render Time: %.3lf ms, FPS: %.3lf", frame_time, 1000.0 / frame_time);
            m_last_timestep = cur_timestep;
            ImGui::End();
        }
    }
}