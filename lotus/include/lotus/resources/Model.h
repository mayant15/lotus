#pragma once

#include "lotus/resources/Material.h"

namespace Lotus
{
    struct Vertex
    {
        Vector3f Position;
        Vector3f Normal;
        Vector2f TexCoords;
        Vector3f Tangent;
    };

    struct SubMesh
    {
        std::vector<Vertex> Vertices;
        std::vector<unsigned int> Indices;

        // TODO: Make meshes API agnostic
        unsigned int VBO = 0;
        unsigned int EBO = 0;
        unsigned int VAO = 0;

        LOTUS_API SubMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    };

    struct Model
    {
        std::vector<SubMesh> Meshes;
    };

    LOADER(Model)
    {
        // TODO: Save the model as a lotus json too. We should not have to rely on Assimp for material data other than the first import.
        //  there is a need to separate "import" and "load".
        [[nodiscard]] SRef<Model> Load(const std::string& path) const;
    };
}
