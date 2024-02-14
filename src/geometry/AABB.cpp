#include "AABB.h"
#include <algorithm>
namespace cc
{
    AABB::AABB(float xs, float xm, float ys, float ym, float zs, float zm)
    {
        x[0] = xs, x[1] = xm;
        y[0] = ys, y[1] = ym;
        z[0] = zs, z[1] = zm;
    }
    AABB::AABB(math::Vector3f const &p1, math::Vector3f const &p2)
    {
        x[0] = std::min(p1.x, p2.x);
        y[0] = std::min(p1.y, p2.y);
        z[0] = std::min(p1.z, p2.z);
        x[1] = std::max(p1.x, p2.x);
        y[1] = std::max(p1.y, p2.y);
        z[1] = std::max(p1.z, p2.z);
    }
    AABB::AABB(math::Vector3f const &a, math::Vector3f const &b, math::Vector3f const &c)
    {
        x[1] = y[1] = z[1] = -FLT_MAX;
        x[0] = y[0] = z[0] = FLT_MAX;
        auto merge_op = [&](math::Vector3f const &vec) -> void
        {
            x[0] = std::min(x[0], vec.x);
            x[1] = std::max(x[1], vec.x);
            y[0] = std::min(y[0], vec.y);
            y[1] = std::max(y[1], vec.y);
            z[0] = std::min(z[0], vec.z);
            z[1] = std::max(z[1], vec.z);
        };
        merge_op(a);
        merge_op(b);
        merge_op(c);
    }
    AABB::AABB(math::Vector4f const &a, math::Vector4f const &b, math::Vector4f const &c)
    {
        x[1] = y[1] = z[1] = -FLT_MAX;
        x[0] = y[0] = z[0] = FLT_MAX;
        auto merge_op = [&](math::Vector4f const &vec) -> void
        {
            x[0] = std::min(x[0], vec.x);
            x[1] = std::max(x[1], vec.x);
            y[0] = std::min(y[0], vec.y);
            y[1] = std::max(y[1], vec.y);
            z[0] = std::min(z[0], vec.z);
            z[1] = std::max(z[1], vec.z);
        };
        merge_op(a);
        merge_op(b);
        merge_op(c);
    }
    bool AABB::inside(const math::Vector3f &p) const
    {
        return p.x <= x[1] && p.x >= x[0] && p.y <= y[1] && p.y >= y[0] && p.z <= z[1] && p.z >= z[0];
    }
    math::Vector3f AABB::get_center() const
    {
        return math::Vector3f{x[0] + x[1], y[0] + y[1], z[0] + z[1]} * 0.5f;
    }
    math::Vector3f AABB::get_extent() const
    {
        return math::Vector3f{x[1] - x[0], y[1] - y[0], z[1] - z[0]} * 0.5f;
    }
    std::array<math::Vector3f, 8> AABB::get_vertices() const
    {
        std::array<math::Vector3f, 8> res;
        int cnt = 0;
        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 2; j++)
                for (int k = 0; k < 2; k++)
                    res[cnt++] = {x[i], y[j], z[k]};
        return res;
    }
    bool AABB::intersect(const AABB &box) const
    {
        if (box.x[1] < x[0] || box.x[0] > x[1])
            return false;
        if (box.y[1] < y[0] || box.y[0] > y[1])
            return false;
        return true;
    }
    bool AABB::inside(math::Vector3f const &a, math::Vector3f const &b, math::Vector3f const &c) const
    {
        if (!inside(a) || !inside(b) || !inside(c))
            return false;
        return true;
    }
    bool AABB::inside(AABB const &bbx) const
    {
        auto verts = bbx.get_vertices();
        bool flag = true;
        for (auto const &vert : verts)
        {
            if (!inside(vert))
            {
                flag = false;
                break;
            }
        }
        return flag;
    }

    auto AABB::transform(math::Matrix4f const &mat) -> void
    {
        auto center = get_center();
        auto extent = get_extent();

        auto temp_res = (mat * math::Vector4f{center, 1.0f});
        auto new_center = math::Vector3f{temp_res};

        // basis from transform matrix
        auto new_axis_x = math::Vector3f{mat[0]};
        auto new_axis_y = math::Vector3f{mat[1]};
        auto new_axis_z = math::Vector3f{mat[2]};

        // scaled extent vector, not axis aligned
        auto scaled_extent_x = new_axis_x * extent.x;
        auto scaled_extent_y = new_axis_y * extent.y;
        auto scaled_extent_z = new_axis_z * extent.z;

        auto aligned_new_extent = math::Vector3f{};
        aligned_new_extent.x = abs(scaled_extent_x.x) + abs(scaled_extent_y.x) + abs(scaled_extent_z.x);
        aligned_new_extent.y = abs(scaled_extent_x.y) + abs(scaled_extent_y.y) + abs(scaled_extent_z.y);
        aligned_new_extent.z = abs(scaled_extent_x.z) + abs(scaled_extent_y.z) + abs(scaled_extent_z.z);

        auto new_pm = new_center - aligned_new_extent;
        auto new_px = new_center + aligned_new_extent;
        x[0] = new_pm.x;
        x[1] = new_px.x;
        y[0] = new_pm.y;
        y[1] = new_px.y;
        z[0] = new_pm.z;
        z[1] = new_px.z;
    }
}