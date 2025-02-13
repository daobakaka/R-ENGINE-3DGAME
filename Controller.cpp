#include "Controller.h"
#include "EnumTotal.h"
#include "MeshDataManager.h"
#include "LifecycleManager.h"
#include "ScriptModel.h"
//#include "CustomModel.h"
using namespace Game;
extern const char* axisVertexShaderSource;
extern const char* axisFragmentShaderSource;
Controller* Controller::instance = nullptr;  // ！在源文件中初始化静态成员变量,而且需要放在类外部进行，它是属于整个程序生命周期的级别，和类隶属同意级别
//这里是定义指针方法的写法，即返回一个指向Controller的指针，需要显示声明
extern CoordinateSystemCus* axisWidget = nullptr;//这种方式定义，避免多重引用错误
Controller::Controller()
    : position(0.0f, 0.0f, 10.0f),  // 位置初始化
    front(0.0f, 0.0f, -1.0f),     // 前向向量初始化
    up(0.0f, 1.0f, 0.0f),        // 上向量初始化
    pitch(0.0f), yaw(-0.0f),    // 俯仰和偏航角初始化
    rightMousePressed(false),    // 鼠标右键初始化为未按下
    lastX(0.0), lastY(0.0),     // 鼠标初始位置
    tempX(0.0), tempY(0.0),     // 临时偏移位置
    mouseMovesContinuously(false), // 鼠标按下时的移动标志
    windowWidth(2400), windowHeight(1200),  // 屏幕尺寸初始化
    fov(45), aspect(windowWidth/windowHeight), nearPlane(0.1f), farPlane(1000.0f)
{
    view = glm::lookAt(position, position + front, up);
    projection = glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);//初始化
    instance = this;  // 单例的初始化
}

Controller::~Controller()
{  
}
Controller* Controller::GetInstance() {
    if (instance == nullptr) {
        instance = new Controller();
    }
    return instance;
}

void Controller::ProcessInput(GLFWwindow* window, glm::vec3& objectPosition)
{
    // 检查按键输入
    float moveSpeed = 0.1f;  // 控制移动的速度

    // 计算摄像机的右向向量
    glm::vec3 right = glm::normalize(glm::cross(front, up));  // 右向向量

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        objectPosition += front * moveSpeed;  // W键：沿着前向向量前移
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        objectPosition -= front * moveSpeed;  // S键：沿着前向向量后移
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        objectPosition -= right * moveSpeed;  // A键：沿着右向向量左移
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        objectPosition += right * moveSpeed;  // D键：沿着右向向量右移
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        objectPosition += up * moveSpeed;  // SPACE键：沿着上向向量向上移动
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        objectPosition -= up * moveSpeed;  // X键：沿着上向向量向下移动
    }

    // 小键盘 1 => 前视图：摄像机位于 (0,0,10)，面向 -Z 轴，up=(0,1,0)
    if (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS) {
        objectPosition = glm::vec3(0.0f, 0.0f, 10.0f);
        front = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));
        up = glm::vec3(0.0f, 1.0f, 0.0f); // 与 front 不平行
    }

    // 小键盘 2 => 后视图：摄像机位于 (0,0,-10)，面向 +Z 轴
    if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS) {
        objectPosition = glm::vec3(0.0f, 0.0f, -10.0f);
        front = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f));
        up = glm::vec3(0.0f, 1.0f, 0.0f); // 同样保持 Y 轴向上
    }

    // 小键盘 3 => 上视图：摄像机位于 (0,10,0)，面向 -Y 轴
    // 把 up 设置为 X 轴或 Z 轴，以避免与 front 平行
    if (glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS) {
        objectPosition = glm::vec3(0.0f, 10.0f, 0.0f);
        front = glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f));
        // 例如将 up 设置为 -Z，使 front(0,-1,0) 与 up(0,0,-1) 垂直
        up = glm::vec3(0.0f, 0.0f, -1.0f);
    }

    // 小键盘 4 => 下视图：摄像机位于 (0,-10,0)，面向 +Y 轴
    // 同理，为避免与 front 平行，可以把 up 设置为 +Z 或其它
    if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS) {
        objectPosition = glm::vec3(0.0f, -10.0f, 0.0f);
        front = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
        // 例如 up=(0,0,1)，front 与 up 保持垂直
        up = glm::vec3(0.0f, 0.0f, 1.0f);
    }

    // 小键盘 5 => 左视图：摄像机位于 (-10,0,0)，面向 +X 轴
    if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS) {
        objectPosition = glm::vec3(-10.0f, 0.0f, 0.0f);
        front = glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f));
        up = glm::vec3(0.0f, 1.0f, 0.0f); // 与 front 不平行
    }

    // 小键盘 6 => 右视图：摄像机位于 (10,0,0)，面向 -X 轴
    if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS) {
        objectPosition = glm::vec3(10.0f, 0.0f, 0.0f);
        front = glm::normalize(glm::vec3(-1.0f, 0.0f, 0.0f));
        up = glm::vec3(0.0f, 1.0f, 0.0f);
    }

}
void Controller::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{

    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // 隐藏鼠标并抓取        
            rightMousePressed = true;
            if (mouseMovesContinuously)
            {
                glfwGetCursorPos(window, &tempX, &tempY);
                mouseMovesContinuously = false;//点击的时候记录一次位置
                lastX = static_cast<float>(tempX - windowWidth / 2.0);
                lastY = static_cast<float>(windowHeight / 2.0 - tempY); // Y轴反向，屏幕上方是负方向
            }
        }
        else if (action == GLFW_RELEASE) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // 恢复鼠标光标
            rightMousePressed = false;
            mouseMovesContinuously = true;//松开鼠标的时候恢复值
        }
    }
}

void Controller::MouseButtonFun(GLFWwindow* window)//手动测试方法，取消GLAPI监听
{
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // 隐藏鼠标并抓取        
        rightMousePressed = true;
        if (mouseMovesContinuously)
        {
            glfwGetCursorPos(window, &tempX, &tempY);
            mouseMovesContinuously = false;//点击的时候记录一次位置
            lastX = static_cast<float>(tempX - windowWidth / 2.0);
            lastY = static_cast<float>(windowHeight / 2.0 - tempY); // Y轴反向，屏幕上方是负方向
        }
    }
    else
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // 恢复鼠标光标       
        rightMousePressed = false;
        mouseMovesContinuously = true;//松开鼠标的时候恢复值
    }
}

void Controller::ProcessMouseInput(GLFWwindow* window, float& pitch, float& yaw, bool& rightMousePressed)
{

    if (rightMousePressed) {  // 只有按住右键时才进行旋转

        yaw = 0;//要先归零，因为现有的视图矩阵，已经被储存更新
        pitch = 0;

        // std::cout << width<< "and" << height << std::endl;

         // 获取当前鼠标的位置
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);//获取基准参数

        if ((xpos - tempX) * (xpos - tempX) + (ypos - tempY) * (ypos - tempY) > 1.0f)//一次点击判断只有不相等时，才移动，也就时移动鼠标时才移动
        {

            // 计算鼠标相对屏幕中心的偏移量,必须手写，OPENGL里没有直接的API
            float offsetX = static_cast<float>(xpos - windowWidth / 2.0);  // 以屏幕中心为原点
            float offsetY = static_cast<float>(windowHeight / 2.0 - ypos); // Y轴反向，屏幕上方是负方向
            // std::cout << offsetX << "and" << offsetY << std::endl;//这个坐标值是准确的
              // 设置旋转的灵敏度
            float sensitivity = 0.01f;
            // usingn = (offsetY > 0) ? 1 : -1;

             // 更新旋转角度
            yaw += (offsetX - lastX) * sensitivity;   // 水平旋转，绕Y轴旋转
            pitch += (  offsetY-lastY) * sensitivity; // 垂直旋转，绕X轴旋转
          //  std::cout << pitch << std::endl;//流操作 在这里每一帧都打印0！
            // 限制俯仰角度，避免翻转
            if (pitch > 89.0f) pitch = 89.0f;
            if (pitch < -89.0f) pitch = -89.0f;

            // 更新鼠标位置
            lastX = offsetX;
            lastY = offsetY;
        }
        // glfwSetCursorPos(window, width / 2.0, height / 2.0); // 每次移动后将鼠标位置重置为屏幕中心
    }
}

void Controller::FrameControlMethod(GLFWwindow* window)
{
    MouseButtonFun(window);
    ProcessInput(window, position);
    ProcessMouseInput(window, pitch, yaw, rightMousePressed);
}

glm::mat4 Controller::GetViewMatrix()
{    
    
    glm::mat4 cameraRotation = glm::rotate(glm::mat4(1.0f), glm::radians(3*pitch), glm::vec3(1.0f, 0.0f, 0.0f)); //俯仰：绕X轴旋转 
    cameraRotation = glm::rotate(cameraRotation, glm::radians(3*yaw), glm::vec3(0.0f, 1.0f, 0.0f)); // 偏航：绕Y轴旋转
  
    if (rightMousePressed)
        front = glm::normalize( glm::vec3(cameraRotation * glm::vec4(front, 0.0f)));//每次完整后归一化
    return  glm::lookAt(position, position + front, up);
}

glm::mat4 Game::Controller::GetFixedViewMatrix()
{
    glm::mat4 cameraRotation = glm::rotate(glm::mat4(1.0f), glm::radians(3 * pitch), glm::vec3(1.0f, 0.0f, 0.0f)); //俯仰：绕X轴旋转 
    cameraRotation = glm::rotate(cameraRotation, glm::radians(3 * yaw), glm::vec3(0.0f, 1.0f, 0.0f)); // 偏航：绕Y轴旋转

    if (rightMousePressed)
        front = glm::normalize(glm::vec3(cameraRotation * glm::vec4(front, 0.0f)));//每次完整后归一化
    return  glm::lookAt(glm::vec3(0,0,-3),glm::vec3(0,0,-3)+ front, up);
}

glm::mat4 Controller::GetProjectionMatrix()
{
    return projection = glm::perspective(glm::radians(fov), windowWidth / float(windowHeight), nearPlane, farPlane);//重新计算投影矩阵
}


void Controller::BuildWidgetShader(MeshDataManager* meshData, LifecycleManager<CustomModel>* manager)
{
     axisWidget = new CoordinateSystemCus(
        axisVertexShaderSource,        // 坐标轴 widget 顶点着色器源码
        axisFragmentShaderSource,      // 坐标轴 widget 片元着色器源码
        meshData->axisWidgetVertices,        //  meshData 中保存了上面 axisWidgetVertices 数据
        meshData->axisWidgetIndices,         //  meshData 中保存了 axisWidgetIndices 数据
        meshData->axisWidgetVertexCount,     // 36
        meshData->axisWidgetIndexCount,      // 6
        false,
        false// 坐标轴 widget 通常不需要光照
    );
    axisWidget->SetVariant(ModelClass::AxisWidget);
    axisWidget->Initialize(glm::vec3(0.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(1.0f));
    manager->RegisterObject(axisWidget);

}