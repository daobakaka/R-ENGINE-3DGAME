#include "IntergratedScripts.h"
using namespace Game;
IntergratedScripts *IntergratedScripts:: instance = nullptr;//静态变量必须在CPP类中声明

IntergratedScripts *IntergratedScripts::GetInstance()
{
    if (instance == nullptr) {
        instance = new IntergratedScripts();
    }
    return instance;
}

IntergratedScripts::IntergratedScripts()
{ // 使用随机设备初始化随机数生成器
    std::random_device rd;
    gen = std::mt19937(rd());  // 初始化梅森旋转引
     randomFloat = 0.1f;
     randomInt = 1;
}

IntergratedScripts::~IntergratedScripts()
{
}

//--
float IntergratedScripts::TGetRandomFloat(float min, float max) {
    std::uniform_real_distribution<float> dis(min, max);
   
    return dis(gen);
}

void IntergratedScripts::TChangeRandom(float min, float max)
{
    randomFloat = TGetRandomFloat(min, max);
    randomInt = TGetRandomInt((int)min, (int)max);
}

// 获取一个范围在[min, max]之间的随机整数
int IntergratedScripts:: TGetRandomInt(int min, int max) {
    std::uniform_int_distribution<int> dis(min, max);

    return dis(gen);
}
//CubeE
void IntergratedScripts::CubeUpdateFun(GameObject *gameObject)
{
    
    gameObject->position += glm::vec3(-0.01+randomFloat, 0, -0.01f+ randomFloat);
    gameObject->rotation *= glm::quat(glm::vec3(0, 0.1f+randomFloat, 0.1f));

}
//TestCubeE

void IntergratedScripts::TestUpdateFun(GameObject* gameObject)
{

    //gameObject->position += glm::vec3(-0.01 + randomFloat, 0, -0.01f + randomFloat);
    gameObject->rotation *= glm::quat(glm::vec3(0, 0.01f + 0.1f*randomFloat, 0.0f));
   // gameObject->position += glm::vec3(0, 0.001f, 0);

}

void Game::IntergratedScripts::ActorButtfly(GameObject* gameObject)
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




int Game::IntergratedScripts::TUpdateFPS()
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

void Game::IntergratedScripts::TControlFrameRate(float targetFrameRate)
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

void Game::IntergratedScripts::TParallelLightRotation(GameObject *object)
{

    object->rotation *= glm::quat(glm::vec3(0, 0.1f + 0.1f * randomFloat, 0.0f));

   
}

void Game::IntergratedScripts::TPosition(GameObject* object)
{


    //std::cout << "OutPosition: ("
    //    <<object->position.x << ", "
    //    <<object-> position.y << ", "
    //    <<object-> position.z << ")"
    //    << std::endl;

}








