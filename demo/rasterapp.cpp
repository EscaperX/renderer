#include "rasterapp.hpp"
#include "raster/raster.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "gl/imgui/imgui.h"
#include "gl/imgui/imgui_impl_glfw.h"
#include "gl/imgui/imgui_impl_opengl3.h"

namespace cc
{
    std::vector<std::string> scene_list = {"cube", "plane", "bunny", "dragon", "house", "sponza"};

    RasterApp::RasterApp() : camera(m_resolution_width, m_resolution_height)
    {
        models.push_back(load_model("D:/renderer/asset/cube.obj"));
        // models.push_back(load_model("D:/renderer/asset/plane.obj"));
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
        raster.initialize(math::Matrix4f{1.0f}, camera.view(), camera.project());
        raster.set_viewport(m_resolution_width, m_resolution_height);

        for (auto model : models)
            for (auto &mesh : model->meshes)
            {
                raster.make_drawcall({std::span{mesh.positions.data(), mesh.positions.size()},
                                      std::span{mesh.normals.data(), mesh.normals.size()},
                                      std::span{mesh.uvs.data(), mesh.uvs.size()},
                                      std::span{mesh.indices.data(), mesh.indices.size()}});
            }

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
            if (ImGui::CollapsingHeader("Camera"))
            {
                if (ImGui::SliderFloat3("Position", reinterpret_cast<float *>(&camera.m_position), -100.f, 200.f, "%.3f", 0))
                {
                    camera.m_view_dirty = true;
                }
            }

            if (ImGui::CollapsingHeader("Render"))
            {
                ImGui::Text("Render Mode");
                ImGui::SameLine();
                static int mode = 1;
                ImGui::RadioButton("Line", &mode, 0);
                ImGui::SameLine();
                ImGui::RadioButton("Triangle", &mode, 1);
                ImGui::SameLine();
                Raster::instance().set_render_mode(Raster::RasterMode{mode});

                ImGui::Separator();

                ImGui::Text("Render Method");
                ImGui::SameLine();
                static int method = 0;
                ImGui::RadioButton("GroundTrue", &method, 0);
                ImGui::SameLine();
                ImGui::RadioButton("Scanline", &method, 1);
                ImGui::SameLine();
                Raster::instance().set_render_method(Raster::RasterMethod{method});

                ImGui::Separator();

                ImGui::Text("Early Z");
                ImGui::Checkbox("Enable:", &Raster::instance().enable_zcull);
            }
            if (ImGui::CollapsingHeader("Scene"))
            {
                if (ImGui::TreeNode("Scene"))
                {
                    static int selected = -1, prev = -1;
                    for (int n = 0; n < scene_list.size(); n++)
                    {
                        // char buf[32];
                        // sprintf(buf, "Object %d", n);
                        if (ImGui::Selectable(scene_list[n].c_str(), selected == n))
                            selected = n;
                    }
                    if (prev != selected && selected != -1)
                    {
                        prev = selected;
                        models.clear();
                        std::string path = "D:/renderer/asset/" + scene_list[selected] + ".obj";
                        models.push_back(load_model(path.c_str()));
                    }
                    ImGui::TreePop();
                }
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

        ImGui::SetNextWindowPos({static_cast<float>(m_viewport_width) - 480.0f, 80.0f});
        if (ImGui::Begin("Stat", nullptr,
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
            ImGui::Text("\t Faces    :%d\n", [&]() -> int
                        {int cnt= 0; for (auto& model: models) cnt += model->face_cnt; return cnt; }());
            // ImGui::Text("\t Vertices :%d\n", model->vertex_cnt);
            // ImGui::Text("\t Submeshes:%d\n", model->submesh_cnt);
            auto stat = Raster::instance().statistics;
            ImGui::Text("Pipeline Statistics:\n");
            ImGui::Separator();
            ImGui::Text("\t Vertex Stage :  Vertex: %d Face: %d Time: %.5f\n", stat.vertex_stage_vertex_num, stat.vertex_stage_face_num, stat.vertex_stage_time * 1000);
            ImGui::Separator();
            ImGui::Text("\t Frustum Stage: Vertex: %d Face: %d Time: %.5f\n", stat.frustum_culling_vertex_num, stat.frustum_culling_face_num, stat.frustum_culling_time * 1000);

            if (Raster::instance().enable_zcull)
            {
                ImGui::Separator();
                ImGui::Text("\t Z Cull Stage : Face: %d Time: %.5f\n", stat.early_z_face_num, stat.early_z_time * 1000);
            }
            ImGui::Separator();
            ImGui::Text("\t Raster Stage : Face: %d Fragment: %d Time: %.5f\n", stat.raster_face_num, stat.raster_fragment_num, stat.raster_fragment_time * 1000);
            ImGui::Separator();
            ImGui::Text("\t Shading Stage: Shading: %d Time: %.5f\n", stat.shading_num, stat.shading_time * 1000);
            ImGui::End();
        }
    }
}