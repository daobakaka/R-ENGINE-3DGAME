#include "ShaderManager.h"
#include <iostream>
#include "shader.h"  // ����һ����������ɫ���ļ�����ֻ�ܱ�����һ��
using namespace Game;

        
    ShaderManager* ShaderManager::instance = nullptr;

    // ��ȡ����ʵ��
    ShaderManager* ShaderManager::GetInstance()
    {
        if (instance == nullptr) {
            instance = new ShaderManager();
        }
        return instance;
    }

    // ���캯��
    ShaderManager::ShaderManager()
    {

        _shaderMame = "-shader";

    }

    // ��������
    ShaderManager::~ShaderManager() {}

    // ���� shader�����ر����� shader ID
    GLuint ShaderManager::ShaderCompile(const char* ver, const char* fra, const std::string& name)
    {
        
        
        std::cout << "��ʼ����"<<_shaderMame << std::endl;
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
        
        GLuint keke = 3;
        _shaderMap["keke"] = keke;

        _shaderMap[name] = shader;

        return shader;
    }

    // ��ȡָ�����Ƶ� shader
    GLuint ShaderManager::GetShader(const std::string& name)
    {
        return _shaderMap[name];  // ���� name ʼ����Ч
    }

    // ʹ��ָ�����Ƶ� shader
    void ShaderManager::UseShader(const std::string& name)
    {
        glUseProgram(_shaderMap[name]);  // ���� name ʼ����Ч

    
    }

    void Game::ShaderManager::IntergratedShaderCompile()
    {

        ShaderCompile(noneLightLightVertexShaderSource, noneLightLightFragmentShaderSource, "noneLight");
        ShaderCompile(colorlightsArrayVertexShaderSource, colorlightsArraySourceFragmentShaderSource, "commonLight");
        ShaderCompile(instanceNoLightingVertexShaderSource, instanceNoLightingFragmentShaderSource, "noneLightInstancer");

        //���ͼ��ɫ��ģ���
        ShaderCompile(depthShaderVertexShaderSource, depthShaderFragmentShaderSource,"depthCal");
        ShaderCompile(depthVisualShaderVertexShaderSource, depthVisualShaderFragmentShaderSource,"depthVisual");
        ShaderCompile(depthTestShaderVertexShaderSource, depthTestShaderFragmentShaderSource,"depthTest");

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

    // ��� shader ���������ʱ�Ĵ���
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

