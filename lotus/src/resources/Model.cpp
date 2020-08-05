#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "lotus/debug.h"
#include "lotus/resources.h"
#include "stb_image.h"

namespace Lotus
{
    LModel::LModel(const std::string& path_, bool flipTextureY_)
    {
        flipTextureY = flipTextureY_;
        path = path_;
    }

    int LModel::import()
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            LOG_ERROR("Assimp: {}", importer.GetErrorString());
            return IMPORT_ERR_CODE;
        }
        directory = path.substr(0, path.find_last_of('/'));
        processNode(scene->mRootNode, scene);

        return IMPORT_SUCCESS_CODE;
    }

    void LModel::processNode(const aiNode* node, const aiScene* scene)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; ++i)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }

        for (unsigned int i = 0; i < node->mNumChildren; ++i)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh LModel::processMesh(const aiMesh* mesh, const aiScene* scene)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<SRef<Texture>> textures;

        for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
        {
            Vector3f position(
                    mesh->mVertices[i].x,
                    mesh->mVertices[i].y,
                    mesh->mVertices[i].z
            );

            Vector3f normal(
                    mesh->mNormals[i].x,
                    mesh->mNormals[i].y,
                    mesh->mNormals[i].z
            );

            Vector2f texCoords(0.0f);
            if (mesh->mTextureCoords[0])
            {
                texCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            }

            Vertex vertex{position, normal, texCoords};
            vertices.push_back(vertex);
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        if (mesh->mMaterialIndex >= 0)
        {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

            // Append diffuse maps
            std::vector<SRef<Texture>> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE,
                                                                        DIFFUSE_TEXTURE);
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            // Append specular maps
            std::vector<SRef<Texture>> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR,
                                                                         SPECULAR_TEXTURE);
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }

        return Mesh(vertices, indices, textures);
    }

    std::vector<SRef<Texture>>
    LModel::loadMaterialTextures(const aiMaterial* mat, aiTextureType type, const std::string& typeName)
    {
        stbi_set_flip_vertically_on_load(flipTextureY);
        std::vector<SRef<Texture>> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            bool skip = AssetManager::exists(path);

            // if texture hasn't been loaded already, load it
            if (!skip)
            {
                std::string path = str.C_Str();
                path = directory + '/' + path;
                SRef<Texture> texture = std::make_shared<Texture>(path, typeName);
                texture->import();
                textures.push_back(texture);
            }
        }

        return textures;
    }

    std::vector<Mesh> LModel::getMeshes() const
    {
        return meshes;
    }
}
