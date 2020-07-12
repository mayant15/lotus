#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "stb_image.h"
#include "lotus/rendering/Model.h"

Model::Model(const std::string& path, bool flipTextureY_) : flipTextureY(flipTextureY_)
{
    loadModel(path);
}

void Model::draw(const Shader& shader)
{
    for (Mesh& mesh : meshes)
    {
        mesh.draw(shader);
    }
}

void Model::loadModel(const std::string& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}

void Model::processNode(const aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; ++i)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(const aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
    {
        glm::vec3 position;
        position.x = mesh->mVertices[i].x;
        position.y = mesh->mVertices[i].y;
        position.z = mesh->mVertices[i].z;

        glm::vec3 normal;
        normal.x = mesh->mNormals[i].x;
        normal.y = mesh->mNormals[i].y;
        normal.z = mesh->mNormals[i].z;

        glm::vec2 texCoords(0.0f, 0.0f);
        if (mesh->mTextureCoords[0])
        {
            texCoords.x = mesh->mTextureCoords[0][i].x;
            texCoords.y = mesh->mTextureCoords[0][i].y;
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

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        // Append diffuse maps
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, DIFFUSE_TEXTURE);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        // Append specular maps
        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, SPECULAR_TEXTURE);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(const aiMaterial* mat, aiTextureType type, const std::string& typeName)
{
    stbi_set_flip_vertically_on_load(flipTextureY);
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (Texture& loadedTexture : loadedTextures)
        {
            if (std::strcmp(loadedTexture.path.c_str(), str.C_Str()) == 0)
            {
                textures.push_back(loadedTexture);
                skip = true;
                break;
            }
        }

        // if texture hasn't been loaded already, load it
        if (!skip)
        {
            Texture texture{
                    textureFromFile(str.C_Str(), directory),
                    typeName,
                    str.C_Str()
            };
            textures.push_back(texture);
            loadedTextures.push_back(texture);
        }
    }

    return textures;
}

unsigned int Model::textureFromFile(const char* name, const std::string& directory_)
{
    std::string fullPath = directory_ + '/' + name;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(fullPath.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
        {
            format = GL_RED;
        } else if (nrComponents == 3)
        {
            format = GL_RGB;
        } else if (nrComponents == 4)
        {
            format = GL_RGBA;
        } else
        {
            std::cout << "ERROR::MODEL::LOAD_TEXTURE" << std::endl;
            throw std::invalid_argument("image color format unknown");
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else
    {
        std::cout << "Texture failed to load at path: " << directory_ << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

