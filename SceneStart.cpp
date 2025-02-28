#include <iostream>
#include "Monster.h"
#include <vector>
#include <GL/glew.h>//加载Opengl扩展，
#include <GLFW/glfw3.h>//创建Opengl扩展，以及管理Opengl上下文
#include "Cube.h"
//#include "shader.h"  // 这是一个独立的着色器文件，且只能被引用一次
#include "Controller.h"
#include "LifecycleManager.h"
#include "IntergratedScripts.h"
#include <list>  // 包含 list 容器
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
//泛型类的实例化指针,这里最初始
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
    //初始化
    glfwMakeContextCurrent(window);//设置当前绘制的上下文环境，即当前窗体
    glfwSetScrollCallback(window, CallbackMouseScrollGlobal);//鼠标滚轮回调
    GLenum glewInitResult = glewInit();
    //设置Opengl使用版本，4.5
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    //设置OpenGL渲染模式，核心模式，非立即渲染,这个添加了之后貌似不卡了
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    if (GLEW_OK != glewInitResult) {
        std::cerr << "Error initializing GLEW: " << glewGetErrorString(glewInitResult) << std::endl;
        return nullptr;
    }
    // 获取窗口尺寸
    glfwGetWindowSize(window, &controller->windowWidth, &controller->windowHeight);

    // 启用深度测试等
    glEnable(GL_DEPTH_TEST);//深度测试
    glEnable(GL_ALPHA_TEST);//alpha测试
    glEnable(GL_STENCIL_TEST);//模板测试
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LESS); // 设置深度函数，通常使用GL_LESS
    //启用混合
   // glEnable(GL_BLEND);
   // glDisable(GL_BLEND);  // 禁用混合
    // glDepthFunc(GL_LESS);
    // glEnable(GL_CULL_FACE);
   // glDisable(GL_CULL_FACE);
    // glCullFace(GL_FRONT);   // 只剔除背面//背面剔除


    return window;
}


void GameAwakeT()
{
    // 获取 Controller 实例,自定义类用于初始化主摄像机，及相关的图形窗口参数
   controller = Controller::GetInstance();
    //获取生命周期管理器,内含动画控制等
    manager = LifecycleManager<CustomModel>::GetInstance();//管理所有继承了MonoBehaviour的类,这里实例化为了CustomModel
    //获取综合脚本控制器
    scripts = IntergratedScripts::GetInstance();
    //获取网格管理器
    meshData = MeshDataManager::GetInstance();
    //获取字体渲染器
    cusText = TextRender::GetInstance();
    //获取协程控制器
    coroutine = CoroutineMethod::GetInstance();
    //获取灯光生成器
    lightSpawner = LightSpawner::GetInstance();
    lightSpawner->modelIdentification = false;
    //获取灯光渲染器
    lightRender = LightRender::GetInstance();
    //获取着色器控制器,构造添加默认编译
     shaderManager = ShaderManager::GetInstance();

}
void SourceInitialize()
{
    //加载模型区域
    MakeModel();
    // 动画控制模块，这个方法动态拓展，用于添加不同的动画，接下来可以高度封装的动画声明及添加
    MakeAnimation();
    // 加载纹理
    MakeTxture();
    //加载字体资源
    MakeFronts();

}

void LightInitialization()
{

    //预定义最大光源数量，这里可以做性能限定，目前光照的实现均为实时光照，目前没有提供注入模式，目前没有限制
//但通过灯光渲染逻辑，现在实现了类似游戏引擎的光线渲染逻辑判断
//初始化用于渲染阴影的平行光深度图，可以在类中直接构造，编译阴影着色器
    lightRender->CreateShadowMapForParallelLight();
    //点光源生成使用灯光控制器完成,测试定义4个灯光，物体形态的变化
    //auto pointLight2 = lightSpawner->SpawPointLight(glm::vec3(2, 2, 2), glm::vec3(1, 1, 1), 10);
    //auto pointLight = lightSpawner->SpawPointLight(glm::vec3(3, 0, 0), glm::vec3(0, 1, 1), 10);
    //auto pointLight3 = lightSpawner->SpawPointLight(glm::vec3(0, 3, 0), glm::vec3(0, 1, 0), 20);
    //auto pointLight4 = lightSpawner->SpawPointLight(glm::vec3(-3, 0, -3), glm::vec3(0, 0, 1), 20);

    //平行光使用灯光生成器生成，默认一个
    auto parallelLight = lightSpawner->SpawParallelLight(glm::vec3(-1), glm::vec3(1, 1, 1), 10);//使用默认值 强度10
    //手电筒光使用灯光生成器生成，默认支持4个
    //auto splashLight = lightSpawner->SpawFlashLight(glm::vec3(0, 5, -2), glm::vec3(0, -1, 0));//使用默认值 强度10

}
void GameStartT()
{



    auto* baseSphere = new CustomModelShader("commonLight", ModelDic["baseSphere"], false, true, true);
    baseSphere->SetVariant(ModelClass::CubeTestE);
    baseSphere->Initialize(glm::vec3(-3.0f, -2.0f, -2.0f), glm::quat(glm::vec3(0.0f, 45.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f));
    manager->RegisterObject(baseSphere);
    baseSphere->AttachTexture(TextureDic["butterfly"][0], 0);

    //测试圆柱
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
    basePlane->AttachPhysicalEngine(true);//声明为静态类型，目前注册为1
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
        glm::vec3(0.0f, 0.0f, 0.0f),//赋默认值
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
    //测试用的方法，生成注册器中的ActorButterFly 方法
  // coroutine->StartSpawnButterfliesByTimer(manager, TextureDic["butterfly"][0],0);
    //综合性赋值方法泛型方法， 可以直接异步初始化各种继承customModel的对象

}


