#include "octree.hpp"
#include <limits>

namespace cc
{
    auto split(OctreeNode *node) -> void
    {
        auto center = node->bbx.get_center();
        auto new_extent = node->bbx.get_extent() * 0.5f;
        for (int i = 0; i < 8; i++)
        {
            auto new_center = center;
            new_center.x += new_extent.x * (i & 4 ? -1.f : 1.f);
            new_center.y += new_extent.y * (i & 2 ? -1.f : 1.f);
            new_center.z += new_extent.z * (i & 1 ? -1.f : 1.f);
            auto new_bbx = AABB{new_center - new_extent, new_center + new_extent};
            auto new_node = new OctreeNode{new_bbx, node->depth + 1, true};
            node->children.push_back(new_node);
        }
    }
    auto insert(OctreeNode *node, NodeData const &data, AABB const &bbx, OctreeConfig const &config) -> void
    {
        node->is_empty = false;
        if (node->is_leaf)
        {
            if (node->depth < config.max_depth)
            {
                split(node);
                node->is_leaf = false;
            }
            else
            {
                node->data.push_back(data);
                return;
            }
        }
        for (int i = 0; i < 8; i++)
            if (node->children[i]->bbx.intersect(bbx))
            {
                insert(node->children[i], data, bbx, config);
            }
    }
    auto build_octree(Model const &model, OctreeConfig const &config) -> std::unique_ptr<OctreeNode>
    {
        auto min_all = math::Vector3f{std::numeric_limits<float>::max()};
        auto max_all = math::Vector3f{std::numeric_limits<float>::min()};
        auto mesh_bbx = std::vector<AABB>{};
        mesh_bbx.reserve(model.meshes.size());
        for (auto const &mesh : model.meshes)
        {
            auto min_p = math::Vector3f{std::numeric_limits<float>::max()};
            auto max_p = math::Vector3f{std::numeric_limits<float>::min()};
            for (auto const &position : mesh.positions)
            {
                min_p = min(min_p, position);
                max_p = max(max_p, position);
            }
            mesh_bbx.emplace_back(min_p, max_p);
            min_all = min(min_p, min_all);
            max_all = max(max_p, max_all);
        }
        auto bbx = AABB{min_all, max_all};
        auto root = std::make_unique<OctreeNode>(bbx, 0, true);
        for (uint32_t i = 0; i < model.meshes.size(); i++)
        {
            insert(root.get(), NodeData{i}, mesh_bbx[i], config);
        }
        return root;
    }
}