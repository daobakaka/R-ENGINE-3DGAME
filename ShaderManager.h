#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <GL/glew.h>

namespace Game
{
    class ShaderManager
    {
    public:
        static ShaderManager* GetInstance();

        GLuint ShaderCompile(const char* ver, const char* fra, const std::string& name);

        GLuint GetShader(const std::string& name);

        void UseShader(const std::string& name);

        void IntegratedShaderCompile();

        // Set methods for various uniform types
        void SetBool(const std::string& shaderName, const std::string& uniformName, bool value) ;
        void SetInt(const std::string& shaderName, const std::string& uniformName, int value) ;
        void SetFloat(const std::string& shaderName, const std::string& uniformName, float value) ;
        void SetVec2(const std::string& shaderName, const std::string& uniformName, const glm::vec2& value) ;
        void SetVec2(const std::string& shaderName, const std::string& uniformName, float x, float y) ;
        void SetVec3(const std::string& shaderName, const std::string& uniformName, const glm::vec3& value) ;
        void SetVec3(const std::string& shaderName, const std::string& uniformName, float x, float y, float z) ;
        void SetVec4(const std::string& shaderName, const std::string& uniformName, const glm::vec4& value) ;
        void SetVec4(const std::string& shaderName, const std::string& uniformName, float x, float y, float z, float w) ;
        void SetMat2(const std::string& shaderName, const std::string& uniformName, const glm::mat2& mat) ;
        void SetMat3(const std::string& shaderName, const std::string& uniformName, const glm::mat3& mat) ;
        void SetMat4(const std::string& shaderName, const std::string& uniformName, const glm::mat4& mat) ;
        std::string _shaderName;
        int num;
    private:
        ShaderManager();
        ~ShaderManager();
        static ShaderManager* instance;

        std::unordered_map<std::string, GLuint> _shaderMap;
       
        // Utility function for checking shader compilation/linking errors.
        void CheckCompileErrors(GLuint shader, std::string type) const;
    };
}

#endif // SHADER_MANAGER_H
