#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "VertexArray.h"		
#include "VertexBufferLayout.h"
#include "Shader.h"

#include "Renderer.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

int main(int argc, char** argv) {

	GLFWwindow* window;
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);


	window = glfwCreateWindow(960, 540, "OpenGL", NULL, NULL);

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
			200.0f, 200.0f, 0.0f, 0.0f,
			400.0f, 200.0f, 1.0f, 0.0f,		  
			400.0f, 400.0f, 1.0f, 1.0f,
			200.0f, 400.0f, 0.0f, 1.0f
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_BLEND));

		VertexArray va;
		VertexBuffer vb(points, sizeof(float) * 4 * 4);
		

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);
		IndexBuffer ib(indices, 6);


		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.f, -1.0f, 1.0f);	//* 2 = 4:3
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, 0.0f, 0.0f));
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200.0f, 100.0f, 0.0f));

		glm::mat4 MVP = proj * view * model;

		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
	

 		Texture texture("res/textures/ba.png");
		texture.Bind();
		shader.SetUniform1i("u_Texture", 0);
		shader.SetUniformMat4f("u_MVP", MVP);	

		shader.Unbind();
		va.Unbind();
		vb.Unbind();
		ib.Unbind();

		Renderer renderer; 

		while (!glfwWindowShouldClose(window)) {
			renderer.Clear();

			shader.Bind();

			renderer.Draw(va, ib, shader);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

	}
	
	glfwTerminate();
	return 0;
}