#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <string>
#include <fstream>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "VertexArray.h"		

static struct ShaderProgramSource {
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

static unsigned int CreateShader(const std::string& VertexShader, const std::string& FragmentShader) {
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

int main(int argc, char** argv) {

	GLFWwindow* window;
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);


	window = glfwCreateWindow(1200, 800, "OpenGL", NULL, NULL);

	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	if (glewInit() != GLEW_OK)
		std::cout << "Error! Glew failed to initialize!" << std::endl;
	std::cout << glGetString(GL_VERSION) << std::endl;
	{
		float points[] = {
			-0.5, -0.5,
			0.5, -0.5,
			0.5, 0.5,
			-0.5, 0.5
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};


		VertexArray va;
		VertexBuffer vb(points, sizeof(float) * 4 * 2);
		

		VertexBufferLayout layout;
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indices, 6);


		ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
		unsigned int shader = CreateShader(source.VertexSource, source.FragemntSource);
		GLCall(glUseProgram(shader));

		GLCall(int location = glGetUniformLocation(shader, "u_color"));
		ASSERT(location != -1);
		GLCall(glUniform4f(location, 0.3f, 0.3f, 0.8f, 1.0f));

		GLCall(glBindVertexArray(0));
		GLCall(glUseProgram(0));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

		float r = 0.8f;
		float increment = 0.05f;
		while (!glfwWindowShouldClose(window)) {
			glClear(GL_COLOR_BUFFER_BIT);

			GLCall(glUseProgram(shader));
			GLCall(glUniform4f(location, r, 0.3f, 0.1f, 1.0f));
			va.Bind();
			ib.Bind();

			if (r > 1.0f)
				increment = -0.05f;
			else if (r < 0.0f)
				increment = 0.05f;
			r += increment;
			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glDeleteProgram(shader);
	}
	
	glfwTerminate();
	return 0;
}