#include "Light.h"
#include <glm/gtc/matrix_transform.hpp>
#include "FileLoadIO.h"
#include "CustomModel.h"
#include "LifecycleManager.h"
#include "ScriptModel.h"
#include "MeshDataManager.h"
#include <algorithm>  // ���� std::sort
#include <iostream>
#include "EnumTotal.h"
#include <glm/gtc/type_ptr.hpp>

using namespace Game;

// ��ʼ����̬����ָ��
LightSpawner* LightSpawner::instance = nullptr;
//����shader
extern const char* noneLightLightVertexShaderSource;
extern const char* noneLightLightFragmentShaderSource;
extern const char* rayVertexShaderSource;
extern const char* rayFragmentShaderSource;
//���ͼ������ɫ��������opengl������̻��ƣ�����һ����ȫ����Ⱦ
extern const char* depthShaderVertexShaderSource;
extern const char* depthShaderFragmentShaderSource;
//���ͼ���ӻ���ɫ��
extern const char* depthVisualShaderVertexShaderSource;
extern const char* depthVisualShaderFragmentShaderSource;
//���ͼ������ɫ��
extern const char* depthTestShaderVertexShaderSource;
extern const char* depthTestShaderFragmentShaderSource;
//C++��Դ�ļ��п���ֱ�Ӷ�����ر�����������Ҫ�����н��������������
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
    // ��ʼ��ȫ��ƽ�й�Ĭ�ϲ���
    manager = LifecycleManager<CustomModel>::GetInstance();
    meshData = MeshDataManager::GetInstance();
    controller = Controller::GetInstance();
    modelIdentification = true;
}

LightSpawner::~LightSpawner() {
    // ������Ҫ�������������
}

CustomPointLight LightSpawner::SpawPointLight(glm::vec3 position, glm::vec3 color, float intensity) {
    CustomPointLight pointLight;
    pointLight.position = position;
    pointLight.color = color;
    pointLight.intensity = intensity;

    // �����ǰ���Դ����δ�ﵽ���ޣ��򱣴浽�ڲ�������
    if (pointLights.size() < 4) {
        pointLights.push_back(pointLight);
    }
    // �������ѡ����Ի�������й�Դ�����ݾ���������
    if (modelIdentification)
    {
        //--ͬʱ����Ӧ��λ�ù���һ�����Դģ��
        auto* pointLightPtr = new LightModel(noneLightLightVertexShaderSource, noneLightLightFragmentShaderSource, ModelDic["baseSphere"], false);
        pointLightPtr->SetVariant(ModelClass::PointLight);
        pointLightPtr->Initialize(position, glm::quat(glm::vec3(0, 0, 1.5f)), glm::vec3(0.3f));
        manager->RegisterObject(pointLightPtr);
        pointLightPtr->AttachTexture(TextureDic["light"]);
        //����ķ��������õƹ����
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
        //--ƽ�й���ȫ�ֹ��գ�λ�������壬���ƹ�Դ������λ����������ģ�ʹ��棬����0.1��ͷ��ʾ�Ƿ��壬Ӧ�޾���˥��
        auto* pointLightPtr = new LightModel(noneLightLightVertexShaderSource, noneLightLightFragmentShaderSource, ModelDic["testMonkey"], false);
        pointLightPtr->SetVariant(ModelClass::ParallelLight);
        pointLightPtr->Initialize(glm::vec3(0), glm::vec3(0), glm::vec3(0.3f));
        manager->RegisterObject(pointLightPtr);
        pointLightPtr->AttachTexture(TextureDic["light"]);
        //����ķ��������õƹ����
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
        //�����ֵ�Ͳ��������
        glm::vec3 defaultDown = glm::vec3(0.0f, -1.0f, 0.0f);  // Բ׶����Ĭ�ϳ��� Y,����������������ķ���
        glm::vec3 dirNorm = glm::normalize(direction);

        float dotVal = glm::dot(defaultDown, dirNorm);
        glm::quat finalRotation;
        const float epsilon = 1e-6f;

        if (dotVal > 1.0f - epsilon) {
            // �� defaultDown ����ͬ�� => ����ת
            finalRotation = glm::quat(1, 0, 0, 0);
        }
        else if (dotVal < -1.0f + epsilon) {
            // ���� => ������ˮƽ����ת180��
            finalRotation = glm::angleAxis(glm::pi<float>(), glm::vec3(1, 0, 0));
        }
        else {
            float angle = acos(dotVal);
            glm::vec3 axis = glm::normalize(glm::cross(defaultDown, dirNorm));
            finalRotation = glm::angleAxis(angle, axis);
        }



        //--ͬʱ����Ӧ��λ�ù���һ���ֵ�Ͳģ��
        auto* pointLightPtr = new LightModel(noneLightLightVertexShaderSource, noneLightLightFragmentShaderSource, ModelDic["baseCone"], false);
        pointLightPtr->SetVariant(ModelClass::FlashLight);
        pointLightPtr->Initialize(position, finalRotation, glm::vec3(0.3f));
        manager->RegisterObject(pointLightPtr);
        pointLightPtr->AttachTexture(TextureDic["light"]);
        //����ķ��������õƹ����
        pointLightPtr->SteLightParameters(color, intensity);

        //--�����ֵ�Ͳ�ο�����-------------
        //��������Զ��嶥�㣬���������㣬���ڸ������߳���
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
    // 2) �������Ƕ�(��)תΪ����
    glm::vec3 radIncrements = glm::radians(dirction);

    // 3) ������ת����(��ʼΪ��λ����)
    glm::mat4 rotMat(1.0f);

    // ����˳������X��תpitch������Y��תyaw������Z��תroll
    rotMat = glm::rotate(rotMat, radIncrements.x, glm::vec3(1, 0, 0));
    rotMat = glm::rotate(rotMat, radIncrements.y, glm::vec3(0, 1, 0));
    rotMat = glm::rotate(rotMat, radIncrements.z, glm::vec3(0, 0, 1));

    // 4) �����������ת������Ե�ǰ��� direction
    //    parallelLight.direction����֡(���ʼ)�ĳ����������ۼ���ת
    glm::vec4 oldDir(parallelLight.direction, 0.0f);
    glm::vec4 newDir = rotMat * oldDir;

    // 5) д�ز���һ��
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

#pragma region  ��Ⱦ�ű�����

#pragma endregion

/// <summary>
/// ��̬ʵ��ָ��
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

    //---3��shaderһ�����
//������Ӱ���������ͼ��Ⱦshader��Ȼ����������ɫ�������������Oepngl �������н��У���˲��ܷ��ڹ���
    _depthShaderProgram = CompileShader(depthShaderVertexShaderSource, depthShaderFragmentShaderSource);
    _depthVisualShader = CompileShader(depthVisualShaderVertexShaderSource, depthVisualShaderFragmentShaderSource);
    _depthTestShader = CompileShader(depthTestShaderVertexShaderSource, depthTestShaderFragmentShaderSource);
    //-------------------------------------------------------   


    // ���������ͼ
    glGenFramebuffers(1, &_depthMapParallelFBO);//���������ͼ��֡������ͼ����GLuint
    glGenTextures(1, &_depthMapParallel);//�����ͼ
    glBindTexture(GL_TEXTURE_2D, _depthMapParallel);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 4800, 2400, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    //����ͬ�����ü���������� ������
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindFramebuffer(GL_FRAMEBUFFER, _depthMapParallelFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthMapParallel, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
   
    return _depthMapParallelFBO;  // �������ɵ������ͼFBO
}
/// <summary>
/// �����Դ��ͼ�������ͼ����֡������
/// </summary>
/// <param name="lightDirection"></param>
void Game::LightRender::RenderDepthMapForParallelLight(glm::vec3 lightDirection, const glm::mat4& cameraView, CustomModel* player, glm::vec3 offset)
{
    // �̶�����ͶӰ����ķ�Χ
    glm::mat4 lightProjection = glm::ortho(-200.0f, 200.0f, -200.0f, 200.0f, 0.10f, 1000.0f);

    // ��Դ�Ļ���λ�ã��������������ϵ��
    glm::vec3 baseLightPos = -lightDirection * 300.0f; // ����λ�� = -���߷��� * ����

    // ������ҵ�λ�ö�̬������Դ��λ��
    glm::vec3 lightPos = baseLightPos + player->position*1.0f; // ��Դλ�� = ����λ�� + ���λ��

    glm::mat4 lightView = glm::lookAt(lightPos, lightPos+lightDirection, glm::vec3(0.0f, 1.0f, 0.0f));

    // �����Դ�ռ����
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
    // 2) ������ɫ������
    shader = glCreateProgram();
    glAttachShader(shader, vertexShader);
    glAttachShader(shader, fragmentShader);
    glLinkProgram(shader);


    // ɾ����ʱShader
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
    case ShaderClass::DepthMapParallel://��ȡ��Ⱦ���֮���ƽ�й������Ӱ��ͼ
        return _depthMapParallel;
    case ShaderClass::DepthMapTest:
        return _depthMapTest;

    default:
        // ��� num ��ֵ���� 1, 2, 3 ֮�ڣ�����һ��Ĭ��ֵ�������
        return 0;  // ���践�� 0 ��������Ĭ��ֵ��Ϊ������
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
        // ��� num ��ֵ���� 1, 2, 3 ֮�ڣ�����ѡ��ִ���κβ��������߷��ش�����
        break;
    }
}

/// <summary>
/// ���֡������Ȱ�
/// </summary>
void Game::LightRender::UnbindFramebuffer()
{

    glBindFramebuffer(GL_FRAMEBUFFER, 0);//�����

    glViewport(0, 0, 2400, 1200);//��Ӱ��ͼ��Ⱦ���֮�󣬽�󻺳������ָ��������ӿ�
}

void Game::LightRender::BindFramebuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, _depthMapParallelFBO);
    glViewport(0, 0, 4800, 2400);//�����ӿ�����Ӧ��Ӱ��ͼ�ֱ���
    glClear(GL_DEPTH_BUFFER_BIT);  // �����Ȼ���
   // glBindFramebuffer(GL_FRAMEBUFFER, 0);//�����

//    glViewport(0, 0, 2400, 1600);
}

void Game::LightRender::RenderShadowTexture(CustomModel* obj,glm::mat4 crossView)
{
    
  
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TextureDic["butterfly"][PictureTpye::BaseP]);

    GLuint depthMapLoc = glGetUniformLocation(_depthVisualShader, "depthMap");
    glUniform1i(depthMapLoc, 0);  // ������Ԫ0���ݸ���ɫ��

    obj->DrawDepthPic(crossView, _depthVisualShader);

}

void Game::LightRender::RenderShadowTexture(GLuint shader)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _depthMapParallel);

    GLuint depthMapLoc = glGetUniformLocation(shader, "depthMap");
    glUniform1i(depthMapLoc, 0);  // ������Ԫ0���ݸ���ɫ��


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

    // ���������ͼ
    glGenFramebuffers(1, &_depthMapTestFBO);//���������ͼ��֡������ͼ����GLuint
    glGenTextures(1, &_depthMapTest);//�����ͼ
    glBindTexture(GL_TEXTURE_2D, _depthMapTest);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 2400, 1200, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    //����ͬ�����ü���������� ������
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindFramebuffer(GL_FRAMEBUFFER, _depthMapTestFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,_depthMapTest, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return _depthMapTestFBO;  // �������ɵ������ͼFBO
}

void Game::LightRender::BindDepthTestBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, _depthMapTestFBO);
    glViewport(0, 0, 2400, 1200);//�����ӿ�����Ӧ��Ӱ��ͼ�ֱ���
    glClear(GL_DEPTH_BUFFER_BIT);  // �����Ȼ���
}

void Game::LightRender::UnbindDepthTestBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);//�����
    glViewport(0, 0, 2400, 1200);//��Ӱ��ͼ��Ⱦ���֮�󣬽�󻺳������ָ��������ӿ�
}

void Game::LightRender::RenderDepthTestTexture(GLuint shader)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _depthMapTest);

    GLuint depthMapLoc = glGetUniformLocation(shader, "depthMap");
    glUniform1i(depthMapLoc, 0);  // ������Ԫ0���ݸ���ɫ��


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
    // �̶�����ͶӰ����ķ�Χ
    glm::mat4 corssProjection = glm::ortho(-200.0f, 200.0f, -200.0f, 200.0f, 0.10f, 1000.0f);


    glm::mat4 crosstView = glm::lookAt(player->position+offset, player->position, glm::vec3(0.0f, 1.0f, 0.0f));

    // �����ӿ���������
    return corssProjection *crosstView;
    
}


 GLuint Game::LightRender::CreatePostProcessingMap()
 {
     // ���� FBO ����
     glGenFramebuffers(1, &_postProcessingMapFBO);
     glBindFramebuffer(GL_FRAMEBUFFER, _postProcessingMapFBO);

     // ������ɫ����
     glGenTextures(1, &_postProcessingMap);
     glBindTexture(GL_TEXTURE_2D, _postProcessingMap);
     // ʹ����ɫ��ʽ GL_RGBA8
     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 4800, 2400, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
     // ����ɫ�����ӵ� FBO ����ɫ����0
     glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _postProcessingMap, 0);

     // �������/ģ����Ⱦ�������RBO��
     GLuint depthRBO;
     glGenRenderbuffers(1, &depthRBO);
     glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
     glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 4800, 2400);
     glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthRBO);

     // ָ�� FBO �Ļ���Ŀ��Ϊ��ɫ���� 0
     GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
     glDrawBuffers(1, drawBuffers);

     // ��� FBO ״̬
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
     glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);  // �����Ȼ���,��ɫ���壬ģ�建��
     glBindFramebuffer(GL_FRAMEBUFFER, _postProcessingMapFBO);//�󶨳�ʼ���õĻ������
     glViewport(0, 0, 4800, 2400);//�����ӿ�����Ӧ��Ӱ��ͼ�ֱ���
     glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);  // �����Ȼ���,��ɫ���壬ģ�建��
 }

 void Game::LightRender::UbindPostProcessingBuffer()
 {
     glBindFramebuffer(GL_FRAMEBUFFER, 0);//�����
     glViewport(0, 0, 2400,1200);//ȫ����ͼ��Ⱦ���֮�󣬻ָ���ԭ���ӿڣ��������4800��2400������ͼ



 }

 void Game::LightRender::RenderPostProcessingTexture(GLuint shader)
 {
     glActiveTexture(GL_TEXTURE0);
     glBindTexture(GL_TEXTURE_2D, _postProcessingMap);

     GLuint postProcessingMapLoc = glGetUniformLocation(shader, "screenTexture");
     glUniform1i(postProcessingMapLoc, 0);  // ������Ԫ0���ݸ���ɫ��


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
    // ���б�Ҫ������Դ����
}

void LightRender::RenderLights(GLuint shaderProgram, glm::vec3 position) {
  
    glUseProgram(shaderProgram);
  
    // 2. ���Դ����
    const auto& pointLights = lightSpawner->GetPointLights();
    std::vector<std::pair<float, int>> pointLightDistances;  // �洢ÿ�����Դ������ľ���ƽ��������,�������Ľṹ
    glm::vec3 objectPosition = position;  // ʹ������ĵ�ǰλ����Ϊ�ο�

    // �������Դ�������ƽ��������ɸѡ������С�ڵ��� 1000 �Ĺ�Դ
    for (int i = 0; i < pointLights.size(); i++) {
        const auto& light = pointLights[i];
        float distanceSquared = glm::dot(objectPosition - light.position, objectPosition - light.position);
        if (distanceSquared <= 1000.0f) {  // ֻѡ�����ƽ��С�ڵ��� 100 �Ĺ�Դ
            pointLightDistances.push_back({ distanceSquared, i });
            std::cout << i << "����" << distanceSquared << std::endl;
        }
    }

    // ���վ���ƽ����������
    std::sort(pointLightDistances.begin(), pointLightDistances.end(), [](const std::pair<float, int>& a, const std::pair<float, int>& b) {
        return a.first < b.first;  // ���վ���ƽ����������
        });
     //���ܶ�̬��������������shader���治֧�֣�����������д�������ǿ���ͨ��������Լ�� 
    GLuint numPointLightsLoc = glGetUniformLocation(shaderProgram, "numPointLights");
    glUniform1i(numPointLightsLoc, std::min(static_cast<int>(pointLightDistances.size()), 4));

    // ֻѡ������� 4 �����Դ
    for (int i = 0; i < std::min(4, static_cast<int>(pointLightDistances.size())); i++) {
        int index = pointLightDistances[i].second;  // ��ȡ����ĵ��Դ����������һ��ֵ�Ǿ��룬�ڶ���ֵ�Ǿ���
        auto light = pointLights[index];
      //  std::cout << "���Դǿ��" << light.intensity << std::endl;
        // �������ƽ�������ݵ� shader
        float distanceSquared = pointLightDistances[i].first;
      //  std::cout <<i<< "����" << distanceSquared << std::endl;
        std::string posName = "lightPos[" + std::to_string(i) + "]";
        std::string colorName = "lightColor[" + std::to_string(i) + "]";
        std::string intensityName = "lightIntensity[" + std::to_string(i) + "]";
        std::string distanceSquaredName = "lightDistanceSquared[" + std::to_string(i) + "]"; // ��������

        GLuint posLoc = glGetUniformLocation(shaderProgram, posName.c_str());
        GLuint colorLoc = glGetUniformLocation(shaderProgram, colorName.c_str());
        GLuint intensityLoc = glGetUniformLocation(shaderProgram, intensityName.c_str());
        GLuint distanceSquaredLoc = glGetUniformLocation(shaderProgram, distanceSquaredName.c_str()); // ��ȡ����ƽ��λ��

        glUniform3fv(posLoc, 1, glm::value_ptr(light.position));
        glUniform3fv(colorLoc, 1, glm::value_ptr(light.color));
        glUniform1f(intensityLoc, light.intensity);
        glUniform1f(distanceSquaredLoc, distanceSquared);  // ���ݾ���ƽ��
    }


    // 4. �ֵ�Ͳ���۹�ƣ�����
     auto flashLights = lightSpawner->GetFlashLights();
    std::vector<std::pair<float, int>> flashLightDistances;  // �洢ÿ���۹�Ƶ�����ľ���ƽ��������

    // �����۹�Ƽ������ƽ��������ɸѡ������С�ڵ��� 100 �ľ۹��
    for (int j = 0; j < flashLights.size(); j++) {
        const auto& flash = flashLights[j];
        float distanceSquared = glm::dot(objectPosition - flash.position, objectPosition - flash.position);
        if (distanceSquared <= 100.0f) {  // ֻѡ�����С�ڵ��� 100 �ľ۹��
            flashLightDistances.push_back({ distanceSquared, j });
        }
    }

    // ���վ���ƽ����������
    std::sort(flashLightDistances.begin(), flashLightDistances.end(), [](const std::pair<float, int>& a, const std::pair<float, int>& b) {
        return a.first < b.first;  // ���վ���ƽ����������
        });

    GLuint numFlashLightsLoc = glGetUniformLocation(shaderProgram, "numFlashLights");
    glUniform1i(numFlashLightsLoc, std::min(static_cast<int>(flashLightDistances.size()), 4));

    // ֻѡ������� 4 ���۹��
    for (int j = 0; j < std::min(4, static_cast<int>(flashLightDistances.size())); j++) {
        int index = flashLightDistances[j].second;  // ��ȡ����ľ۹�Ƶ�����
         auto flash = flashLights[index];

        // �������ƽ�������ݵ� shader
        float distanceSquared = flashLightDistances[j].first;

        std::string flashPosName = "flashLightPos[" + std::to_string(j) + "]";
        std::string flashDirName = "flashLightDirection[" + std::to_string(j) + "]";
        std::string flashColorName = "flashLightColor[" + std::to_string(j) + "]";
        std::string flashIntensityName = "flashLightIntensity[" + std::to_string(j) + "]";
        std::string flashCutoffName = "flashLightCutoff[" + std::to_string(j) + "]";
        std::string flashDistanceSquaredName = "flashLightDistanceSquared[" + std::to_string(j) + "]"; // ��������

        GLuint flashPosLoc = glGetUniformLocation(shaderProgram, flashPosName.c_str());
        GLuint flashDirLoc = glGetUniformLocation(shaderProgram, flashDirName.c_str());
        GLuint flashColorLoc = glGetUniformLocation(shaderProgram, flashColorName.c_str());
        GLuint flashIntensityLoc = glGetUniformLocation(shaderProgram, flashIntensityName.c_str());
        GLuint flashCutoffLoc = glGetUniformLocation(shaderProgram, flashCutoffName.c_str());
        GLuint flashDistanceSquaredLoc = glGetUniformLocation(shaderProgram, flashDistanceSquaredName.c_str()); // ��ȡ����ƽ��λ��

        glUniform3fv(flashPosLoc, 1, glm::value_ptr(flash.position));
        glUniform3fv(flashDirLoc, 1, glm::value_ptr(flash.direction));
        glUniform3fv(flashColorLoc, 1, glm::value_ptr(flash.color));
        glUniform1f(flashIntensityLoc, flash.intensity);
        glUniform1f(flashCutoffLoc, flash.cutoff);
        glUniform1f(flashDistanceSquaredLoc, distanceSquared);  // ���ݾ���ƽ��
    }
}


