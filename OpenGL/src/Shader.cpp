#include "Shader.h"
#include "Renderer.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>


Shader::Shader(const std::string& filepath)
	:m_FilePath(filepath), m_RendererID(0)
{

	ShaderProgramSource ShaderCode = ParseShader(filepath);
	m_RendererID = CreateShader(ShaderCode.VertexSource, ShaderCode.FragemntSource);
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

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{

	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
	GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	GLCall(glUniform1i(GetUniformLocation(name), value));

}


int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return  m_UniformLocationCache[name];

	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	m_UniformLocationCache[name] = location;

	if (location == -1)
		std::cout << "Warning: uniform " << name << " doesn't exist!" << std::endl;
	return location;
	 
}

ShaderProgramSource Shader::ParseShader(const std::string& filePath) {

	std::ifstream stream(filePath);

	enum class ShaderType {
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else {
			ss[int(type)] << line << "\n";
		}
	}
	return { ss[0].str(), ss[1].str() };

}

unsigned int Shader::CreateShader(const std::string& VertexShader, const std::string& FragmentShader) {
	GLCall(unsigned int program = glCreateProgram());
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, VertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, FragmentShader);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));

	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;

}



unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
	GLCall(unsigned int shader = glCreateShader(type));
	const char* src = source.c_str();

	GLCall(glShaderSource(shader, 1, &src, nullptr));
	GLCall(glCompileShader(shader));

	int result = GL_FALSE;
	GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &result));

	if (result == GL_FALSE) {
		int length;
		GLCall(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length));
		char* ShaderLog = (char*)alloca(sizeof(char) * length);
		GLCall(glGetShaderInfoLog(shader, length, &length, ShaderLog));

		std::cout << "Error! Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << ShaderLog << std::endl;
		GLCall(glDeleteShader(shader));
		return 0;
	}
	return shader;
}