#pragma once

#include "lotus/resources/Material.h"

namespace Lotus
{
    struct LOTUS_API Vertex
    {
        Vector3f Position;
        Vector3f Normal;
        Vector2f TexCoords;
        Vector3f Tangent;
    };

    struct LOTUS_API SubMesh
    {
        std::vector<Vertex> Vertices;
        std::vector<uint32_t> Indices;
        Handle<Material> Material;

        // TODO: Make meshes API agnostic
        unsigned int VBO = 0;
        unsigned int EBO = 0;
        unsigned int VAO = 0;

        SubMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, Handle<Lotus::Material> material);
    };

    struct LOTUS_API Model
    {
        std::vector<SubMesh> Meshes;
    };

    LOADER(ModelLoader, Model)
    {
        SRef<Model> Load(const std::string& path) const;
    };
}
