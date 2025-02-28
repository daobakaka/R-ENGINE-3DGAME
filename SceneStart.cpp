#include <iostream>
#include "Monster.h"
#include <vector>
#include <GL/glew.h>//����Opengl��չ��
#include <GLFW/glfw3.h>//����Opengl��չ���Լ�����Opengl������
#include "Cube.h"
//#include "shader.h"  // ����һ����������ɫ���ļ�����ֻ�ܱ�����һ��
#include "Controller.h"
#include "LifecycleManager.h"
#include "IntergratedScripts.h"
#include <list>  // ���� list ����
#include "MeshDataManager.h"
#include "CustomModel.h"
#include "Light.h"
#include "FileLoadIO.h"
#include "TextRender.h"
#include "CoroutineMethod.h"
#include "ScriptModel.h"
#include <chrono>
#include <thread>
#include "ShaderManager.h"

using namespace Game;
//�������ʵ����ָ��,�������ʼ
template <typename T>
LifecycleManager<T>* LifecycleManager<T>::instance = nullptr;
Controller* controller;
LifecycleManager<CustomModel>* manager ;
IntergratedScripts* scripts ;
MeshDataManager* meshData ;
TextRender* cusText ;
CoroutineMethod* coroutine ;
LightSpawner* lightSpawner ;
LightRender* lightRender ;
ShaderManager* shaderManager;


void CallbackMouseScrollGlobal(GLFWwindow* window, double xoffset, double yoffset)
{


    Controller::GetInstance()->ProcessMouseScrollGlobal(static_cast<float>(yoffset));
}

GLFWwindow* GLinitializeT()
{
    GLFWwindow* window = glfwCreateWindow(controller->windowWidth, controller->windowHeight, "OpenGL Window", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        std::cerr << "Failed to create OpenGL window!" << std::endl;
        return nullptr;
    }
    //��ʼ��
    glfwMakeContextCurrent(window);//���õ�ǰ���Ƶ������Ļ���������ǰ����
    glfwSetScrollCallback(window, CallbackMouseScrollGlobal);//�����ֻص�
    GLenum glewInitResult = glewInit();
    //����Openglʹ�ð汾��4.5
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    //����OpenGL��Ⱦģʽ������ģʽ����������Ⱦ,��������֮��ò�Ʋ�����
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    if (GLEW_OK != glewInitResult) {
        std::cerr << "Error initializing GLEW: " << glewGetErrorString(glewInitResult) << std::endl;
        return nullptr;
    }
    // ��ȡ���ڳߴ�
    glfwGetWindowSize(window, &controller->windowWidth, &controller->windowHeight);

    // ������Ȳ��Ե�
    glEnable(GL_DEPTH_TEST);//��Ȳ���
    glEnable(GL_ALPHA_TEST);//alpha����
    glEnable(GL_STENCIL_TEST);//ģ�����
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LESS); // ������Ⱥ�����ͨ��ʹ��GL_LESS
    //���û��
   // glEnable(GL_BLEND);
   // glDisable(GL_BLEND);  // ���û��
    // glDepthFunc(GL_LESS);
    // glEnable(GL_CULL_FACE);
   // glDisable(GL_CULL_FACE);
    // glCullFace(GL_FRONT);   // ֻ�޳�����//�����޳�


    return window;
}


void GameAwakeT()
{
    // ��ȡ Controller ʵ��,�Զ��������ڳ�ʼ���������������ص�ͼ�δ��ڲ���
   controller = Controller::GetInstance();
    //��ȡ�������ڹ�����,�ں��������Ƶ�
    manager = LifecycleManager<CustomModel>::GetInstance();//�������м̳���MonoBehaviour����,����ʵ����Ϊ��CustomModel
    //��ȡ�ۺϽű�������
    scripts = IntergratedScripts::GetInstance();
    //��ȡ���������
    meshData = MeshDataManager::GetInstance();
    //��ȡ������Ⱦ��
    cusText = TextRender::GetInstance();
    //��ȡЭ�̿�����
    coroutine = CoroutineMethod::GetInstance();
    //��ȡ�ƹ�������
    lightSpawner = LightSpawner::GetInstance();
    lightSpawner->modelIdentification = false;
    //��ȡ�ƹ���Ⱦ��
    lightRender = LightRender::GetInstance();
    //��ȡ��ɫ��������,�������Ĭ�ϱ���
     shaderManager = ShaderManager::GetInstance();

}
void SourceInitialize()
{
    //����ģ������
    MakeModel();
    // ��������ģ�飬���������̬��չ��������Ӳ�ͬ�Ķ��������������Ը߶ȷ�װ�Ķ������������
    MakeAnimation();
    // ��������
    MakeTxture();
    //����������Դ
    MakeFronts();

}

void LightInitialization()
{

    //Ԥ��������Դ��������������������޶���Ŀǰ���յ�ʵ�־�Ϊʵʱ���գ�Ŀǰû���ṩע��ģʽ��Ŀǰû������
//��ͨ���ƹ���Ⱦ�߼�������ʵ����������Ϸ����Ĺ�����Ⱦ�߼��ж�
//��ʼ��������Ⱦ��Ӱ��ƽ�й����ͼ������������ֱ�ӹ��죬������Ӱ��ɫ��
    lightRender->CreateShadowMapForParallelLight();
    //���Դ����ʹ�õƹ���������,���Զ���4���ƹ⣬������̬�ı仯
    //auto pointLight2 = lightSpawner->SpawPointLight(glm::vec3(2, 2, 2), glm::vec3(1, 1, 1), 10);
    //auto pointLight = lightSpawner->SpawPointLight(glm::vec3(3, 0, 0), glm::vec3(0, 1, 1), 10);
    //auto pointLight3 = lightSpawner->SpawPointLight(glm::vec3(0, 3, 0), glm::vec3(0, 1, 0), 20);
    //auto pointLight4 = lightSpawner->SpawPointLight(glm::vec3(-3, 0, -3), glm::vec3(0, 0, 1), 20);

    //ƽ�й�ʹ�õƹ����������ɣ�Ĭ��һ��
    auto parallelLight = lightSpawner->SpawParallelLight(glm::vec3(-1), glm::vec3(1, 1, 1), 10);//ʹ��Ĭ��ֵ ǿ��10
    //�ֵ�Ͳ��ʹ�õƹ����������ɣ�Ĭ��֧��4��
    //auto splashLight = lightSpawner->SpawFlashLight(glm::vec3(0, 5, -2), glm::vec3(0, -1, 0));//ʹ��Ĭ��ֵ ǿ��10

}
void GameStartT()
{



    auto* baseSphere = new CustomModelShader("commonLight", ModelDic["baseSphere"], false, true, true);
    baseSphere->SetVariant(ModelClass::CubeTestE);
    baseSphere->Initialize(glm::vec3(-3.0f, -2.0f, -2.0f), glm::quat(glm::vec3(0.0f, 45.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f));
    manager->RegisterObject(baseSphere);
    baseSphere->AttachTexture(TextureDic["butterfly"][0], 0);

    //����Բ��
    auto* baseCylinder = new CustomModelShader("commonLight", ModelDic["baseCylinder"], false, true, true);
    baseCylinder->SetVariant(ModelClass::TestPhysics);
    baseCylinder->Initialize(glm::vec3(-0.0f, 1.0f, -2.0f), glm::quat(glm::vec3(0.0f, 45.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f));
    manager->RegisterObject(baseCylinder);
    baseCylinder->AttachTexture(TextureDic["default"][0], 0);
    baseCylinder->AttachPhysicalEngine();
    baseCylinder->AttachCollider();

    auto* basePlane = new  CustomModelShader("commonLight", ModelDic["basePlane"], false, false, false);
    basePlane->SetVariant(ModelClass::StaticPlane);
    basePlane->Initialize(glm::vec3(0.0f, -5.0f, 0.0f), glm::quat(glm::vec3(0.0f, .0f, 0.0f)), glm::vec3(50.0f, 0.1f, 50.0f));
    manager->RegisterObject(basePlane);
    basePlane->AttachTexture(TextureDic["stone"][0], 0, glm::vec2(1, 1));
    basePlane->AttachPhysicalEngine(true);//����Ϊ��̬���ͣ�Ŀǰע��Ϊ1
    basePlane->AttachCollider(CollisionType::Box,10);

    auto* butterflyInstance = new  CustomModelInstance("noneLightInstancer", ModelDic["butterfly"], false, false, false, 10000, glm::vec3(100, 100, -100));
    butterflyInstance->SetVariant(ModelClass::CubeE);
    butterflyInstance->Initialize(glm::vec3(0.0f, -5.0f, 0.0f), glm::quat(glm::vec3(0.0f, .0f, 0.0f)), glm::vec3(0.3f, 0.3f, 0.30f));
    manager->RegisterObject(butterflyInstance);
    butterflyInstance->AttachTexture(TextureDic["butterfly"][0], 0, glm::vec2(1, 1));

    StepVector3 step;
    step.position = glm::vec3(0, -0.5f, 0);
    step.scale = glm::vec3(0.9f);
    coroutine->StartSpawnByTimerAnimation<ButterflyScriptShader>(
        manager,
        "commonLight",
        true,
        true,
        ModelDic["butterfly"],
        AnimationDic["butterfly"]["fly"],
        TextureDic["butterfly"][0],
        0,
        glm::vec2(10, 10),
        ModelClass::TsetButterfly,
        1, 1,
        step,
        glm::vec3(0.0f, 0.0f, 0.0f),//��Ĭ��ֵ
        {},
        glm::vec3(1.1f)
    );
    //auto* testButterfly = new  ButterflyScriptShader("commonLight", ModelDic["butterfly"], true, false, true);
    //testButterfly->SetVariant(ModelClass::TsetButterfly);
    //testButterfly->Initialize(glm::vec3(0.0f, 3.0f, 0.0f), glm::quat(glm::vec3(0.0f, .0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f));
    //manager->RegisterObject(testButterfly);
    //testButterfly->AttachTexture(TextureDic["stone"][0], 0, glm::vec2(1, 1));
    //testButterfly->AttachAnimationController(AnimationDic["butterfly"]["fly"]);


    //CustomModel* testMonkey = new CustomModel(colorlightsArrayVertexShaderSource, colorlightsArraySourceFragmentShaderSource, ModelDic["testMonkey"], false, true);
    //testMonkey->SetVariant(ModelClass::CubeTestE);
    //testMonkey->Initialize(glm::vec3(-0.0f, 3.0f, 2.0f), glm::quat(glm::vec3(0.0f, 45.0f, 0.0f)), glm::vec3(0.5f));
    //manager->RegisterObject(testMonkey);
    //testMonkey->AttachTexture(TextureDic["butterfly"][0], 0,glm::vec2(1,1));
    //�����õķ���������ע�����е�ActorButterFly ����
  // coroutine->StartSpawnButterfliesByTimer(manager, TextureDic["butterfly"][0],0);
    //�ۺ��Ը�ֵ�������ͷ����� ����ֱ���첽��ʼ�����ּ̳�customModel�Ķ���

}


