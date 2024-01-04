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
    AABB::AABB(math::Vector3f p1, math::Vector3f p2)
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
    bool AABB::inside(const math::Vector3f &p) const
    {
        return p.x <= x[1] && p.x >= x[0] && p.y <= y[1] && p.y >= y[0] && p.z <= z[1] && p.z >= z[0];
    }
    math::Vector3f AABB::get_center() const
    {
        return math::Vector3f{x[0] + x[1], y[0] + y[1], z[0] + z[1]} * 0.5f;
    }
    std::array<math::Vector3f, 8> AABB::get_vertices()
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
        auto judge_op = [&](math::Vector3f const &i) -> bool
        {
            if (i.x > x[1] || i.x <= x[0])
            {
                return false;
            }
            if (i.y > y[1] || i.y <= y[0])
            {
                return false;
            }
            if (i.z > z[1] || i.z <= z[0])
            {
                return false;
            }
        };
        if (!judge_op(a) || !judge_op(b) || !judge_op(c))
            return false;
    }
}