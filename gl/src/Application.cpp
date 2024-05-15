#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "tests/TestClearColor.h"
// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Window dimensions
const GLuint WIDTH = 600, HEIGHT = 600;

GLFWwindow* env_init() {
    std::cout << "Starting GLFW context, OpenGL 4.3" << std::endl;
    GLint nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return window;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return window;
    }

    // Define the viewport dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);



    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC1_ALPHA));
    GLCall(glEnable(GL_BLEND));

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 130";
    ImGui_ImplOpenGL3_Init(glsl_version);
    return window;
}
void test_tex_combine(GLFWwindow* window)
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


    VertexBuffer vb(vertices, 4 * 8 * sizeof(float));

    VertexBufferLayout vblayout;
    vblayout.Push<float>(3);
    vblayout.Push<float>(3);
    vblayout.Push<float>(2);

    VertexArray va;
    va.AddBuffer(vb, vblayout);

    IndexBuffer ib(indices, 6);

    Shader sh("res/shader/tex.shaderg");


    Texture textureA("res/texture/2.png");
    Texture textureB("res/texture/3.png");
    textureA.Bind(0);
    textureB.Bind(2);

    sh.Bind();
    sh.SetUniform1i("u_TextureA", 0);
    sh.SetUniform1i("u_TextureB", 2);


    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    sh.Unbind();

    Renderer renderer;

    test::TestClearColor test;
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

   

        test.OnUpdate(0.0f);
        test.OnRender();
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        test.OnImGuiRender();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        //renderer.Draw(va, ib, sh);

        glfwSwapBuffers(window);
    }

    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
}
void main() {

    test_tex_combine(env_init());
}
// Is called whenever a key is pressed/released via GLFW
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    std::cout << key << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}