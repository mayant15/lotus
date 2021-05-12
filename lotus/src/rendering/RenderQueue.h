#pragma once

#include <lotus/rendering/CMeshRenderer.h>
#include <lotus/ecs/Entity.h>

namespace Lotus::Renderer
{
    struct MeshBufferSet
    {
        unsigned int VBO = 0;
        unsigned int EBO = 0;
        unsigned int VAO = 0;
        std::vector<unsigned int> Indices;

        MeshBufferSet(unsigned int vbo, unsigned int ebo, unsigned int vao, const std::vector<unsigned int>& indices)
                : VBO(vbo), EBO(ebo), VAO(vao), Indices(indices)
        {}
    };

    struct ModelInfo
    {
        std::vector<MeshBufferSet> Buffers;
        Handle<Material> MeshMaterial;
        EntityID id;
    };

    /**
     * @brief Class that defines objects that are to be rendered
     * TODO: Support instancing https://learnopengl.com/Advanced-OpenGL/Instancing
     */
    class RenderQueue
    {
        std::vector<ModelInfo> models;
    public:
        void Insert(const ModelInfo& info)
        {
            models.push_back(info);
        }

        /**
         * Essentially just for using range-based for loops.
         * Sometime in the future this iterator could get more complex, but this
         * will do for now
         */
        using iterator = std::vector<ModelInfo>::iterator;
        using const_iterator = std::vector<ModelInfo>::const_iterator;

        [[nodiscard]] const_iterator begin() const
        {
            return models.begin();
        }

        [[nodiscard]] const_iterator end() const
        {
            return models.end();
        }
    };
}
