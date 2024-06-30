#pragma once
#include "Mesh.h"
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>
class Model
{
public:
    /*  ��Ա����   */
    Model();
    Model(const std::string& path);
    
    void Draw(Shader& shader);
private:
    /*  ģ������  */
    std::vector<Mesh> meshes;
    std::string directory;

    /*  ˽�г�Ա����   */
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<TextureInfo> loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName);
};

