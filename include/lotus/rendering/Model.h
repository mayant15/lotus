#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include "Shader.h"

#define DIFFUSE_TEXTURE "texture_diffuse"
#define SPECULAR_TEXTURE "texture_specular"

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct Texture
{
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh
{
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Mesh(
            std::vector<Vertex> vertices_,
            std::vector<unsigned int> indices_,
            std::vector<Texture> textures_
    );

    ~Mesh();

    void draw(const Shader& shader);

private:
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;

    void setupMesh();
};

class Model
{
public:
    Model(const std::string& path, bool flipTextureY_ = true);
    void draw(const Shader& shader);

private:
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> loadedTextures;
    bool flipTextureY;

    void loadModel(const std::string& path);

    void processNode(const aiNode* node, const aiScene* scene);

    Mesh processMesh(const aiMesh* mesh, const aiScene* scene);

    std::vector<Texture> loadMaterialTextures(const aiMaterial* mat, aiTextureType type, const std::string& typeName);

    unsigned int textureFromFile(const char* name, const std::string& directory_);
};
