#include "test.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <filesystem>
#include <vector>
#include <cmath>
#include "FFTW/fftw3.h"

// Function to load audio data
bool loadAudioData(const std::string& filename, std::vector<float>& samples, unsigned int& sampleRate) {
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
int AudioFFTTest() {
    std::vector<float> samples;
    unsigned int sampleRate;
    std::string audioPath = "res/audio/forher.mp3";

    if (!loadAudioData(audioPath, samples, sampleRate)) {
        std::cerr << "Failed to load audio data." << std::endl;
        return -1;
    }

    const int N = 512; // Signal length for FFT
    fftw_complex* in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N);
    fftw_complex* out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N);
    fftw_plan plan = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    sf::RenderWindow window(sf::VideoMode(800, 600), "Audio Visualization");

    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(audioPath))
        return -1;

    sf::Sound sound;
    sound.setBuffer(buffer);
    sound.play();

    sf::Clock clock;
    sf::Clock fpsClock;
    size_t sampleOffset = 0;

    unsigned int frameCount = 0;
    float fps = 0.0f;

    sf::Font font;
    if (!font.loadFromFile("res/fonts/arial.ttf")) {
        std::cerr << "Failed to load font." << std::endl;
        return -1;
    }

    sf::Text fpsText;
    fpsText.setFont(font);
    fpsText.setCharacterSize(24);
    fpsText.setFillColor(sf::Color::White);
    fpsText.setPosition(10.f, 10.f);
    const sf::Time frameTime = sf::seconds(1.f / 60.f);
    while (window.isOpen()) {
        sf::Time frameStart = clock.getElapsedTime();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Calculate the current sample offset based on the playback position
        sf::Time elapsed = clock.getElapsedTime();
        sampleOffset = static_cast<size_t>(elapsed.asSeconds() * sampleRate);

        if (sampleOffset + N >= samples.size()) {
            break; // End of samples
        }

        // Fill the FFT input array with audio samples
        for (int i = 0; i < N; ++i) {
            in[i][0] = samples[sampleOffset + i];
            in[i][1] = 0.0;
        }

        // Execute the FFT
        fftw_execute(plan);

        // Calculate the magnitude of the FFT output
        std::vector<double> magnitudes(N / 2);
        for (int i = 0; i < N / 2; ++i) {
            magnitudes[i] = sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]);
        }

        // Draw the frequency spectrum
        window.clear();
        for (int i = 0; i < N / 2; ++i) {
            float magnitude = magnitudes[i];
            float x = (i * (800 / (N / 2)));
            float y = 600 - (magnitude * 100);
            sf::RectangleShape line(sf::Vector2f(800 / (N / 2), 600 - y));
            line.setPosition(x, y);
            line.setFillColor(sf::Color::Green);
            window.draw(line);
        }

        // Update FPS every second
        frameCount++;
        if (fpsClock.getElapsedTime().asSeconds() >= 1.0f) {
            fps = frameCount / fpsClock.getElapsedTime().asSeconds();
            fpsText.setString("FPS: " + std::to_string(fps));
            frameCount = 0;
            fpsClock.restart();
        }

        // Draw FPS text
        window.draw(fpsText);
        window.display();

        sf::Time frameEnd = clock.getElapsedTime();
        sf::Time frameDuration = frameEnd - frameStart;
        if (frameDuration < frameTime) {
            sf::sleep(frameTime - frameDuration);
        }
    }

    fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out);

}
int main() {
    AudioFFTTest();
    test();
    return 0;
}

//#include "test.h"
//#include <SFML/Graphics.hpp>
//#include <SFML/Audio.hpp>
//#include <SFML/Window.hpp>
//#include <iostream>
//#include <filesystem>
//#include "FFTW/fftw3.h"
//bool loadAudioData(const std::string& filename, std::vector<float>& samples, unsigned int& sampleRate) {
//    sf::SoundBuffer buffer;
//    if (!buffer.loadFromFile(filename)) {
//        return false;
//    }
//
//    const sf::Int16* sampleData = buffer.getSamples();
//    sampleRate = buffer.getSampleRate();
//
//    std::size_t sampleCount = buffer.getSampleCount();
//    samples.resize(sampleCount);
//    for (std::size_t i = 0; i < sampleCount; ++i) {
//        samples[i] = sampleData[i] / 32768.0f; // Normalize the sample to the range [-1.0, 1.0]
//    }
//
//    return true;
//}
//int main() {
//    // �źŲ���
//    const int N = 1024; // �źų���
//    const double dt = 0.01; // �������
//    const double f0 = 10.0; // �ź�Ƶ��
//    const double A = 1.0; // �ź����
//
//    // ����������������
//    fftw_complex* in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N);
//    fftw_complex* out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N);
//
//    // ���� FFTW ������ɢ����Ҷ�任�ļƻ�
//    fftw_plan plan = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
//
//    // �����ź�
//    for (int i = 0; i < N; ++i) {
//        double t = i * dt;
//        in[i][0] = A * std::sin(2.0 * 3.14 * f0 * t); // ʵ��
//        in[i][1] = 0.0; // �鲿
//    }
//
//    // ִ�и���Ҷ�任
//    fftw_execute(plan);
//
//    // ����Ƶ��
//    for (int i = 0; i < N; ++i) {
//        double freq = i / (N * dt); // Ƶ��
//        double magnitude = sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]); // ����
//        std::cout << "Frequency: " << freq << ", Magnitude: " << magnitude << std::endl;
//    }
//
//    // �ͷ���Դ
//    fftw_destroy_plan(plan);
//    fftw_free(in);
//    fftw_free(out);
//
//
//    // Print the current working directory
//    std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;
//
//	// Define relative path to the audio file
//	std::string relativeAudioPath = "res/audio/forher.mp3";
//
//	// Convert relative path to absolute path using filesystem (C++17)
//	std::filesystem::path absPath = std::filesystem::absolute(relativeAudioPath);
//
//    // Print the absolute path
//    std::cout << "Absolute path to audio file: " << absPath << std::endl;
//
//	sf::SoundBuffer buffer;
//	if (!buffer.loadFromFile(relativeAudioPath))
//		return -1;
//	
//	sf::Sound sound;
//	sound.setBuffer(buffer);
//	sound.play();
//	test();
//	return 0;
//   
//}