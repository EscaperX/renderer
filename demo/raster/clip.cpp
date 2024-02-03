#include "clip.hpp"

namespace cc
{
    const std::vector<math::Vector4f> view_planes = {
        // Near
        math::Vector4f(0, 0, 1, 1),
        // far
        math::Vector4f(0, 0, -1, 1),
        // left
        math::Vector4f(1, 0, 0, 1),
        // top
        math::Vector4f(0, 1, 0, 1),
        // right
        math::Vector4f(-1, 0, 0, 1),
        // bottom
        math::Vector4f(0, -1, 0, 1)};
    auto intersect_plane(Vertex const &v1, Vertex const &v2, glm::vec4 const &plane_normal) -> Vertex
    {
        float da = glm::dot(v1.position, plane_normal);
        float db = glm::dot(v2.position, plane_normal);

        float weight = da / (da - db);
        auto new_p = v1.position * (1 - weight) + v2.position * weight;
        auto new_n = v1.normal * (1 - weight) + v2.normal * weight;
        auto new_uv = v1.uv * (1 - weight) + v2.uv * weight;
        return {new_p, new_n, new_uv};
    }
    auto inside_plane(math::Vector4f const &plane_normal, math::Vector4f const &point) -> bool
    {
        if (glm::dot(plane_normal, point) >= -1e-5)
            return true;
        return false;
    }
    auto inside_frustum(Vertex const &v0, float near, float far) -> bool
    {
        return (near <= v0.position.w) && (v0.position.w <= far) &&
               fabs(v0.position.x) <= fabs(v0.position.w) && fabs(v0.position.y) <= fabs(v0.position.w);
    }
    auto frustum_clipping(Vertex const &v0, Vertex const &v1, Vertex const &v2) -> std::vector<Vertex>
    {
        std::vector<Vertex> output = {v0, v1, v2};
        for (auto &plane : view_planes)
        {
            std::vector<Vertex> input(output);
            output.clear();
            for (int i = 0; i < input.size(); i++)
            {
                auto current = input[i];
                auto last = i == 0 ? input.back() : input[i - 1];
                if (inside_plane(plane, current.position))
                {
                    if (!inside_plane(plane, last.position))
                    {
                        auto edge_point = intersect_plane(last, current, plane);
                        output.push_back(edge_point);
                    }
                    output.push_back(current);
                }
                else if (inside_plane(plane, last.position))
                {
                    auto edge_point = intersect_plane(last, current, plane);
                    output.push_back(edge_point);
                }
            }
        }
        return output;
    }
}