#include "TestStringArt.h"
#define M_PI 3.1415926f
test::TestStringArt::TestStringArt() : flag(true),need_fresh_buffer_flag(false) {


    // 将初始顶点数据插入到成员变量 m_Vertices 中
    m_Shader = std::make_unique<Shader>("res/shader/bbase.shaderg");

    // Generate and bind the Vertex Array Object (VAO)
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // Generate and bind the Vertex Buffer Object (VBO)
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(GLfloat), m_Vertices.data(), GL_DYNAMIC_DRAW);


    // Specify the layout of the vertex data
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

    // Unbind the VAO to prevent unintended modifications
    glBindVertexArray(0);
  

    std::cout << "Generating Koch snowflake!" << std::endl;

    // Initial triangle vertices
    GLfloat ax = -0.5f, ay = -0.5f, az = 0.0f;
    GLfloat bx = 0.5f, by = -0.5f, bz = 0.0f;
    GLfloat cx = 0.0f, cy = sqrt(3.0f) / 2 - 0.5f, cz = 0.0f;

    //int iterations = 10; // Number of iterations for the Koch snowflake
    //generateKochSnowflake(ax, ay, az, bx, by, bz, iterations);
    //generateKochSnowflake(bx, by, bz, cx, cy, cz, iterations);
    //generateKochSnowflake(cx, cy, cz, ax, ay, az, iterations);
    m_Queue.emplace(ax, ay, az, bx, by, bz, 0);
    m_Queue.emplace(bx, by, bz, cx, cy, cz, 0);
    m_Queue.emplace(cx, cy, cz, ax, ay, az, 0);
}

test::TestStringArt::~TestStringArt() {

    glDeleteBuffers(1, &m_VBO);
    glDeleteVertexArrays(1, &m_VAO);
}

void test::TestStringArt::OnUpdate(float deltaTime) {
    static float sum_t = 0;
    static int iter = 0;
    sum_t += deltaTime;
    if (sum_t > 2) {
        sum_t = 0;
        upadteLines(iter++);
    }
    if (flag) {
        flag = false;
        
    }
    

    //std::cout << "deltaTime:" << deltaTime<<"sum:"<<ff<<std::endl;
}

void test::TestStringArt::OnRender() {
    drawLines();
}

void test::TestStringArt::OnImGuiRender() {
}

void test::TestStringArt::drawLines() {
    if (need_fresh_buffer_flag) {
        need_fresh_buffer_flag = false;
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(GLfloat), m_Vertices.data(), GL_DYNAMIC_DRAW);
    }
    m_Shader->Bind();
    glBindVertexArray(m_VAO);

    // Draw lines between vertices (using GL_LINE_LOOP to close the shape)
    glDrawArrays(GL_LINES, 0, m_Vertices.size());

    // Unbind the VAO
    glBindVertexArray(0);
}
void test::TestStringArt::addLine(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2){
    // add the new vertices to the list
    m_Vertices.push_back(x1);
    m_Vertices.push_back(y1);
    m_Vertices.push_back(z1);

    m_Vertices.push_back(x2);
    m_Vertices.push_back(y2);
    m_Vertices.push_back(z2);
    need_fresh_buffer_flag = true;

}
void test::TestStringArt::generateKochSnowflake(GLfloat ax, GLfloat ay, GLfloat az, GLfloat bx, GLfloat by, GLfloat bz, int iterations) {
    if (iterations == 0) {
        addLine(ax, ay, az, bx, by, bz);
        return;
    }

    GLfloat mx = (2 * ax + bx) / 3;
    GLfloat my = (2 * ay + by) / 3;
    GLfloat mz = (2 * az + bz) / 3;

    GLfloat nx = (2 * bx + ax) / 3;
    GLfloat ny = (2 * by + ay) / 3;
    GLfloat nz = (2 * bz + az) / 3;

    GLfloat ox = mx + (nx - mx) * cos(M_PI / 3) - (ny - my) * sin(M_PI / 3);
    GLfloat oy = my + (nx - mx) * sin(M_PI / 3) + (ny - my) * cos(M_PI / 3);
    GLfloat oz = mz;

    generateKochSnowflake(ax, ay, az, mx, my, mz, iterations - 1);
    generateKochSnowflake(mx, my, mz, ox, oy, oz, iterations - 1);
    generateKochSnowflake(ox, oy, oz, nx, ny, nz, iterations - 1);
    generateKochSnowflake(nx, ny, nz, bx, by, bz, iterations - 1);
}
void test::TestStringArt::upadteLines(int iter) {
    m_Vertices.clear();
    while (!m_Queue.empty()) {
        auto [ax, ay, az, bx, by, bz, i] = m_Queue.front();
        

        if (i == iter) {
            m_Queue.pop();
            addLine(ax, ay, az, bx, by, bz);
        }
        else if(i == iter+1) {
            break;
        }
        else {
            std::cout << "wrong" << std::endl;
        }

        GLfloat mx = (2 * ax + bx) / 3;
        GLfloat my = (2 * ay + by) / 3;
        GLfloat mz = (2 * az + bz) / 3;

        GLfloat nx = (2 * bx + ax) / 3;
        GLfloat ny = (2 * by + ay) / 3;
        GLfloat nz = (2 * bz + az) / 3;

        /*GLfloat ox = mx + (nx - mx) * cos(M_PI / 3) - (ny - my) * sin(M_PI / 3);
        GLfloat oy = my + (nx - mx) * sin(M_PI / 3) + (ny - my) * cos(M_PI / 3);*/
        GLfloat ox = mx/2 + nx/2  + (ny - my) * sin(M_PI / 3);
        GLfloat oy = my/2+ny/2 - (nx - mx) * sin(M_PI / 3) ;
        GLfloat oz = mz;

        m_Queue.emplace(ax, ay, az, mx, my, mz, iter + 1);
        m_Queue.emplace(mx, my, mz, ox, oy, oz, iter + 1);
        m_Queue.emplace(ox, oy, oz, nx, ny, nz, iter + 1);
        m_Queue.emplace(nx, ny, nz, bx, by, bz, iter + 1);
    }

}