#include "GameObjectCube.h"
#include "Renderer.h"

GameObjectCube::GameObjectCube(std::string n) : GameObject(n) {
    type = "Cube";
    name = name + "(" + type + ")";
    name_cstr = name.c_str();
    std::cout << name << " was created" << std::endl;
    // Initialize graphics resources
    GLfloat vertices[] = {
        // Positions          // Colors           // Texture Coords
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
    };
    GLuint indices[] = {
       0, 1, 2, 2, 3, 0, // Back face
       4, 5, 6, 6, 7, 4, // Front face
       0, 1, 5, 5, 4, 0, // Bottom face
       2, 3, 7, 7, 6, 2, // Top face
       0, 3, 7, 7, 4, 0, // Left face
       1, 2, 6, 6, 5, 1  // Right face
    };

    m_Shader = std::make_unique<Shader>("res/shader/mvp.shaderg");
    m_VAO = std::make_unique<VertexArray>();
    m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 36);
    m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, 8 * 8 * sizeof(float));

    VertexBufferLayout vblayout;
    vblayout.Push<float>(3);
    vblayout.Push<float>(3);
    vblayout.Push<float>(2);

    m_VAO->AddBuffer(*m_VertexBuffer, vblayout);
}
void GameObjectCube::Render(const glm::mat4& parentTransform, const glm::mat4& projection, const glm::mat4& view){

    glm::mat4 globalTransform = parentTransform * m_ModelMatrix;

    m_Shader->Bind();
    m_Shader->SetUniformMat4fv("projection", projection);
    m_Shader->SetUniformMat4fv("view", view);
    m_Shader->SetUniformMat4fv("model", globalTransform);

    Renderer renderer;
    renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);

    // Render all children
    for (auto& child : m_Children) {
        child->Render(globalTransform, projection, view);
    }
}