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

	};
}
