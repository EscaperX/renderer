#include "raster.hpp"
#include <iostream>
#include "draw_line.hpp"
#include <math/AABB.h>
#include <geometry/triangle.hpp>
namespace cc
{
    auto Raster::instance() -> Raster &
    {
        static Raster _instance;
        return _instance;
    }
    auto Raster::initialize(ModelPtr ptr, math::Matrix4f const &view, math::Matrix4f const &project) -> void
    {
        model = ptr;
        this->view = view;
        this->project = project;
    }
    auto Raster::clear_color(math::Vector3u8i const &v) -> void
    {
        color.clear(v);
    }
    auto Raster::clear_depth() -> void
    {
        depth.clear();
    }
    auto Raster::get_color() const -> math::Vector3u8i const *
    {
        return color.data();
    }
    auto Raster::get_depth() const -> double const *
    {
        return depth.data();
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

        // transformation
        // Should be part of vertex shader
        auto mvp = project * view;
        auto inv_trans = glm::transpose(glm::inverse(view));

        for (auto &mesh : model->meshes)
        {

            vertex_buffer.reserve(mesh.positions.size());
            vertex_buffer.clear();

            for (int i = 0; i < mesh.positions.size(); i++)
            {
                auto pos = mesh.positions[i];
                auto pos4 = mvp * math::Vector4f{pos, 1.0f};
                auto w = pos4.w;
                pos4 /= pos4.w;
                pos4.x = 0.5 * viewport_width * (pos4.x + 1.0f);
                pos4.y = 0.5 * viewport_height * (pos4.y + 1.0f);
                auto normal = inv_trans * math::Vector4f{mesh.normals[i], 0.0f};
                printf("%d x: %f y: %f z: %f origin z: %f w: %f\n", i, pos4.x, pos4.y, pos4.z, pos.z, w);
                vertex_buffer.emplace_back(pos4.xyz(), normal.xyz(), mesh.uvs[i], w);
            }
            puts("");
            // Raster triangles
            for (int i = 0; i < mesh.indices.size(); i += 3)
            {
                auto id_0 = mesh.indices[i + 0];
                auto id_1 = mesh.indices[i + 1];
                auto id_2 = mesh.indices[i + 2];
                auto vo_0 = vertex_buffer[id_0];
                auto vo_1 = vertex_buffer[id_1];
                auto vo_2 = vertex_buffer[id_2];
                int minx = std::min(vo_0.position.x, std::min(vo_1.position.x, vo_2.position.x));
                int maxx = std::max(vo_0.position.x, std::max(vo_1.position.x, vo_2.position.x));
                int miny = std::min(vo_0.position.y, std::min(vo_1.position.y, vo_2.position.y));
                int maxy = std::max(vo_0.position.y, std::max(vo_1.position.y, vo_2.position.y));

                for (int x = std::max(0, minx); x <= std::min(viewport_height - 1, maxx); x++)
                    for (int y = std::max(0, miny); y <= std::min(viewport_height - 1, maxy); y++)
                    {
                        if (inside_triangle_2D(x, y, vo_0.position, vo_1.position, vo_2.position))
                        {
                            color.at(x, y) = {255, 255, 255};
                        }
                    }
            }
        }
    }
}