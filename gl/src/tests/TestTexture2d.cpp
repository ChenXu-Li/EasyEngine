#include "TestTexture2d.h"
#include "Renderer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
test::TestTexture2d::TestTexture2d()
{
    GLfloat vertices[] = {
        // 位置              // 颜色          //纹理
     -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f,// 左下
     0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 0.0f,   2.0f, 0.0f,// 右下
     0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 1.0f,   2.0f, 2.0f,//  右上
    -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 2.0f,// 左上    
    };
    GLuint indices[] = {  // Note that we start from 0!
        0, 1, 2, // First Triangle
         2, 3,0// Second Triangle
    };

    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC1_ALPHA));
    GLCall(glEnable(GL_BLEND));

    m_Shader = std::make_unique<Shader>("res/shader/tex.shaderg");
    m_VAO = std::make_unique<VertexArray>();
    m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);
    m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, 4 * 8 * sizeof(float));
    //VertexBuffer vb(vertices, 4 * 8 * sizeof(float));

    VertexBufferLayout vblayout;
    vblayout.Push<float>(3);
    vblayout.Push<float>(3);
    vblayout.Push<float>(2);

    //VertexArray va;
    m_VAO->AddBuffer(*m_VertexBuffer, vblayout);

    //IndexBuffer ib(indices, 6);

    //Shader sh("res/shader/tex.shaderg");

    m_Texture1 = std::make_unique<Texture>("res/texture/1.png");
    m_Texture2 = std::make_unique<Texture>("res/texture/2.png");
    //Texture textureA("res/texture/2.png");
    //Texture textureB("res/texture/3.png");
    //textureA.Bind(0);
    //textureB.Bind(2);

    /*sh.Bind();
    sh.SetUniform1i("u_TextureA", 0);
    sh.SetUniform1i("u_TextureB", 2);*/


}

test::TestTexture2d::~TestTexture2d()
{
}

void test::TestTexture2d::OnUpdate(float deltaTime)
{
}

void test::TestTexture2d::OnRender()
{

    GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
    m_Texture1->Bind(0);
    m_Texture2->Bind(1);
    m_Shader->Bind();
    m_Shader->SetUniform1i("u_TextureA", 0);
    m_Shader->SetUniform1i("u_TextureB", 1);
    Renderer renderer;
    renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
}

void test::TestTexture2d::OnImGuiRender()
{
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}
