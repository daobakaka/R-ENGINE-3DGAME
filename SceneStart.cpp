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
#define PI 3.1415926  //定义PI宏

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
    lightSpawner->modelIdentification = true;
    //获取灯光渲染器
    lightRender = LightRender::GetInstance();
    //获取着色器控制器,构造添加默认编译
     shaderManager = ShaderManager::GetInstance();

}
void SourceInitialize()
{
    //加载模型区域
    MakeModel();
    //加载FBX模型区域
    MakeModelFbx();
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
    auto pointLight2 = lightSpawner->SpawPointLight(glm::vec3(60, 5, 20), glm::vec3(1, 1, 1), 10);
    auto pointLight = lightSpawner->SpawPointLight(glm::vec3(30, 5, 0), glm::vec3(0, 1, 1), 10);
    auto pointLight3 = lightSpawner->SpawPointLight(glm::vec3(-15, 5, 0), glm::vec3(0, 1, 0), 10);
    auto pointLight4 = lightSpawner->SpawPointLight(glm::vec3(-30, 5, -30), glm::vec3(0, 0, 1), 10);

    //平行光使用灯光生成器生成，默认一个
    auto parallelLight = lightSpawner->SpawParallelLight(glm::vec3(-1,-1,-1), glm::vec3(1, 1, 1), 1);//使用默认值 强度10
    //手电筒光使用灯光生成器生成，默认支持4个
    auto splashLight = lightSpawner->SpawFlashLight(glm::vec3(0, 7, 20), glm::vec3(0, -1, 0),glm::vec3(1,0,0),20);//使用默认值 强度10

}
CustomModel* GameStartT()
{
    //生成基础面 默认第一，ID 为0
    auto* basePlane = new  CustomModelShader("commonLight", ModelDic["basePlane"], false, true, false);
    basePlane->SetVariant(ModelClass::StaticPlane);
    basePlane->Initialize(glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, .0f, 0.0f)), glm::vec3(1000.0f, 0.1f, 1000.0f));
    manager->RegisterObject(basePlane);
    basePlane->AttachTexture(TextureDic["grass"], 0, glm::vec2(50, 50));
    basePlane->AttachPhysicalEngine(true);//声明为静态类型，目前注册为1
    basePlane->AttachCollider(CollisionType::Box,SpecialType::BasePlane,1);//注册特殊碰撞体--地板

    //生成游戏玩家
    auto* gamePlayer = new  GamePlayer("commonLight", ModelDic["testMonkey"], true, true, true);
    gamePlayer->SetVariant(ModelClass::Player);
    gamePlayer->Initialize(glm::vec3(0.0f, 5.0f, 0.0f), glm::quat(glm::vec3(0.0f,0, 0.0f)), glm::vec3(3));
    manager->RegisterObject(gamePlayer);
    manager->RegisterSpecialObjects(gamePlayer, "player");//注册入特殊对象储存器，方便在在玩家类内部使用，且在更新方法中引用
    gamePlayer->AttachTexture(TextureDic["butterfly"], 0, glm::vec2(1, 1));
    gamePlayer->AttachPhysicalEngine();
    gamePlayer->GetComponent<PhysicalEngine>()->SetElasticity(0);//设置弹性系数为0
    gamePlayer->GetComponent<PhysicalEngine>()->SetFriction(0.9f);//设置摩擦系数为0.5，减少滑动
    gamePlayer->GetComponent<PhysicalEngine>()->SetRotationDamping(1);//设置旋转阻尼为1，避免碰撞旋转
    gamePlayer->GetComponent<PhysicalEngine>()->SetRotationAdjust(1);//同时设置碰撞调整系数为1，杜绝物理系统的碰撞旋转
    gamePlayer->GetComponent<PhysicalEngine>()->SetMass(5);//设置较大质量，增强碰撞抗性
    gamePlayer->AttachCollider(CollisionType::Box,SpecialType::SPlayer,1);//注册为玩家，设置较小碰撞半径避免与地面直接摩擦        
    gamePlayer->GetComponent<CollisionBody>()->SetGameProperties(1000, 20, 1);//设置玩家攻击力


    
    //测试小球
    for (int i = 0; i <3; i++)
    {
        auto* baseSphere = new CustomModelShader("commonLight", ModelDic["baseSphere"], false, true, true);
        baseSphere->SetVariant(ModelClass::TestPhysics);
        baseSphere->Initialize(glm::vec3(-70+3.0f+3.5f*i, 10.0f+i, -50.0f), glm::quat(glm::vec3(0.0f, 45.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f));
        manager->RegisterObject(baseSphere);
        baseSphere->AttachTexture(TextureDic["butterfly"], 0);
        baseSphere->AttachPhysicalEngine(false, 10);
        baseSphere->GetComponent<PhysicalEngine>()->SetVelocity(glm::vec3(0, 0, 10));  
        baseSphere->AttachCollider();
        baseSphere->GetComponent<CollisionBody>()->SetRadius(1);
    }
    //测试树
    for (int i = 0; i < 1; i++)
    {
        auto* tree = new NoneLightModel("commonNoneLight", ModelDic["tree"], false, false, true);
        tree->SetVariant(ModelClass::CubeE);
        tree->Initialize(glm::vec3(60,0,-20), glm::quat(glm::vec3(0.0f, 45.0f, 0.0f)), glm::vec3(15));
        manager->RegisterObject(tree);
        tree->AttachTexture(TextureDic["tree"], 0,glm::vec2(4, 4));

    }
    //测试宝箱
    for (int i = 0; i < 1; i++)
    {
        auto* chest = new CustomModelShader("noneLight", ModelDic["chest"], false, false, true);
        chest->SetVariant(ModelClass::CubeE);
        chest->Initialize(glm::vec3(20, 0, 0), glm::quat(glm::vec3(0.0f, 45.0f, 0.0f)), glm::vec3(10));
        manager->RegisterObject(chest);
        chest->AttachTexture(TextureDic["chest"], 0);

    }
    //测试石头怪
    for (int i = 0; i < 1; i++)
    {
        auto* stoneMonster = new CustomModelShader("commonLight", ModelDic["stoneMonster"], false, true, true);
        stoneMonster->SetVariant(ModelClass::CubeE);
        stoneMonster->Initialize(glm::vec3(-40, 20, 30), glm::quat(glm::vec3(0, 0.0f, 0.0f)), glm::vec3(0.3F));
        manager->RegisterObject(stoneMonster);
        stoneMonster->AttachTexture(TextureDic["stoneMonster"], 0,glm::vec2(1,1));


    }

    //测试盒子
    for (int i = 0; i < 10; i++)
    {
        auto* baseCube = new CustomModelShader("commonLight", ModelDic["baseCube"], false, true, true);
        baseCube->SetVariant(ModelClass::TestPhysics);
        baseCube->Initialize(glm::vec3(-100+6.0f + 10.0f * i, 6.0f, -30.0f), glm::quat(glm::vec3(0.0f, 60.0f, 0.0f)), glm::vec3(3));
        manager->RegisterObject(baseCube);
        baseCube->AttachTexture(TextureDic["stone"], 0);
        baseCube->AttachPhysicalEngine();
        baseCube->GetComponent<PhysicalEngine>()->SetFriction(0.4f);
        baseCube->AttachCollider();
    }

    //测试蝴蝶实例化
    auto* butterflyInstance = new  CustomModelInstance("noneLightInstancer", ModelDic["butterfly"], false, false, false, 10000, glm::vec3(100, 100, -100));
    butterflyInstance->SetVariant(ModelClass::InstanceCube);
    butterflyInstance->Initialize(glm::vec3(0.0f, 5.0f, 0.0f), glm::quat(glm::vec3(0.0f, .0f, 0.0f)), glm::vec3(0.3f, 0.3f, 0.30f));
    manager->RegisterObject(butterflyInstance);
    butterflyInstance->AttachTexture(TextureDic["butterfly"], 0, glm::vec2(1, 1));

    //树实例化
    auto* treeInstance = new  CustomModelInstance("noneLightInstancer", ModelDic["tree"], false, false, false, 20000, glm::vec3(10),glm::vec3(0,1,0),
        ModelClass::InstanceCircle);
    treeInstance->SetVariant(ModelClass::InstanceCircle);
    treeInstance->Initialize(glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, .0f, 0.0f)), glm::vec3(5));
    manager->RegisterObject(treeInstance);
    treeInstance->AttachTexture(TextureDic["tree"], 0, glm::vec2(1, 1));

    //碎石实例化
    auto* stone1Instance = new  CustomModelInstance("noneLightInstancer", ModelDic["stone1"], false, false, false, 500, glm::vec3(2), glm::vec3(0, 0.4F, 0),
        ModelClass::InstanceRound);
    stone1Instance->SetVariant(ModelClass::InstanceRound);
    stone1Instance->Initialize(glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, .0f, 0.0f)), glm::vec3(2));
    manager->RegisterObject(stone1Instance);
    stone1Instance->AttachTexture(TextureDic["stoneInstance"], 0, glm::vec2(1, 1));

    //碎石实例化
    auto* stone2Instance = new  CustomModelInstance("noneLightInstancer", ModelDic["stone2"], false, false, false, 500, glm::vec3(2), glm::vec3(0, 0.3F, 0),
        ModelClass::InstanceRound);
    stone2Instance->SetVariant(ModelClass::InstanceRound);
    stone2Instance->Initialize(glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, .0f, 0.0f)), glm::vec3(2));
    manager->RegisterObject(stone2Instance);
    stone2Instance->AttachTexture(TextureDic["stoneInstance"], 0, glm::vec2(2, 2));

    StepVector3 step;
    step.position = glm::vec3(2, -0.5f, 0);
    step.scale = glm::vec3(1.0f);
    coroutine->StartSpawnByTimerAnimation<ButterflyScriptShader>(
        manager,
        "commonLight",
        true,
        true,
        ModelDic["butterfly"],
        AnimationDic["butterfly"]["fly"],
        TextureDic["butterfly"],
        0,
        glm::vec2(10, 10),
        ModelClass::TsetButterfly,
        10, 20,
        step,
        glm::vec3(0.0f, 0, 0.0f),//赋默认值
        glm::vec3(0.0f, 0, 0.0f),//赋默认值
        glm::vec3(1.1f)
    );

    return gamePlayer;
    //CustomModel* testMonkey = new CustomModel(colorlightsArrayVertexShaderSource, colorlightsArraySourceFragmentShaderSource, ModelDic["testMonkey"], false, true);
    //testMonkey->SetVariant(ModelClass::CubeTestE);
    //testMonkey->Initialize(glm::vec3(-0.0f, 3.0f, 2.0f), glm::quat(glm::vec3(0.0f, 45.0f, 0.0f)), glm::vec3(0.5f));
    //manager->RegisterObject(testMonkey);
    //testMonkey->AttachTexture(TextureDic["butterfly"][0], 0,glm::vec2(1,1));
    //测试用的方法，生成注册器中的ActorButterFly 方法
  // coroutine->StartSpawnButterfliesByTimer(manager, TextureDic["butterfly"][0],0);
    //综合性赋值方法泛型方法， 可以直接异步初始化各种继承customModel的对象

}


