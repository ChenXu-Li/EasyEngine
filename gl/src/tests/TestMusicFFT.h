#pragma once
#include "Test.h"
#include <memory>
#include"IndexBuffer.h"
#include"VertexBuffer.h"
#include "VertexBufferLayout.h"
#include"Shader.h"
#include"VertexArray.h"
#include"Texture.h"
#include "FFTW/fftw3.h"
namespace test {

	class TestMusicFFT : public Test
	{
	public:
		TestMusicFFT();
		~TestMusicFFT();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender()override;
		void OnStart()override;
		bool loadAudioData(const std::string& filename, std::vector<float>& samples, unsigned int& sampleRate);
	private:
		std::unique_ptr<VertexArray> m_VAO;
		std:: unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std:: unique_ptr<Shader> m_Shader;
		std::unique_ptr<std::vector<float>> m_samples;
		std::unique_ptr<std::vector<float>> m_channel;
		unsigned int m_sampleRate;
		int m_N;
		fftw_complex* m_in;
		fftw_complex* m_out;
		fftw_plan m_plan;

		glm::mat4 m_model;
		glm::mat4 m_view;
		glm::mat4 m_projection;
		glm::mat4 m_mvp;
		float m_r;
		float m_radius;
		float m_angle_x;
		float m_angle_y;
		float m_increment;

	};


}

