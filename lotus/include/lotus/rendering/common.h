#pragma once

#include <lotus/physics/Vector.h>

namespace Lotus
{
    struct Vertex
    {
        Vector3f Position;
        Vector3f Normal;
        Vector2f TexCoords;
        Vector3f Tangent;
    };

    struct Geometry
    {
        std::vector<Vertex> Vertices;
        std::vector<unsigned int> Indices;
    };

    using Color = glm::vec4;
    constexpr Color COLOR_WHITE = {1.0f, 1.0f, 1.0f, 1.0f};
}
