#include "ShaderManager.h"
#include <iostream>
#include "shader.h"  // 这是一个独立的着色器文件，且只能被引用一次
using namespace Game;

ShaderManager* ShaderManager::instance = nullptr;

// 获取单例实例
ShaderManager* ShaderManager::GetInstance()
{
    if (instance == nullptr) {
        instance = new ShaderManager();
    }
    return instance;
}

// 构造函数
ShaderManager::ShaderManager()
{

    _shaderName = "-shader";

}

// 析构函数
ShaderManager::~ShaderManager() {}

// 编译 shader，返回编译后的 shader ID
GLuint ShaderManager::ShaderCompile(const char* ver, const char* fra, const std::string& name)
{


    std::cout << "开始编译" << _shaderName << std::endl;
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &ver, nullptr);
    glCompileShader(vertexShader);
    CheckCompileErrors(vertexShader, "VERTEX");

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fra, nullptr);
    glCompileShader(fragmentShader);
    CheckCompileErrors(fragmentShader, "FRAGMENT");

    GLuint shader = glCreateProgram();
    glAttachShader(shader, vertexShader);
    glAttachShader(shader, fragmentShader);
    glLinkProgram(shader);
    CheckCompileErrors(shader, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //GLuint keke = 3;
    //_shaderMap["keke"] = keke;

    _shaderMap[name] = shader;

    return shader;
}

// 获取指定名称的 shader
GLuint ShaderManager::GetShader(const std::string& name)
{
    return _shaderMap[name];  // 假设 name 始终有效
}

// 使用指定名称的 shader
void ShaderManager::UseShader(const std::string& name)
{
    glUseProgram(_shaderMap[name]);  // 假设 name 始终有效


}

void Game::ShaderManager::IntegratedShaderCompile()
{
    //基础shader
    ShaderCompile(noneLightVertexShaderSource, noneLightFragmentShaderSource, "noneLight");
    //功能全面的通用光照shader
    ShaderCompile(colorlightsArrayVertexShaderSource, colorlightsArraySourceFragmentShaderSource, "commonLight");
    //无光照的基础实例化shader
    ShaderCompile(instanceNoLightingVertexShaderSource, instanceNoLightingFragmentShaderSource, "noneLightInstancer");
    //无光照的基础实例化shader,可进行深度图进行后处理
    ShaderCompile(instanceNoLightingVertexShaderSource, instanceNoLightingForViewPortDepthMapFragmentShaderSource, "noneLightDepthCalInstancer");
    //功能全面的无光照的通用shader(可生成阴影)
    ShaderCompile(CustomNoLightShadowShaderVertexSource, CustomNoLightShadowShaderFragmentSource, "commonNoneLight");

    //独立shader,构建玩家飞行道具
    ShaderCompile(waveVertexShaderSource, colorlightsArraySourceFragmentShaderSource, "waveShader");
    //独立shader,玩家模板测试
    ShaderCompile(stencilTestVertexShaderSource, stencilTestFragmentShaderSource, "stencilTestShader");
    //独立实例化shader,用于生成萤光效果
    ShaderCompile(instanceNoLightingVertexShaderSource, instanceNoLightingFireFlyFragmentShaderSource, "fireflyInstanceShader");



    //深度图着色器模块
    ShaderCompile(depthShaderVertexShaderSource, depthShaderFragmentShaderSource, "depthCal");
    ShaderCompile(depthVisualShaderVertexShaderSource, depthVisualShaderFragmentShaderSource, "depthVisual");
    ShaderCompile(depthTestShaderVertexShaderSource, depthTestShaderFragmentShaderSource, "depthTest");
    //投影深度图计算,投影深度图可视化
    ShaderCompile(depthViewPortShaderVertexShaderSource, depthViewPortShaderFragmentShaderSource, "depthViewPortCal");
    ShaderCompile(depthVisualShaderVertexShaderSource, depthLinerVisualShaderFragmentShaderSource, "depthLinerVisual");
}

// Set methods for various uniform types

void ShaderManager::SetBool(const std::string& shaderName, const std::string& uniformName, bool value)
{
    glUseProgram(_shaderMap[shaderName]);
    glUniform1i(glGetUniformLocation(_shaderMap[shaderName], uniformName.c_str()), (int)value);
}

void ShaderManager::SetInt(const std::string& shaderName, const std::string& uniformName, int value)
{
    glUseProgram(_shaderMap[shaderName]);
    glUniform1i(glGetUniformLocation(_shaderMap[shaderName], uniformName.c_str()), value);
}

void ShaderManager::SetFloat(const std::string& shaderName, const std::string& uniformName, float value)
{
    glUseProgram(_shaderMap[shaderName]);
    glUniform1f(glGetUniformLocation(_shaderMap[shaderName], uniformName.c_str()), value);
}

void ShaderManager::SetVec2(const std::string& shaderName, const std::string& uniformName, const glm::vec2& value)
{
    glUseProgram(_shaderMap[shaderName]);
    glUniform2fv(glGetUniformLocation(_shaderMap[shaderName], uniformName.c_str()), 1, &value[0]);
}

void ShaderManager::SetVec2(const std::string& shaderName, const std::string& uniformName, float x, float y)
{
    glUseProgram(_shaderMap[shaderName]);
    glUniform2f(glGetUniformLocation(_shaderMap[shaderName], uniformName.c_str()), x, y);
}

void ShaderManager::SetVec3(const std::string& shaderName, const std::string& uniformName, const glm::vec3& value)
{
    glUseProgram(_shaderMap[shaderName]);
    glUniform3fv(glGetUniformLocation(_shaderMap[shaderName], uniformName.c_str()), 1, &value[0]);
}

void ShaderManager::SetVec3(const std::string& shaderName, const std::string& uniformName, float x, float y, float z)
{
    glUseProgram(_shaderMap[shaderName]);
    glUniform3f(glGetUniformLocation(_shaderMap[shaderName], uniformName.c_str()), x, y, z);
}

void ShaderManager::SetVec4(const std::string& shaderName, const std::string& uniformName, const glm::vec4& value)
{
    glUseProgram(_shaderMap[shaderName]);
    glUniform4fv(glGetUniformLocation(_shaderMap[shaderName], uniformName.c_str()), 1, &value[0]);
}

void ShaderManager::SetVec4(const std::string& shaderName, const std::string& uniformName, float x, float y, float z, float w)
{
    glUseProgram(_shaderMap[shaderName]);
    glUniform4f(glGetUniformLocation(_shaderMap[shaderName], uniformName.c_str()), x, y, z, w);
}

void ShaderManager::SetMat2(const std::string& shaderName, const std::string& uniformName, const glm::mat2& mat)
{
    glUseProgram(_shaderMap[shaderName]);
    glUniformMatrix2fv(glGetUniformLocation(_shaderMap[shaderName], uniformName.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void ShaderManager::SetMat3(const std::string& shaderName, const std::string& uniformName, const glm::mat3& mat)
{
    glUseProgram(_shaderMap[shaderName]);
    glUniformMatrix3fv(glGetUniformLocation(_shaderMap[shaderName], uniformName.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void ShaderManager::SetMat4(const std::string& shaderName, const std::string& uniformName, const glm::mat4& mat)
{
    glUseProgram(_shaderMap[shaderName]);
    glUniformMatrix4fv(glGetUniformLocation(_shaderMap[shaderName], uniformName.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Game::ShaderManager::SetTexture(const std::string& shaderName, const std::string& uniformName, GLuint textureID, int order)
{
    glUseProgram(_shaderMap[shaderName]);
    // 获取 Uniform 的位置
    GLint uniformLocation = glGetUniformLocation(_shaderMap[shaderName], uniformName.c_str());
    // 激活指定的纹理单元
    glActiveTexture(GL_TEXTURE0 + order);

    // 绑定纹理
    glBindTexture(GL_TEXTURE_2D, textureID);

    // 将纹理单元传递给片段着色器
    glUniform1i(uniformLocation, order);

}

// 检查 shader 编译或链接时的错误
void ShaderManager::CheckCompileErrors(GLuint shader, std::string type) const
{
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

