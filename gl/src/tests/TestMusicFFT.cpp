#include "TestMusicFFT.h"
#include "Renderer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


#include <chrono>
#include <SFML/Audio.hpp>
#include <filesystem>
#include <vector>
#include<iostream>
#include <cmath>

const GLuint WIDTH = 700, HEIGHT = 700;
namespace test {
    TestMusicFFT::TestMusicFFT()
        :m_r(0.5f), m_angle(0.0f), m_increment(0.05f)
    {
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC1_ALPHA));
        GLCall(glEnable(GL_BLEND));

        GLfloat vertices[] = {
            // 位置              // 颜色          //纹理
         -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
         0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 0.0f,   // 右下
         0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 1.0f,   //  右上
        -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   // 左上    
        };
        GLuint indices[] = {  // Note that we start from 0!
            0, 1, 2, // First Triangle
             2, 3,0// Second Triangle
        };

        m_Shader = std::make_unique<Shader>("res/shader/basic.shaderg");
        m_VAO = std::make_unique<VertexArray>();
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);
        m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, 4 * 6 * sizeof(float));


        VertexBufferLayout vblayout;
        vblayout.Push<float>(3);
        vblayout.Push<float>(3);

        m_VAO->AddBuffer(*m_VertexBuffer, vblayout);

        m_projection = glm::mat4(1.0f);
        m_projection = glm::perspective(45.0f, (float)WIDTH / HEIGHT, 0.1f, 100.0f);
        m_view = glm::mat4(1.0f);
        m_view = glm::translate(m_view, glm::vec3(0.0f, 0.0f, -1.0f));
        m_model = glm::mat4(1.0f);
        m_model = glm::scale(m_model, glm::vec3(1.0f, 1.0f, 1.0f));
       
        
        m_N = 128; // Signal length for FFT
        m_in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * m_N);
        m_out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * m_N);
        m_plan = fftw_plan_dft_1d(m_N, m_in, m_out, FFTW_FORWARD, FFTW_ESTIMATE);
        m_channel = std::make_unique<std::vector<float>>(m_N / 2);
        m_samples = std::make_unique<std::vector<float>>();
        OnStart();
    }

    TestMusicFFT::~TestMusicFFT()
    {
    }

    void TestMusicFFT::OnUpdate(float deltaTime)
    {
       /* if (m_r > 1) m_increment = -0.05f;
        if (m_r < 0) m_increment = 0.05f;
        m_r += m_increment;
        m_model = glm::rotate(m_model, glm::radians(0.5f), glm::vec3(0.0, 0.0, 1.0));
        glm::mat4 tmp = glm::rotate(glm::mat4(1.0f), glm::radians(m_angle), glm::vec3(1.0f, 0.0f, 0.0f));
        m_mvp = m_projection * m_view * tmp * m_model;*/
        static std::chrono::high_resolution_clock::time_point beginFrameTime;
        static bool flag = true;
        if (flag) {
            beginFrameTime = std::chrono::high_resolution_clock::now();
            flag = false;
        }
            
        std::chrono::high_resolution_clock::time_point nowFrameTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> timeSpan = duration_cast<std::chrono::duration<float>>(nowFrameTime - beginFrameTime);
        //std::cout << timeSpan.count() << std::endl;
        static int sampleOffset = 0;
        sampleOffset = static_cast<size_t>(timeSpan.count() * m_sampleRate);
        if (sampleOffset + m_N >= (*m_samples).size()) {
            return; // End of samples
        }
        //sampleOffset += 10;
        // Fill the FFT input array with audio samples
        for (int i = 0; i < m_N; ++i) {
            m_in[i][0] = (*m_samples)[sampleOffset + i];
            m_in[i][1] = 0.0;
        }

        // Execute the FFT
        fftw_execute(m_plan);

        // Calculate the magnitude of the FFT output
        static std::vector<double> magnitudes(m_N / 2);
        static std::vector<double> magnitudes_max_history(m_N / 2);
        for (int i = 0; i < (m_N / 2); ++i) {
            magnitudes[i] = sqrt(m_out[i][0] * m_out[i][0] + m_out[i][1] * m_out[i][1]);
            if (magnitudes[i] >= magnitudes_max_history[i]) {
                magnitudes_max_history[i] = magnitudes[i];
                (*m_channel)[i] = 1;
            }
            else {
                (*m_channel)[i] = (float)(magnitudes[i] / magnitudes_max_history[i]);
            }
           //std::cout << (*m_channel)[i] << "-";
        }
        //std::cout << std::endl;
        
    }

    void TestMusicFFT::OnRender()
    {

        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
        /*for (int i = 0; i < (m_N / 2); ++i) {
            
            std::cout << (*m_channel)[i] << "-";
        }
        std::cout << std::endl;*/
        m_Shader->Bind();
        m_Shader->SetUniform1f("ur", m_r);
        Renderer renderer;
        for (int i = 0; i < (m_N / 2); ++i) {

            m_Shader->SetUniformMat4fv("transform", m_projection * m_view * glm::translate(glm::mat4(1.0f), glm::vec3(i*2.0f/m_N-0.5f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f / (m_N / 2), (*m_channel)[i], 1.0f)) * m_model);
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }
        
        
    }

    void TestMusicFFT::OnImGuiRender()
    {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        //ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
        //ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        //ImGui::SliderFloat("float", (float*)&m_angle, 40.0f, -120.0f);    // Edit 3 floats representing a color

       


    }
    void TestMusicFFT::OnStart()
    {

        std::string audioPath = "res/audio/forher.mp3";

        if (!loadAudioData(audioPath, (*m_samples), m_sampleRate)) {
            std::cerr << "Failed to load audio data." << std::endl;
            return ;
        }

        
        //播放音乐----------------
        static sf::SoundBuffer buffer;
        if (!buffer.loadFromFile(audioPath)) {
            std::cerr << "Failed to play audio." << std::endl;
            return;
        }
           
        static sf::Sound sound;
        sound.setBuffer(buffer);
        sound.play();
        
    }
    bool TestMusicFFT::loadAudioData(const std::string& filename, std::vector<float>& samples, unsigned int& sampleRate) {
        sf::SoundBuffer buffer;
        if (!buffer.loadFromFile(filename)) {
            return false;
        }

        const sf::Int16* sampleData = buffer.getSamples();
        sampleRate = buffer.getSampleRate();

        std::size_t sampleCount = buffer.getSampleCount();
        samples.resize(sampleCount);
        for (std::size_t i = 0; i < sampleCount; ++i) {
            samples[i] = sampleData[i] / 32768.0f; // Normalize the sample to the range [-1.0, 1.0]
        }

        return true;
    }

}
