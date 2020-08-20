#include "glad/glad.h"
#include "lotus/resources/AssetRegistry.h"
#include "lotus/debug.h"
#include "stb_image.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace Lotus
{
    void processNode(const aiNode* node, const aiScene* scene, SRef<Model> model);
    SubMesh processMesh(const aiMesh* mesh, const aiScene* scene);
    std::vector<Handle<Texture>> loadMaterialTextures(const aiMaterial* mat, aiTextureType type, const std::string& typeName, bool flipY = true);

    SRef<Model> ModelLoader::Load(const std::string& path) const
    {
        SRef<Model> model = std::make_shared<Model>();

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            LOG_ERROR("Assimp: {}", importer.GetErrorString());
            throw std::invalid_argument(importer.GetErrorString());
        }

        processNode(scene->mRootNode, scene, model);
        return model;
    }

    void processNode(const aiNode* node, const aiScene* scene, SRef<Model> model)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; ++i)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            model->Meshes.push_back(processMesh(mesh, scene));
        }

        for (unsigned int i = 0; i < node->mNumChildren; ++i)
        {
            processNode(node->mChildren[i], scene, model);
        }
    }

    std::vector<Handle<Texture>>
    loadMaterialTextures(const aiMaterial* mat, aiTextureType type, const std::string& typeName, bool flipY)
    {
        auto& cache = AssetRegistry::Get();
        stbi_set_flip_vertically_on_load(flipY);
        std::vector<Handle<Texture>> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i)
        {
            // TODO: String identifiers should be unified across libraries
            aiString str;
            mat->GetTexture(type, i, &str);
            Handle<Texture> texture = cache.LoadTexture(str.C_Str(), typeName);
            textures.push_back(texture);
        }

        return textures;
    }

    SubMesh processMesh(const aiMesh* mesh, const aiScene* scene)
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        std::vector<Handle<Texture>> textures;

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
            std::vector<Handle<Texture>> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE,
                                                                        DIFFUSE_TEXTURE);
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            // Append specular maps
            std::vector<Handle<Texture>> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR,
                                                                         SPECULAR_TEXTURE);
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }

        return SubMesh(vertices, indices, textures);
    }

    SubMesh::SubMesh(
        std::vector<Vertex> vertices, 
        std::vector<uint32_t> indices,
        const std::vector<Handle<Texture>>& textures
    )
    {
        Indices = indices;
        Vertices = vertices;
        Textures = textures;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // Can do this because structs are laid sequentially in memory

        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, Normal));

        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, TexCoords));

        glBindVertexArray(0);
    }
}
