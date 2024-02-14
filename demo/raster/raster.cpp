#include "raster.hpp"
#include "draw_line.hpp"
#include "clip.hpp"
#include "raster_method.hpp"

#include <geometry/AABB.h>
#include <geometry/triangle.hpp>

#include <gl/imgui/imgui.h>
#include <chrono>
#include <iostream>

namespace cc
{
    auto get_time() -> std::chrono::system_clock::time_point
    {
        auto tt = std::chrono::system_clock::now();
        return tt;
    }
    auto get_duration(std::chrono::system_clock::time_point const &t) -> float
    {
        auto tt = std::chrono::system_clock::now();
        return std::chrono::duration_cast<std::chrono::duration<float>>(tt - t).count();
    }

    auto Raster::instance() -> Raster &
    {
        static Raster _instance;
        return _instance;
    }
    auto Raster::initialize(math::Matrix4f const &model, math::Matrix4f const &view, math::Matrix4f const &project) -> void
    {
        drawcalls.clear();
        model_mat = model;
        view_mat = view;
        project_mat = project;
    }
    auto Raster::make_drawcall(DrawCallBuffer const &buf) -> void
    {
        drawcalls.push_back(buf);
    }
    auto Raster::clear_color(math::Vector3u8i const &v) -> void
    {
        color.clear(v);
    }
    auto Raster::clear_depth() -> void
    {
        depth.clear(FLT_MAX);
    }
    auto Raster::mutable_color() -> Buffer2D<math::Vector3u8i> &
    {
        return color;
    }
    auto Raster::get_color() const -> math::Vector3u8i const *
    {
        return color.data();
    }
    auto Raster::get_depth() const -> float const *
    {
        return depth.data();
    }
    auto Raster::set_render_mode(RasterMode mode) -> void
    {
        raster_mode = mode;
    }
    auto Raster::set_render_method(RasterMethod method) -> void
    {
        raster_method = method;
    }
    auto Raster::set_viewport(int w, int h) -> void
    {
        viewport_width = w;
        viewport_height = h;
        color.resize(w, h);
        depth.resize(w, h);
    }
    auto Raster::rasterize() -> void
    {

        statistics = ProfileStat{};
        std::chrono::system_clock::time_point _start;
        // transformation
        // Should be part of vertex shader
        float far = project_mat[2][3] / (project_mat[2][2] - 1);
        float near = project_mat[2][3] / (project_mat[2][2] + 1);
        auto mvp = project_mat * view_mat * model_mat;
        auto inv_trans = glm::transpose(glm::inverse(view_mat));

        if (enable_zcull)
        {
            if (hiez == nullptr)
            {
                hiez = std::make_unique<HieZbuffer>(viewport_width, viewport_height);
            }

            hiez->initialize(&last_frame_depth);
        }

        for (auto &dc : drawcalls)
        {

            // Vertex Shader
            _start = get_time();
            {
                vertex_buffer.clear();
                vertex_buffer.reserve(dc.positions.size());

                for (int i = 0; i < dc.positions.size(); i++)
                {
                    auto pos = dc.positions[i];
                    auto pos4 = mvp * math::Vector4f{pos, 1.0f};
                    auto normal = inv_trans * math::Vector4f{dc.normals[i], 0.0f};
                    vertex_buffer.emplace_back(pos4, normal.xyz(), dc.uvs[i]);
                }
            }
            statistics.vertex_stage_face_num += dc.indices.size() / 3;
            statistics.vertex_stage_vertex_num += vertex_buffer.size();
            statistics.vertex_stage_time += get_duration(_start);

            // Frustum culling & clipping
            _start = get_time();
            {
                index_buffer.clear();
                index_buffer.reserve(dc.indices.size());
                for (int i = 0; i < dc.indices.size(); i += 3)
                {
                    auto id_0 = dc.indices[i + 0];
                    auto id_1 = dc.indices[i + 1];
                    auto id_2 = dc.indices[i + 2];
                    auto vo_0 = vertex_buffer[id_0];
                    auto vo_1 = vertex_buffer[id_1];
                    auto vo_2 = vertex_buffer[id_2];

                    bool flag0 = inside_frustum(vo_0, near, far);
                    bool flag1 = inside_frustum(vo_1, near, far);
                    bool flag2 = inside_frustum(vo_2, near, far);

                    if (!flag0 && !flag1 && !flag2)
                        continue;

                    if (flag0 && flag1 && flag2)
                    {
                        index_buffer.push_back(id_0);
                        index_buffer.push_back(id_1);
                        index_buffer.push_back(id_2);
                        continue;
                    }

                    std::vector<Vertex> clipped_vertex = frustum_clipping(vo_0, vo_1, vo_2);
                    if (clipped_vertex.size() == 0)
                        continue;
                    int starter = vertex_buffer.size();
                    vertex_buffer.insert(vertex_buffer.end(), clipped_vertex.begin(), clipped_vertex.end());
                    for (int i = starter + 1; i < vertex_buffer.size() - 1; i++)
                    {
                        index_buffer.push_back(starter);
                        index_buffer.push_back(i);
                        index_buffer.push_back(i + 1);
                    }
                }

                // viewport transformation
                for (auto &v : vertex_buffer)
                {
                    auto w = v.position.w;
                    v.position /= w;
                    v.position.x = 0.5f * (viewport_width - 1.0f) * (v.position.x + 1.0f);
                    v.position.y = 0.5f * (viewport_height - 1.0f) * (v.position.y + 1.0f);
                }
            }
            statistics.frustum_culling_face_num += index_buffer.size() / 3;
            statistics.frustum_culling_vertex_num += vertex_buffer.size();
            statistics.frustum_culling_time += get_duration(_start);

            // Hie-Z culling or early z
            _start = get_time();
            if (enable_zcull)
            {
                auto temp_index_buffer = std::vector<uint32_t>();
                temp_index_buffer.reserve(index_buffer.size());

                for (int i = 0; i < index_buffer.size(); i += 3)
                {
                    auto id_0 = index_buffer[i + 0];
                    auto id_1 = index_buffer[i + 1];
                    auto id_2 = index_buffer[i + 2];
                    auto vo_0 = vertex_buffer[id_0];
                    auto vo_1 = vertex_buffer[id_1];
                    auto vo_2 = vertex_buffer[id_2];
                    if (hiez->is_hidden(AABB{vo_0.position, vo_1.position, vo_2.position}))
                        continue;
                    else
                    {
                        temp_index_buffer.push_back(id_0);
                        temp_index_buffer.push_back(id_1);
                        temp_index_buffer.push_back(id_2);
                    }
                }
                index_buffer = std::move(temp_index_buffer);
            }
            statistics.early_z_face_num += index_buffer.size() / 3;
            statistics.early_z_time += get_duration(_start);

            _start = get_time();
            if (raster_mode == RasterMode::Line)
            {

                for (int i = 0; i < index_buffer.size(); i += 3)
                {
                    auto id_0 = index_buffer[i + 0];
                    auto id_1 = index_buffer[i + 1];
                    auto id_2 = index_buffer[i + 2];
                    auto vo_0 = vertex_buffer[id_0];
                    auto vo_1 = vertex_buffer[id_1];
                    auto vo_2 = vertex_buffer[id_2];
                    auto p1 = vo_0.position.xyz();
                    auto p2 = vo_1.position.xyz();
                    auto p3 = vo_2.position.xyz();
                    draw_line(p1, p2, color);
                    draw_line(p1, p3, color);
                    draw_line(p3, p2, color);
                }
                statistics.shading_num += index_buffer.size();
                statistics.shading_time += get_duration(_start);
                continue;
            }
            if (raster_mode == RasterMode::None)
                continue;

            // Raster triangles
            fragment_buffer.clear();
            fragment_buffer.reserve(index_buffer.size());
            if (raster_method == RasterMethod::Groundtruth)
                fragment_buffer = ground_truth_raster(vertex_buffer, index_buffer);
            else if (raster_method == RasterMethod::Scanline)
                fragment_buffer = simple_scanline_raster(vertex_buffer, index_buffer, viewport_height, viewport_width);
            else if (raster_method == RasterMethod::AdvancedScanline)
                fragment_buffer = scanline_raster(vertex_buffer, index_buffer, viewport_height, viewport_width);
            statistics.raster_face_num += index_buffer.size() / 3;
            statistics.raster_fragment_num += fragment_buffer.size();
            statistics.raster_fragment_time += get_duration(_start);

            // Shading Stage
            _start = get_time();
            {
                for (auto &frag : fragment_buffer)
                {
                    if (frag.z < depth.at(frag.fragcoord.x, frag.fragcoord.y))
                    {
                        auto normal_color = (frag.normal + 1.0f) * 0.5f * 255.f;
                        color.at(frag.fragcoord.x, frag.fragcoord.y) = math::Vector3u8i{normal_color.x, normal_color.y, normal_color.z};
                        depth.at(frag.fragcoord.x, frag.fragcoord.y) = frag.z;
                    }
                }
            }
            statistics.shading_num += fragment_buffer.size();
            statistics.shading_time += get_duration(_start);
        }
        last_frame_depth = depth;
    }
}