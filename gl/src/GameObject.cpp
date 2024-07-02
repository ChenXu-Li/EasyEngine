#include "GameObject.h"
#include "Renderer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <iostream>
GameObject::GameObject(std::string n)
    : name(n),m_Position(glm::vec3(0.0f,0.0f,0.0f)), m_Rotation(glm::vec3(0.0f, 0.0f, 0.0f)), m_Scale(glm::vec3(1.0f, 1.0f, 1.0f)), m_ModelMatrix(glm::mat4(1.0f)), m_Selected(false),m_Crushed(false)
{
    type = "Empty";
    name = name+"("+type+")";
    name_cstr = name.c_str();
    std::cout << name << " was created" << std::endl;

    m_Selected = false;
    o_flag = false;
}

GameObject::~GameObject() {
    std::cout << name << " was destroy" << std::endl;
}

void GameObject::AddChild(std::shared_ptr<GameObject> child) {
    m_Children.push_back(child);
}

void GameObject::SetPosition(const glm::vec3& position) {
    m_Position = position;
    UpdateModelMatrix();
}

void GameObject::SetRotation(const glm::vec3& rotation) {
    m_Rotation = rotation;
    UpdateModelMatrix();
}

void GameObject::SetScale(const glm::vec3& scale) {
    m_Scale = scale;
    UpdateModelMatrix();
}

void GameObject::Update(float deltaTime) {

    UpdateModelMatrix();

    for (auto& child : m_Children) {
        child->Update(deltaTime);
    }
}

void GameObject::Render(const glm::mat4& parentTransform, const glm::mat4& projection, const glm::mat4& view, const glm::vec3& lightPos, const glm::vec3& lightColor, const glm::vec3& viewPos) {

    glm::mat4 globalTransform = parentTransform * m_ModelMatrix;


    // Render all children
    for (auto& child : m_Children) {
        child->Render(globalTransform,projection,view, lightPos, lightColor, viewPos);
    }
}

void GameObject::ImGuiRender()
{
    if (ImGui::TreeNode(name_cstr)){
        ImGui::SameLine();
      
        bool previousSelected = m_Selected;
        ImGui::Checkbox("##Selected", &m_Selected);
        // 如果选择状态改变，调用 SelectState2AllChild
        if (m_Selected != previousSelected) {
            SelectState2AllChild(m_Selected);
        }
        ImGui::SameLine();
        //ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::TreeNode("Self_Attribute"))
        {
            ImGui::DragFloat3("Position", &m_Position[0], 0.1f);
            ImGui::DragFloat3("Rotation", &m_Rotation[0], 0.1f);
            ImGui::DragFloat3("Scale", &m_Scale[0], 0.1f);         
            ImGui::TreePop();
        }
        

        for (auto& child : m_Children) {
            child->ImGuiRender();
        }
        ImGui::TreePop();
    }
    
}

void GameObject::UpdateModelMatrix() {
    m_ModelMatrix = glm::translate(glm::mat4(1.0f), m_Position) *
        glm::rotate(glm::mat4(1.0f), m_Rotation.x, glm::vec3(1, 0, 0)) *
        glm::rotate(glm::mat4(1.0f), m_Rotation.y, glm::vec3(0, 1, 0)) *
        glm::rotate(glm::mat4(1.0f), m_Rotation.z, glm::vec3(0, 0, 1)) *
        glm::scale(glm::mat4(1.0f), m_Scale);
}

void GameObject::SelectState2AllChild(bool state)
{
    m_Selected = state;
    for (auto& child : m_Children) {
        child->SelectState2AllChild(state);
    }
}
void GameObject::CursorChoose() {
    if (mouse_state.buttons[GLFW_MOUSE_BUTTON_LEFT] == true && keys[GLFW_KEY_LEFT_CONTROL] == true && (!m_Selected) && CheckInside()) {
        m_Selected = true;
    }
    else if (mouse_state.buttons[GLFW_MOUSE_BUTTON_RIGHT] == true && keys[GLFW_KEY_LEFT_CONTROL] == true && (m_Selected) && CheckInside()) {
        m_Selected = false;
    }
}
void GameObject::SetBoundingBox()
{
    m_boundingBox = { 0.0f, -0.0f, 0.0f, -0.0f, 0.0f, -0.0f };
}
bool GameObject::CheckInside() {
    glm::vec3 ppp = ScreenToModel::screenToModel(mouse_state.x, mouse_state.y, mouse_state.z_buffer, WIDTH, HEIGHT,
        g_ProjectionMatrix, g_ViewMatrix, m_ParentMatrix * m_ModelMatrix);
    std::cout << ppp.x << " " << ppp.y << " " << ppp.z  << std::endl;
    if ((ppp.x <= m_boundingBox.Right_X + 0.1f) && (ppp.x >= m_boundingBox.Left_X - 0.1f)
        && (ppp.y <= m_boundingBox.UP_Y + 0.1f) && (ppp.y >= m_boundingBox.Down_Y - 0.1f)
        && (ppp.z <= m_boundingBox.Front_Z + 0.1f) && (ppp.z >= m_boundingBox.Back_Z - 0.1f)) {

        return true;
    }
    else {
        return false;
    }
}
