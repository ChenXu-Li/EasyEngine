#pragma once
#include "Test.h"
#include <memory>
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

		glm::mat4 m_view;
		glm::mat4 m_projection;

		Camera m_camera;
		Light  m_light;
		std::unique_ptr<GameObject> m_rootObject;

		// 创建一个包含16个std::unique_ptr<GameObjectCube>的vector
		std::vector<std::shared_ptr<GameObject>> my_Actors;
		//存储演员坐标
		glm::vec3 vec3Array[16];

		//存储演员的方向角度
		glm::vec3 vec3Angle[16];
		float elapsedTimeSinceLastMove = 0.0f; // 自上次移动以来经过的时间  
		const float MOVE_INTERVAL = 0.05f; // 设置移动间隔为1秒  
		float speed = 0.04;//演员移动速度
		float adjust = 0.05;//碰撞调整

		//坐标边界    左右边界 0--8.0    前后边界 0--9.0
		float left_border = 0 - 4.5f;
		float right_border = 8.0 - 4.5f;
		float front_border = 0 + 3.0f;
		float back_border = 9.0 + 3.0f;

		//演员随机移动
		float x_random[16];////左右
		float y_random[16];////前后
		//演员移动步数
		int step = 0;
		int step_action = 5;//几步后更新演员方向，移动方向
	};
}
