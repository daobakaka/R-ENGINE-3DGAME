#include "Controller.h"
#include "EnumTotal.h"
#include "MeshDataManager.h"
#include "LifecycleManager.h"
#include "ScriptModel.h"
//#include "CustomModel.h"
using namespace Game;
extern const char* axisVertexShaderSource;
extern const char* axisFragmentShaderSource;
//�����ɫ��������opengl������̻��ƣ�����һ����ȫ����Ⱦ
extern const char* depthShaderVertexShaderSource;
extern const char* depthShaderFragmentShaderSource;
Controller* Controller::instance = nullptr;  // ����Դ�ļ��г�ʼ����̬��Ա����,������Ҫ�������ⲿ���У������������������������ڵļ��𣬺�������ͬ�⼶��
//�����Ƕ���ָ�뷽����д����������һ��ָ��Controller��ָ�룬��Ҫ��ʾ����
extern CoordinateSystemCus* axisWidget = nullptr;//���ַ�ʽ���壬����������ô���
Controller::Controller()
    : position(0.0f, 3.0f, 10.0f),  // λ�ó�ʼ��
    front(0.0f, 0.0f, -1.0f),     // ǰ��������ʼ��
    up(0.0f, 1.0f, 0.0f),        // ��������ʼ��
    pitch(0.0f), yaw(-90.0f),    // ������ƫ���ǳ�ʼ��
    rightMousePressed(false),    // ����Ҽ���ʼ��Ϊδ����
    lastX(0.0), lastY(0.0),     // ����ʼλ��
    tempX(0.0), tempY(0.0),     // ��ʱƫ��λ��
    mouseMovesContinuously(false), // ��갴��ʱ���ƶ���־
    windowWidth(2400), windowHeight(1200),  // ��Ļ�ߴ��ʼ��
    fov(45), aspect(windowWidth/windowHeight), nearPlane(0.1f), farPlane(1000.0f),zoom(45.0f)
{
    view = glm::lookAt(position, position + front, up);
    projection = glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);//��ʼ��
    instance = this;  // �����ĳ�ʼ��
    

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
    // ��鰴������
    float moveSpeed = 0.5f;  // �����ƶ����ٶ�

    // ���������������������ˮƽ������������
    //glm::vec3 right = glm::normalize(glm::cross(front, up));  // ��������
  //  glm::vec3 up = glm::normalize(glm::cross(right, front));       // ��������

    // ǰ���ͺ��ˣ�����ˮƽ����
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        // ǰ������ˮƽ�����ƶ������ı�X��Z���꣩
        objectPosition += glm::vec3(front.x, 0.0f, front.z) * moveSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        // ���ˣ���ˮƽ�����ƶ������ı�X��Z���꣩
        objectPosition -= glm::vec3(front.x, 0.0f, front.z) * moveSpeed;
    }

    // �����ƶ���ʼ�ձ�����ˮƽ����
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        // ���ƣ������������ķ����ƶ������ı�X��Z���꣩
        objectPosition -= glm::vec3(right.x, 0.0f, right.z) * moveSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        // ���ƣ������������ķ����ƶ������ı�X��Z���꣩
        objectPosition += glm::vec3(right.x, 0.0f, right.z) * moveSpeed;
    }

    // �ո������ֱ������ƶ�
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        objectPosition += up * moveSpeed;  // ����
    }

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        objectPosition -= up * moveSpeed;  // ����
    }



    // С���� 1 => ǰ��ͼ�������λ�� (0,0,10)������ -Z �ᣬup=(0,1,0)
    if (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS) {
        objectPosition = glm::vec3(0.0f, 0.0f, 10.0f);
        front = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));
        up = glm::vec3(0.0f, 1.0f, 0.0f); // �� front ��ƽ��

    }

    // С���� 2 => ����ͼ�������λ�� (0,0,-10)������ +Z ��
    if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS) {
        objectPosition = glm::vec3(0.0f, 0.0f, -10.0f);
        front = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f));
        up = glm::vec3(0.0f, 1.0f, 0.0f); // ͬ������ Y ������
        //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // �ָ������
    }

    // С���� 3 => ����ͼ�������λ�� (0,10,0)������ -Y ��
    // �� up ����Ϊ X ��� Z �ᣬ�Ա����� front ƽ��
    if (glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS) {
        objectPosition = glm::vec3(0.0f, 10.0f, 0.0f);
        front = glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f));
        // ���罫 up ����Ϊ -Z��ʹ front(0,-1,0) �� up(0,0,-1) ��ֱ
        up = glm::vec3(0.0f, 0.0f, -1.0f);
    }

    // С���� 4 => ����ͼ�������λ�� (0,-10,0)������ +Y ��
    // ͬ��Ϊ������ front ƽ�У����԰� up ����Ϊ +Z ������
    if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS) {
        objectPosition = glm::vec3(0.0f, -10.0f, 0.0f);
        front = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
        // ���� up=(0,0,1)��front �� up ���ִ�ֱ
        up = glm::vec3(0.0f, 0.0f, 1.0f);
    }

    // С���� 5 => ����ͼ�������λ�� (-10,0,0)������ +X ��
    if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS) {
        objectPosition = glm::vec3(-10.0f, 0.0f, 0.0f);
        front = glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f));
        up = glm::vec3(0.0f, 1.0f, 0.0f); // �� front ��ƽ��
    }

    // С���� 6 => ����ͼ�������λ�� (10,0,0)������ -X ��
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
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // ������겢ץȡ        
            rightMousePressed = true;
            if (mouseMovesContinuously)
            {
                glfwGetCursorPos(window, &tempX, &tempY);
                mouseMovesContinuously = false;//�����ʱ���¼һ��λ��
                lastX = static_cast<float>(tempX - windowWidth / 2.0);
                lastY = static_cast<float>(windowHeight / 2.0 - tempY); // Y�ᷴ����Ļ�Ϸ��Ǹ�����
            }
        }
        else if (action == GLFW_RELEASE) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // �ָ������
            rightMousePressed = false;
            mouseMovesContinuously = true;//�ɿ�����ʱ��ָ�ֵ
        }
    }
}

void Controller::MouseButtonFun(GLFWwindow* window)//�ֶ����Է�����ȡ��GLAPI����
{
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // ������겢ץȡ        
        rightMousePressed = true;
        if (mouseMovesContinuously)
        {
            glfwGetCursorPos(window, &tempX, &tempY);
            mouseMovesContinuously = false;//�����ʱ���¼һ��λ��
            lastX = static_cast<float>(tempX - windowWidth / 2.0);
            lastY = static_cast<float>(windowHeight / 2.0 - tempY); // Y�ᷴ����Ļ�Ϸ��Ǹ�����
        }
    }
    else
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // �ָ������       
        rightMousePressed = false;
        mouseMovesContinuously = true;//�ɿ�����ʱ��ָ�ֵ
    }
}

void Controller::ProcessMouseInput(GLFWwindow* window, float& pitch, float& yaw, bool& rightMousePressed)
{
    
    double xpos, ypos;
    double xOffset, yOffset;
    glfwGetCursorPos(window, &xpos, &ypos);//��ȡ��׼��
    
    if (rightMousePressed) {  // ֻ�а�ס�Ҽ�ʱ�Ž�����ת

        //yaw = 0;//Ҫ�ȹ��㣬��Ϊ���е���ͼ�����Ѿ����������
        //pitch = 0;

        // std::cout << width<< "and" << height << std::endl;

      //  glfwSetCursorPos(window, windowWidth / 2.0, windowHeight / 2.0); // ÿ���ƶ������λ������Ϊ��Ļ����

        if ((xpos - tempX) * (xpos - tempX) + (ypos - tempY) * (ypos - tempY) > 1.0f)//һ�ε���ж�ֻ�в����ʱ�����ƶ���Ҳ��ʱ�ƶ����ʱ���ƶ�
        {
         
            // ������������Ļ���ĵ�ƫ����,������д��OPENGL��û��ֱ�ӵ�API
            float offsetX = static_cast<float>(xpos - windowWidth / 2.0);  // ����Ļ����Ϊԭ��
            float offsetY = static_cast<float>(windowHeight / 2.0 - ypos); // Y�ᷴ����Ļ�Ϸ��Ǹ�����
            // std::cout << offsetX << "and" << offsetY << std::endl;//�������ֵ��׼ȷ��
              // ������ת��������
            float sensitivity = 0.01f;
            // usingn = (offsetY > 0) ? 1 : -1;

             // ������ת�Ƕ�
            yaw += (offsetX - lastX) * sensitivity;   // ˮƽ��ת����Y����ת
            pitch += (  offsetY-lastY) * sensitivity; // ��ֱ��ת����X����ת
          //  std::cout << pitch << std::endl;//������ ������ÿһ֡����ӡ0��
            // ���Ƹ����Ƕȣ����ⷭת
            if (pitch > 89.0f) pitch = 89.0f;
            if (pitch < -89.0f) pitch = -89.0f;

            // �������λ��
            lastX = offsetX;
            lastY = offsetY;
          //  glfwSetCursorPos(window, windowWidth / 2.0, windowHeight / 2.0); // ÿ���ƶ������λ������Ϊ��Ļ����
        }
      //  glfwSetCursorPos(window, windowWidth / 2.0, windowHeight / 2.0); // ÿ���ƶ������λ������Ϊ��Ļ����
    }
}

void Controller::FrameControlMethod(GLFWwindow* window)
{
    MouseButtonFun(window);
    ProcessInput(window, position);
    ProcessMouseInput(window, pitch, yaw, rightMousePressed);
}

void Game::Controller::ProcessMouseScrollGlobal(float yoffset)
{

    
        zoom -= (float)yoffset;
        if (zoom < 10.0f)
            zoom = 10.0f;
        if (zoom > 60.0f)
            zoom = 60.0f;
    
    
}

glm::mat4 Controller::GetViewMatrix()
{
    if (rightMousePressed)
    {
        // ����ǰ���������� X �� (pitch) �� Y �� (yaw) ��ת
        glm::vec3 _front;
        _front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));  // ˮƽ�ʹ�ֱ������תӰ�� X
        _front.y = sin(glm::radians(pitch));  // �� pitch Ӱ�� Y��������
        _front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));  // ˮƽ��תӰ�� Z

        // ��һ��ǰ������
        front = glm::normalize(_front);

        //// ���¼�����������������
        //right = glm::normalize(glm::cross(front, up));  // ��������ǰ���������������Ĳ��
        //up = glm::normalize(glm::cross(right, front));   // ����������������ǰ�������Ĳ��
    }
    // ���¼�����������������
    right = glm::normalize(glm::cross(front, up));  // ��������ǰ���������������Ĳ��
    up = glm::normalize(glm::cross(right, front));   // ����������������ǰ�������Ĳ��
  
    // ͨ�� position, front �� up ����������ͼ����
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Game::Controller::GetFixedViewMatrix()
{
    glm::mat4 cameraRotation = glm::rotate(glm::mat4(1.0f), glm::radians(3 * pitch), glm::vec3(1.0f, 0.0f, 0.0f)); //��������X����ת 
    cameraRotation = glm::rotate(cameraRotation, glm::radians(3 * yaw), glm::vec3(0.0f, 1.0f, 0.0f)); // ƫ������Y����ת

    if (rightMousePressed)
        front = glm::normalize(glm::vec3(cameraRotation * glm::vec4(front, 0.0f)));//ÿ���������һ��
    return  glm::lookAt(position, position + front, up);

}

glm::mat4 Controller::GetProjectionMatrix()
{
    return projection = glm::perspective(glm::radians(zoom), 2.0F, nearPlane, farPlane);//��ʼ��


}


void Controller::BuildWidgetShader(MeshDataManager* meshData, LifecycleManager<CustomModel>* manager)
{
     axisWidget = new CoordinateSystemCus(
        axisVertexShaderSource,        // ������ widget ������ɫ��Դ��
        axisFragmentShaderSource,      // ������ widget ƬԪ��ɫ��Դ��
        meshData->axisWidgetVertices,        //  meshData �б��������� axisWidgetVertices ����
        meshData->axisWidgetIndices,         //  meshData �б����� axisWidgetIndices ����
        meshData->axisWidgetVertexCount,     // 36
        meshData->axisWidgetIndexCount,      // 6
        false,
        false// ������ widget ͨ������Ҫ����
    );
    axisWidget->SetVariant(ModelClass::AxisWidget);
    axisWidget->Initialize(glm::vec3(0.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(1.0f));
    manager->RegisterObject(axisWidget);

}

void Game::Controller::UseDepthShaderProgram()
{
    glUseProgram(_depthShaderProgram);

}

GLint Game::Controller::GetDepthShaderProgram()
{
    return _depthShaderProgram;
}

void Game::Controller::BuildDepthShader()
{

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &depthShaderVertexShaderSource, nullptr);
    glCompileShader(vertexShader);


    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &depthShaderFragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);


    // 2) ������ɫ������
    _depthShaderProgram = glCreateProgram();
    glAttachShader(_depthShaderProgram, vertexShader);
    glAttachShader(_depthShaderProgram, fragmentShader);
    glLinkProgram(_depthShaderProgram);


    // ɾ����ʱShader
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


}
