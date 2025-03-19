// GameObject.cpp
#include "GameObject.h"

using namespace Game;
GameObject::GameObject() {
    // Ĭ�Ϲ��캯�������� initialize() ����Ĭ�ϱ任
    Initialize(glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f));
    variant = ModelClass::OriginalE;
    ifLight = false;

}

GameObject::~GameObject()
{
   // std::cout << "Object type: " << typeid(*this).name() << "has been delete" << std::endl;
}

void GameObject::Initialize(glm::vec3 position, glm::quat rotation, glm::vec3 scale) {
    this->position = position;
    this->rotation = rotation;
    this->scale = scale;
    UpdateTransform();  // ��ʼ��ʱ���±任����

}

void GameObject::UpdateTransform() {
    // ����任����ƽ�ơ���ת������
    transform = glm::mat4(1.0f);  // ��ʼ��Ϊ��λ����
    transform = glm::translate(transform, position);  
    transform *= glm::mat4_cast(rotation);  // ��ת
    transform = glm::scale(transform, scale);  // ����
}
#pragma region ��������ģ��
void GameObject::SetVariant(ModelClass par)
{
    variant = par;
}

ModelClass GameObject::GetVariant()
{
    return variant;
}

#pragma endregion


glm::mat4 GameObject::GetModelMatrix() const {
    return transform;
}


#pragma region  ��������
void GameObject::Start()
{
    std::cout << "Object type: " << typeid(*this).name() << std::endl;
}
void GameObject::Destroy()
{
    std::cout << "Object type: " << typeid(*this).name() <<" is deleting" << std::endl;
    delete this;
}
//���庯������д
void GameObject::UpdateVariant(glm::mat4 view, glm::mat4 projection)
{
    std::cout << "Object type: " << typeid(*this).name() << " the varaint is not active" << std::endl;
}

void Game::GameObject::DrawDepthPic(glm::mat4 lightSpaceMatrix,GLuint shader)
{
}

void Game::GameObject::DrawDepthPicDynamical(glm::mat4 lightSpaceMatrix, GLuint shader)
{
}

void Game::GameObject::UpdateDepthPic(glm::mat4 lightSpaceMatrix,GLuint shader)
{
}

bool Game::GameObject::GetIfCollision()
{
    return false;
}

#pragma endregion
#pragma region ��Ⱦģ��
bool GameObject::Draw(glm::mat4 view, glm::mat4 projection ) {
    glUseProgram(shaderProgram);

    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    //�����Ƕ� ��������� ��������ץȡ����shaderProgram ���棬���������д�ڶ�Ӧ��shader������ж����
    glUniformMatrix4fv(modelLoc, 1, 0, glm::value_ptr(transform));
    glUniformMatrix4fv(viewLoc, 1, 0, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, 0, glm::value_ptr(projection)); 
    glBindVertexArray(VAO);
    //�󶨶����������VAO���������� VAO �����Ķ��㻺�����VBO���������������EBO����
    //���涥�����Ե����ã�����λ�á���ɫ�����ߡ���������ȣ��� VBO �� EBO ���������ڴ洢�������ݺͻ���������
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //��������洢�� EBO��Element Buffer Object���У�EBO �Ѱ󶨵� VAO�������Σ�36�����㣬�޷����������飬��0��ʼ
    glBindVertexArray(0);

    return true;
}
//��ɫ��������
void GameObject::CheckShaderCompilation(GLuint shader)
{
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader Compilation Failed: " << infoLog << std::endl;
    }
    else {
        std::cout << "Shader Compilation Success!" << std::endl;
    }
}
//��ɫ�����Ӽ��
void GameObject::CheckShaderProgramLinking(GLuint program)
{
    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Shader Program Linking Failed test: " << infoLog << std::endl;
    }
    else {
        std::cout << "Shader Program Linking Success!" << std::endl;
    }
}


#pragma endregion

