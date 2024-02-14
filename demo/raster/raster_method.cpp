#include "raster_method.hpp"
#include <geometry/triangle.hpp>
#include <array>
#include <stack>
#include <iostream>
#include <algorithm>
#include <unordered_map>

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

    auto create_edge(math::Vector4f const &v0, math::Vector4f const &v1, math::Vector4f const &v2, uint32_t index, std::vector<std::vector<ActiveEdge>> &edge_list) -> void
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

        auto edge_constructor = [&](int l, int r, bool flag) -> ActiveEdge
        {
            return ActiveEdge{
                int(points[l].x),
                int(points[l].y),
                int(points[r].x),
                int(points[r].y),
                points[l].z,
                (c == 0 ? 0 : -a / c), // dzdx
                (c == 0 ? 0 : -b / c), // dzdy
                flag,
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

            auto edge1 = edge_constructor(0, 2, true);
            auto edge2 = edge_constructor(1, 2, false);

            edge_list[edge1.ys].push_back(edge1);
            edge_list[edge2.ys].push_back(edge2);
        }
        // \/
        else if (points[1].y == points[2].y)
        {
            if (points[1].x > points[2].x)
                std::swap(points[1], points[2]);

            auto edge1 = edge_constructor(0, 1, true);
            auto edge2 = edge_constructor(0, 2, false);

            edge_list[edge1.ys].push_back(edge1);
            edge_list[edge2.ys].push_back(edge2);
        }
        else
        {
            if (c > 0)
            {
                auto edge1 = edge_constructor(0, 2, true);
                auto edge2 = edge_constructor(0, 1, false);
                auto edge3 = edge_constructor(1, 2, false);
                edge3.next_line();
                edge_list[edge1.ys].push_back(edge1);
                edge_list[edge2.ys].push_back(edge2);
                if (!edge3.terminate())
                    edge_list[edge3.ys].push_back(edge3);
            }
            else
            {
                auto edge1 = edge_constructor(0, 1, true);
                auto edge2 = edge_constructor(1, 2, true);
                edge2.next_line();
                auto edge3 = edge_constructor(0, 2, false);
                edge_list[edge1.ys].push_back(edge1);
                edge_list[edge2.ys].push_back(edge2);
                edge_list[edge3.ys].push_back(edge3);
            }
        }
    }

    auto simple_scanline_raster(std::vector<Vertex> const &vertex_buffer, std::vector<uint32_t> const &index_buffer, int height, int width) -> std::vector<Fragment>
    {
        auto edges = std::vector<std::vector<ActiveEdge>>{size_t(height), std::vector<ActiveEdge>{}};
        std::vector<ActiveEdge> active_edges;
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
            std::sort(active_edges.begin(), active_edges.end(), [](ActiveEdge const &a, ActiveEdge const &b)
                      { if (a.tri_id == b.tri_id) return a.in;
                        return a.tri_id < b.tri_id; });
            for (int i = 0; i + 1 < active_edges.size(); i++)
                if (active_edges[i].in)
                {
                    int j = i + 1;
                    if (active_edges[i].tri_id == active_edges[j].tri_id)
                    {
                        auto &cur_edge1 = active_edges[i];
                        auto &cur_edge2 = active_edges[j];
                        float z = cur_edge1.z;
                        for (int x = cur_edge1.xs; x <= cur_edge2.xs; x += 1, z += cur_edge1.dzdx)
                        {
                            auto id = index_buffer[cur_edge1.tri_id];
                            res.emplace_back(math::Vector2i{x, cur_y}, z, vertex_buffer[id].normal, vertex_buffer[id].uv);
                        }
                    }
                }

            for (auto &edge : active_edges)
            {
                edge.next_line();
            }
            std::erase_if(active_edges, [](ActiveEdge e)
                          { return e.terminate(); });
        }
        return res;
    }

    auto scanline_raster(std::vector<Vertex> const &vertex_buffer, std::vector<uint32_t> const &index_buffer, int height, int width) -> std::vector<Fragment>
    {
        auto edges = std::vector<std::vector<ActiveEdge>>{size_t(height), std::vector<ActiveEdge>{}};
        std::vector<ActiveEdge> active_edges;
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

            std::sort(active_edges.begin(), active_edges.end(), [](ActiveEdge const &a, ActiveEdge const &b) -> bool
                      { if (a.xs == b.xs && a.z == b.z) return a.dzdx < b.dzdx;
                        if (a.xs == b.xs) return a.z < b.z; 
                        return a.xs < b.xs; });

            std::stack<ActiveEdge> candidate_edges;
            int last_x = -1;
            auto has_out = std::unordered_map<uint32_t, bool>{};
            for (int i = 0; i < active_edges.size(); i++)
            {
                // insert a edge;
                auto &cur_edge = active_edges[i];
                if (cur_edge.in)
                {
                    if (candidate_edges.empty())
                    {
                        if (last_x == -1)
                            last_x = cur_edge.xs;
                        candidate_edges.push(cur_edge);
                    }
                    else
                    {
                        auto &top_edge = candidate_edges.top();
                        while (has_out.count(top_edge.tri_id))
                        {
                            candidate_edges.pop();
                            top_edge = candidate_edges.top();
                        }
                        auto delta_x = cur_edge.xs - top_edge.xs;
                        if (top_edge.z + delta_x * top_edge.dzdx <= cur_edge.z)
                        {
                            auto cur_z = top_edge.z + (last_x - top_edge.xs) * top_edge.dzdx;
                            for (int x = last_x; x <= cur_edge.xs; x++)
                            {
                                auto &triangle = vertex_buffer[index_buffer[top_edge.tri_id]];
                                res.emplace_back(math::Vector2i{x, cur_y}, cur_z, triangle.normal, triangle.uv);
                                cur_z += top_edge.dzdx;
                            }
                            last_x = cur_edge.xs;
                        }
                        candidate_edges.push(cur_edge);
                    }
                }
                else
                {
                    auto &top_edge = candidate_edges.top();
                    if (top_edge.tri_id == cur_edge.tri_id)
                    {
                        auto cur_z = top_edge.z + (last_x - top_edge.xs) * top_edge.dzdx;
                        for (int x = last_x; x <= cur_edge.xs; x++)
                        {
                            auto &triangle = vertex_buffer[index_buffer[top_edge.tri_id]];
                            res.emplace_back(math::Vector2i{x, cur_y}, cur_z, triangle.normal, triangle.uv);
                            cur_z += top_edge.dzdx;
                        }
                        last_x = cur_edge.xs;
                    }
                    else
                        has_out[cur_edge.tri_id] = true;
                }
            }

            for (auto &edge : active_edges)
                edge.next_line();
            std::erase_if(active_edges, [](ActiveEdge e)
                          { return e.terminate(); });
        }
        return res;
    }

} // namespace cc
