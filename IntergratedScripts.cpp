#include "IntergratedScripts.h"
using namespace Game;
IntergratedScripts *IntergratedScripts:: instance = nullptr;//��̬����������CPP��������

IntergratedScripts *IntergratedScripts::GetInstance()
{
    if (instance == nullptr) {
        instance = new IntergratedScripts();
    }
    return instance;
}

IntergratedScripts::IntergratedScripts()
{ // ʹ������豸��ʼ�������������
    std::random_device rd;
    gen = std::mt19937(rd());  // ��ʼ��÷ɭ��ת��
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

// ��ȡһ����Χ��[min, max]֮����������
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
    // ʹ�� GLFW ��ȡȫ��ʱ�䣨��λ���룩
    float time = static_cast<float>(glfwGetTime());

    // ���� sin �������������˶�ϵ����
    // ��ֵʱ�����ػ�������ɳ�����ֵʱ�������򷵻�
    float oscillationFrequency = 0.5f;    // ���������˶��Ŀ���
    float oscillationFactor = sin(time * oscillationFrequency);

    // �����˶�����������Ϊ (-1, 0, -1)��
    glm::vec3 forward = glm::normalize(glm::vec3(-1.0f, 0.0f, -1.0f));
    float baseSpeed = 0.01f;
    // ���������˶����������� * �ٶ� * ����ϵ��
    glm::vec3 baseMovement = forward * baseSpeed * oscillationFactor;

    // ���Ҳ��������������ɺ������й����е�ƽ������Ч��
    float amplitude = 0.05f;    // ���������ƫ������С
    float frequency = 1.0f;     // Ƶ�ʣ����Ʋ�������
    // �������Һ����Һ�������������ƫ�ƣ�ģ�����Ұڶ������������
    float offsetX = cos(time * frequency) * amplitude * 0.5f;
    float offsetY = sin(time * frequency * 1.5f) * amplitude;  // ��ֱ�������
    float offsetZ = sin(time * frequency) * amplitude * 0.5f;

    // ����΢С������Ŷ���ʹ�˶��켣����Ȼ
    // ���ɵ����ֵ��һ���� [-0.005, 0.005] ��Χ��
    float noiseX = ((rand() % 100) / 100.0f - 0.5f) * 0.01f;
    float noiseY = ((rand() % 100) / 100.0f - 0.5f) * 0.01f;
    float noiseZ = ((rand() % 100) / 100.0f - 0.5f) * 0.01f;

    // �ϳ������˶����������������˶� + ���Ҳ���ƫ�� + ����Ŷ�
    glm::vec3 movement = baseMovement
        + glm::vec3(offsetX, offsetY, offsetZ)
        + glm::vec3(noiseX, noiseY, noiseZ);
    // ���� GameObject ��λ��
    gameObject->position += movement;

    // Ϊ��ת��������Ա仯��ģ����������е���΢Ťת�򡰻Ӷ���
    float rotX = sin(time * 1.5f) * 0.1f;
    float rotY = cos(time * 1.7f) * 0.1f;
    float rotZ = sin(time * 2.3f) * 0.1f;
    gameObject->rotation *= glm::quat(glm::vec3(rotX, rotY, rotZ));
}




int Game::IntergratedScripts::TUpdateFPS()
{
    static double lastTime = 0.0; // ��һ֡��ʱ�䣬ʹ�� static ������״̬
    static int frameCount = 0;     // ��ǰ֡����ʹ�� static ������״̬
    double fps = 0.0;              // ��ǰ֡�ʣ��ֲ�����
    double currentTime = glfwGetTime();  // ��ȡ��ǰʱ��
    frameCount++;

    fps = frameCount / (currentTime - lastTime);
    frameCount = 0;
    lastTime = currentTime;
    return static_cast<int>(fps);  // �����������͵�֡��
}

void Game::IntergratedScripts::TControlFrameRate(float targetFrameRate)
{
    // ��ȡ��ǰʱ��
    static double lastTime = glfwGetTime();
    double deltaTime = 0.0;
    double frameTime = glfwGetTime() - lastTime;

    // ���㵱ǰ֡ʱ����Ŀ��֡ʱ��Ĳ��
    deltaTime += frameTime;

    // Ŀ��֡ʱ��
    double targetFrameTime = 1.0 / targetFrameRate;

    // �����ǰ֡ʱ��С��Ŀ��֡ʱ�䣬�����Ա�֤�ȶ�֡��
    if (deltaTime < targetFrameTime) {
        double sleepTime = targetFrameTime - deltaTime;
        // ʹ�� std::this_thread::sleep_for ������ glfwSleep
        std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime));  // ʹ�� std::chrono ������ʱ,������Խ����ӳ�
    }

    // ���� lastTime
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








