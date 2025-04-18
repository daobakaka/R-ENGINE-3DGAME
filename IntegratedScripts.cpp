#include "IntegratedScripts.h"
#include "ScriptModel.h"
#include "CustomModel.h"
#include "LifecycleManager.h"
using namespace Game;
IntegratedScripts *IntegratedScripts:: instance = nullptr;//静态变量必须在CPP类中声明
extern LifecycleManager<CustomModel>* manager;


IntegratedScripts *IntegratedScripts::GetInstance()
{
    if (instance == nullptr) {
        instance = new IntegratedScripts();
    }
    return instance;
}

IntegratedScripts::IntegratedScripts()
{ // 使用随机设备初始化随机数生成器
    std::random_device rd;
    gen = std::mt19937(rd());  // 初始化梅森旋转引
     randomFloat = 0.1f;
     randomInt = 1;
     //初始化控制器
     manager = LifecycleManager<CustomModel>::GetInstance();
}

IntegratedScripts::~IntegratedScripts()
{
}

//--
float IntegratedScripts::TGetRandomFloat(float min, float max) {
    std::uniform_real_distribution<float> dis(min, max);
   
    return dis(gen);
}

void IntegratedScripts::TChangeRandom(float min, float max)
{
    randomFloat = TGetRandomFloat(min, max);
    randomInt = TGetRandomInt((int)min, (int)max);
}

// 获取一个范围在[min, max]之间的随机整数
int IntegratedScripts:: TGetRandomInt(int min, int max) {
    std::uniform_int_distribution<int> dis(min, max);

    return dis(gen);
}
//CubeE
void IntegratedScripts::CubeUpdateFun(GameObject *gameObject)
{
    
    gameObject->position += glm::vec3(-0.01+randomFloat, 0, -0.01f+ randomFloat);
    gameObject->rotation *= glm::quat(glm::vec3(0, 0.1f+randomFloat, 0.1f));

}
//TestCubeE

void IntegratedScripts::TestUpdateFun(GameObject* gameObject)
{

    //gameObject->position += glm::vec3(-0.01 + randomFloat, 0, -0.01f + randomFloat);
    gameObject->rotation *= glm::quat(glm::vec3(0, 0.01f + 0.01f*randomFloat, 0.0f))*0.1F;
   // gameObject->position += glm::vec3(0, 0.001f, 0);

}

void Game::IntegratedScripts::ActorButtfly(GameObject* gameObject)
{
    // 使用 GLFW 获取全局时间（单位：秒）
    float time = static_cast<float>(glfwGetTime());

    // 利用 sin 函数生成往返运动系数：
    // 正值时蝴蝶沿基础方向飞出，负值时蝴蝶反向返回
    float oscillationFrequency = 0.5f;    // 控制往返运动的快慢
    float oscillationFactor = sin(time * oscillationFrequency);

    // 基础运动方向（这里设为 (-1, 0, -1)）
    glm::vec3 forward = glm::normalize(glm::vec3(-1.0f, 0.0f, -1.0f));
    float baseSpeed = 0.01f;
    // 基础往返运动向量：方向 * 速度 * 往返系数
    glm::vec3 baseMovement = forward * baseSpeed * oscillationFactor;

    // 正弦波参数，用于生成蝴蝶飞行过程中的平滑波动效果
    float amplitude = 0.05f;    // 振幅，控制偏移量大小
    float frequency = 1.0f;     // 频率，控制波动快慢
    // 利用正弦和余弦函数产生周期性偏移（模拟左右摆动和上下起伏）
    float offsetX = cos(time * frequency) * amplitude * 0.5f;
    float offsetY = sin(time * frequency * 1.5f) * amplitude;  // 垂直方向起伏
    float offsetZ = sin(time * frequency) * amplitude * 0.5f;

    // 加入微小的随机扰动，使运动轨迹更自然
    // 生成的随机值归一化到 [-0.005, 0.005] 范围内
    float noiseX = ((rand() % 100) / 100.0f - 0.5f) * 0.01f;
    float noiseY = ((rand() % 100) / 100.0f - 0.5f) * 0.01f;
    float noiseZ = ((rand() % 100) / 100.0f - 0.5f) * 0.01f;

    // 合成最终运动向量：基础往返运动 + 正弦波动偏移 + 随机扰动
    glm::vec3 movement = baseMovement
        + glm::vec3(offsetX, offsetY, offsetZ)
        + glm::vec3(noiseX, noiseY, noiseZ);
    // 更新 GameObject 的位置
    gameObject->position += movement;

    // 为旋转添加周期性变化，模拟蝴蝶飞行中的轻微扭转或“挥动”
    float rotX = sin(time * 1.5f) * 0.1f;
    float rotY = cos(time * 1.7f) * 0.1f;
    float rotZ = sin(time * 2.3f) * 0.1f;
    gameObject->rotation *= glm::quat(glm::vec3(rotX, rotY, rotZ));
}

void Game::IntegratedScripts::PlayerControl(GLFWwindow* window, CustomModel* other)
{
    // 发射周期控制
    if (!_enableShoot)
    {
        _shootPrepare += 0.0167f;
        if (_shootPrepare > 1)
        {
            _enableShoot = true;
        }
    }

    // 角色控制按钮
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        if (_enableShoot)
        {
            std::cout << "press F" << std::endl;

            // 生成一发子弹
            auto* bullet = new GameBullet("waveShader", ModelDic["baseSphere"], false, false, true);
            bullet->SelfIns();//初始化道具随机变量
            bullet->SetVariant(ModelClass::PlayerBullet);
            bullet->Initialize(other->position + glm::vec3(0, 8, 0), glm::quat(glm::vec3(0)), glm::vec3(3.0f));
            manager->RegisterObject(bullet);
            bullet->AttachTexture(TextureDic["water"], true,0, glm::vec2(1, 1));//设置批量绘制为true,渲染逻辑需要在外部调用
            bullet->UniformParametersIns();
            bullet->AttachPhysicalEngine();
            bullet->AttachCollider();
            bullet->GetComponent<PhysicalEngine>()->SetMass(0.3f); // 给子弹较小的质量
            bullet->GetComponent<PhysicalEngine>()->SetAcceleration(glm::vec3(0, -5.0f, 0)); // 模拟子弹受较小的重力加速度
            bullet->GetComponent<CollisionBody>()->SetTrigger(false); // 被子弹碰撞的物体不会受到物理系统的影响，子弹为触发器，只受到速度和加速度影响
            bullet->GetComponent<CollisionBody>()->SetGameProperties(1, 20, 1); // 设置子弹的攻击力为1
            bullet->GetComponent<CollisionBody>()->SetRadius(1.3);//增加子弹碰撞半径
            // 获取 other->rotation 的方向向量
            glm::vec3 forwardDirection = other->rotation * glm::vec3(0, 0,1); // 假设向前是 -Z 方向
            bullet->GetComponent<PhysicalEngine>()->SetVelocity(forwardDirection * 50.0f); // 给子弹50的前向速度

            _enableShoot = false;
            _shootPrepare = 0;
        }
    }
    //这里的内存通过生命周期管理器进行管理
}




int Game::IntegratedScripts::TUpdateFPS()
{
    static double lastTime = 0.0; // 上一帧的时间，使用 static 保持其状态
    static int frameCount = 0;     // 当前帧数，使用 static 保持其状态
    double fps = 0.0;              // 当前帧率，局部变量
    double currentTime = glfwGetTime();  // 获取当前时间
    frameCount++;

    fps = frameCount / (currentTime - lastTime);
    frameCount = 0;
    lastTime = currentTime;
    return static_cast<int>(fps);  // 返回整数类型的帧率
}

void Game::IntegratedScripts::TControlFrameRate(float targetFrameRate)
{
    // 获取当前时间
    static double lastTime = glfwGetTime();
    double deltaTime = 0.0;
    double frameTime = glfwGetTime() - lastTime;

    // 计算当前帧时间与目标帧时间的差距
    deltaTime += frameTime;

    // 目标帧时间
    double targetFrameTime = 1.0 / targetFrameRate;

    // 如果当前帧时间小于目标帧时间，休眠以保证稳定帧率
    if (deltaTime < targetFrameTime) {
        double sleepTime = targetFrameTime - deltaTime;
        // 使用 std::this_thread::sleep_for 来代替 glfwSleep
        std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime));  // 使用 std::chrono 进行延时,这里可以进行延迟
    }

    // 更新 lastTime
    lastTime = glfwGetTime();
}

void Game::IntegratedScripts::TParallelLightRotation(GameObject *object)
{

   // object->rotation *= glm::quat(glm::vec3(0, 0.1f + 0.1f * randomFloat, 0.0f));
    object->rotation *= glm::quat(glm::vec3(0, 0.2f , 0.0f));
   
}

void Game::IntegratedScripts::TPosition(GameObject* object)
{


    //std::cout << "OutPosition: ("
    //    <<object->position.x << ", "
    //    <<object-> position.y << ", "
    //    <<object-> position.z << ")"
    //    << std::endl;

}








