#include "Light.h"
#include <glm/gtc/matrix_transform.hpp>
#include "FileLoadIO.h"
#include "CustomModel.h"
#include "LifecycleManager.h"
#include "ScriptModel.h"
#include "MeshDataManager.h"
#include <algorithm>  // 包含 std::sort
#include <iostream>
#include "EnumTotal.h"
#include <glm/gtc/type_ptr.hpp>

using namespace Game;

// 初始化静态单例指针
LightSpawner* LightSpawner::instance = nullptr;
//引用shader
extern const char* noneLightLightVertexShaderSource;
extern const char* noneLightLightFragmentShaderSource;
extern const char* rayVertexShaderSource;
extern const char* rayFragmentShaderSource;
//深度图生成着色器，利用opengl面向过程机制，定义一个，全局渲染
extern const char* depthShaderVertexShaderSource;
extern const char* depthShaderFragmentShaderSource;
//深度图可视化着色器
extern const char* depthVisualShaderVertexShaderSource;
extern const char* depthVisualShaderFragmentShaderSource;
//深度图测试着色器
extern const char* depthTestShaderVertexShaderSource;
extern const char* depthTestShaderFragmentShaderSource;
//C++的源文件中可以直接定义相关变量，而不必要在类中进行声明，更灵活
extern LifecycleManager<CustomModel>* manager; 
extern MeshDataManager* meshData ;
extern Controller* controller ;
extern LightSpawner* lightSpawner;
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
    controller = Controller::GetInstance();
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
        pointLightPtr->AttachTexture(TextureDic["light"]);
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
        pointLightPtr->AttachTexture(TextureDic["light"]);
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
    if (flashLights.size() < 4)
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
        pointLightPtr->AttachTexture(TextureDic["light"]);
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

void Game::LightSpawner::FlashLightController(glm::vec3 dirction, glm::vec3 color, float intensity, float cutoff, int lightNumber)
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

GLuint Game::LightRender::CreateShadowMapForParallelLight()
{

    //---3个shader一起编译
//创建阴影或者深度贴图渲染shader，然后编译深度着色器，这里必须在Oepngl 上下文中进行，因此不能放在构造
    _depthShaderProgram = CompileShader(depthShaderVertexShaderSource, depthShaderFragmentShaderSource);
    _depthVisualShader = CompileShader(depthVisualShaderVertexShaderSource, depthVisualShaderFragmentShaderSource);
    _depthTestShader = CompileShader(depthTestShaderVertexShaderSource, depthTestShaderFragmentShaderSource);
    //-------------------------------------------------------   


    // 创建深度贴图
    glGenFramebuffers(1, &_depthMapParallelFBO);//基于深度贴图的帧缓冲贴图对象GLuint
    glGenTextures(1, &_depthMapParallel);//深度贴图
    glBindTexture(GL_TEXTURE_2D, _depthMapParallel);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 4800, 2400, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    //这里同样设置几种纹理参数 近线性
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindFramebuffer(GL_FRAMEBUFFER, _depthMapParallelFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthMapParallel, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
   
    return _depthMapParallelFBO;  // 返回生成的深度贴图FBO
}
/// <summary>
/// 计算光源视图，绑定深度图进入帧缓冲区
/// </summary>
/// <param name="lightDirection"></param>
void Game::LightRender::RenderDepthMapForParallelLight(glm::vec3 lightDirection, const glm::mat4& cameraView, CustomModel* player, glm::vec3 offset)
{
    // 固定正交投影矩阵的范围
    glm::mat4 lightProjection = glm::ortho(-200.0f, 200.0f, -200.0f, 200.0f, 0.10f, 1000.0f);

    // 光源的基础位置（相对于世界坐标系）
    glm::vec3 baseLightPos = -lightDirection * 300.0f; // 基础位置 = -光线方向 * 距离

    // 根据玩家的位置动态调整光源的位置
    glm::vec3 lightPos = baseLightPos + player->position*1.0f; // 光源位置 = 基础位置 + 玩家位置

    glm::mat4 lightView = glm::lookAt(lightPos, lightPos+lightDirection, glm::vec3(0.0f, 1.0f, 0.0f));

    // 计算光源空间矩阵
    _lightSpaceMatrix = lightProjection * lightView;
}
glm::mat4 Game::LightRender::GetLightMatrix()
{
    return _lightSpaceMatrix;
}
GLuint Game::LightRender::CompileShader(const char* ver, const char* fra)
{
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &ver, nullptr);
    glCompileShader(vertexShader);


    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fra, nullptr);
    glCompileShader(fragmentShader);

    GLuint shader;
    // 2) 创建着色器程序
    shader = glCreateProgram();
    glAttachShader(shader, vertexShader);
    glAttachShader(shader, fragmentShader);
    glLinkProgram(shader);


    // 删除临时Shader
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    
    
    
    return shader;
}
GLuint Game::LightRender::GetDepthShaderProgram(ShaderClass shader)
{
    switch (shader)
    {
    case ShaderClass::DepthCalculate:
        return _depthShaderProgram;
    case ShaderClass::DepthRender:
        return _depthVisualShader;
    case ShaderClass::DepthTest:
        return _depthTestShader;
    case ShaderClass::DepthMapParallel://获取渲染完成之后的平行光深度阴影贴图
        return _depthMapParallel;
    case ShaderClass::DepthMapTest:
        return _depthMapTest;

    default:
        // 如果 num 的值不在 1, 2, 3 之内，返回一个默认值或处理错误
        return 0;  // 假设返回 0 或者其他默认值作为错误处理
    }
}

void Game::LightRender::UseDepthShaderProgram(ShaderClass shader)
{
    switch (shader)
    {
    case ShaderClass::DepthCalculate:
        glUseProgram(_depthShaderProgram);
        break;
    case ShaderClass::DepthRender:
        glUseProgram(_depthVisualShader);
        break;
    case ShaderClass::DepthTest:
        glUseProgram(_depthTestShader);
        break;
    default:
        // 如果 num 的值不在 1, 2, 3 之内，可以选择不执行任何操作，或者返回错误处理
        break;
    }
}

/// <summary>
/// 清空帧缓冲深度绑定
/// </summary>
void Game::LightRender::UnbindFramebuffer()
{

    glBindFramebuffer(GL_FRAMEBUFFER, 0);//解除绑定

    glViewport(0, 0, 2400, 1200);//阴影贴图渲染完成之后，解绑缓冲区，恢复本来的视口
}

void Game::LightRender::BindFramebuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, _depthMapParallelFBO);
    glViewport(0, 0, 4800, 2400);//调整视口以适应阴影贴图分辨率
    glClear(GL_DEPTH_BUFFER_BIT);  // 清空深度缓冲
   // glBindFramebuffer(GL_FRAMEBUFFER, 0);//解除绑定

//    glViewport(0, 0, 2400, 1600);
}

void Game::LightRender::RenderShadowTexture(CustomModel* obj,glm::mat4 crossView)
{
    
  
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TextureDic["butterfly"][PictureTpye::BaseP]);

    GLuint depthMapLoc = glGetUniformLocation(_depthVisualShader, "depthMap");
    glUniform1i(depthMapLoc, 0);  // 将纹理单元0传递给着色器

    obj->DrawDepthPic(crossView, _depthVisualShader);

}

void Game::LightRender::RenderShadowTexture(GLuint shader)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _depthMapParallel);

    GLuint depthMapLoc = glGetUniformLocation(shader, "depthMap");
    glUniform1i(depthMapLoc, 0);  // 将纹理单元0传递给着色器


    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

GLuint Game::LightRender::CreateDepthMapForTest()
{

    // 创建深度贴图
    glGenFramebuffers(1, &_depthMapTestFBO);//基于深度贴图的帧缓冲贴图对象GLuint
    glGenTextures(1, &_depthMapTest);//深度贴图
    glBindTexture(GL_TEXTURE_2D, _depthMapTest);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 2400, 1200, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    //这里同样设置几种纹理参数 近线性
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindFramebuffer(GL_FRAMEBUFFER, _depthMapTestFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,_depthMapTest, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return _depthMapTestFBO;  // 返回生成的深度贴图FBO
}

void Game::LightRender::BindDepthTestBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, _depthMapTestFBO);
    glViewport(0, 0, 2400, 1200);//调整视口以适应阴影贴图分辨率
    glClear(GL_DEPTH_BUFFER_BIT);  // 清空深度缓冲
}

void Game::LightRender::UnbindDepthTestBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);//解除绑定
    glViewport(0, 0, 2400, 1200);//阴影贴图渲染完成之后，解绑缓冲区，恢复本来的视口
}

void Game::LightRender::RenderDepthTestTexture(GLuint shader)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _depthMapTest);

    GLuint depthMapLoc = glGetUniformLocation(shader, "depthMap");
    glUniform1i(depthMapLoc, 0);  // 将纹理单元0传递给着色器


    if (quadVAO1 == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO1);
        glGenBuffers(1, &quadVBO1);
        glBindVertexArray(quadVAO1);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO1);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO1);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);

}

 glm::mat4 Game::LightRender::GetDepthMapCrossView(const glm::mat4& camerView, CustomModel* player, glm::vec3 offset)
{
    // 固定正交投影矩阵的范围
    glm::mat4 corssProjection = glm::ortho(-200.0f, 200.0f, -200.0f, 200.0f, 0.10f, 1000.0f);


    glm::mat4 crosstView = glm::lookAt(player->position+offset, player->position, glm::vec3(0.0f, 1.0f, 0.0f));

    // 计算视口正交矩阵
    return corssProjection *crosstView;
    
}


 GLuint Game::LightRender::CreatePostProcessingMap()
 {
     // 创建 FBO 对象
     glGenFramebuffers(1, &_postProcessingMapFBO);
     glBindFramebuffer(GL_FRAMEBUFFER, _postProcessingMapFBO);

     // 创建颜色纹理
     glGenTextures(1, &_postProcessingMap);
     glBindTexture(GL_TEXTURE_2D, _postProcessingMap);
     // 使用颜色格式 GL_RGBA8
     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 4800, 2400, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
     // 将颜色纹理附加到 FBO 的颜色附件0
     glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _postProcessingMap, 0);

     // 创建深度/模板渲染缓冲对象（RBO）
     GLuint depthRBO;
     glGenRenderbuffers(1, &depthRBO);
     glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
     glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 4800, 2400);
     glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthRBO);

     // 指定 FBO 的绘制目标为颜色附件 0
     GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
     glDrawBuffers(1, drawBuffers);

     // 检查 FBO 状态
     GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
     if (status != GL_FRAMEBUFFER_COMPLETE)
     {
         std::cout << "Post Processing FBO is incomplete! Status: 0x"
             << std::hex << status << std::endl;
     }

     glBindFramebuffer(GL_FRAMEBUFFER, 0);
     return _postProcessingMapFBO;
 }

 void Game::LightRender::BindPostProcessingBuffer()
 {
     glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);  // 清空深度缓冲,颜色缓冲，模板缓冲
     glBindFramebuffer(GL_FRAMEBUFFER, _postProcessingMapFBO);//绑定初始化好的缓冲参数
     glViewport(0, 0, 4800, 2400);//调整视口以适应阴影贴图分辨率
     glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);  // 清空深度缓冲,颜色缓冲，模板缓冲
 }

 void Game::LightRender::UbindPostProcessingBuffer()
 {
     glBindFramebuffer(GL_FRAMEBUFFER, 0);//解除绑定
     glViewport(0, 0, 2400,1200);//全局贴图渲染完成之后，恢复到原本视口，这里采用4800：2400高清贴图



 }

 void Game::LightRender::RenderPostProcessingTexture(GLuint shader)
 {
     glActiveTexture(GL_TEXTURE0);
     glBindTexture(GL_TEXTURE_2D, _postProcessingMap);

     GLuint postProcessingMapLoc = glGetUniformLocation(shader, "screenTexture");
     glUniform1i(postProcessingMapLoc, 0);  // 将纹理单元0传递给着色器


     if (quadVAO2 == 0)
     {
         float quadVertices[] = {
             // positions        // texture Coords
             -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
             -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
              1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
              1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
         };
         // setup plane VAO
         glGenVertexArrays(1, &quadVAO2);
         glGenBuffers(1, &quadVBO2);
         glBindVertexArray(quadVAO2);
         glBindBuffer(GL_ARRAY_BUFFER, quadVBO2);
         glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
         glEnableVertexAttribArray(0);
         glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
         glEnableVertexAttribArray(1);
         glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
     }
     glBindVertexArray(quadVAO2);
     glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
     glBindVertexArray(0);
 }

 LightRender::LightRender() {
    

    
    controller = Controller::GetInstance();
    lightSpawner = LightSpawner::GetInstance();



}

LightRender::~LightRender() {
    // 如有必要进行资源清理
}

void LightRender::RenderLights(GLuint shaderProgram, glm::vec3 position) {
  
    glUseProgram(shaderProgram);
  
    // 2. 点光源数据
    const auto& pointLights = lightSpawner->GetPointLights();
    std::vector<std::pair<float, int>> pointLightDistances;  // 存储每个点光源到物体的距离平方和索引,新声明的结构
    glm::vec3 objectPosition = position;  // 使用物体的当前位置作为参考

    // 遍历点光源计算距离平方，并且筛选出距离小于等于 1000 的光源
    for (int i = 0; i < pointLights.size(); i++) {
        const auto& light = pointLights[i];
        float distanceSquared = glm::dot(objectPosition - light.position, objectPosition - light.position);
        if (distanceSquared <= 1000.0f) {  // 只选择距离平方小于等于 100 的光源
            pointLightDistances.push_back({ distanceSquared, i });
            std::cout << i << "距离" << distanceSquared << std::endl;
        }
    }

    // 按照距离平方升序排序
    std::sort(pointLightDistances.begin(), pointLightDistances.end(), [](const std::pair<float, int>& a, const std::pair<float, int>& b) {
        return a.first < b.first;  // 按照距离平方升序排序
        });
     //不能动态传入数组数量，shader里面不支持，数组索引需写死，但是可以通过常量来约束 
    GLuint numPointLightsLoc = glGetUniformLocation(shaderProgram, "numPointLights");
    glUniform1i(numPointLightsLoc, std::min(static_cast<int>(pointLightDistances.size()), 4));

    // 只选择最近的 4 个点光源
    for (int i = 0; i < std::min(4, static_cast<int>(pointLightDistances.size())); i++) {
        int index = pointLightDistances[i].second;  // 获取最近的点光源的索引，第一个值是距离，第二个值是距离
        auto light = pointLights[index];
      //  std::cout << "点光源强度" << light.intensity << std::endl;
        // 计算距离平方并传递到 shader
        float distanceSquared = pointLightDistances[i].first;
      //  std::cout <<i<< "距离" << distanceSquared << std::endl;
        std::string posName = "lightPos[" + std::to_string(i) + "]";
        std::string colorName = "lightColor[" + std::to_string(i) + "]";
        std::string intensityName = "lightIntensity[" + std::to_string(i) + "]";
        std::string distanceSquaredName = "lightDistanceSquared[" + std::to_string(i) + "]"; // 新增变量

        GLuint posLoc = glGetUniformLocation(shaderProgram, posName.c_str());
        GLuint colorLoc = glGetUniformLocation(shaderProgram, colorName.c_str());
        GLuint intensityLoc = glGetUniformLocation(shaderProgram, intensityName.c_str());
        GLuint distanceSquaredLoc = glGetUniformLocation(shaderProgram, distanceSquaredName.c_str()); // 获取距离平方位置

        glUniform3fv(posLoc, 1, glm::value_ptr(light.position));
        glUniform3fv(colorLoc, 1, glm::value_ptr(light.color));
        glUniform1f(intensityLoc, light.intensity);
        glUniform1f(distanceSquaredLoc, distanceSquared);  // 传递距离平方
    }


    // 4. 手电筒（聚光灯）数据
     auto flashLights = lightSpawner->GetFlashLights();
    std::vector<std::pair<float, int>> flashLightDistances;  // 存储每个聚光灯到物体的距离平方和索引

    // 遍历聚光灯计算距离平方，并且筛选出距离小于等于 100 的聚光灯
    for (int j = 0; j < flashLights.size(); j++) {
        const auto& flash = flashLights[j];
        float distanceSquared = glm::dot(objectPosition - flash.position, objectPosition - flash.position);
        if (distanceSquared <= 100.0f) {  // 只选择距离小于等于 100 的聚光灯
            flashLightDistances.push_back({ distanceSquared, j });
        }
    }

    // 按照距离平方升序排序
    std::sort(flashLightDistances.begin(), flashLightDistances.end(), [](const std::pair<float, int>& a, const std::pair<float, int>& b) {
        return a.first < b.first;  // 按照距离平方升序排序
        });

    GLuint numFlashLightsLoc = glGetUniformLocation(shaderProgram, "numFlashLights");
    glUniform1i(numFlashLightsLoc, std::min(static_cast<int>(flashLightDistances.size()), 4));

    // 只选择最近的 4 个聚光灯
    for (int j = 0; j < std::min(4, static_cast<int>(flashLightDistances.size())); j++) {
        int index = flashLightDistances[j].second;  // 获取最近的聚光灯的索引
         auto flash = flashLights[index];

        // 计算距离平方并传递到 shader
        float distanceSquared = flashLightDistances[j].first;

        std::string flashPosName = "flashLightPos[" + std::to_string(j) + "]";
        std::string flashDirName = "flashLightDirection[" + std::to_string(j) + "]";
        std::string flashColorName = "flashLightColor[" + std::to_string(j) + "]";
        std::string flashIntensityName = "flashLightIntensity[" + std::to_string(j) + "]";
        std::string flashCutoffName = "flashLightCutoff[" + std::to_string(j) + "]";
        std::string flashDistanceSquaredName = "flashLightDistanceSquared[" + std::to_string(j) + "]"; // 新增变量

        GLuint flashPosLoc = glGetUniformLocation(shaderProgram, flashPosName.c_str());
        GLuint flashDirLoc = glGetUniformLocation(shaderProgram, flashDirName.c_str());
        GLuint flashColorLoc = glGetUniformLocation(shaderProgram, flashColorName.c_str());
        GLuint flashIntensityLoc = glGetUniformLocation(shaderProgram, flashIntensityName.c_str());
        GLuint flashCutoffLoc = glGetUniformLocation(shaderProgram, flashCutoffName.c_str());
        GLuint flashDistanceSquaredLoc = glGetUniformLocation(shaderProgram, flashDistanceSquaredName.c_str()); // 获取距离平方位置

        glUniform3fv(flashPosLoc, 1, glm::value_ptr(flash.position));
        glUniform3fv(flashDirLoc, 1, glm::value_ptr(flash.direction));
        glUniform3fv(flashColorLoc, 1, glm::value_ptr(flash.color));
        glUniform1f(flashIntensityLoc, flash.intensity);
        glUniform1f(flashCutoffLoc, flash.cutoff);
        glUniform1f(flashDistanceSquaredLoc, distanceSquared);  // 传递距离平方
    }
}


