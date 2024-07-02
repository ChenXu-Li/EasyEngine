#include "Light.h"
#include "Renderer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
Light::Light(std::string n)
    : name(n), m_Position(glm::vec3(0.0f, 0.0f, 0.0f)), m_Rotation(glm::vec3(0.0f, 0.0f, 0.0f)), m_Scale(glm::vec3(1.0f, 1.0f, 1.0f)), m_ModelMatrix(glm::mat4(1.0f))
{
    type = "Light";
    name = name + "(" + type + ")";
    name_cstr = name.c_str();
    std::cout << name << " was created" << std::endl;
    m_Position = glm::vec3(1.0f,1.0f,1.0f);
    m_LightColor = glm::vec3(1.0f, 0.0f, 1.0f);
    // Initialize graphics resources
    GLfloat vertices[] = {
        // Positions         
        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f, 
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f, 
         0.5f, -0.5f,  0.5f, 
         0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 
    };
    GLuint indices[] = {
       0, 1, 2, 2, 3, 0, // Back face
       4, 5, 6, 6, 7, 4, // Front face
       0, 1, 5, 5, 4, 0, // Bottom face
       2, 3, 7, 7, 6, 2, // Top face
       0, 3, 7, 7, 4, 0, // Left face
       1, 2, 6, 6, 5, 1  // Right face
    };

   // m_Shader = std::make_unique<Shader>("res/shader/light.shaderg");
    m_Shader = ShaderManager::GetInstance().GetShader("res/shader/light.shaderg");
    m_VAO = std::make_unique<VertexArray>();
    m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 36);
    m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, 8 * 3 * sizeof(float));
    VertexBufferLayout vblayout;
    vblayout.Push<float>(3);

    m_VAO->AddBuffer(*m_VertexBuffer, vblayout);
}

void Light::SetPosition(const glm::vec3& position) {
    m_Position = position;
    UpdateModelMatrix();
}

glm::vec3& Light::GetPosition()
{
    return m_Position;
}

void Light::SetLightColor(const glm::vec3& color)
{
    m_LightColor = color;

}

glm::vec3& Light::GetLightColor()
{
    return m_LightColor;
}

void Light::SetRotation(const glm::vec3& rotation) {
    m_Rotation = rotation;
    UpdateModelMatrix();
}

void Light::SetScale(const glm::vec3& scale) {
    m_Scale = scale;
    UpdateModelMatrix();
}

void Light::Update(float deltaTime) {

    UpdateModelMatrix();

}

void Light::Render(const glm::mat4& parentTransform, const glm::mat4& projection, const glm::mat4& view)
{
    glm::mat4 globalTransform = parentTransform * m_ModelMatrix;

    m_Shader->Bind();
    m_Shader->SetUniformMat4fv("projection", projection);
    m_Shader->SetUniformMat4fv("view", view);
    m_Shader->SetUniformMat4fv("model", globalTransform);
    m_Shader->SetUniform3f("selfcolor", m_LightColor);
   

    Renderer renderer;
    renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);


}

void Light::ImGuiRender()
{
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(m_LightColor.x, m_LightColor.y, m_LightColor.z, 1.0f));
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::TreeNode("Light")) {
        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::TreeNode("Light Position"))
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
        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::TreeNode("Light Color"))
        {
            ImGui::ColorEdit4("clear color", (float*)&m_LightColor);

            ImGui::TreePop();
        }  
        ImGui::TreePop();
    }
    ImGui::PopStyleColor(1);
}

void Light::UpdateModelMatrix()
{
    m_ModelMatrix = glm::translate(glm::mat4(1.0f), m_Position) *
        glm::rotate(glm::mat4(1.0f), m_Rotation.x, glm::vec3(1, 0, 0)) *
        glm::rotate(glm::mat4(1.0f), m_Rotation.y, glm::vec3(0, 1, 0)) *
        glm::rotate(glm::mat4(1.0f), m_Rotation.z, glm::vec3(0, 0, 1)) *
        glm::scale(glm::mat4(1.0f), m_Scale);
}


