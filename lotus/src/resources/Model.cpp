#include "glad/glad.h"
#include "lotus/resources/AssetRegistry.h"
#include "lotus/debug.h"
#include "stb_image.h"

#include <assimp/Importer.hpp>
#include <utility>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace Lotus
{
    void processNode(const aiNode* node, const aiScene* scene, SRef<Model> model);
    SubMesh processMesh(const aiMesh* mesh, const aiScene* scene);
    std::variant<Vector3f, Handle<Texture>> loadMaterialTextures(const aiMaterial* mat, aiTextureType type, bool flipY = true);

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

    std::variant<Vector3f, Handle<Texture>>
    loadMaterialTextures(const aiMaterial* mat, aiTextureType type, bool flipY)
    {
        auto& cache = AssetRegistry::Get();
        stbi_set_flip_vertically_on_load(flipY);

        if (mat->GetTextureCount(type) == 0)
        {
            // Return a plain color if there's no texture
            return MaterialDefaults::Get(type);
        }
        else
        {
            // TODO: String identifiers should be unified across libraries
            aiString str;
            mat->GetTexture(type, 0, &str);
            return cache.LoadTexture(str.C_Str());
        }
    }

    SubMesh processMesh(const aiMesh* mesh, const aiScene* scene)
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

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


        // TODO: Save the model as a lotus json too. We should not have to rely on Assimp for material data other than the first import.
        // TODO: Related to the above point, there is a need to separate "import" and "load".
        auto& cache = AssetRegistry::Get();
        aiMaterial* assimpMat = scene->mMaterials[mesh->mMaterialIndex];
    
        // Append diffuse maps
        // auto diffuse = loadMaterialTextures(assimpMat, aiTextureType_DIFFUSE);
        //
        // // Append specular maps
        // auto specular = loadMaterialTextures(assimpMat, aiTextureType_SPECULAR);
        //
        // float shininess = 16.0f;
        // Handle<Material> material = cache.LoadMaterial(diffuse, specular, shininess);

        // TODO: Store material path in the model json
        Handle<Material> material = cache.LoadMaterial(R"(D:\code\lotus\examples\quickstart\resources\materials\box.json)");

        return SubMesh(vertices, indices, material);
    }

    SubMesh::SubMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, Handle<Lotus::Material> material)
    {
        Indices = indices;
        Vertices = vertices;
        Material = std::move(material);

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
