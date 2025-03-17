#include <iostream>
#include "Monster.h"
#include <vector>
#include <GL/glew.h>//����Opengl��չ��
#include <GLFW/glfw3.h>//����Opengl��չ���Լ�����Opengl������
#include "Cube.h"
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
#include "DesignModel.h"
#define PI 3.1415926  //����PI��

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
//ȫ��������Ⱦ
extern void RenderingTextureGlobal(GLuint shaderProgram, const std::unordered_map<PictureTpye, GLuint>& textures, glm::vec2 textureScale = glm::vec2(1, 1), int textureOrder = 0);
//ȫ�ֲ�������
extern void PassUniformParametersGlobal(GLuint shaderProgram, glm::vec3 baseColor, glm::vec3 emission, float metallic, float roughness, float opacity, float IOR, float ao, float dissolveThreshold = 0);
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
    glDepthFunc(GL_LESS); // ������Ⱥ�����ͨ��ʹ��GL_LESS
    glEnable(GL_BLEND);// ������Ⱥ�����ͨ��ʹ��GL_LESS
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
    lightSpawner->modelIdentification = true;
    //��ȡ�ƹ���Ⱦ��
    lightRender = LightRender::GetInstance();
    //��ȡ��ɫ��������,�������Ĭ�ϱ���
     shaderManager = ShaderManager::GetInstance();

}
void SourceInitialize()
{
    //����ģ������
    MakeModel();
    //����FBXģ������
    MakeModelFbx();
    // ��������ģ�飬���������̬��չ��������Ӳ�ͬ�Ķ��������������Ը߶ȷ�װ�Ķ������������
    MakeAnimation();
    // ��������
    MakeTxture();
    //����������Դ
    MakeFronts();

}

void LightInitialization()
{

    //Ԥ��������Դ��������������������޶���Ŀǰ���յ�ʵ�־�Ϊʵʱ����
    //��ʼ��������Ⱦ��Ӱ��ƽ�й����ͼ������������ֱ�ӹ��죬������Ӱ��ɫ��
    lightRender->CreateShadowMapForParallelLight();
    //��ʼ��������Ⱦ�ӿ����ͼ�Ļ�������������Ⱦ�ӿ����ͼ,���ں�����Ⱦ���������
    lightRender->CreateDepthMapForTest();
    //��ʼ�����ں�������֡��������������Ļͼ��ĺ���
    lightRender->CreatePostProcessingMap();
    //���Դ����ʹ�õƹ���������,���Զ���4���ƹ⣬������̬�ı仯
  //  auto pointLight2 = lightSpawner->SpawPointLight(glm::vec3(60, 5, 20), glm::vec3(1, 1, 1), 10);
    auto pointLight = lightSpawner->SpawPointLight(glm::vec3(30, 5, 0), glm::vec3(0, 1, 1), 10);
    auto pointLight3 = lightSpawner->SpawPointLight(glm::vec3(-15, 5, 0), glm::vec3(0, 1, 0), 10);
    auto pointLight4 = lightSpawner->SpawPointLight(glm::vec3(-30, 5, -30), glm::vec3(0, 0, 1), 10);
    //���ӳ����ƹ�
    auto pointLight5 = lightSpawner->SpawPointLight(glm::vec3(-70, 5, -10), glm::vec3(1, 0, 1), 10);
   // auto pointLight6 = lightSpawner->SpawPointLight(glm::vec3(70, 5, 30), glm::vec3(1, 0, 0), 10);
    //ƽ�й�ʹ�õƹ����������ɣ�Ĭ��һ��
    auto parallelLight = lightSpawner->SpawParallelLight(glm::vec3(1,-1,1), glm::vec3(1, 1, 1), 1);//ʹ��Ĭ��ֵ ǿ��10
    //�ֵ�Ͳ��ʹ�õƹ����������ɣ�Ĭ��֧��4��
    auto splashLight = lightSpawner->SpawFlashLight(glm::vec3(-20, 10, 20), glm::vec3(0, -1, 0),glm::vec3(1,0,0),30);//ʹ��Ĭ��ֵ ǿ��10

}
CustomModel* GameStartT()
{
    //���ջ����� Ĭ�ϵ�һ��ID Ϊ1
    auto* basePlane = new  CustomModelShader("planeCommonLight", ModelDic["basePlane"], false, true, false,false);
    basePlane->SetVariant(ModelClass::StaticPlane);
    basePlane->Initialize(glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, .0f, 0.0f)), glm::vec3(1000.0f, 0.1f, 1000.0f));
    manager->RegisterObject(basePlane);
    basePlane->AttachTexture(TextureDic["grass"], 0, glm::vec2(50, 50));
    basePlane->UniformParametersIns(glm::vec3(0.3f), glm::vec3(0.1f), 0.9f, 0.5f, 1, 1.33f, 0.6f, 0);
    basePlane->AttachPhysicalEngine(true);//����Ϊ��̬���ͣ�Ŀǰע��Ϊ1
    basePlane->AttachCollider(CollisionType::Box,SpecialType::BasePlane,1);//ע��������ײ��--�ذ�

    //���ɹ�����Ϸ���
    auto* gamePlayer = new  GamePlayer("playerCommonLight", ModelDic["testMonkey"], false, true, true);
    gamePlayer->SetVariant(ModelClass::Player);
    gamePlayer->Initialize(glm::vec3(0.0f, 5.0f, 0.0f), glm::quat(glm::vec3(0.0f,0, 0.0f)), glm::vec3(3));
    manager->RegisterSpecialObjects(gamePlayer, "player");//ע����������󴢴�������������������ڲ�ʹ�ã����ڸ��·���������
    gamePlayer->AttachTexture(TextureDic["wheel"], 0, glm::vec2(1, 1));
    gamePlayer->UniformParametersIns(glm::vec3(0.9f),glm::vec3(0.1f),0.9f,0.5f,1,1.33f,0.6f,0);
    gamePlayer->AttachPhysicalEngine();
    gamePlayer->GetComponent<PhysicalEngine>()->SetElasticity(0.0F);//���õ���ϵ��Ϊ0
    gamePlayer->GetComponent<PhysicalEngine>()->SetFriction(0.9f);//����Ħ��ϵ��Ϊ0.5�����ٻ���
    gamePlayer->GetComponent<PhysicalEngine>()->SetRotationDamping(1);//������ת����Ϊ1��������ײ��ת
    gamePlayer->GetComponent<PhysicalEngine>()->SetRotationAdjust(1);//ͬʱ������ײ����ϵ��Ϊ1���ž�����ϵͳ����ײ��ת
    gamePlayer->GetComponent<PhysicalEngine>()->SetMass(5);//���ýϴ���������ǿ��ײ����
    gamePlayer->AttachCollider(CollisionType::Box,SpecialType::SPlayer,1.0F);//ע��Ϊ��ң����ý�С��ײ�뾶���������ֱ��Ħ��        
    gamePlayer->GetComponent<CollisionBody>()->SetGameProperties(1000, 20, 10);//������ҹ�����
     
    //�޹��պڶ�
    for (int i = 0; i <1; i++)
    {
        auto* baseSphere = new BlackHole("blackHoleCommonNoneLight", ModelDic["blackHole"], false, false, false);
        baseSphere->SetVariant(ModelClass::BlackHoleE);
        baseSphere->Initialize(glm::vec3(-500,500,-900), glm::quat(glm::vec3(0.0f, 45.0f, 0.0f)), glm::vec3(300));
        manager->RegisterObject(baseSphere);
        baseSphere->AttachTexture(TextureDic["blackHole"], 0,glm::vec3(10));
        baseSphere->UniformParametersIns(glm::vec3(0.5f), glm::vec3(0.1f), 0.9f, 0.5f, 1, 1.33f, 0.6f, 0);
    }
    //������
    for (int i = 0; i < 1; i++)
    {
        auto* tree = new CustomModelShader("treeCommonLight", ModelDic["tree"], false, true, true);
        tree->SetVariant(ModelClass::CubeE);
        tree->Initialize(glm::vec3(60,0,-20), glm::quat(glm::vec3(0.0f, 45.0f, 0.0f)), glm::vec3(15));
        manager->RegisterObject(tree);
        tree->AttachTexture(TextureDic["tree"], 0,glm::vec2(1, 1));
        tree->UniformParametersIns(glm::vec3(0.3f), glm::vec3(0.1f), 0.9f, 0.5f, 1, 1.33f, 0.6f, 0);
    }
    //���ձ���
    for (int i = 0; i < 1; i++)
    {
        auto* chest = new CustomModelShader("chestCommonLight", ModelDic["chest"], false, true, true);
        chest->SetVariant(ModelClass::CubeE);
        chest->Initialize(glm::vec3(20, 0, 0), glm::quat(glm::vec3(0.0f, 45.0f, 0.0f)), glm::vec3(10));
        manager->RegisterObject(chest);
        chest->AttachTexture(TextureDic["chest"], 0);
        chest->UniformParametersIns(glm::vec3(0.3f), glm::vec3(0.1f), 0.9f, 0.5f, 1, 1.33f, 0.6f, 0);
    }

    //�޹��ձ���
    for (int i = 0; i < 1; i++)
    {
        auto* chest = new NoneLightModel("chestCommonNoneLight", ModelDic["chest"], false, false, true);
        chest->SetVariant(ModelClass::CubeE);
        chest->Initialize(glm::vec3(20, 0, 30), glm::quat(glm::vec3(0.0f, 45.0f, 0.0f)), glm::vec3(10));
        manager->RegisterObject(chest);
        chest->AttachTexture(TextureDic["chest"], 0);
        chest->UniformParametersIns(glm::vec3(0.3f), glm::vec3(0.1f), 0.9f, 0.5f, 1, 1.33f, 0.6f, 0);
    }

    //����ʯͷ��
    for (int i = 0; i < 1; i++)
    {//��������˶���Ҫ������Ƥ�����ڶ�̬���������л���
        auto* stoneMonster = new GameStoneMonser("stoneMonsterCommonLight", ModelDic["stoneMonster"], true, true, true);
        stoneMonster->SetVariant(ModelClass::StoneMonser);
        stoneMonster->Initialize(glm::vec3(50, 10, -100), glm::quat(glm::vec3(0, 0, 0.0f)), glm::vec3(0.1F));
        manager->RegisterObject(stoneMonster);
        stoneMonster->AttachTexture(TextureDic["stoneMonster"], 0,glm::vec2(1,1));
        stoneMonster->UniformParametersIns(glm::vec3(0.3f), glm::vec3(0.1f), 0.0f, 0.5f, 1, 1.33f, 0.6f, 0);
        stoneMonster->AttachAnimationController(AnimationDic["stoneMonster"]["run"]);     
        stoneMonster->SelfIns();//�����ر���������첽����������ͨ�Ĳ������ý���
    }

    //����ʯ��
    for (int i = 0; i < 5; i++)
    {
        auto* baseCube = new CustomModelShader("stoneCommonLight", ModelDic["baseCube"], false, true, true);
        baseCube->SetVariant(ModelClass::TestPhysics);
        baseCube->Initialize(glm::vec3(-100+6.0f + 35.0f * i, 6.0f, -30.0f), glm::quat(glm::vec3(0.0f, 60.0f, 0.0f)), glm::vec3(3));
        manager->RegisterObject(baseCube);
        baseCube->AttachTexture(TextureDic["stone"], 0);
        baseCube->UniformParametersIns(glm::vec3(0.3f), glm::vec3(0.1f), 0.0f, 0.5f, 1, 1.33f, 0.6f, 0);
        baseCube->AttachPhysicalEngine();
        baseCube->GetComponent<PhysicalEngine>()->SetFriction(0.7f);
        baseCube->GetComponent<PhysicalEngine>()->SetMass(10);//����ʯ��ϴ�����
        baseCube->AttachCollider();
        baseCube->GetComponent<CollisionBody>()->SetGameProperties(100000, 3);
    }

    //��̬ө���
    auto* fireflyInstance = new  FireflyInstance("fireflyInstanceShader", ModelDic["baseSphere"], false, false, false, 25, glm::vec3(3,1.0F,3), glm::vec3(0, 0.3f, 0), 
        ModelClass::InstanceSphere);
    fireflyInstance->SetVariant(ModelClass::InstanceSphere);
    fireflyInstance->SelfIns();//�����������ʼ��
    manager->RegisterObject(fireflyInstance);
    fireflyInstance->Initialize(glm::vec3(-40.0f, 5.0f, 30.0f), glm::quat(glm::vec3(0.0f, .0f, 0.0f)), glm::vec3(0.10F));
    fireflyInstance->AttachTexture(TextureDic["butterfly"], 0, glm::vec2(1, 1));

    //��ʵ����,ʹ���ӿ����ͼ������Ⱦ�Ľ�����Ի����ӿ����ͼ�����������Ϣ������ػ���
    auto* treeInstance = new  CustomModelInstance("noneLightDepthCalInstancer", ModelDic["tree"], false, false, false, 20000, glm::vec3(10),glm::vec3(0,1,0),
        ModelClass::InstanceCircle);
    treeInstance->EnableDepthcal();//����ʹ���ӿ����ͼ���м���
    treeInstance->SetVariant(ModelClass::InstanceCircle);
    treeInstance->Initialize(glm::vec3(0.0f, -0.5f, 0.0f), glm::quat(glm::vec3(0.0f, .0f, 0.0f)), glm::vec3(5));
    manager->RegisterSpecialObjects(treeInstance,"treeInstance");//���������Ϊ������������ע�룬�Ա�֮�������Ȳ��Ե���alphaֵ
    treeInstance->AttachTexture(TextureDic["tree"], 0, glm::vec2(1, 1));

    //��ʯ1ʵ����
    auto* stone1Instance = new  CustomModelInstance("stoneNoneLightInstancer", ModelDic["stone1"], false, false, false, 500, glm::vec3(2), glm::vec3(0, 0.4F, 0),
        ModelClass::InstanceRound);
    stone1Instance->SetVariant(ModelClass::InstanceRound);
    stone1Instance->Initialize(glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, .0f, 0.0f)), glm::vec3(2));
    manager->RegisterObject(stone1Instance);
    stone1Instance->AttachTexture(TextureDic["stoneInstance"], 0, glm::vec2(1, 1));

    //��ʯ2ʵ����
    auto* stone2Instance = new  CustomModelInstance("stoneNoneLightInstancer", ModelDic["stone2"], false, false, false, 500, glm::vec3(2), glm::vec3(0, 0.3F, 0),
        ModelClass::InstanceRound);
    stone2Instance->SetVariant(ModelClass::InstanceRound);
    stone2Instance->Initialize(glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, .0f, 0.0f)), glm::vec3(2));
    manager->RegisterObject(stone2Instance);
    stone2Instance->AttachTexture(TextureDic["stoneInstance"], 0, glm::vec2(1, 1));

    //��1ʵ����
    auto* grass1Instance = new  CustomModelInstance("grassNoneLightInstancer", ModelDic["grass2"], false, false, false, 1000, glm::vec3(1,1,1), glm::vec3(0, 0.9F, 0),
        ModelClass::InstanceRound);
    grass1Instance->SetVariant(ModelClass::InstanceRound);
    grass1Instance->Initialize(glm::vec3(0.0f,-0.01f, 0.0f), glm::quat(glm::vec3(0.0f, .0f, 0.0f)), glm::vec3(3));
    manager->RegisterObject(grass1Instance);
    grass1Instance->AttachTexture(TextureDic["grass2"], 0, glm::vec2(3));
    //���첽���ɸ���ʯͷ��
    StepVector3 stepStone;
    stepStone.position = glm::vec3(1, 0.5f, 1);
    stepStone.scale = glm::vec3(1.0f);
    coroutine->StartSpawnByTimerAnimation<GameStoneMonser>(
        manager,
        "stoneMonsterCommonLight",
        true,
        true,
        ModelDic["stoneMonster"],
        AnimationDic["stoneMonster"]["run"],
        TextureDic["stoneMonster"],
        0,
        glm::vec2(1, 1),
        ModelClass::StoneMonser,
        30, 200,
        stepStone,
        glm::vec3(-50.0f, 10.0, -50.0f),//��Ĭ��ֵ
        glm::vec3(0.0f, 0, 0.0f),//��Ĭ��ֵ
        glm::vec3(0.1F)
    );
    //��һ�������ʯͷ��
    coroutine->StartSpawnByTimerAnimation<GameStoneMonser>(
        manager,
        "stoneMonsterCommonLight",
        true,
        true,
        ModelDic["stoneMonster"],
        AnimationDic["stoneMonster"]["run"],
        TextureDic["stoneMonster"],
        0,
        glm::vec2(1, 1),
        ModelClass::StoneMonser,
        70, 200,
        stepStone,
        glm::vec3(50.0f, 10.0, 50.0f),//��Ĭ��ֵ
        glm::vec3(0.0f, 0, 0.0f),//��Ĭ��ֵ
        glm::vec3(0.1F)
    );

    //���첽���ɺ�������
    StepVector3 step;
    step.position = glm::vec3(2, -0.5f, 0);
    step.scale = glm::vec3(1.0f);
    coroutine->StartSpawnByTimerAnimation<ButterflyScriptShader>(
        manager,
        "butterflyCommonLight",
        true,
        true,
        ModelDic["butterfly"],
        AnimationDic["butterfly"]["fly"],
        TextureDic["butterfly"],
        0,
        glm::vec2(1, 1),
        ModelClass::TsetButterfly,
        10, 20000,
        step,
        glm::vec3(0.0f, 0, 0.0f),//��Ĭ��ֵ
        glm::vec3(0.0f, 0, 0.0f),//��Ĭ��ֵ
        glm::vec3(0.5F)
    );

    return gamePlayer;

}


