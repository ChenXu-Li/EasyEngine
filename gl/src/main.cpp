#include "test.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <filesystem>

int main() {

    // Print the current working directory
    std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;

	// Define relative path to the audio file
	std::string relativeAudioPath = "res/audio/forher.mp3";

	// Convert relative path to absolute path using filesystem (C++17)
	std::filesystem::path absPath = std::filesystem::absolute(relativeAudioPath);

    // Print the absolute path
    std::cout << "Absolute path to audio file: " << absPath << std::endl;

	sf::SoundBuffer buffer;
	if (!buffer.loadFromFile(relativeAudioPath))
		return -1;
	
	sf::Sound sound;
	sound.setBuffer(buffer);
	sound.play();
	test();
	return 0;
   
}