#include "GameObjectModel.h"

#include "Renderer.h"

GameObjectModel::GameObjectModel(std::string n) : GameObject(n) {
    type = "Model";
    name = name + "(" + type + ")";
    name_cstr = name.c_str();
    std::cout << name << " was created" << std::endl;
   

    //m_Shader = std::make_unique<Shader>("res/shader/modelpong.shaderg");
    m_Shader = ShaderManager::GetInstance().GetShader("res/shader/modelpong.shaderg");
   
    m_model = Model("D:/littlecode/OpenglProjects/basegl/gl/res/model/man.obj");

}
void GameObjectModel::Render(const glm::mat4& parentTransform, const glm::mat4& projection, const glm::mat4& view, const glm::vec3& lightPos, const glm::vec3& lightColor, const glm::vec3& viewPos) {

    glm::mat4 globalTransform = parentTransform * m_ModelMatrix;

    m_Shader->Bind();
    m_Shader->SetUniformMat4fv("projection", projection);
    m_Shader->SetUniformMat4fv("view", view);
    m_Shader->SetUniformMat4fv("model", globalTransform);
   /* m_Texture->Bind(0);
    m_Shader->SetUniform1i("u_Texture", 0);*/
    m_Shader->SetUniform3f("lightColor", lightColor);
    m_Shader->SetUniform3f("lightPos", lightPos);
    m_Shader->SetUniform3f("viewPos", viewPos);
    m_Shader->SetUniform3f("material.ambient", glm::vec3(0.5f, 0.5f, 0.5f));
    m_Shader->SetUniform3f("material.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
    m_Shader->SetUniform3f("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    m_Shader->SetUniform1f("material.shininess", 32.0f);
    m_model.Draw(*m_Shader);

   /* Renderer renderer;
    renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);*/

    // Render all children
    for (auto& child : m_Children) {
        child->Render(globalTransform, projection, view, lightPos, lightColor, viewPos);
    }
}