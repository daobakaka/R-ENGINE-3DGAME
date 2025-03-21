#include <iostream>
#include "Monster.h"
#include <vector>
#include <GL/glew.h>//加载Opengl扩展，
#include <GLFW/glfw3.h>//创建Opengl扩展，以及管理Opengl上下文
#include "Cube.h"
#include "Controller.h"
#include "LifecycleManager.h"
#include "IntegratedScripts.h"
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
#include "DesignModel.h"
#define PI 3.1415926  //定义PI宏

using namespace Game;
//泛型类的实例化指针,这里最初始
template <typename T>
LifecycleManager<T>* LifecycleManager<T>::instance = nullptr;
Controller* controller;
LifecycleManager<CustomModel>* manager ;
IntegratedScripts* scripts ;
MeshDataManager* meshData ;
TextRender* cusText ;
CoroutineMethod* coroutine ;
LightSpawner* lightSpawner ;
LightRender* lightRender ;
ShaderManager* shaderManager;
//全局纹理渲染
extern void RenderingTextureGlobal(GLuint shaderProgram, const std::unordered_map<PictureTpye, GLuint>& textures, glm::vec2 textureScale = glm::vec2(1, 1), int textureOrder = 0);
//全局参数传入
extern void PassUniformParametersGlobal(GLuint shaderProgram, glm::vec3 baseColor, glm::vec3 emission, float metallic, float roughness, float opacity, float IOR, float ao, float dissolveThreshold = 0);
void CallbackMouseScrollGlobal(GLFWwindow* window, double xoffset, double yoffset)
{


    Controller::GetInstance()->ProcessMouseScrollGlobal(static_cast<float>(yoffset));
}

GLFWwindow* GLinitializeT()
{
    GLFWwindow* window = glfwCreateWindow(controller->windowWidth, controller->windowHeight, "R-ENGINE-GAME", nullptr, nullptr);
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
    //设置OpenGL渲染模式，核心模式
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
    glDepthFunc(GL_LESS); // 设置深度函数，使用GL_LESS
    glEnable(GL_BLEND);//启用混合
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//透明混合
    glEnable(GL_MULTISAMPLE);  // 启用多重采样抗锯齿
    glEnable(GL_CULL_FACE);       // 启用背面剔除
    glCullFace(GL_BACK);          // 只剔除背面，保留正面
   // glEnable(GL_FRAMEBUFFER_SRGB); // 启用伽马矫正,这里需要暗色场景暂时不用
    return window;
}


void GameAwakeT()
{
    // 获取 Controller 实例,自定义类用于初始化主摄像机，及相关的图形窗口参数
   controller = Controller::GetInstance();
    //获取生命周期管理器,内含动画控制等
    manager = LifecycleManager<CustomModel>::GetInstance();//管理所有继承了MonoBehaviour的类,这里实例化为了CustomModel
    //获取综合脚本控制器
    scripts = IntegratedScripts::GetInstance();
    //获取网格管理器
    meshData = MeshDataManager::GetInstance();
    //获取字体渲染器
    cusText = TextRender::GetInstance();
    //获取协程控制器
    coroutine = CoroutineMethod::GetInstance();
    //获取灯光生成器
    lightSpawner = LightSpawner::GetInstance();
    //开启/关闭灯光标识
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

    //预定义最大光源数量，这里可以做性能限定，目前光照的实现均为实时光照
    //初始化用于渲染阴影的平行光深度图，可以在类中直接构造，编译阴影着色器
    lightRender->CreateShadowMapForParallelLight();
    //初始化用于渲染视口深度图的缓冲区，用于渲染视口深度图,用于后续渲染的深度运用
    lightRender->CreateDepthMapForTest();
    //初始化用于后处理离屏帧缓冲区，用于屏幕图像的后处理
    lightRender->CreatePostProcessingMap();
    //点光源生成使用灯光控制器完成,测试定义4个灯光，物体形态的变化
  //  auto pointLight2 = lightSpawner->SpawPointLight(glm::vec3(60, 5, 20), glm::vec3(1, 1, 1), 10);
    auto pointLight = lightSpawner->SpawnPointLight(glm::vec3(30, 5, 0), glm::vec3(0, 1, 1), 10);
    auto pointLight3 = lightSpawner->SpawnPointLight(glm::vec3(-15, 5, 0), glm::vec3(0, 1, 0), 10);
    auto pointLight4 = lightSpawner->SpawnPointLight(glm::vec3(-30, 5, -30), glm::vec3(0, 0, 1), 10);
    //增加场景灯光
    auto pointLight5 = lightSpawner->SpawnPointLight(glm::vec3(-70, 5, -10), glm::vec3(1, 0, 1), 10);
   // auto pointLight6 = lightSpawner->SpawPointLight(glm::vec3(70, 5, 30), glm::vec3(1, 0, 0), 10);
    //平行光使用灯光生成器生成，默认一个
    auto parallelLight = lightSpawner->SpawnParallelLight(glm::vec3(1,-1,1), glm::vec3(1, 1, 1), 1);//使用默认值 强度10
    //手电筒光使用灯光生成器生成，默认支持4个
    auto splashLight = lightSpawner->SpawnFlashLight(glm::vec3(30, 30, 0), glm::vec3(0, -1, 0),glm::vec3(0.8F,0.5F,0.3F),30,0.6f);//使用默认值 强度10

}
CustomModel* GameStartT()
{
    //光照基础面 默认第一，ID 为1
    auto* basePlane = new  CustomModelShader("aloneCommonLight", ModelDic["basePlane"], false, true, false,false);
    basePlane->SetVariant(ModelClass::StaticPlane);
    basePlane->Initialize(glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, .0f, 0.0f)), glm::vec3(1000.0f, 0.1f, 1000.0f));
    manager->RegisterObject(basePlane);
    basePlane->AttachTexture(TextureDic["grass"],false ,0 ,glm::vec2(50, 50));
    basePlane->UniformParametersIns(glm::vec3(0.3f), glm::vec3(0.1f), 0.0f, 0.99f, 1, 1.33f, 0.1f, 0);
    basePlane->AttachPhysicalEngine(true);//声明为静态类型，目前注册为1
    basePlane->AttachCollider(CollisionType::Box,SpecialType::BasePlane,1);//注册特殊碰撞体--地板

    //生成游戏玩家
    auto* gamePlayer = new  GamePlayer("aloneCommonLight", ModelDic["testMonkey"], false, true, true,true);
    gamePlayer->SetVariant(ModelClass::Player);
    gamePlayer->Initialize(glm::vec3(0.0f, 5.0f, 0.0f), glm::quat(glm::vec3(0.0f,0, 0.0f)), glm::vec3(3));
    manager->RegisterSpecialObjects("player",gamePlayer);//注册入特殊对象储存器，方便在在玩家类内部使用，且在更新方法中引用
    gamePlayer->AttachTexture(TextureDic["wheel"]);
    gamePlayer->UniformParametersIns(glm::vec3(0.3f),glm::vec3(0.1f),0.9f,0.5f,1,1.33f,0.6f,0);
    gamePlayer->AttachPhysicalEngine();
    gamePlayer->GetComponent<PhysicalEngine>()->SetElasticity(0.0F);//设置弹性系数为0
    gamePlayer->GetComponent<PhysicalEngine>()->SetFriction(0.9f);//设置摩擦系数为0.5，减少滑动
    gamePlayer->GetComponent<PhysicalEngine>()->SetRotationDamping(1);//设置旋转阻尼为1，避免碰撞旋转
    gamePlayer->GetComponent<PhysicalEngine>()->SetRotationAdjust(1);//同时设置碰撞调整系数为1，杜绝物理系统的碰撞旋转
    gamePlayer->GetComponent<PhysicalEngine>()->SetMass(5);//设置较大质量，增强碰撞抗性
    gamePlayer->AttachCollider(CollisionType::Box,SpecialType::SPlayer,1.0F);//注册为玩家，设置较小碰撞半径避免与地面直接摩擦        
    gamePlayer->GetComponent<CollisionBody>()->SetGameProperties(1000, 20, 10);//设置玩家攻击力
     
    //无光照黑洞
    for (int i = 0; i <1; i++)
    {
        auto* baseSphere = new BlackHole("commonNoneLight", ModelDic["blackHole"], false, false, false);
        baseSphere->SetVariant(ModelClass::BlackHoleE);
        baseSphere->Initialize(glm::vec3(-500,500,-900), glm::quat(glm::vec3(0.0f, 45.0f, 0.0f)), glm::vec3(300));
        manager->RegisterObject(baseSphere);
        baseSphere->AttachTexture(TextureDic["blackHole"],false, 0,glm::vec2(10));
        baseSphere->UniformParametersIns(glm::vec3(0.1f), glm::vec3(0.2f,0.1F,0.1F), 0.9f, 0.5f, 1, 1.33f, 0.6f, 0);
    }
    //光照树
    for (int i = 0; i < 1; i++)
    {
        auto* tree = new CustomModelShader("commonNoneLight", ModelDic["tree"], false, true, true);
        tree->SetVariant(ModelClass::CubeE);
        tree->Initialize(glm::vec3(60,0,-40), glm::quat(glm::vec3(0.0f, 45.0f, 0.0f)), glm::vec3(15));
        manager->RegisterObject(tree);
        tree->AttachTexture(TextureDic["tree"]);
        tree->UniformParametersIns(glm::vec3(0.8f), glm::vec3(0.1f), 0.0f, 0.99f, 1, 1.33f, 0.6f, 0);
    }
    //光照宝箱
    for (int i = 0; i < 1; i++)
    {
        auto* chest = new CustomModelShader("aloneCommonLight", ModelDic["chest"], false, true, true);
        chest->SetVariant(ModelClass::CubeE);
        chest->Initialize(glm::vec3(20, 0, 0), glm::quat(glm::vec3(0.0f, 45.0f, 0.0f)), glm::vec3(10));
        manager->RegisterObject(chest);
        chest->AttachTexture(TextureDic["chest"], 0);
        chest->UniformParametersIns(glm::vec3(0.3f), glm::vec3(0.1f), 0.0f, 0.5f, 1, 1.33f, 0.6f, 0);
    }

    //无光照宝箱
    for (int i = 0; i < 1; i++)
    {
        auto* chest = new CustomModelShader("commonNoneLight", ModelDic["chest"], false, false, true);
        chest->SetVariant(ModelClass::CubeE);
        chest->Initialize(glm::vec3(20, 0, 30), glm::quat(glm::vec3(0.0f, 45.0f, 0.0f)), glm::vec3(10));
        manager->RegisterObject(chest);
        chest->AttachTexture(TextureDic["chest"], 0);
        chest->UniformParametersIns(glm::vec3(0.7f), glm::vec3(0.1f), 0.0f, 0.99f, 1, 1.33f, 0.6f, 0);
    }

    //光照石头怪
    for (int i = 0; i < 1; i++)
    {//这里添加了动画要声明蒙皮网格，在动态绘制区进行绘制
        auto* stoneMonster = new GameStoneMonser("commonLight", ModelDic["stoneMonster"], true, true, true);
        stoneMonster->SetVariant(ModelClass::StoneMonser);
        stoneMonster->Initialize(glm::vec3(50, 10, -100), glm::quat(glm::vec3(0, 0, 0.0f)), glm::vec3(0.1F));
        manager->RegisterObject(stoneMonster);
        stoneMonster->AttachTexture(TextureDic["stoneMonster"],true);//采用材质批量绘制，在外部显示调用全局绘制方法
        stoneMonster->UniformParametersIns(glm::vec3(0.3f), glm::vec3(0.1f), 0.0f, 0.99f, 1, 1.33f, 0.6f, 0);
        stoneMonster->AttachAnimationController(AnimationDic["stoneMonster"]["run"]);     
        stoneMonster->SelfIns();//用于特别情况或者异步函数或者普通的参数设置解耦
    }

    //光照石块
    for (int i = 0; i < 5; i++)
    {
        auto* baseCube = new CustomModelShader("commonLight", ModelDic["baseCube"], false, true, true);
        baseCube->SetVariant(ModelClass::TestPhysics);
        baseCube->Initialize(glm::vec3(-100+6.0f + 35.0f * i, 6.0f, -30.0f), glm::quat(glm::vec3(0.0f, 60.0f, 0.0f)), glm::vec3(3));
        manager->RegisterObject(baseCube);
        baseCube->AttachTexture(TextureDic["stone"],true, 0,glm::vec3(1));//采用同批材质批量绘制，在外部显示调用全局绘制方法
        baseCube->UniformParametersIns(glm::vec3(0.3f), glm::vec3(0.1f), 0.0f, 0.99f, 1, 1.33f, 0.6f, 0);
        baseCube->AttachPhysicalEngine();
        baseCube->GetComponent<PhysicalEngine>()->SetFriction(0.7f);
        baseCube->GetComponent<PhysicalEngine>()->SetMass(10);//设置石块较大质量
        baseCube->AttachCollider();
        baseCube->GetComponent<CollisionBody>()->SetGameProperties(100000, 3);
    }

    //动态萤火虫
    auto* fireflyInstance = new  FireflyInstance("fireflyInstanceShader", ModelDic["baseSphere"], false, false, false, 25, glm::vec3(3,1.0F,3), glm::vec3(0, 0.3f, 0), 
        ModelClass::InstanceSphere);
    fireflyInstance->SetVariant(ModelClass::InstanceSphere);
    fireflyInstance->SelfIns();//派生类特殊初始化
    manager->RegisterObject(fireflyInstance);
    fireflyInstance->Initialize(glm::vec3(-40.0f, 5.0f, 30.0f), glm::quat(glm::vec3(0.0f, .0f, 0.0f)), glm::vec3(0.10F));
    fireflyInstance->AttachTexture(TextureDic["butterfly"]);

    //树实例化,使用视口深度图，该渲染的结果可以基于视口深度图，利用深度信息进行相关绘制
    auto* treeInstance = new  CustomModelInstance("noneLightDepthCalInstancer", ModelDic["tree"], false, false, false, 20000, glm::vec3(10),glm::vec3(0,1,0),
        ModelClass::InstanceCircle);
    treeInstance->EnableDepthcal();//允许使用视口深度图进行计算
    treeInstance->SetVariant(ModelClass::InstanceCircle);
    treeInstance->Initialize(glm::vec3(0.0f, -0.5f, 0.0f), glm::quat(glm::vec3(0.0f, .0f, 0.0f)), glm::vec3(5));
    manager->RegisterSpecialObjects("treeInstance",treeInstance);//这里的树作为特殊容器对象注入，以便之后进行深度测试调整alpha值
    treeInstance->AttachTexture(TextureDic["tree"]);

    //碎石1实例化
    auto* stone1Instance = new  CustomModelInstance("noneLightInstancer", ModelDic["stone1"], false, false, false, 500, glm::vec3(2), glm::vec3(0, 0.4F, 0),
        ModelClass::InstanceRound);
    stone1Instance->SetVariant(ModelClass::InstanceRound);
    stone1Instance->Initialize(glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, .0f, 0.0f)), glm::vec3(2));
    manager->RegisterObject(stone1Instance);
    stone1Instance->AttachTexture(TextureDic["stoneInstance"]);

    //碎石2实例化
    auto* stone2Instance = new  CustomModelInstance("noneLightInstancer", ModelDic["stone2"], false, false, false, 500, glm::vec3(2), glm::vec3(0, 0.3F, 0),
        ModelClass::InstanceRound);
    stone2Instance->SetVariant(ModelClass::InstanceRound);
    stone2Instance->Initialize(glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, .0f, 0.0f)), glm::vec3(2));
    manager->RegisterObject(stone2Instance);
    stone2Instance->AttachTexture(TextureDic["stoneInstance"]);

    //草1实例化
    auto* grass1Instance = new  CustomModelInstance("noneLightInstancer", ModelDic["grass2"], false, false, false, 1000, glm::vec3(1,1,1), glm::vec3(0, 0.9F, 0),
        ModelClass::InstanceRound);
    grass1Instance->SetVariant(ModelClass::InstanceRound);
    grass1Instance->Initialize(glm::vec3(0.0f,-0.01f, 0.0f), glm::quat(glm::vec3(0.0f, .0f, 0.0f)), glm::vec3(3));
    manager->RegisterObject(grass1Instance);
    grass1Instance->AttachTexture(TextureDic["grass2"],false, 0, glm::vec2(3));
    //类异步生成更多石头怪
    StepVector3 stepStone;
    stepStone.position = glm::vec3(1, 0.5f, 1);
    stepStone.scale = glm::vec3(1.0f);
    coroutine->StartSpawnByTimerAnimation<GameStoneMonser>(
        manager,
        "commonLight",
        true,
        true,
        ModelDic["stoneMonster"],
        AnimationDic["stoneMonster"]["run"],
        TextureDic["stoneMonster"],
        true,
        0,
        glm::vec2(1, 1),
        ModelClass::StoneMonser,
        30, 200,
        stepStone,
        glm::vec3(-50.0f, 10.0, -50.0f),//赋默认值
        glm::vec3(0.0f, 0, 0.0f),//赋默认值
        glm::vec3(0.1F)
    );
    //另一个方向的石头怪
    coroutine->StartSpawnByTimerAnimation<GameStoneMonser>(
        manager,
        "commonLight",
        true,
        true,
        ModelDic["stoneMonster"],
        AnimationDic["stoneMonster"]["run"],
        TextureDic["stoneMonster"],
        true,
        0,
        glm::vec2(1, 1),
        ModelClass::StoneMonser,
        70, 200,
        stepStone,
        glm::vec3(50.0f, 10.0, 50.0f),//赋默认值
        glm::vec3(0.0f, 0, 0.0f),//赋默认值
        glm::vec3(0.1F)
    );

    //类异步生成蝴蝶对象
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
        true,
        0,
        glm::vec2(1, 1),
        ModelClass::TsetButterfly,
        10, 20000,
        step,
        glm::vec3(0.0f, 0, 0.0f),//赋默认值
        glm::vec3(0.0f, 0, 0.0f),//赋默认值
        glm::vec3(0.5F)
    );

    return gamePlayer;

}


