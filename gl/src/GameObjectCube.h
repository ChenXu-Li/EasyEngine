#pragma once
#include "GameObject.h"

class GameObjectCube: public GameObject
{
public:
	GameObjectCube(std::string n = "default");
	virtual void Render(const glm::mat4& parentTransform, const glm::mat4& projection, const glm::mat4& view, const glm::vec3& lightPos, const glm::vec3& lightColor, const glm::vec3& viewPos);
private:
	void SelectedRender();
	std::shared_ptr<Shader> m_Shader_selected;
	
};

