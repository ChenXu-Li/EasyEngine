#pragma once
#include "Test.h"
#include <memory>
#include"IndexBuffer.h"
#include"VertexBuffer.h"
#include "VertexBufferLayout.h"
#include"Shader.h"
#include"VertexArray.h"
#include"Texture.h"
#include <vector>
namespace test {
	class TestStringArt : public Test
	{
	public:
		TestStringArt();
		~TestStringArt();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender()override;
		void drawLines();
		void addLine(float x1, float y1, float z1, float x2, float y2, float z2);
		void generateKochSnowflake(GLfloat ax, GLfloat ay, GLfloat az, GLfloat bx, GLfloat by, GLfloat bz, int iterations);
	private:
		std::vector<GLfloat> m_Vertices;
		//std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<Shader> m_Shader;
		glm::mat4 m_model;
		glm::mat4 m_view;
		glm::mat4 m_projection;
		glm::mat4 m_mvp;
		bool flag;
		bool need_fresh_buffer_flag;
		GLuint m_VBO;
		GLuint m_VAO;


	};
}



