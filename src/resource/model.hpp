#pragma once

#include "mesh.hpp"
#include <filesystem>
namespace cc
{
    struct Model
    {
        std::vector<Mesh> meshes;
        math::Vector3f position = math::Vector3f{0.0f};
        math::Vector3f scale = math::Vector3f{1.0f};
        math::Vector3f rotation = math::Vector3f{0.0f};

        int vertex_cnt, face_cnt, submesh_cnt;
    };

    using ModelPtr = std::shared_ptr<Model>;
    auto load_model(std::filesystem::path const &path) -> ModelPtr;
} // namespace cc
