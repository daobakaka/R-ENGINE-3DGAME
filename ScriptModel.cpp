#include "ScriptModel.h"
using namespace Game;
#pragma region 测试蝴蝶
void ButterflyScript::UpdateVariant(glm::mat4 view, glm::mat4 projection)
{

    //这里就可以写变体独立的帧变换方法

    position += glm::vec3(0.0f, 0.01f, 0.00f);

}
#pragma endregion

#pragma region 参考坐标
void CoordinateSystemCus::Update(glm::mat4 view, glm::mat4 projection)
{
    //UpdateTransform(); 这里可以不用更新位置信息了， 参考坐标系不需要移动 
    DrawLineFixedWidget(view, projection);
}
#pragma endregion

#pragma region 光源模型

void LightModel::SteLightParameters(glm::vec3 color, float intensity, glm::vec3 dirction)
{
    myColor = color;
    myIntensity = intensity;
    myDirection = dirction;
}

void LightModel::RenderingTexture()
{
    // 1) 使用当前 LightModel 的着色器
    glUseProgram(shaderProgram);

    // 2) 设置 baseColor
    GLint baseColorLoc = glGetUniformLocation(shaderProgram, "baseColor");
    glUniform3fv(baseColorLoc, 1, glm::value_ptr(myColor));
    // 3) 设置 lightIntensity
    GLint intensityLoc = glGetUniformLocation(shaderProgram, "lightIntensity");
    glUniform1f(intensityLoc, myIntensity);
    //3.1)设置 lightDirection
    GLuint dirtionLoc = glGetUniformLocation(shaderProgram, "lightDirction");
    glUniform3fv(dirtionLoc, 1, glm::value_ptr(myDirection));

    // 4) 找到 sampler uniform "texture1"
    GLint samplerLoc = glGetUniformLocation(shaderProgram, "texture1");


    // 5) 激活纹理单元 (GL_TEXTURE0 + textureOrder)
    glActiveTexture(GL_TEXTURE0 + textureOrder);

    // 6) 绑定你的纹理对象 (texture) 到该单元
    glBindTexture(GL_TEXTURE_2D, texture);

    // 7) 告诉 sampler uniform "texture1" 使用第 textureOrder 号纹理单元
    glUniform1i(samplerLoc, textureOrder);

}
#pragma endregion

#pragma region 自定义射线

void CustomizeRay::Update(glm::mat4 view, glm::mat4 projection)
{
    UpdateTransform();
    DrawLine(view,projection);
}

bool CustomizeRay::DrawLine(glm::mat4 view, glm::mat4 projection)
{
    
    // 激活当前的着色器程序
    glUseProgram(shaderProgram);


    // 2) 设置射线颜色,这里是对drawline的父级方法进行重写
    GLint baseColorLoc = glGetUniformLocation(shaderProgram, "baseColor");
    glUniform3fv(baseColorLoc, 1, glm::value_ptr(myColor));

    // 获取 shader 中 "model"、"view" 和 "projection" uniform 变量的位置
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");

    // 传入变换矩阵到 shader（第三个参数为 GL_FALSE 表示矩阵不需要转置）
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // 绑定当前模型的 VAO
    glBindVertexArray(VAO);

    // 直接使用 GL_LINES 模式绘制线段
    glDrawElements(GL_LINES, index, GL_UNSIGNED_INT, 0);

    // 解绑 VAO
    glBindVertexArray(0);


    return true;
}

void CustomizeRay::SetRayPar(glm::vec3 color,float intensity)
{
    myColor = color;
    myIntensity = intensity;
}

#pragma endregion
