#pragma once
#include "Mesh.h"
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>
class Model
{
public:
    /*  成员函数   */
    Model();
    Model(const std::string& path);
    
    void Draw(Shader& shader);
    void FindBestValue(float& max_x, float& min_x, float& max_y, float& min_y, float& max_z, float& min_z);
private:
    /*  模型数据  */
    std::vector<Mesh> meshes;
    std::string directory;

    /*  私有成员函数   */
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<TextureInfo> loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName);
};

