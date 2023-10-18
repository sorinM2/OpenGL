#pragma once
#include <string>
#include <unordered_map>

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragemntSource;

};

class Shader
{
private:
    unsigned int m_RendererID;
    std::string m_FilePath;
    std::unordered_map<std::string, int> m_UniformLocationCache;
    //caching for uniforms

public:
    Shader(const std::string& filepath);
    ~Shader();
    void Bind() const;
    void Unbind()  const;

    void SetUniform1f(const std::string& name, float value);
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

private:

    int GetUniformLocation(const std::string& name);
   
    ShaderProgramSource ParseShader(const std::string& filePath);

    unsigned int CreateShader(const std::string& VertexShader, const std::string& FragmentShader);
    unsigned int CompileShader(unsigned int type, const std::string& source);



};