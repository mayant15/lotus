#include <lotus/resources/Model.h>
#include <lotus/debug.h>

#include <rendering/RHI.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace Lotus
{
    Vector3f to_vec(aiVector3t<ai_real> vec)
    {
        return {vec.x, vec.y, vec.z};
    }

    Geometry processMesh(const aiMesh* mesh)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
        {
            Vertex vert {};
            if (mesh->mTextureCoords[0] != nullptr)
            {
                vert.TexCoords = Vector2f {
                    mesh->mTextureCoords[0][i].x,
                    mesh->mTextureCoords[0][i].y
                };
            }
            else
            {
                vert.TexCoords = Vector2f { 0.0f };
            }

            vert.Position = to_vec(mesh->mVertices[i]);
            vert.Normal = to_vec(mesh->mNormals[i]);
            vert.Tangent = to_vec(mesh->mTangents[i]);
            vertices.push_back(vert);
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
            }
        }

        return { vertices, indices };
    }

    void processNode(const aiNode* node, const aiScene* scene, SRef<Model> model)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; ++i)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            model->Meshes.push_back(processMesh(mesh));
        }

        for (unsigned int i = 0; i < node->mNumChildren; ++i)
        {
            processNode(node->mChildren[i], scene, model);
        }
    }

    SRef<Model> ModelLoader::Load(const std::string& path) const
    {
        SRef<Model> model = std::make_shared<Model>();

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            LOG_ERROR("Assimp: {}", importer.GetErrorString());
            throw std::invalid_argument(importer.GetErrorString());
        }

        processNode(scene->mRootNode, scene, model);
        return model;
    }
}
