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
#include <queue>
namespace test {
	class TestStringArt : public Test
	{
	public:
		TestStringArt();
		~TestStringArt();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender()override;
		
		
	private:
		void addLine(int a,int b);
		void upadteBuffer();
		void drawGraph();
		void drawPoints();
		std::vector<GLfloat> m_Vertices;
		std::vector<std::tuple<GLfloat, GLfloat, GLfloat>> m_Points;
		std::vector<std::tuple<int, int>> m_Graph;

		std::unique_ptr<Shader> m_Shader;

		bool need_fresh_buffer_flag;
		GLuint m_numPoints;
		GLuint m_VBO;
		GLuint m_VAO;

		GLuint m_VBO2;
		GLuint m_VAO2;


	};
}



