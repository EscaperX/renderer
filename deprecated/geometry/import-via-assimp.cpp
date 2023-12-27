#include "import-via-assimp.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
namespace nanami::geometry
{
    static std::string directory;
    auto import_mesh_only(const std::string &path) -> std::vector<TriangleMesh>
    {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
            exit(0);
        }
        directory = path.substr(0, path.find_last_of('/'));

        std::vector<TriangleMesh> res;

        auto convert_mesh = [](aiMesh *mesh, const aiScene *scene) -> TriangleMesh
        {
            std::vector<Vector3f> positions;
            std::vector<size_t> indices;
            std::vector<Vector3f> normals;
            std::vector<Vector2f> uvs;

            for (size_t i = 0; i < mesh->mNumVertices; i++)
            {
                positions.emplace_back(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
                normals.emplace_back(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
                if (mesh->mTextureCoords[0])
                    uvs.emplace_back(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            }
            for (size_t i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                for (size_t j = 0; j < face.mNumIndices; j++)
                    indices.push_back(face.mIndices[j]);
            }
            return TriangleMesh(positions, normals, uvs, indices);
        };

        auto travel_node = [&res](aiNode *node, const aiScene *scene) -> void
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            res.push_back(mesh);
        };
    }

}