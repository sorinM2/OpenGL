#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <iostream>

static unsigned int CompileShader(unsigned int type,const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();   
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // TODO: Syntax Error Handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile" << 
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << std::endl;
        std::cout << message << std::endl; 
        glDeleteShader(id);
        return 0;
    }
    

    return id;
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
int main(void)
{

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1200, 800, "Hello World", NULL, NULL);


    
    if (!window)
    {

        //////////////coie
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "Error! Initialization failed..." << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    float points[] = { -0.5f, -0.5f,
                       0.0f, 0.0f,
                       0.5f, -0.5f };
    unsigned int Buffer;
    glGenBuffers(1, &Buffer);
    glBindBuffer(GL_ARRAY_BUFFER, Buffer);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, points, GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (const void*)0);

    std::string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 1) in vec4 position;\n"
        "\n"
        "void main()\n"
        "{\n"
        "gl_Position = position;\n"
        "}\n";

    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 1) out vec4 color;\n"
        "\n" 
        "void main()\n"
        "{\n"   
        "color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n";
    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    /* Loop until the user closes the window */;
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 1, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}