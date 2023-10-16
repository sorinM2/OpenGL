#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "VertexArray.h"		
#include "Shader.h"


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

		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
	

		shader.Unbind();
		va.Unbind();
		vb.Unbind();
		ib.Unbind();

		float r = 0.8f;
		float increment = 0.05f;
		while (!glfwWindowShouldClose(window)) {
			glClear(GL_COLOR_BUFFER_BIT);

			shader.Bind();
			shader.SetUniform4f("u_color", r, 0.3f, 0.8f, 1.0f);

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

	}
	
	glfwTerminate();
	return 0;
}