// GameObject.cpp
#include "GameObject.h"

using namespace Game;
GameObject::GameObject() {
    // 默认构造函数，调用 initialize() 设置默认变换
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
    UpdateTransform();  // 初始化时更新变换矩阵

}

void GameObject::UpdateTransform() {
    // 计算变换矩阵：平移、旋转、缩放
    transform = glm::mat4(1.0f);  // 初始化为单位矩阵
    transform = glm::translate(transform, position);  
    transform *= glm::mat4_cast(rotation);  // 旋转
    transform = glm::scale(transform, scale);  // 缩放
}
#pragma region 方法变体模块
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


#pragma region  生命周期
void GameObject::Start()
{
    std::cout << "Object type: " << typeid(*this).name() << std::endl;
}
void GameObject::Destroy()
{
    std::cout << "Object type: " << typeid(*this).name() <<" is deleting" << std::endl;
    delete this;
}
//变体函数被重写
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
#pragma region 渲染模块
bool GameObject::Draw(glm::mat4 view, glm::mat4 projection ) {
    glUseProgram(shaderProgram);

    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    //这里是对 三个定义的 参数进行抓取，在shaderProgram 里面，这个参数是写在对应的shader里面进行定义的
    glUniformMatrix4fv(modelLoc, 1, 0, glm::value_ptr(transform));
    glUniformMatrix4fv(viewLoc, 1, 0, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, 0, glm::value_ptr(projection)); 
    glBindVertexArray(VAO);
    //绑定顶点数组对象（VAO），激活与 VAO 关联的顶点缓冲对象（VBO）和索引缓冲对象（EBO）。
    //保存顶点属性的配置，例如位置、颜色、法线、纹理坐标等，与 VBO 和 EBO 关联，用于存储顶点数据和绘制索引。
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //索引数组存储在 EBO（Element Buffer Object）中，EBO 已绑定到 VAO，三角形，36个顶点，无符号整形数组，由0开始
    glBindVertexArray(0);

    return true;
}
//着色器编译检查
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
//着色器链接检查
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

