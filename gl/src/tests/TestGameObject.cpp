#include "TestGameObject.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <iostream>
#include "Transform.h"
extern GLuint WIDTH, HEIGHT;
extern MouseState mouse_state;
test::TestGameObject::~TestGameObject()
{

    GLCall(glDisable(GL_DEPTH_TEST));
}
test::TestGameObject::TestGameObject()
{
    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glDepthFunc(GL_LESS));
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

    //auto bb = std::make_shared<GameObjectCube>("ffff");
    //bb->SetPosition(glm::vec3(-12.0f, 0.0f, 1.5f)); // ����ÿ����̨���λ��
    //bb->SetScale(glm::vec3(3.0f, 11.0f, 0.5f)); // ������̨��Ĵ�СΪ3x11x0.5
    //m_rootObject->AddChild(bb);
     //�������� A
    auto objectA = std::make_shared<GameObject>("A");
    objectA->SetPosition(glm::vec3(-14.5f, 0.0f, 0.0f));
    m_rootObject->AddChild(objectA);

     //�ڶ��� A �´���29��1x12x1�ķ���
    for (int i = 0; i < 29; ++i) {
        auto cube = std::make_shared<GameObjectCube>("A_" + std::to_string(i + 1));
        cube->SetPosition(glm::vec3(1.0f * i, 6.0f, 0.0f)); // ����ÿ�������λ��
        cube->SetScale(glm::vec3(1.0f, 12.0f, 1.0f)); // ���÷���Ĵ�СΪ1x12x1
        objectA->AddChild(cube);
    }

    // �������� B
    auto objectB = std::make_shared<GameObject>("B");
    objectB->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    m_rootObject->AddChild(objectB);

    // �������� B1
    auto objectB1 = std::make_shared<GameObject>("B1");
    objectB1->SetPosition(glm::vec3(-4.5f, -2.5f, 5.0f));
    objectB->AddChild(objectB1);

    // �ڶ��� B1 �´��� 10 �� 9 �� 1x5x1 ����̨��
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 9; ++j) {
            auto stageBlock = std::make_shared<GameObjectCube>("B1_" + std::to_string(i) + "_" + std::to_string(j));
            stageBlock->SetPosition(glm::vec3(1.0f * j, 0.0f, 1.0f * i)); // ����ÿ����̨���λ��
            stageBlock->SetScale(glm::vec3(1.0f, 5.0f, 1.0f)); // ������̨��Ĵ�СΪ1x5x1
            objectB1->AddChild(stageBlock);
        }
    }
    // �������� B2
    auto objectB2 = std::make_shared<GameObject>("B2");
    objectB2->SetPosition(glm::vec3(-7.5f, -1.5f, 1.0f));
    objectB->AddChild(objectB2);

    // �ڶ��� B2 �´��� 4 �� 15 �� 1x3x1 ����̨��
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 15; ++j) {
            auto stageBlock = std::make_shared<GameObjectCube>("B2_" + std::to_string(i) + "_" + std::to_string(j));
            stageBlock->SetPosition(glm::vec3(1.0f * j, 0.0f, 1.0f * i)); // ����ÿ����̨���λ��
            stageBlock->SetScale(glm::vec3(1.0f, 3.0f, 1.0f)); // ������̨��Ĵ�СΪ1x3x1
            objectB2->AddChild(stageBlock);
        }
    }

     //�������� C
    auto objectC = std::make_shared<GameObject>("C");
    objectC->SetPosition(glm::vec3(0.0f, 5.5f, 2.0f));
    m_rootObject->AddChild(objectC);

    // �ڶ��� C �´��� 8 �� 3x11x0.5 ����̨�飬���Ҹ� 4 ��
    for (int i = 0; i < 4; ++i) {
        // ���
        auto stageBlockLeft = std::make_shared<GameObjectCube>("C_Left_" + std::to_string(i));
        stageBlockLeft->SetPosition(glm::vec3(-12.0f, 0.0f, 1.5f * i)); // ����ÿ����̨���λ��
        stageBlockLeft->SetScale(glm::vec3(3.0f, 11.0f, 0.5f)); // ������̨��Ĵ�СΪ3x11x0.5
        objectC->AddChild(stageBlockLeft);

        // �ұ�
        auto stageBlockRight = std::make_shared<GameObjectCube>("C_Right_" + std::to_string(i));
        stageBlockRight->SetPosition(glm::vec3(12.0f, 0.0f, 1.5f * i)); // ����ÿ����̨���λ��
        stageBlockRight->SetScale(glm::vec3(3.0f, 11.0f, 0.5f)); // ������̨��Ĵ�СΪ3x11x0.5
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
    ImGui::Text("Mouse Position: (%.1f, %.1f)", mouse_state.x, mouse_state.y);
    ImGui::Text("Mouse Position ZBuffer: (%.8f)", mouse_state.z_buffer);
    glm::vec3 temp = ScreenToModel::screenToWorld(mouse_state.x, mouse_state.y, mouse_state.z_buffer, WIDTH, HEIGHT, m_projection, m_view);
    ImGui::Text("Point World Position: (%.1f, %.1f,  %.1f)", temp.x, temp.y, temp.z);


    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    m_camera.OnImGuiRender();
    m_light.ImGuiRender();
    m_rootObject->ImGuiRender();


}
