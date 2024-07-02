#pragma once
#include "ControlState.h"
#include <vector>
#include<string>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ShaderManager.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "TextureManager.h"
#include "Transform.h"
extern MouseState mouse_state;
extern GLuint WIDTH, HEIGHT;
extern bool keys[1024];
struct BoundingBox {
    float UP_Y;
    float Down_Y;
    float Right_X;
    float Left_X;
    float Front_Z;
    float Back_Z;
};
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
    BoundingBox m_boundingBox;
    glm::vec3 o_Position;//跟随鼠标移动时的原点
    bool o_flag;//跟随鼠标移动开始标志

    glm::mat4 m_ModelMatrix;
    glm::mat4 m_ParentMatrix;
    glm::mat4 g_ViewMatrix;
    glm::mat4 g_ProjectionMatrix;

    std::vector<std::shared_ptr<GameObject>> m_Children;

    void UpdateModelMatrix();
    void SelectState2AllChild(bool selfstate);
    bool CheckInside();
    void CursorChoose();
    virtual void SetBoundingBox();
};
