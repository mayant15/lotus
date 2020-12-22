#include <glad/glad.h>

#include <lotus/resources/Model.h>
#include <lotus/debug.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace Lotus
{
    Vector3f to_vec(aiVector3t<ai_real> vec)
    {
        return {vec.x, vec.y, vec.z};
    }

    SubMesh processMesh(const aiMesh* mesh)
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

        return SubMesh(vertices, indices);
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

    SubMesh::SubMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
    {
        Indices = indices;
        Vertices = vertices;

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
