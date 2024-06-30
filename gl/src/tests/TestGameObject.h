#pragma once
#include "Test.h"
#include <memory>
#include"IndexBuffer.h"
#include"VertexBuffer.h"
#include "VertexBufferLayout.h"
#include"Shader.h"
#include"VertexArray.h"
#include"Texture.h"
#include <Camera.h>
#include "Light.h"
#include "GameObject.h"
#include "GameObjectCube.h"
#include "GameObjectModel.h"
#include <GLFW/glfw3.h>
namespace test {
	class TestGameObject : public Test
	{
	public:

		TestGameObject();
		~TestGameObject();
		void CreateScene();
		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender()override;
	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<Shader> m_Shader;
		//glm::mat4 m_model;
		glm::mat4 m_view;
		glm::mat4 m_projection;

		Camera m_camera;
		Light  m_light;
		std::unique_ptr<GameObject> m_rootObject;

	};
}
