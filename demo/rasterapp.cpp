#include "rasterapp.hpp"
#include "raster/raster.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "gl/imgui/imgui.h"
#include "gl/imgui/imgui_impl_glfw.h"
#include "gl/imgui/imgui_impl_opengl3.h"

namespace cc
{
    RasterApp::RasterApp() : camera(m_resolution_width, m_resolution_height)
    {
        model = load_model("D:/renderer/asset/cube.obj");
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
    auto RasterApp::update() -> void
    {
        auto &raster = Raster::instance();
        raster.clear_color();
        raster.clear_depth();
        raster.initialize(model, camera.view(), camera.project());
        raster.set_viewport(m_resolution_width, m_resolution_height);
        raster.rasterize();
    }
    auto RasterApp::render() -> void
    {
        auto &raster = Raster::instance();
        glTextureSubImage2D(texture, 0, 0, 0, m_resolution_width, m_resolution_height, GL_RGB, GL_UNSIGNED_BYTE, reinterpret_cast<uint8_t const *>(raster.get_color()));

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

        static bool config_window = true;
        ImGui::Begin("Config", &config_window);
        {
            if (ImGui::SliderFloat3("Position", reinterpret_cast<float *>(&camera.m_position), -100.f, 100.f))
            {
                camera.m_view_dirty = true;
            }
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

        ImGui::SetNextWindowPos({static_cast<float>(m_viewport_width) - 280.0f, 80.0f});
        if (ImGui::Begin("Model Stat", nullptr,
                         ImGuiWindowFlags_NoMove |
                             ImGuiWindowFlags_NoDecoration |
                             ImGuiWindowFlags_AlwaysAutoResize |
                             ImGuiWindowFlags_NoSavedSettings |
                             ImGuiWindowFlags_NoFocusOnAppearing |
                             ImGuiWindowFlags_NoNav |
                             ImGuiWindowFlags_NoScrollbar |
                             ImGuiWindowFlags_NoScrollWithMouse))
        {

            ImGui::Text("Model Stat:\n");
            ImGui::Text("\t Faces    :%d\n", model->face_cnt);
            ImGui::Text("\t Vertices :%d\n", model->vertex_cnt);
            ImGui::Text("\t Submeshes:%d\n", model->submesh_cnt);
            ImGui::End();
        }
    }
}