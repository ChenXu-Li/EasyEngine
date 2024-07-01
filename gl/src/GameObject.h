#pragma once
#define GLFW_MOUSE_BUTTON_1         0
#define GLFW_MOUSE_BUTTON_2         1
#define GLFW_MOUSE_BUTTON_3         2
#define GLFW_MOUSE_BUTTON_4         3
#define GLFW_MOUSE_BUTTON_5         4
#define GLFW_MOUSE_BUTTON_6         5
#define GLFW_MOUSE_BUTTON_7         6
#define GLFW_MOUSE_BUTTON_8         7
#define GLFW_MOUSE_BUTTON_LAST      GLFW_MOUSE_BUTTON_8
#define GLFW_MOUSE_BUTTON_LEFT      GLFW_MOUSE_BUTTON_1
#define GLFW_MOUSE_BUTTON_RIGHT     GLFW_MOUSE_BUTTON_2
#define GLFW_MOUSE_BUTTON_MIDDLE    GLFW_MOUSE_BUTTON_3
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
//#include"Texture.h"
#include "TextureManager.h"
class GameObject {
public:
    GameObject(std::string n = "default");
    virtual ~GameObject();

    void AddChild(std::shared_ptr<GameObject> child);
    void SetPosition(const glm::vec3& position);
    void SetRotation(const glm::vec3& rotation);
    void SetScale(const glm::vec3& scale);

    virtual void Update(float deltaTime);
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

    glm::vec3 o_Position;
    bool o_flag;

    glm::mat4 m_ModelMatrix;

    std::vector<std::shared_ptr<GameObject>> m_Children;

    // Graphics resources
    //std::unique_ptr<Shader> m_Shader;
    std::shared_ptr<Shader> m_Shader;
    //std::unique_ptr<Texture> m_Texture;
    std::shared_ptr<Texture> m_Texture;
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<IndexBuffer> m_IndexBuffer;
    std::unique_ptr<VertexBuffer> m_VertexBuffer;

    void UpdateModelMatrix();
};
