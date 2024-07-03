#include "TestGameObject.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <iostream>
#include "Transform.h"

#include<random>

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

 
/*创建
    auto objectM = std::make_shared <GameObjectModel>("Man");
    objectM->SetPosition(glm::vec3(0.0f, 0.0f, 10.0f));
    //objectM->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
    m_rootObject->AddChild(objectM);
     
 */   
     //创建对象 A
    auto objectA = std::make_shared<GameObject>("A");
    objectA->SetPosition(glm::vec3(-14.5f, 0.0f, 0.0f));
    m_rootObject->AddChild(objectA);

     //在对象 A 下创建29个1x12x1的方块
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
    objectB1->SetPosition(glm::vec3(-4.5f, -2.5f, 5.0f));
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
    objectB2->SetPosition(glm::vec3(-7.5f, -1.5f, 1.0f));
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

     //创建对象 C
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
   




    ///************************************************************
    // 创建对象 D
    auto objectD = std::make_shared<GameObject>("D");
    objectD->SetPosition(glm::vec3(0.0f,0.0f, 2.0f));
    m_rootObject->AddChild(objectD);

    // 假设你有一个常量来表示圆的半径和方块的数量
    const float radius = 3.0f; // 圆的半径  
    const int numBlocks = 16; // 方块的数量  
    const float angleIncrement = 2.0f * glm::pi<float>() / numBlocks; // 每个方块之间的角度增量  



    // ... (之前的代码保持不变)  

    // 在对象D 下创建 numBlocks 块的舞台块，组成环形  
    for (int i = 0; i < numBlocks; ++i) {
        // 计算当前方块的角度  
        float angle = i * angleIncrement;

        // 创建舞台块  
        auto stageBlock = std::make_shared <GameObjectModel>("Man" + std::to_string(i));
       // auto stageBlock = std::make_shared<GameObjectCube>("D_Block_" + std::to_string(i));
        my_Actors.emplace_back(stageBlock);
        // 计算舞台块在圆上的位置（x和y坐标）  
        glm::vec3 circlePosition(radius * std::cos(angle), 0.0f, radius * std::sin(angle)); // z坐标为0，确保环形在xy平面上  

        // 设置每个舞台块的位置（这里我们可能想要让方块稍微升高一点，以便它们不会重叠到地面）  
        stageBlock->SetPosition(circlePosition + glm::vec3(0.0f, -3.0f, 3.0f)); // 假设y轴向上，这里+0.5f是为了让方块稍微离开地面  


        vec3Array[i] = circlePosition + glm::vec3(0.0f, 0.0f, 3.0f);

        vec3Angle[i] = glm::vec3(0.0f, 0.0f,0.0f);

        // 设置舞台块的大小（注意：SetScale中的值看起来像是缩放因子，而不是直接的大小）  
        stageBlock->SetScale(glm::vec3(0.5f, 0.5f, 0.5f)); // 假设这是缩放因子，不是直接大小  

        // 设置舞台块的旋转（如果需要的话，这里绕z轴旋转一个固定角度或根据需要进行旋转）  
        // 例如，让方块与环形平面平行（通常不需要额外旋转，除非有特定需求）  
        //stageBlock->SetRotation(/* 绕z轴旋转的四元数或欧拉角 */);  

        // 将舞台块添加到对象D中  
        objectD->AddChild(stageBlock);
    }

    //printf("我被回调了");
           ///************************************************************


}



bool isColliding(const glm::vec3& a, const glm::vec3& b, float cubeSize) {
    // 检查两个方块是否碰撞  
    float halfSize = cubeSize / 2.0f;
    if (std::abs(a.x - b.x) <= halfSize && std::abs(a.z - b.z) <= halfSize) {
        return true;
    }
    return false;
}

void resolveCollision(glm::vec3& a, glm::vec3& b, float cubeSize) {
    // 假设gridSizeX和gridSizeZ是网格的大小  
    // 这里我们假设Y轴位置不参与碰撞检测，只考虑XZ平面  

    // 计算两个方块中心的距离  
    glm::vec2 diff = glm::vec2(a.x - b.x, a.z - b.z);

    // 确定移动方向（选择X轴或Z轴，这里假设优先选择X轴）  
    bool moveX = std::abs(diff.x) > std::abs(diff.y);

    // 计算移动量，这里我们简单地移动到相邻的网格位置  
    float moveAmount = (moveX ? 1.0f : 1.0f) * cubeSize; // 如果是X轴，则移动一个网格宽度；如果是Z轴，则移动一行网格的宽度  

    // 确定移动方向（正或负）  
    if (diff.x < 0 && moveX || diff.y < 0 && !moveX) {
        moveAmount = -moveAmount; // 如果需要向左或向下移动，则取负值  
    }

    // 移动方块以避免碰撞  
    if (moveX) {
        a.x += moveAmount; // 或者可以是b.x，取决于你希望移动哪个方块  
    }
    else {
        a.z += moveAmount; // 或者可以是b.z  
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


    ///************************************************************
    // 更新经过的时间  
    elapsedTimeSinceLastMove += deltaTime;

    // 检查是否应该移动actor  
    if (elapsedTimeSinceLastMove >= MOVE_INTERVAL) {
        // 重置经过的时间，以便下次从0开始计数  
        elapsedTimeSinceLastMove -= MOVE_INTERVAL;
    }
    else

        return;

    step++;
    for (int i = 0; i < 16; i++) {

        //更新
        if (step % step_action == 0) {
            // 设置随机种子（可选，通常基于当前时间）  
            std::random_device rd;
            std::mt19937 gen(rd());

            // 创建一个在[0.0, 1.0)之间的均匀分布  
            std::uniform_real_distribution<> dis(-1.0f, 1.0f);

            x_random[i] = dis(gen) * speed;//左右
            y_random[i] = dis(gen) * speed;//前后

        }
        float x = x_random[i];
        float y = y_random[i];

        if (x + vec3Array[i].x < left_border) {
            x += speed * adjust;
        }
        else if (x + vec3Array[i].x > right_border) {

            x -= speed * adjust;
        }
        if (y + vec3Array[i].z < front_border) {
            y += speed * adjust;

        }
        else if (y + vec3Array[i].z > back_border) {
            y -= speed * adjust;
        }


        vec3Array[i] = vec3Array[i] + glm::vec3(x, 0.0f, y);
        vec3Angle[i]= vec3Angle[i]+ glm::vec3(0.0f,x*2,0.0f);
    }


    int size = 16;
    for (int i = 0; i < size; ++i) {
        for (int j = i + 1; j < size; ++j) {
            if (isColliding(vec3Array[i], vec3Array[j], 0.5)) {
                // 假设我们总是移动第二个方块（j）来避免碰撞  
                resolveCollision(vec3Array[j], vec3Array[i], 0.5);
                printf("发生碰撞的演员为%d和%d,他们的坐标分别为(%f,%f),(%f,%f)\n", i + 1, j + 1, vec3Array[i].x, vec3Array[i].z, vec3Array[j].x, vec3Array[j].z);
                // 注意：在实际应用中，可能需要检查新的位置是否仍然与其他方块碰撞  
                // 并可能需要多次迭代来解决复杂的碰撞情况  
            }
        }
    }



    int j = 0;
    for (auto& actor : my_Actors) {
        actor->SetPosition(vec3Array[j]);
        actor->SetRotation(vec3Angle[j++]);
    }
    ///************************************************************

}



void resolveCollision(glm::vec3& a, glm::vec3& b, float cubeSize, int gridSizeX, int gridSizeZ) {
    // 假设gridSizeX和gridSizeZ是网格的大小  
    // 这里我们假设Y轴位置不参与碰撞检测，只考虑XZ平面  

    // 计算两个方块中心的距离  
    glm::vec2 diff = glm::vec2(a.x - b.x, a.z - b.z);

    // 确定移动方向（选择X轴或Z轴，这里假设优先选择X轴）  
    bool moveX = std::abs(diff.x) > std::abs(diff.y);

    // 计算移动量，这里我们简单地移动到相邻的网格位置  
    float moveAmount = (moveX ? 1.0f : gridSizeZ) * cubeSize; // 如果是X轴，则移动一个网格宽度；如果是Z轴，则移动一行网格的宽度  

    // 确定移动方向（正或负）  
    if (diff.x < 0 && moveX || diff.y < 0 && !moveX) {
        moveAmount = -moveAmount; // 如果需要向左或向下移动，则取负值  
    }

    // 移动方块以避免碰撞  
    if (moveX) {
        a.x += moveAmount; // 或者可以是b.x，取决于你希望移动哪个方块  
    }
    else {
        a.z += moveAmount; // 或者可以是b.z  
    }
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
