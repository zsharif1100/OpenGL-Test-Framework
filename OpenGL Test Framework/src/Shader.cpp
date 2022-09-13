#include "Shader.h"

#include <GL/glew.h>
#include "GLErrorManager.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

Shader::Shader(const std::string& filepath)
	:m_filepath(filepath)
{
	ShaderSource source = ParseShader(filepath);
	m_RendererID = CreateShaderProgram(source.vertexSource, source.fragmentSource);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

unsigned int Shader::GetID() const
{
	return m_RendererID;
}

void Shader::ReloadShader(const std::string& filepath)
{
	ShaderSource source = ParseShader(filepath);
	m_RendererID = CreateShaderProgram(source.vertexSource, source.fragmentSource);
	GLCall(glUseProgram(m_RendererID));
	m_LocationCache = {};
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_LocationCache.count(name))
		return m_LocationCache[name];

	GLCall(m_LocationCache[name] = glGetUniformLocation(m_RendererID, name.c_str()));
	if (m_LocationCache[name] == -1)
		std::cout << "[Warning]: Uniform '" << name << "' does not exist" << std::endl;
	return m_LocationCache[name];
}

void Shader::SetUniform1i(const std::string& name, int v0)
{
	this->Bind();
	GLCall(glUniform1i(GetUniformLocation(name), v0));
}

void Shader::SetUniform1f(const std::string& name, float v0)
{
	this->Bind();
	GLCall(glUniform1f(GetUniformLocation(name), v0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	this->Bind();
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& name, glm::mat4& matrix)
{
	this->Bind();
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

ShaderSource Shader::ParseShader(std::string filepath)
{
	std::ifstream fStream(filepath);


	if (!fStream) {
		std::cout << "Failed to open: " << filepath << std::endl;
		return {"", ""};
	}
	enum class shaderMode {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::stringstream shader[2];
	std::string line;
	
	shaderMode mode;

	while (getline(fStream, line)) {
		if (line.find("#fragmentShader") != std::string::npos) {
			mode = shaderMode::FRAGMENT;
			continue;
		}
		else if ((line.find("#vertexShader") != std::string::npos)) {
			mode = shaderMode::VERTEX;
			continue;
		}

		shader[(int)mode] << line << "\n";
	}

	return {shader[0].str(), shader[1].str()};
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);

	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	//error handling
	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE) {
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		GLCall(glDeleteShader(id));
		return 0;
	}

	return id;
}

unsigned int Shader::CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader) {
	GLCall(unsigned int program = glCreateProgram());

	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	int validated, linked;

	GLCall(glGetProgramiv(program, GL_VALIDATE_STATUS, &validated));
	GLCall(glGetProgramiv(program, GL_LINK_STATUS, &linked));
	if (!validated || !linked) {
		int length;
		GLCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));
		GLCall(glGetProgramInfoLog(program, length, &length, message));
		std::cout << "Failed to validate or link:\n" << message << std::endl;
		GLCall(glDeleteProgram(program));
		return 0;
	}

	glDeleteShader(vs);
	glDeleteShader(fs);
	return program;
}
