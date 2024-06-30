#include "TestGameObject.h"
//#include "Renderer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <iostream>

extern GLuint WIDTH, HEIGHT;
//extern GLFWwindow* MainWindow;


// Is called whenever a key is pressed/released via GLFW
extern void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
//GameObject rootObject;
test::TestGameObject::~TestGameObject()
{
    /*  GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC1_ALPHA));
      GLCall(glEnable(GL_BLEND));*/
    GLCall(glDisable(GL_DEPTH_TEST));
}
test::TestGameObject::TestGameObject()
{
   /* GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC1_ALPHA));
    GLCall(glEnable(GL_BLEND));*/
    GLCall(glEnable(GL_DEPTH_TEST));

    CreateScene();

}

void test::TestGameObject::CreateScene()
{
    m_camera = Camera(glm::vec3(0.0f, 30.0f, 30.0f));
    m_light = Light("light");
    m_light.SetPosition(glm::vec3(0.0f, 20.0f, 0.0f));
    m_light.SetLightColor(glm::vec3(1.0f, 1.0f, 1.0f));

    m_rootObject = std::make_unique<GameObject>("SceneRoot");
    m_rootObject->SetPosition(glm::vec3(0.0f, 0.0f, -1.0f));

 

    auto objectM = std::make_shared <GameObjectModel>("Man");
    objectM->SetPosition(glm::vec3(0.0f, 0.0f, 10.0f));
    objectM->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
    m_rootObject->AddChild(objectM);
    // 创建对象 A
    auto objectA = std::make_shared<GameObject>("A");
    objectA->SetPosition(glm::vec3(-14.5f, 0.0f, 0.0f));
    m_rootObject->AddChild(objectA);

    // 在对象 A 下创建29个1x12x1的方块
    for (int i = 0; i < 29; ++i) {
        auto cube = std::make_shared<GameObjectCube>("A_" + std::to_string(i + 1));
        cube->SetPosition(glm::vec3(1.0f * i, 6.0f, 0.0f)); // 设置每个方块的位置
        cube->SetScale(glm::vec3(1.0f, 12.0f, 1.0f)); // 设置方块的大小为1x12x1
        objectA->AddChild(cube);
    }

    // 创建对象 B
    auto objectB = std::make_shared<GameObject>("B");
    objectB->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    m_rootObject->AddChild(objectB);

    // 创建对象 B1
    auto objectB1 = std::make_shared<GameObject>("B1");
    objectB1->SetPosition(glm::vec3(-4.5f, -2.5f, 3.0f));
    objectB->AddChild(objectB1);

    // 在对象 B1 下创建 10 排 9 列 1x5x1 的舞台块
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 9; ++j) {
            auto stageBlock = std::make_shared<GameObjectCube>("B1_" + std::to_string(i) + "_" + std::to_string(j));
            stageBlock->SetPosition(glm::vec3(1.0f * j, 0.0f, 1.0f * i)); // 设置每个舞台块的位置
            stageBlock->SetScale(glm::vec3(1.0f, 5.0f, 1.0f)); // 设置舞台块的大小为1x5x1
            objectB1->AddChild(stageBlock);
        }
    }
    // 创建对象 B2
    auto objectB2 = std::make_shared<GameObject>("B2");
    objectB2->SetPosition(glm::vec3(-7.5f, -1.5f, 0.0f));
    objectB->AddChild(objectB2);

    // 在对象 B2 下创建 4 排 15 列 1x3x1 的舞台块
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 15; ++j) {
            auto stageBlock = std::make_shared<GameObjectCube>("B2_" + std::to_string(i) + "_" + std::to_string(j));
            stageBlock->SetPosition(glm::vec3(1.0f * j, 0.0f, 1.0f * i)); // 设置每个舞台块的位置
            stageBlock->SetScale(glm::vec3(1.0f, 3.0f, 1.0f)); // 设置舞台块的大小为1x3x1
            objectB2->AddChild(stageBlock);
        }
    }

    // 创建对象 C
    auto objectC = std::make_shared<GameObject>("C");
    objectC->SetPosition(glm::vec3(0.0f, 5.5f, 2.0f));
    m_rootObject->AddChild(objectC);

    // 在对象 C 下创建 8 块 3x11x0.5 的舞台块，左右各 4 块
    for (int i = 0; i < 4; ++i) {
        // 左边
        auto stageBlockLeft = std::make_shared<GameObjectCube>("C_Left_" + std::to_string(i));
        stageBlockLeft->SetPosition(glm::vec3(-12.0f, 0.0f, 1.5f * i)); // 设置每个舞台块的位置
        stageBlockLeft->SetScale(glm::vec3(3.0f, 11.0f, 0.5f)); // 设置舞台块的大小为3x11x0.5
        objectC->AddChild(stageBlockLeft);

        // 右边
        auto stageBlockRight = std::make_shared<GameObjectCube>("C_Right_" + std::to_string(i));
        stageBlockRight->SetPosition(glm::vec3(12.0f, 0.0f, 1.5f * i)); // 设置每个舞台块的位置
        stageBlockRight->SetScale(glm::vec3(3.0f, 11.0f, 0.5f)); // 设置舞台块的大小为3x11x0.5
        objectC->AddChild(stageBlockRight);
    }
   
}


void test::TestGameObject::OnUpdate(float deltaTime)
{
    m_camera.OnUpdate(deltaTime);
    m_light.Update(deltaTime);
 
   // m_model = glm::rotate(m_model, glm::radians(0.5f), glm::vec3(0.0, 0.0, 1.0));

    m_view = m_camera.GetViewMatrix();

    m_projection = glm::perspective(m_camera.Zoom, (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);

    m_rootObject->Update(deltaTime);
}

void test::TestGameObject::OnRender()
{
    GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    
    m_light.Render(glm::mat4(1.0f), m_projection, m_view);

    m_rootObject->Render(glm::mat4(1.0f), m_projection, m_view, m_light.GetPosition(), m_light.GetLightColor(), m_camera.Position);
}

void test::TestGameObject::OnImGuiRender()
{
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    m_camera.OnImGuiRender();
    m_light.ImGuiRender();
    m_rootObject->ImGuiRender();


}
