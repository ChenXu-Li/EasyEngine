#pragma once
#include "GameObject.h"

class GameObjectCube: public GameObject
{
public:
	GameObjectCube(std::string n = "default");
	virtual void Update(float deltaTime);
	virtual void Render(const glm::mat4& parentTransform, const glm::mat4& projection, const glm::mat4& view, const glm::vec3& lightPos, const glm::vec3& lightColor, const glm::vec3& viewPos);
protected:
	void SelectedUpdate(float deltaTime);
	std::shared_ptr<Shader> m_Shader;
	std::shared_ptr<Texture> m_Texture;
	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<IndexBuffer> m_IndexBuffer;
	std::unique_ptr<VertexBuffer> m_VertexBuffer;
	std::shared_ptr<Shader> m_Shader_selected;
	virtual void SetBoundingBox();
	
};

