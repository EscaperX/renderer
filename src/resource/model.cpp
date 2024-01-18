#include "model.hpp"
#include <utils/obj_loader.h>
namespace cc
{
    auto load_model(std::filesystem::path const &path) -> ModelPtr
    {
        objl::Loader Loader;
        bool loadout = Loader.LoadFile(path.string());
        assert(loadout);

        ModelPtr model = std::make_shared<Model>();
        model->submesh_cnt = Loader.LoadedMeshes.size();
        model->meshes.reserve(model->submesh_cnt);

        for (auto mesh : Loader.LoadedMeshes)
        {
            model->meshes.emplace_back();
            auto &cur_mesh = model->meshes.back();

            model->vertex_cnt += mesh.Vertices.size();
            model->face_cnt += mesh.Indices.size() / 3;

            for (auto vert : mesh.Vertices)
            {
                cur_mesh.positions.emplace_back(vert.Position.X, vert.Position.Y, vert.Position.Z);
                cur_mesh.normals.emplace_back(vert.Normal.X, vert.Normal.Y, vert.Normal.Z);
                cur_mesh.uvs.emplace_back(vert.TextureCoordinate.X, vert.TextureCoordinate.Y);
            }
            cur_mesh.indices = mesh.Indices;
        }
        return model;
    }
};