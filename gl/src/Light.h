#pragma once
#include <vector>
#include<string>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ShaderManager.h"
//#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include"Texture.h"
class Light
{
public:
	Light(std::string n = "default");
    void SetPosition(const glm::vec3& position);
    glm::vec3& GetPosition();
    void SetLightColor(const glm::vec3& color);
    glm::vec3& GetLightColor();
    void SetRotation(const glm::vec3& rotation);
    void SetScale(const glm::vec3& scale);

    void Update(float deltaTime);
    void Render(const glm::mat4& parentTransform = glm::mat4(1.0f), const glm::mat4& projection = glm::mat4(1.0f), const glm::mat4& view = glm::mat4(1.0f));

    void ImGuiRender();
protected:

    std::string name;
    std::string type;
    const char* name_cstr;
    glm::vec3 m_Position;
    glm::vec3 m_Rotation;
    glm::vec3 m_Scale;
    glm::vec3 m_LightColor;

    glm::mat4 m_ModelMatrix;


    // Graphics resources
    std::shared_ptr<Shader> m_Shader;
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<IndexBuffer> m_IndexBuffer;
    std::unique_ptr<VertexBuffer> m_VertexBuffer;

    void UpdateModelMatrix();
};

