#pragma once

#include <string>
#include <unordered_map>
#include "glm/glm.hpp"	

struct ShaderSource
{
	std::string vertexSource;
	std::string fragmentSource;
	std::string geometrySource;
};

class Shader
{
private:
	unsigned int m_RendererID;
	std::string m_filepath;
	std::unordered_map<std::string, int> m_LocationCache;

	ShaderSource ParseShader(std::string filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShaderProgram(const std::string& vertexShader, 
		const std::string& fragmentShader, const std::string& geometryShader = "");

public:

	Shader() = delete;
	Shader(const std::string& filepath);
	~Shader();
	void Bind() const;
	void Unbind() const;
	unsigned int GetID() const;

	int GetUniformLocation(const std::string& name);

	void SetUniform1i(const std::string& name, int v0);
	void SetUniform1f(const std::string& name, float v0);

	void SetUniform1iv(const std::string& name, unsigned int count, const int* v);
	void SetUniform1fv(const std::string& name, unsigned int count, const float* v);

	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
};