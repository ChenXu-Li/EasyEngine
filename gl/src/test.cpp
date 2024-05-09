#include <iostream>
//#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
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

    return window;
}
void test_uniform(GLFWwindow* window) {
   

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

    

    VertexBuffer vb(vertices, 4 * 8 * sizeof(float));

    VertexBufferLayout vblayout;
    vblayout.Push<float>(3);
    vblayout.Push<float>(3);
    vblayout.Push<float>(2);

    VertexArray va;
    va.AddBuffer(vb, vblayout);

    IndexBuffer ib(indices, 6);

    Shader sh("res/shader/basic.shaderg");


    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    sh.Unbind();

    Renderer renderer;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 130";
    ImGui_ImplOpenGL3_Init(glsl_version);

    float r = 0.5f;
    float increment = 0.05f;
    //test1
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 trans2 = glm::mat4(1.0f);
    trans2 = glm::translate(trans2, glm::vec3(1.0f, 1.0f, 0.0f));
    std::cout << vec.x << vec.y << vec.z << std::endl;
    vec = trans2 * vec;
    std::cout << vec.x << vec.y << vec.z << std::endl;
    //test2
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0.0, 0.0, 1.0));
    trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
    //test3
    glm::mat4 model = glm::mat4(1.0f);
    //model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 view = glm::mat4(1.0f);
    // 注意，我们将矩阵向我们要进行移动场景的反向移动。
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(45.0f, (float)WIDTH / HEIGHT, 0.1f, 100.0f);
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // Game loop
    bool show_demo_window = false;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        sh.Bind();
        sh.SetUniform1f("ur", r);
        static float f = -60.0f;
        static int counter = 0;
        trans = glm::rotate(trans, glm::radians(0.5f), glm::vec3(0.0, 0.0, 1.0));
        model = glm::rotate(glm::mat4(1.0f), glm::radians(f), glm::vec3(1.0f, 0.0f, 0.0f));
        //trans = glm::rotate(trans, (GLfloat)glfwGetTime() * 50.0f, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 result = projection * view * model*trans;
        sh.SetUniform1Mat4fv("transform", result);

        renderer.Draw(va, ib, sh);

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, -90.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        
        glfwSwapBuffers(window);
        // Swap the screen buffers
        if (r > 1) increment = -0.05f;
        if (r < 0) increment = 0.05f;
        r += increment;
    }

    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
}
// The MAIN function, from here we start the application and run the game loop
void test_tex(GLFWwindow* window)
{
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


    VertexBuffer vb(vertices, 4*8*sizeof(float));

    VertexBufferLayout vblayout;
    vblayout.Push<float>(3);
    vblayout.Push<float>(3);
    vblayout.Push<float>(2);

    VertexArray va;
    va.AddBuffer(vb, vblayout);

    IndexBuffer ib(indices, 6);

    Shader sh("res/shader/tex.shaderg");
    

    sh.Bind();
    Texture texture("res/texture/2.png");
    texture.Bind();


    sh.SetUniform1f("cnm", 0.1f);
    sh.SetUniform1i("u_TextureA", 0);


    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    sh.Unbind();

    Renderer renderer;

    float r = 0.5f;
    float increment = 0.05f;
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        
        renderer.Draw(va, ib, sh);
       
        glfwSwapBuffers(window);
    }

    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
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
    sh.SetUniform1i("u_TextureA",0);
    sh.SetUniform1i("u_TextureB",2);


    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    sh.Unbind();

    Renderer renderer;

   
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderer.Draw(va, ib, sh);

        glfwSwapBuffers(window);
    }

    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
}
void test() {
   
    test_uniform(env_init());
    //test_tex(env_init());
    //test_tex_combine(env_init());
}
// Is called whenever a key is pressed/released via GLFW
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    std::cout << key << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}