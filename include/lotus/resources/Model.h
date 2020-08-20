#pragma once

#include "lotus/physics.h"
#include "Texture.h"

namespace Lotus
{
    struct LOTUS_API Vertex
    {
        Vector3f Position;
        Vector3f Normal;
        Vector2f TexCoords;
    };

    struct LOTUS_API SubMesh
    {
        std::vector<Vertex> Vertices;
        std::vector<uint32_t> Indices;
        std::vector<Handle<Texture>> Textures;

        // TODO: Make meshes API agnostic
        unsigned int VBO = 0;
        unsigned int EBO = 0;
        unsigned int VAO = 0;

        SubMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices,
                const std::vector<Handle<Texture>>& textures);
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
