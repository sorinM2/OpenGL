#pragma once
#include <GL/glew.h>
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"

#include <fstream>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
        x;\
        ASSERT(GLLogCall(#x, __FILE__, __LINE__));


void GLClearError();

bool GLLogCall(const char* function, const char* file, const int& line);


class Renderer
{

public:
    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& id, const Shader& shader) const;
};