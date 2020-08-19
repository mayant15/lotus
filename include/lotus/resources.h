#pragma once

#include "physics.h"
#include "lcommon.h"
#include <assimp/scene.h>

#include <vector>

#define DIFFUSE_TEXTURE "texture_diffuse"
#define SPECULAR_TEXTURE "texture_specular"

#if RENDER_API == OPEN_GL
#define RED GL_RED
#define RGB GL_RGB
#define RGBA GL_RGBA
#elif RENDER_API == VULKAN
#define RED // TODO
#define RGB
#define RGBA
#endif

#define IMPORT_ERR_CODE -1
#define IMPORT_SUCCESS_CODE -2

namespace Lotus
{
    class LOTUS_API AssetManager
    {
        AssetManager() = default;

    public:
        AssetManager(const AssetManager&) = delete;

        AssetManager& operator=(const AssetManager&) = delete;

        static AssetManager& get();

        template<typename T>
        static void registerAsset(T asset);

        static void registerAsset(const std::string& path);

        static bool exists(const std::string& path);
    };

    class LOTUS_API IResource
    {
    protected:
        std::string path;
    public:
        virtual int import() = 0;
    };

    class Texture : public IResource
    {
    public:
        unsigned int id = 0;

        std::string type;

        Texture(const std::string& path_, const std::string& type_);

        int import() override;
    };

    struct Vertex
    {
        Vector3f position;
        Vector3f normal;
        Vector2f texCoords;
    };

    // TODO: Make this a resource?
    // TODO: Make meshes independent of the graphics API
    class Mesh
    {
    public:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<SRef<Texture>> textures;
        unsigned int VBO = 0;
        unsigned int EBO = 0;
        unsigned int VAO = 0;

        Mesh(
                std::vector<Vertex> vertices_,
                std::vector<unsigned int> indices_,
                std::vector<SRef<Texture>> textures_
        );

    private:
        void setupMesh();

    };

    struct LOTUS_API LModel : IResource
    {
    public:
        LModel(const std::string& path, bool flipTextureY_ = true);

        std::vector<Mesh> getMeshes() const;

        int import() override;

    private:
        std::vector<Mesh> meshes;
        std::string directory;
        bool flipTextureY;

        void processNode(const aiNode* node, const aiScene* scene);

        Mesh processMesh(const aiMesh* mesh, const aiScene* scene);

        std::vector<SRef<Texture>>
        loadMaterialTextures(const aiMaterial* mat, aiTextureType type, const std::string& typeName);
    };
}