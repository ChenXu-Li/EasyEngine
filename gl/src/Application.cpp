#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "renderer.h"
//#include "IndexBuffer.h"
//#include "VertexBuffer.h"
//#include "VertexArray.h"
//#include "VertexBufferLayout.h"
//#include "Shader.h"
//#include "Texture.h"
//#include "glm/glm.hpp"
//#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "tests/TestClearColor.h"
#include "tests/TestTexture2d.h"
#include "tests/TestMatrix.h"
//#include "tests/TestMusicFFT.h"
#include "tests/TestFractal.h"
#include "tests/TestStringArt.h"
#include "tests/TestCamera.h"
#include "tests/TestGameObject.h"
#include "ControlState.h"
// Function prototypes
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
// Window dimensions
GLuint WIDTH = 1000, HEIGHT = 1000;
GLFWwindow* MainWindow;
bool keys[1024];
MouseState mouse_state;

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
    MainWindow = window;
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
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    //glewExperimental = GL_TRUE;
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

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 130";
    ImGui_ImplOpenGL3_Init(glsl_version);

    memset(mouse_state.buttons, 0, sizeof(mouse_state.buttons));
    mouse_state.x = mouse_state.y = mouse_state.originX = mouse_state.originY = mouse_state.offsetX = mouse_state.offsetY = 0.0;
    mouse_state.last_state_click = false;
    return window;
}
void test_framework(GLFWwindow* window)
{


    test::Test* currentTest = nullptr;
    test::Testmenu* testMenu = new test::Testmenu(currentTest);
    currentTest = testMenu;
    testMenu->RegisterTest<test::TestClearColor>("Test Clear Color");
    testMenu->RegisterTest<test::TestTexture2d>("Test Texture Combine");
    testMenu->RegisterTest<test::TestMatrix>("Test Matrix Transfer");
    //testMenu->RegisterTest<test::TestMusicFFT>("Test Music FFT");
    testMenu->RegisterTest<test::TestFractal>("Test Fractal");
    testMenu->RegisterTest<test::TestStringArt>("Test StringArt");
    testMenu->RegisterTest<test::TestCamera>("Test Camera");
    testMenu->RegisterTest<test::TestGameObject>("Test GameObject"); 
    

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // 
    double lastTime = glfwGetTime();
    double currentTime;
    float deltaTime;
    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        //GLCall(glClear(GL_COLOR_BUFFER_BIT));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        currentTime = glfwGetTime();
        deltaTime = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        if (currentTest) {
            currentTest->OnUpdate(deltaTime);
            currentTest->OnRender();
            ImGui::Begin("Test");
            if (currentTest != testMenu && ImGui::Button("<-")) {
                delete currentTest;
                currentTest = testMenu;
            }
            currentTest->OnImGuiRender();
            ImGui::End();
        }
        ImGui::Begin("Mouse State");
        ImGui::Text("Mouse Position: (%.1f, %.1f)", mouse_state.x, mouse_state.y);
        ImGui::Text("Mouse Position ZBuffer: (%.8f)", mouse_state.z_buffer);
        ImGui::Text("Left Button Pressed: %s", mouse_state.buttons[GLFW_MOUSE_BUTTON_LEFT] ? "Yes" : "No");
        ImGui::Text("Mouse Offset: (%.1f, %.1f)", mouse_state.offsetX, mouse_state.offsetY);
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        

        glfwSwapBuffers(window);
    }
    delete currentTest;
    if (currentTest != testMenu) {
        delete testMenu;
    }
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
}
void main() {

    test_framework(env_init());
}
// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    //std::cout << key << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button >= 0 && button < GLFW_MOUSE_BUTTON_LAST) {
        if (action == GLFW_PRESS) {
            mouse_state.buttons[button] = true;
        }
        else if (action == GLFW_RELEASE) {
            mouse_state.buttons[button] = false;
        }
    }
    
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    mouse_state.x = xpos;
    mouse_state.y = ypos;
    float z_value;
    GLCall(glReadPixels(int(xpos), int(HEIGHT - ypos), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z_value));
    mouse_state.z_buffer = z_value;

    if (mouse_state.buttons[GLFW_MOUSE_BUTTON_LEFT] == true&& mouse_state.last_state_click == false) {//¸Õµã°´
        mouse_state.last_state_click = true;
        std::cout << "aaaaa";
        mouse_state.originX = xpos;
        mouse_state.originY = ypos;
    }
    if (mouse_state.buttons[GLFW_MOUSE_BUTTON_LEFT] == true) {
        mouse_state.offsetX = xpos - mouse_state.originX;
        mouse_state.offsetY = ypos - mouse_state.originY;
    }
    if (mouse_state.buttons[GLFW_MOUSE_BUTTON_LEFT] == false && mouse_state.last_state_click == true) {//¸ÕËÉ¿ª
        mouse_state.last_state_click = false;
        mouse_state.offsetX = mouse_state.offsetY = 0;
        
    }
}
