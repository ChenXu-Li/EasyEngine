#pragma once
#include <vector>
#include<string>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include"Texture.h"
class GameObject {
public:
    GameObject(std::string n = "default");
    virtual ~GameObject();

    void AddChild(std::shared_ptr<GameObject> child);
    void SetPosition(const glm::vec3& position);
    void SetRotation(const glm::vec3& rotation);
    void SetScale(const glm::vec3& scale);

    void Update(float deltaTime);
    virtual void Render(const glm::mat4& parentTransform = glm::mat4(1.0f), const glm::mat4& projection = glm::mat4(1.0f), const glm::mat4& view = glm::mat4(1.0f), const glm::vec3& lightPos = glm::vec3(1.0f), const glm::vec3& lightColor = glm::vec3(1.0f), const glm::vec3& viewPos = glm::vec3(1.0f));
    void ImGuiRender();
    bool m_Selected;
    bool m_Crushed;
protected:
    
    std::string name;
    std::string type;
    const char* name_cstr;
    glm::vec3 m_Position;
    glm::vec3 m_Rotation;
    glm::vec3 m_Scale;

    glm::mat4 m_ModelMatrix;

    std::vector<std::shared_ptr<GameObject>> m_Children;

    // Graphics resources
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<Texture> m_Texture;
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<IndexBuffer> m_IndexBuffer;
    std::unique_ptr<VertexBuffer> m_VertexBuffer;

    void UpdateModelMatrix();
};
