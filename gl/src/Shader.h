#pragma once
#include<string>
#include<sstream>
#include<fstream>
#include<iostream>
#include<unordered_map>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};
class Shader
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader(const std::string& filepath);

	~Shader();
	void Bind() const;
	void Unbind()const;

	void SetUniform1f(const std::string& name,float v0);
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1ui(const std::string& name, unsigned int value);
	void SetUniform1Mat4fv(const std::string& name, glm::mat4 trans);
	int GetUniformLocation(const std::string& name);
	
	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexshader, const std::string& fragmentshader);
	
	unsigned int GetID() {
		return m_RendererID;
	}
};

