#include "raster_method.hpp"
#include <geometry/triangle.hpp>
#include <array>
#include <stack>
#include <iostream>
#include <algorithm>
namespace cc
{
    const float eps = 1e-5;
    auto ground_truth_raster(std::vector<Vertex> const &vertex_buffer, std::vector<uint32_t> const &index_buffer) -> std::vector<Fragment>
    {
        std::vector<Fragment> res;
        for (int i = 0; i < index_buffer.size(); i += 3)
        {
            auto id_0 = index_buffer[i + 0];
            auto id_1 = index_buffer[i + 1];
            auto id_2 = index_buffer[i + 2];
            auto vo_0 = vertex_buffer[id_0];
            auto vo_1 = vertex_buffer[id_1];
            auto vo_2 = vertex_buffer[id_2];
            int minx = (int)std::min(vo_0.position.x, std::min(vo_1.position.x, vo_2.position.x));
            int maxx = (int)std::max(vo_0.position.x, std::max(vo_1.position.x, vo_2.position.x));
            int miny = (int)std::min(vo_0.position.y, std::min(vo_1.position.y, vo_2.position.y));
            int maxy = (int)std::max(vo_0.position.y, std::max(vo_1.position.y, vo_2.position.y));
            auto p1 = vo_0.position.xyz();
            auto p2 = vo_1.position.xyz();
            auto p3 = vo_2.position.xyz();
            for (int x = minx; x <= maxx; x++)
                for (int y = miny; y <= maxy; y++)
                {
                    if (inside_triangle_2D(x, y, p1, p2, p3))
                    {
                        float w[3] = {vo_0.position.w, vo_1.position.w, vo_2.position.w};
                        float z[3] = {p1.z, p2.z, p3.z};
                        auto [alpha, beta, gamma] = compute_barycentric_2D(x + 0.5f, y + 0.5f, p1, p2, p3);
                        auto interpolated_normal = alpha * vo_0.normal + beta * vo_1.normal + gamma * vo_2.normal;
                        auto interpolated_uv = alpha * vo_0.uv + beta * vo_1.uv + gamma * vo_2.uv;
                        float Z = 1.0f / (alpha / w[0] + beta / w[1] + gamma / w[2]);
                        float zp = alpha * z[0] / w[0] + beta * z[1] / w[1] + gamma * z[2] / w[2];
                        zp *= Z;
                        res.emplace_back(math::Vector2i{x, y}, zp, interpolated_normal, interpolated_uv);
                    }
                }
        }
        return std::move(res);
    }

    auto create_edge(math::Vector4f const &v0, math::Vector4f const &v1, math::Vector4f const &v2, uint32_t index, std::vector<std::vector<ActiveEdgePair>> &edge_list) -> void
    {
        auto points = std::array<math::Vector4f, 3>{v0, v1, v2};

        // from bottom to top: 0, 1, 2
        if (points[1].y < points[0].y && points[1].y <= points[2].y)
            std::swap(points[0], points[1]);
        else if (points[2].y < points[0].y && points[2].y <= points[1].y)
            std::swap(points[0], points[2]);
        if (points[1].y > points[2].y)
            std::swap(points[1], points[2]);

        auto a = ((points[1].y - points[0].y) * (points[2].z - points[0].z) - (points[1].z - points[0].z) * (points[2].y - points[0].y));
        auto b = ((points[1].z - points[0].z) * (points[2].x - points[0].x) - (points[1].x - points[0].x) * (points[2].z - points[0].z));
        auto c = ((points[1].x - points[0].x) * (points[2].y - points[0].y) - (points[1].y - points[0].y) * (points[2].x - points[0].x));
        auto d = (0 - (a * points[0].x + b * points[0].y + c * points[0].z));

        auto edge_constructor = [&](int startl, int endl, int startr, int endr) -> ActiveEdgePair
        {
            return ActiveEdgePair{
                points[startl].x,
                points[startl].z,
                ((points[endl].y - points[startl].y < eps) ? 0 : (points[endl].x - points[startl].x) / (points[endl].y - points[startl].y)), // dxdy
                points[startr].x,
                points[startr].z,
                ((points[endr].y - points[startr].y < eps) ? 0 : (points[endr].x - points[startr].x) / (points[endr].y - points[startr].y)), // dxdy

                int(points[startl].y),                  // start_y
                int(points[endl].y - points[startl].y), // dy_remain
                (c == 0 ? 0 : -a / c),                  // dzdx
                (c == 0 ? 0 : -b / c),                  // dzdy
                index};
        };

        // culling single line
        if (c == 0)
            return;

        // if (c > 0) left edge is across the whole space
        //
        if (points[1].y == points[0].y)
        {
            if (points[0].x > points[1].x)
                std::swap(points[0], points[1]);

            auto edge = edge_constructor(0, 2, 1, 2);

            edge_list[edge.start_y].push_back(edge);
        }
        // \/
        else if (points[1].y == points[2].y)
        {
            if (points[1].x > points[2].x)
                std::swap(points[1], points[2]);

            auto edge = edge_constructor(0, 1, 0, 2);

            edge_list[edge.start_y].push_back(edge);
        }
        else
        {
            auto edges = std::array<ActiveEdgePair, 2>{};
            if (c > 0)
            {
                edges[0] = edge_constructor(0, 2, 0, 1);
                edges[0].dy_remain = int(points[1].y - points[0].y);

                edges[1] = edge_constructor(0, 2, 1, 2);
                auto delta_x = edges[0].dy_remain * edges[1].dxdyl;
                edges[1].start_y = int(points[1].y);
                edges[1].dy_remain = int(points[2].y - points[1].y);
                edges[1].xl += delta_x;
                edges[1].zl += delta_x * edges[1].dzdx + edges[0].dy_remain * edges[1].dzdy;
            }
            else
            {
                edges[0] = edge_constructor(0, 1, 0, 2);
                edges[0].dy_remain = int(points[1].y - points[0].y);

                edges[1] = edge_constructor(1, 2, 0, 2);
                auto delta_x = edges[0].dy_remain * edges[1].dxdyr;
                edges[1].start_y = int(points[1].y);
                edges[1].dy_remain = int(points[2].y - points[1].y);
                edges[1].xr += delta_x;
                edges[1].zr += delta_x * edges[1].dzdx + edges[0].dy_remain * edges[1].dzdy;
            }
            edge_list[edges[0].start_y].push_back(edges[0]);
            edge_list[edges[1].start_y].push_back(edges[1]);
        }
    }

    auto simple_scanline_raster(std::vector<Vertex> const &vertex_buffer, std::vector<uint32_t> const &index_buffer, int height, int width) -> std::vector<Fragment>
    {
        auto edges = std::vector<std::vector<ActiveEdgePair>>{size_t(height), std::vector<ActiveEdgePair>{}};
        std::vector<ActiveEdgePair> active_edges;
        std::vector<Fragment> res;
        for (int i = 0; i < index_buffer.size(); i += 3)
        {
            auto id_0 = index_buffer[i + 0];
            auto id_1 = index_buffer[i + 1];
            auto id_2 = index_buffer[i + 2];
            auto vo_0 = vertex_buffer[id_0];
            auto vo_1 = vertex_buffer[id_1];
            auto vo_2 = vertex_buffer[id_2];
            create_edge(vo_0.position, vo_1.position, vo_2.position, i, edges);
        }
        for (int cur_y = 0; cur_y < height; cur_y++)
        {
            for (auto &edge : edges[cur_y])
                active_edges.push_back(edge);
            for (int i = 0; i < active_edges.size(); i++)
            {
                auto &cur_edge = active_edges[i];
                float z = cur_edge.zl;
                for (float x = cur_edge.xl; x <= cur_edge.xr; x += 1, z += cur_edge.dzdx)
                {
                    auto id = index_buffer[cur_edge.tri_id];
                    res.emplace_back(math::Vector2i{int(x), cur_y}, z, vertex_buffer[id].normal, vertex_buffer[id].uv);
                }
            }

            for (auto &edge : active_edges)
            {
                edge.next_line();
            }
            std::erase_if(active_edges, [](ActiveEdgePair e)
                          { return e.terminate(); });
        }
        return res;
    }

    auto scanline_raster(std::vector<Vertex> const &vertex_buffer, std::vector<uint32_t> const &index_buffer, int height, int width) -> std::vector<Fragment>
    {
        auto edges = std::vector<std::vector<ActiveEdgePair>>{size_t(height), std::vector<ActiveEdgePair>{}};
        std::vector<ActiveEdgePair> active_edges;
        std::vector<Fragment> res;
        for (int i = 0; i < index_buffer.size(); i += 3)
        {
            auto id_0 = index_buffer[i + 0];
            auto id_1 = index_buffer[i + 1];
            auto id_2 = index_buffer[i + 2];
            auto vo_0 = vertex_buffer[id_0];
            auto vo_1 = vertex_buffer[id_1];
            auto vo_2 = vertex_buffer[id_2];
            create_edge(vo_0.position, vo_1.position, vo_2.position, i, edges);
        }
        for (int cur_y = 0; cur_y < height; cur_y++)
        {
            for (auto &edge : edges[cur_y])
                active_edges.push_back(edge);

            std::sort(active_edges.begin(), active_edges.end(), [](ActiveEdgePair const &a, ActiveEdgePair const &b) -> bool
                      { if (a.xl < b.xl) return true; 
                        if (a.xl == b.xl && a.zl == b.zl) return a.dzdx < b.dzdx;
                        if (a.xl == b.xl) return a.zl < b.zl; });

            std::stack<ActiveEdgePair> candidate_edge;
            float last_x = 0;

            for (int i = 0; i < active_edges.size(); i++)
            {
                // insert a edge;
                auto &cur_edge = active_edges[i];
                        }

            for (auto &edge : active_edges)
                edge.next_line();
            std::erase_if(active_edges, [](ActiveEdgePair e)
                          { return e.terminate(); });
        }
        return res;
    }

} // namespace cc
