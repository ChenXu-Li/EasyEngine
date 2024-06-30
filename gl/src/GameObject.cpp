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
        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::TreeNode("Self_Attribute"))
        {
            if (ImGui::TreeNode("Position"))
            {
                ImGui::BeginGroup();
                ImGui::PushItemWidth(80);
                ImGui::DragFloat("X", &m_Position.x, 0.1f);
                ImGui::SameLine();
                ImGui::DragFloat("Y", &m_Position.y, 0.1f);
                ImGui::SameLine();
                ImGui::DragFloat("Z", &m_Position.z, 0.1f);
                ImGui::PopItemWidth();
                ImGui::EndGroup();

                ImGui::TreePop();
            }

            // 显示相机角度
            if (ImGui::TreeNode("Rotation"))
            {

                ImGui::BeginGroup();
                ImGui::PushItemWidth(80);
                ImGui::DragFloat("X", &m_Rotation.x, 0.1f);
                ImGui::SameLine();
                ImGui::DragFloat("Y", &m_Rotation.y, 0.1f);
                ImGui::SameLine();
                ImGui::DragFloat("Z", &m_Rotation.z, 0.1f);
                ImGui::PopItemWidth();
                ImGui::EndGroup();
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Scale"))
            {

                ImGui::BeginGroup();
                ImGui::PushItemWidth(80);
                //ImGui::DragFloat("X", &m_Scale.x, 0.1f, 0.0f, 5.0f);
                ImGui::DragFloat("X", &m_Scale.x, 0.1f);
                ImGui::SameLine();
                ImGui::DragFloat("Y", &m_Scale.y, 0.1f);
                ImGui::SameLine();
                ImGui::DragFloat("Z", &m_Scale.z, 0.1f);
                ImGui::PopItemWidth();
                ImGui::EndGroup();
                ImGui::TreePop();
            }
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
