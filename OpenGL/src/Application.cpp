#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <string>
#include <fstream>
#include <sstream>

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragemntSource;

};

static ShaderProgramSource ParseShader(const std::string& filePath) {

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
	return { ss[0].str(), ss[1].str()};

}
static unsigned int CompileShader(unsigned int type, const std::string& source) {
	unsigned int shader = glCreateShader(type);
	const char* src = source.c_str();

	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

	int result = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		char* ShaderLog = (char*)alloca(sizeof(char) * length);
		glGetShaderInfoLog(shader, length, &length, ShaderLog);

		std::cout << "Error! Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << ShaderLog << std::endl;
		glDeleteShader(shader);
		return 0;
	}
	return shader;
}

static unsigned int CreateShader(const std::string& VertexShader, const std::string& FragmentShader) {
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, VertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, FragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;

}

int main(int argc, char** argv) {

	GLFWwindow* window;
	if (!glfwInit())
		return -1;
	window = glfwCreateWindow(1200, 800, "OpenGL", NULL, NULL);

	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		std::cout << "Error! Glew failed to initialize!" << std::endl;
	std::cout << glGetString(GL_VERSION) << std::endl;

	float points[] = {
		-0.5, -0.5,
	    0.5, -0.5,
		0.5, 0.5,

		0.5, 0.5,
		-0.5, 0.5,
		-0.5, -0.5
	};
	unsigned int buffer;
	glCreateBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 2, points, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);


	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
	unsigned int shader = CreateShader(source.VertexSource, source.FragemntSource);
	glUseProgram(shader);
	
	//std::cout << "VERTEX" << std::endl;
	//std::cout << source.VertexSource << std::endl;
	//std::cout << "FRAGMENT" << std::endl;
	//std::cout << source.FragemntSource << std::endl; 

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteProgram(shader);
	glfwTerminate();
	return 0;
}