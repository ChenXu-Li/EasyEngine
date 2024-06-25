#include "TestMatrix.h"
#include "Renderer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
extern GLuint WIDTH, HEIGHT;
test::TestMatrix::TestMatrix()
    :m_r(0.5f), m_angle(0.0f), m_increment(0.05f)
{
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC1_ALPHA));
    GLCall(glEnable(GL_BLEND));

    GLfloat vertices[] = {
        // 位置              // 颜色          //纹理
     -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f,// 左下
     0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 0.0f,   1.0f, 0.0f,// 右下
     0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 1.0f,   1.0f, 1.0f,//  右上
    -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 1.0f,// 左上    
    };
    GLuint indices[] = {  // Note that we start from 0!
        0, 1, 2, // First Triangle
         2, 3,0// Second Triangle
    };

    m_Shader = std::make_unique<Shader>("res/shader/basic.shaderg");
    m_VAO = std::make_unique<VertexArray>();
    m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);
    m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, 4 * 8 * sizeof(float));
  

    VertexBufferLayout vblayout;
    vblayout.Push<float>(3);
    vblayout.Push<float>(3);
    vblayout.Push<float>(2);

    //VertexArray va;
    m_VAO->AddBuffer(*m_VertexBuffer, vblayout);

    m_projection = glm::mat4(1.0f);
    m_projection = glm::perspective(45.0f, (float)WIDTH / HEIGHT, 0.1f, 100.0f);
    m_view = glm::mat4(1.0f);
    m_view = glm::translate(m_view, glm::vec3(0.0f, 0.0f, -1.0f));
    m_model = glm::mat4(1.0f);
    m_model = glm::rotate(m_model, glm::radians(45.0f), glm::vec3(0.0, 0.0, 1.0));
    m_model = glm::scale(m_model, glm::vec3(0.5, 0.5, 0.5));


}

test::TestMatrix::~TestMatrix()
{
    GLCall(glDisable(GL_BLEND));
    GLCall(glBlendFunc(GL_ONE, GL_ZERO));
}

void test::TestMatrix::OnUpdate(float deltaTime)
{
    if (m_r > 1) m_increment = -0.05f;
    if (m_r < 0) m_increment = 0.05f;
    m_r += m_increment;
    m_model = glm::rotate(m_model, glm::radians(0.5f), glm::vec3(0.0, 0.0, 1.0));
    glm::mat4 tmp = glm::rotate(glm::mat4(1.0f), glm::radians(m_angle), glm::vec3(1.0f, 0.0f, 0.0f));
    m_mvp= m_projection * m_view *tmp* m_model;
}

void test::TestMatrix::OnRender()
{

    GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
  
    m_Shader->Bind();
    m_Shader->SetUniform1f("ur", m_r);
    
    m_Shader->SetUniformMat4fv("transform", m_mvp);
  
    Renderer renderer;
    renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
    
}

void test::TestMatrix::OnImGuiRender()
{
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    //ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
   
    ImGui::SliderFloat("float", (float*)&m_angle, 40.0f, -120.0f);    // Edit 3 floats representing a color

    ////if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
    ////    counter++;
    //ImGui::SameLine();
    ////ImGui::Text("counter = %d", counter);

    
}
