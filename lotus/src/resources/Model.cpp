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
    using json = nlohmann::json;

    void processNode(const aiNode*, const aiScene*, SRef<Model>, Handle<Material>);
    SubMesh processMesh(const aiMesh*, const aiScene*, Handle<Material>);

    SRef<Model> ModelLoader::Load(const std::string& path) const
    {
        std::ifstream file (path);
        json data;
        file >> data;

        // TODO: Save the model as a lotus json too. We should not have to rely on Assimp for material data other than the first import.
        // TODO: Related to the above point, there is a need to separate "import" and "load".
        auto& cache = AssetRegistry::Get();

        // TODO: Modify to accommodate sub meshes
        Handle<Material> material = cache.LoadMaterial(RESOURCE(data["material"]));

        SRef<Model> model = std::make_shared<Model>();

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(RESOURCE(data["mesh"]), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            LOG_ERROR("Assimp: {}", importer.GetErrorString());
            throw std::invalid_argument(importer.GetErrorString());
        }

        processNode(scene->mRootNode, scene, model, material);
        return model;
    }

    void processNode(const aiNode* node, const aiScene* scene, SRef<Model> model, Handle<Material> mat)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; ++i)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            model->Meshes.push_back(processMesh(mesh, scene, mat));
        }

        for (unsigned int i = 0; i < node->mNumChildren; ++i)
        {
            processNode(node->mChildren[i], scene, model, mat);
        }
    }

    SubMesh processMesh(const aiMesh* mesh, const aiScene* scene, Handle<Material> mat)
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

            Vector3f tangent {
                mesh->mTangents[i].x,
                mesh->mTangents[i].y,
                mesh->mTangents[i].z
            };

            Vertex vertex{position, normal, texCoords, tangent};
            vertices.push_back(vertex);
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        return SubMesh(vertices, indices, mat);
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

        // vertex tangents
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, Tangent));

        glBindVertexArray(0);
    }
}
