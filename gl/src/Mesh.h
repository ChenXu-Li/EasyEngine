#pragma once
#include "Texture.h"
#include <vector>
#include<string>
#include <Assimp/postprocess.h>
struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};
struct TextureInfo
{
    GLuint id;
    std::string type;
    aiString path;
};
class Mesh
{
public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<TextureInfo> textureinfos;
    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<TextureInfo> texture);
    void Draw(Shader& shader);

private:
    GLuint VAO, VBO, EBO;
    void setupMesh();
};
