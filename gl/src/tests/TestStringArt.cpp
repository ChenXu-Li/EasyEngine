#include "TestStringArt.h"
#define M_PI 3.1415926f
test::TestStringArt::TestStringArt() : need_fresh_buffer_flag(false) {
    glEnable(GL_PROGRAM_POINT_SIZE);

    m_Shader = std::make_unique<Shader>("res/shader/bbase.shaderg");

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(GLfloat), m_Vertices.data(), GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

    glBindVertexArray(0);

    




    const int numPoints = 500;
    m_numPoints = numPoints;
    const GLfloat radius = 1.0f;

    // 生成1000个点
    for (int i = 0; i < numPoints; ++i) {
        GLfloat angle = 2.0f * M_PI * i / numPoints; // 计算每个点的角度
        GLfloat x = radius * std::cos(angle); // 计算x坐标
        GLfloat y = radius * std::sin(angle); // 计算y坐标
        GLfloat z = 0.0f; // z坐标为0
        m_Points.emplace_back(x, y, z); // 将点加入到向量中
    }

    glGenVertexArrays(1, &m_VAO2);
    glBindVertexArray(m_VAO2);
    std::vector<GLfloat> pointsData;
    for (const auto& point : m_Points) {
        pointsData.push_back(std::get<0>(point));
        pointsData.push_back(std::get<1>(point));
        pointsData.push_back(std::get<2>(point));
    }
    glGenBuffers(1, &m_VBO2);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO2);
    glBufferData(GL_ARRAY_BUFFER, pointsData.size() * sizeof(GLfloat), pointsData.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

    glBindVertexArray(0);
    
}

test::TestStringArt::~TestStringArt() {

    glDeleteBuffers(1, &m_VBO);
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO2);
    glDeleteVertexArrays(1, &m_VAO2);
    glDisable(GL_PROGRAM_POINT_SIZE);
}

void test::TestStringArt::OnUpdate(float deltaTime) {
    static int i = 0;
     
     static float sum_t = 0;
     sum_t += deltaTime;
     if (sum_t > 0.5) {
         sum_t = 0;
         addLine(i, (i + 100) % m_numPoints);
         i+=20;
         i %= m_numPoints;
     }
    
}

void test::TestStringArt::OnRender() {
    drawPoints();
    drawGraph();
}

void test::TestStringArt::OnImGuiRender() {
}


void test::TestStringArt::addLine(int a, int b) {
    // add the new vertices to the list
    m_Graph.emplace_back(a,b);

    m_Vertices.push_back(std::get<0>(m_Points[a]));
    m_Vertices.push_back(std::get<1>(m_Points[a]));
    m_Vertices.push_back(std::get<2>(m_Points[a]));

    m_Vertices.push_back(std::get<0>(m_Points[b]));
    m_Vertices.push_back(std::get<1>(m_Points[b]));
    m_Vertices.push_back(std::get<2>(m_Points[b]));
    need_fresh_buffer_flag = true;

}

void test::TestStringArt::upadteBuffer()
{
   
}

void test::TestStringArt::drawGraph()
{
    if (need_fresh_buffer_flag) {
        need_fresh_buffer_flag = false;
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(GLfloat), m_Vertices.data(), GL_DYNAMIC_DRAW);
    }
    m_Shader->Bind();
    glBindVertexArray(m_VAO);

    glDrawArrays(GL_LINES, 0, m_Vertices.size()/3);
    //glDrawArrays(GL_POINTS, 0, m_Vertices.size());

    glBindVertexArray(0);
}
void test::TestStringArt::drawPoints()
{

    m_Shader->Bind();
    glBindVertexArray(m_VAO2);

    glDrawArrays(GL_POINTS, 0, m_Points.size());

    glBindVertexArray(0);
}

