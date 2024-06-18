#pragma once
#include "GameObject.h"

class GameObjectCube: public GameObject
{
public:
	GameObjectCube(std::string n = "default");
	virtual void Render(const glm::mat4& parentTransform, const glm::mat4& projection, const glm::mat4& view);
};

