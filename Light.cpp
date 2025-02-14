#include "Light.h"
#include <glm/gtc/matrix_transform.hpp>
#include "FileLoadIO.h"
#include "CustomModel.h"
#include "LifecycleManager.h"
#include "ScriptModel.h"
#include "MeshDataManager.h"

using namespace Game;

// 初始化静态单例指针
LightSpawner* LightSpawner::instance = nullptr;
//引用shader
extern const char* noneLightLightVertexShaderSource;
extern const char* noneLightLightFragmentShaderSource;
extern const char* rayVertexShaderSource;
extern const char* rayFragmentShaderSource;
//C++的源文件中可以直接定义相关变量，而不必要在类中进行声明，更灵活
LifecycleManager<CustomModel>* manager = nullptr;
MeshDataManager* meshData = nullptr;
LightSpawner* LightSpawner::GetInstance() {
    if (instance == nullptr) {
        instance = new LightSpawner();
    }
    return instance;
}

LightSpawner::LightSpawner() {
    // 初始化全局平行光默认参数
    manager = LifecycleManager<CustomModel>::GetInstance();
    meshData = MeshDataManager::GetInstance();
    modelIdentification = true;
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
    if (modelIdentification)
    {
        //--同时在相应的位置构建一个点光源模型
        auto* pointLightPtr = new LightModel(noneLightLightVertexShaderSource, noneLightLightFragmentShaderSource, ModelDic["baseSphere"], false);
        pointLightPtr->SetVariant(ModelClass::PointLight);
        pointLightPtr->Initialize(position, glm::quat(glm::vec3(0, 0, 1.5f)), glm::vec3(0.3f));
        manager->RegisterObject(pointLightPtr);
        pointLightPtr->AttachTexture(TextureDic["light"][0]);
        //子类的方法，设置灯光参数
        pointLightPtr->SteLightParameters(color, intensity);
    }

   
    return pointLight;
}

CustomParallelLight LightSpawner::SpawParallelLight(glm::vec3 direction, glm::vec3 color, float intensity) {
   
    
    parallelLight.direction = glm::normalize(direction);
    parallelLight.color = color;
    parallelLight.intensity = intensity;

    if (modelIdentification)
    {
        //--平行光是全局光照，位置无意义，类似光源的虚拟位置用其他的模型代替，设置0.1猴头显示是否定义，应无距离衰减
        auto* pointLightPtr = new LightModel(noneLightLightVertexShaderSource, noneLightLightFragmentShaderSource, ModelDic["testMonkey"], false);
        pointLightPtr->SetVariant(ModelClass::ParallelLight);
        pointLightPtr->Initialize(glm::vec3(0), glm::vec3(0), glm::vec3(0.3f));
        manager->RegisterObject(pointLightPtr);
        pointLightPtr->AttachTexture(TextureDic["light"][0]);
        //子类的方法，设置灯光参数
        pointLightPtr->SteLightParameters(color, intensity);

    }


    return parallelLight;
}

CustomFlashLight LightSpawner::SpawFlashLight(glm::vec3 position, glm::vec3 direction, glm::vec3 color, float intensity, float cutoff) {
    CustomFlashLight flashLight;
    flashLight.position = position;
    flashLight.direction = -glm::normalize(direction);
    flashLight.color = color;
    flashLight.intensity = intensity;
    flashLight.cutoff = cutoff;
    if (flashLights.size()<4)
    {
        flashLights.push_back(flashLight);
    }
    if (modelIdentification)
    {
        //处理手电筒朝向问题
        glm::vec3 defaultDown = glm::vec3(0.0f, -1.0f, 0.0f);  // 圆锥底面默认朝向 Y,这里修正光线射出的方向
        glm::vec3 dirNorm = glm::normalize(direction);

        float dotVal = glm::dot(defaultDown, dirNorm);
        glm::quat finalRotation;
        const float epsilon = 1e-6f;

        if (dotVal > 1.0f - epsilon) {
            // 与 defaultDown 几乎同向 => 不旋转
            finalRotation = glm::quat(1, 0, 0, 0);
        }
        else if (dotVal < -1.0f + epsilon) {
            // 反向 => 绕任意水平轴旋转180度
            finalRotation = glm::angleAxis(glm::pi<float>(), glm::vec3(1, 0, 0));
        }
        else {
            float angle = acos(dotVal);
            glm::vec3 axis = glm::normalize(glm::cross(defaultDown, dirNorm));
            finalRotation = glm::angleAxis(angle, axis);
        }



        //--同时在相应的位置构建一个手电筒模型
        auto* pointLightPtr = new LightModel(noneLightLightVertexShaderSource, noneLightLightFragmentShaderSource, ModelDic["baseCone"], false);
        pointLightPtr->SetVariant(ModelClass::FlashLight);
        pointLightPtr->Initialize(position, finalRotation, glm::vec3(0.3f));
        manager->RegisterObject(pointLightPtr);
        pointLightPtr->AttachTexture(TextureDic["light"][0]);
        //子类的方法，设置灯光参数
        pointLightPtr->SteLightParameters(color, intensity);

        //--构建手电筒参考射线-------------
        //这里采用自定义顶点，这样更方便，便于更改射线长度
        GLfloat customizeRayLength[6] = { 0,0,0,0,-intensity,0 };
        GLuint  customizeRayIndecis[2] = { 0,1 };
        auto* rayCus = new CustomizeRay(rayVertexShaderSource, rayFragmentShaderSource, customizeRayLength, customizeRayIndecis, 6, 2, false);
        rayCus->SetVariant(ModelClass::Ray);
        rayCus->Initialize(position, finalRotation);
        manager->RegisterObject(rayCus);
        rayCus->SetRayPar(color, intensity);
    }
    return flashLight;
}

void Game::LightSpawner::ParalletLightController(glm::vec3 dirction, glm::vec3 color, float intensity)
{
    // 2) 将增量角度(度)转为弧度
    glm::vec3 radIncrements = glm::radians(dirction);

    // 3) 构建旋转矩阵(初始为单位矩阵)
    glm::mat4 rotMat(1.0f);

    // 假设顺序：先绕X旋转pitch，再绕Y旋转yaw，再绕Z旋转roll
    rotMat = glm::rotate(rotMat, radIncrements.x, glm::vec3(1, 0, 0));
    rotMat = glm::rotate(rotMat, radIncrements.y, glm::vec3(0, 1, 0));
    rotMat = glm::rotate(rotMat, radIncrements.z, glm::vec3(0, 0, 1));

    // 4) 用这个增量旋转矩阵乘以当前光的 direction
    //    parallelLight.direction是上帧(或初始)的朝向，这里做累加旋转
    glm::vec4 oldDir(parallelLight.direction, 0.0f);
    glm::vec4 newDir = rotMat * oldDir;

    // 5) 写回并归一化
    parallelLight.direction = glm::normalize(glm::vec3(newDir));
   /* parallelLight.color = color;
    parallelLight.intensity = intensity;*/

}

void Game::LightSpawner::PointLightController(glm::vec3 color, float intensity, int lightNumber)
{
    pointLights[lightNumber].color = color;
    pointLights[lightNumber].intensity = intensity;
}

void Game::LightSpawner::FlashLightController(glm::vec3 dirction, glm::vec3 color, float intensity, float cutoff,int lightNumber)
{
    flashLights[lightNumber].direction = dirction;
    flashLights[lightNumber].color = color;
    flashLights[lightNumber].cutoff = cutoff;
    flashLights[lightNumber].intensity = intensity;

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