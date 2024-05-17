#pragma once
#include "Test.h"
#include <memory>
#include"IndexBuffer.h"
#include"VertexBuffer.h"
#include "VertexBufferLayout.h"
#include"Shader.h"
#include"VertexArray.h"
#include"Texture.h"
namespace test {

	class TestMatrix : public Test
	{
	public:
		TestMatrix();
		~TestMatrix();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender()override;
	private:
		std::unique_ptr<VertexArray> m_VAO;
		std:: unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std:: unique_ptr<Shader> m_Shader;
		glm::mat4 m_model;
		glm::mat4 m_view;
		glm::mat4 m_projection;
		glm::mat4 m_mvp;
		float m_r ;
		float m_angle;
		float m_increment ;
	};


}

