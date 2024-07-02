#pragma once
#include "GameObject.h"
#include "Model.h"
class GameObjectModel : public GameObject
{
public:
	
	GameObjectModel(std::string n = "Model");
	virtual void Update(float deltaTime);
	virtual void Render(const glm::mat4& parentTransform, const glm::mat4& projection, const glm::mat4& view, const glm::vec3& lightPos, const glm::vec3& lightColor, const glm::vec3& viewPos);
private:
	Model m_model;
	std::shared_ptr<Shader> m_Shader;
	virtual void SetBoundingBox();
};


