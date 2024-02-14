#pragma once
#include <array>
#include <vector>
#include <geometry/AABB.h>
#include <resource/model.hpp>

namespace cc
{
    struct OctreeConfig
    {
        uint16_t max_depth = 3;
        uint16_t max_data_cnt = 10;
    };
    struct NodeData
    {
        uint32_t mesh_id;
    };
    struct OctreeNode
    {
        OctreeNode(AABB const &bbx, int d, bool flag) : bbx(bbx), depth(d), is_leaf(flag) {}
        ~OctreeNode()
        {
            for (auto ch : children)
                delete ch;
        }
        int depth;
        bool is_leaf;
        bool is_empty;
        AABB bbx;
        std::vector<OctreeNode *> children;
        std::vector<NodeData> data;
    };

    auto build_octree(Model const &model, OctreeConfig const &config) -> std::unique_ptr<OctreeNode>;
} // namespace cc
