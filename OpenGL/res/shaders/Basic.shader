#shader vertex
#version 330 core

layout(location = 0) in
vec4 position;
out
vec4 VertexColor;

void main()
{
    gl_Position = position;
    VertexColor = vec4(0.2, 0.3, 0.8, 1.0);
}

#shader fragment
#version 330 core
layout(location = 0) out
vec4 color;
in
vec4 VertexColor;

void main()
{
    color = VertexColor;
}
