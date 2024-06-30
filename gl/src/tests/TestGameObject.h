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

		// 创建一个包含16个std::unique_ptr<GameObjectCube>的vector
		std::vector<std::shared_ptr<GameObjectCube>> my_Actors;
		//存储演员坐标
		glm::vec3 vec3Array[16];

		float elapsedTimeSinceLastMove = 0.0f; // 自上次移动以来经过的时间  
		const float MOVE_INTERVAL = 0.05f; // 设置移动间隔为1秒  
		float speed = 0.05;//演员移动速度
		float adjust = 0.5;

		//坐标边界    左右边界 0--8.0    前后边界 0--9.0
		float left_border = 0 - 4.5f;
		float right_border = 8.0 - 4.5f;
		float front_border = 0 + 3.0f;
		float back_border = 9.0 + 3.0f;
		//演员的的海拔高度
		float actor_altitute = 1.0f;

	};
}
