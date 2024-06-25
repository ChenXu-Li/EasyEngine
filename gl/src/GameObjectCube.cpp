#include "GameObjectCube.h"
#include "Renderer.h"

GameObjectCube::GameObjectCube(std::string n) : GameObject(n) {
    type = "Cube";
    name = name + "(" + type + ")";
    name_cstr = name.c_str();
    std::cout << name << " was created" << std::endl;
    // Initialize graphics resources
   
    GLfloat vertices[] = {
        // Positions          // Texture Coords   // Normals
        // Back face
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,       0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,       0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,       0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,       0.0f,  0.0f, -1.0f,

        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,       0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,       0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,       0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,       0.0f,  0.0f,  1.0f,

        // Left face
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,      -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,      -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,      -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,      -1.0f,  0.0f,  0.0f,

        // Right face
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,       1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,       1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,       1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,       1.0f,  0.0f,  0.0f,

         // Bottom face
         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,       0.0f, -1.0f,  0.0f,
          0.5f, -0.5f, -0.5f,  1.0f, 1.0f,       0.0f, -1.0f,  0.0f,
          0.5f, -0.5f,  0.5f,  1.0f, 0.0f,       0.0f, -1.0f,  0.0f,
         -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,       0.0f, -1.0f,  0.0f,

         // Top face
         -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,       0.0f,  1.0f,  0.0f,
          0.5f,  0.5f, -0.5f,  1.0f, 1.0f,       0.0f,  1.0f,  0.0f,
          0.5f,  0.5f,  0.5f,  1.0f, 0.0f,       0.0f,  1.0f,  0.0f,
         -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,       0.0f,  1.0f,  0.0f,
    };

    GLuint indices[] = {
        // Back face
        0, 1, 2, 2, 3, 0,
        // Front face
        4, 5, 6, 6, 7, 4,
        // Left face
        8, 9, 10, 10, 11, 8,
        // Right face
        12, 13, 14, 14, 15, 12,
        // Bottom face
        16, 17, 18, 18, 19, 16,
        // Top face
        20, 21, 22, 22, 23, 20,
    };

    m_Shader = std::make_unique<Shader>("res/shader/pong.shaderg");
    m_VAO = std::make_unique<VertexArray>();
    m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 36);
    m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, 24 * 8 * sizeof(float));
    m_Texture = std::make_unique<Texture>("res/texture/1.png");
    VertexBufferLayout vblayout;
    vblayout.Push<float>(3);
    vblayout.Push<float>(2);
    vblayout.Push<float>(3);

    m_VAO->AddBuffer(*m_VertexBuffer, vblayout);
}
void GameObjectCube::Render(const glm::mat4& parentTransform, const glm::mat4& projection, const glm::mat4& view, const glm::vec3& lightPos, const glm::vec3& lightColor, const glm::vec3& viewPos){

    glm::mat4 globalTransform = parentTransform * m_ModelMatrix;

    m_Shader->Bind();
    m_Shader->SetUniformMat4fv("projection", projection);
    m_Shader->SetUniformMat4fv("view", view);
    m_Shader->SetUniformMat4fv("model", globalTransform);
    m_Texture->Bind(0);
    m_Shader->SetUniform1i("u_Texture", 0);
    m_Shader->SetUniform3f("lightColor", lightColor);
    m_Shader->SetUniform3f("lightPos", lightPos);
    m_Shader->SetUniform3f("viewPos", viewPos);

    Renderer renderer;
    renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);

    // Render all children
    for (auto& child : m_Children) {
        child->Render(globalTransform, projection, view, lightPos, lightColor, viewPos);
    }
}