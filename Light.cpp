#include "Light.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace Game;

// 初始化静态单例指针
LightSpawner* LightSpawner::instance = nullptr;

LightSpawner* LightSpawner::GetInstance() {
    if (instance == nullptr) {
        instance = new LightSpawner();
    }
    return instance;
}

LightSpawner::LightSpawner() {
    // 初始化全局平行光默认参数

}

LightSpawner::~LightSpawner() {
    // 如有需要，进行清理操作
}

CustomPointLight LightSpawner::SpawPointLight(glm::vec3 position, glm::vec3 color, float intensity) {
    CustomPointLight pointLight;
    pointLight.position = position;
    pointLight.color = color;
    pointLight.intensity = intensity;

    // 如果当前点光源数量未达到上限，则保存到内部容器中
    if (pointLights.size() < 4) {
        pointLights.push_back(pointLight);
    }
    // 否则可以选择忽略或更新已有光源，根据具体需求处理
    return pointLight;
}

CustomParallelLight LightSpawner::SpawParallelLight(glm::vec3 direction, glm::vec3 color, float intensity) {
   
    
    parallelLight.direction = glm::normalize(direction);
    parallelLight.color = color;
    parallelLight.intensity = intensity;

    return parallelLight;
}

CustomFlashLight LightSpawner::SpawFlashLight(glm::vec3 position, glm::vec3 direction, glm::vec3 color, float intensity, float cutoff) {
    CustomFlashLight flashLight;
    flashLight.position = position;
    flashLight.direction = glm::normalize(direction);
    flashLight.color = color;
    flashLight.intensity = intensity;
    flashLight.cutoff = cutoff;
    if (flashLights.size()<4)
    {
        flashLights.push_back(flashLight);
    }
    return flashLight;
}

#pragma region  渲染脚本区域

#pragma endregion

/// <summary>
/// 静态实例指针
/// </summary>
LightRender* LightRender::instance = nullptr;

LightRender* LightRender::GetInstance() {
    if (instance == nullptr) {
        instance = new LightRender();
    }
    return instance;
}

LightRender::LightRender() {
    // 构造函数为空
}

LightRender::~LightRender() {
    // 如有必要进行资源清理
}

void LightRender::RenderLights(GLuint shaderProgram, const Controller* controllerPtr, const Game::LightSpawner* spawnerPtr) {
    if (!controllerPtr || !spawnerPtr) return; // 简单检查指针有效性

     glUseProgram(shaderProgram);
     // 获取摄像机数据：直接用 controller->front
        GLuint viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
        glm::vec3 viewP = glm::vec3(controllerPtr->front); // 假设 controller->front 是一个可直接使用的向量
        glUniform3f(viewPosLoc, viewP.x, viewP.y, viewP.z);


    // 2. 点光源数据
    const auto& pointLights = spawnerPtr->GetPointLights();
    for (int i = 0; i < 4; i++) {
        std::string posName = "lightPos[" + std::to_string(i) + "]";
        std::string colorName = "lightColor[" + std::to_string(i) + "]";
        std::string intensityName = "lightIntensity[" + std::to_string(i) + "]";
        GLuint posLoc = glGetUniformLocation(shaderProgram, posName.c_str());
        GLuint colorLoc = glGetUniformLocation(shaderProgram, colorName.c_str());
        GLuint intensityLoc = glGetUniformLocation(shaderProgram, intensityName.c_str());

        if (i < pointLights.size()) {
            const auto& light = pointLights[i];
            glUniform3fv(posLoc, 1, glm::value_ptr(light.position));
            glUniform3fv(colorLoc, 1, glm::value_ptr(light.color));
            glUniform1f(intensityLoc, light.intensity);
        }
        else {
            glUniform3f(posLoc, 0.0f, 0.0f, 0.0f);
            glUniform3f(colorLoc, 0.0f, 0.0f, 0.0f);
            glUniform1f(intensityLoc, 0.0f);
        }
    }

    // 3. 平行光数据
    Game::CustomParallelLight parallelLight = spawnerPtr->GetParallelLight();
    GLint parallelDirLoc = glGetUniformLocation(shaderProgram, "parallelLightDirection");
    GLint parallelColorLoc = glGetUniformLocation(shaderProgram, "parallelLightColor");
    GLint parallelIntensityLoc = glGetUniformLocation(shaderProgram, "parallelLightIntensity");
    glUniform3fv(parallelDirLoc, 1, glm::value_ptr(parallelLight.direction));
    glUniform3fv(parallelColorLoc, 1, glm::value_ptr(parallelLight.color));
    glUniform1f(parallelIntensityLoc, parallelLight.intensity);

    // 4. 手电筒（聚光灯）数据
    const auto& flashLights = spawnerPtr->GetFlashLights();
    for (int j = 0; j < 4; j++) {
        std::string flashPosName = "flashLightPos[" + std::to_string(j) + "]";
        std::string flashDirName = "flashLightDirection[" + std::to_string(j) + "]";
        std::string flashColorName = "flashLightColor[" + std::to_string(j) + "]";
        std::string flashIntensityName = "flashLightIntensity[" + std::to_string(j) + "]";
        std::string flashCutoffName = "flashLightCutoff[" + std::to_string(j) + "]";
        GLuint flashPosLoc = glGetUniformLocation(shaderProgram, flashPosName.c_str());
        GLuint flashDirLoc = glGetUniformLocation(shaderProgram, flashDirName.c_str());
        GLuint flashColorLoc = glGetUniformLocation(shaderProgram, flashColorName.c_str());
        GLuint flashIntensityLoc = glGetUniformLocation(shaderProgram, flashIntensityName.c_str());
        GLuint flashCutoffLoc = glGetUniformLocation(shaderProgram, flashCutoffName.c_str());

        if (j < flashLights.size()) {
            const auto& flash = flashLights[j];
            glUniform3fv(flashPosLoc, 1, glm::value_ptr(flash.position));
            glUniform3fv(flashDirLoc, 1, glm::value_ptr(flash.direction));
            glUniform3fv(flashColorLoc, 1, glm::value_ptr(flash.color));
            glUniform1f(flashIntensityLoc, flash.intensity);
            glUniform1f(flashCutoffLoc, flash.cutoff);
        }
        else {
            glUniform3f(flashPosLoc, 0.0f, 0.0f, 0.0f);
            glUniform3f(flashDirLoc, 0.0f, 0.0f, 0.0f);
            glUniform3f(flashColorLoc, 0.0f, 0.0f, 0.0f);
            glUniform1f(flashIntensityLoc, 0.0f);
            glUniform1f(flashCutoffLoc, 0.0f);
        }
    }
}