#include "ScriptModel.h"
#include "ShaderManager.h"
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
#pragma region  场景阴影贴图
void ShadowTexture::DrawDepthPic(glm::mat4 lightSpaceMatrix, GLuint shader)
{
  
    //传入模型世界坐标，视图和透视矩阵，在lightRender中生成
    GLuint modelLoc = glGetUniformLocation(shader, "model");
    glUniformMatrix4fv(modelLoc, 1, 0, glm::value_ptr(transform));


    glBindVertexArray(VAO);
    justDrawVerteies == true ? glDrawArrays(GL_TRIANGLES, 0, index) : glDrawElements(GL_TRIANGLES, index, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);


}
void ShadowTexture::DrawDepthPicDynamical(glm::mat4 lightSpaceMatrix, GLuint shader)
{



    //传入模型世界坐标，视图和透视矩阵，在lightRender中生成
    GLuint modelLoc = glGetUniformLocation(shader, "model");
    glUniformMatrix4fv(modelLoc, 1, 0, glm::value_ptr(transform));

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER,
        verticesTras.size() * sizeof(Vertex),
        verticesTras.data(),
        GL_DYNAMIC_DRAW);
    justDrawVerteies == true ? glDrawArrays(GL_TRIANGLES, 0, index) : glDrawElements(GL_TRIANGLES, index, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);


}
#pragma endregion
#pragma region 通用shader模型 

CustomModelShader::CustomModelShader(const std::string& name, const ModelData& modelData, bool isSkinnedMesh, bool ifLightIn, bool ifShadow)
{
   
    vertexCount = modelData.verticesStruct.size();
    index = modelData.indices.size();
    verticesTras = modelData.verticesStruct;
    IsSkinnedMesh = isSkinnedMesh;
    ifLight = ifLightIn;
    _ifShadow = ifShadow;
    
    // 获取着色器程序
    shaderProgram = ShaderManager::GetInstance()->GetShader(name);

    // 生成 VAO, VBO, EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // 绑定VAO
    glBindVertexArray(VAO);

    // 根据是否为动态网格（例如骨骼动画网格）决定使用动态或静态缓冲区
    GLuint bufferUsage = isSkinnedMesh ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;

    // VBO: 传入所有顶点
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
        modelData.verticesStruct.size() * sizeof(Vertex),
        modelData.verticesStruct.data(),
        bufferUsage);  // 根据是否为动态网格选择缓冲区类型

    // EBO: 传入索引
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        modelData.indices.size() * sizeof(unsigned int),
        modelData.indices.data(),
        bufferUsage);  // 同样根据是否为动态网格选择缓冲区类型

    // 设置顶点属性指针
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, Position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, TexCoords));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, Normal));

    // 解绑VAO
    glBindVertexArray(0);
}



CustomModelShader::CustomModelShader()
{
}

bool CustomModelShader::Draw(glm::mat4 view, glm::mat4 projection)
{
    //集成纹理渲染方法,要在draw之前调用，才能生效
    RenderingTexture();

    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, 0, glm::value_ptr(transform));
    glBindVertexArray(VAO);
    justDrawVerteies == true ? glDrawArrays(GL_TRIANGLES, 0, index) : glDrawElements(GL_TRIANGLES, index, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    //集成纹理渲染方法
  // RenderingTexture();
    return true;
}

bool Game::CustomModelShader::DrawDynamical(glm::mat4 view, glm::mat4 projection)
{

    //集成纹理渲染方法,要在draw之前调用，才能生效
    RenderingTexture();

    // glUseProgram(shaderProgram);

    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");

    glUniformMatrix4fv(modelLoc, 1, 0, glm::value_ptr(transform));
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
        verticesTras.size() * sizeof(Vertex),
        verticesTras.data(),
        GL_DYNAMIC_DRAW);
    justDrawVerteies == true ? glDrawArrays(GL_TRIANGLES, 0, index) : glDrawElements(GL_TRIANGLES, index, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    return true;
}

void Game::CustomModelShader::RenderingTexture()
{
    //这里调用引用的shader避免重复计算
    glUseProgram(shaderProgram);
    if (_drawTexture)
    {

        // 传入纹理缩放因子
        GLuint textureScaleLoc = glGetUniformLocation(shaderProgram, "textureScale");
        glUniform2f(textureScaleLoc, _textureScale.x, _textureScale.y); // 设置纹理缩放因子

        GLuint picData = glGetUniformLocation(shaderProgram, "baseTexture");//预写入图像的shader定义内容
        glActiveTexture(GL_TEXTURE0 + textureOrder);          // 激活纹理单元 0+order
        glBindTexture(GL_TEXTURE_2D, texture);  // 绑定纹理对象到纹理单元 0+order,这里添加DicTexture集合的纹理对象
        // 绑定纹理到纹理单元 0+order，这个顺序的所有纹理单元都遍历绑定一次
        glUniform1i(picData, textureOrder);
    }

}
#pragma endregion
#pragma region 实例化模型
CustomModelInstance::CustomModelInstance()
{
}

CustomModelInstance::CustomModelInstance(const std::string& name, const ModelData& modelData, bool isSkinnedMesh, bool ifLightIn, bool ifShadow, int instanceCount, glm::vec3 positionOffset, glm::vec3 rotationAxis)
{
 
    _instanceCount = instanceCount;
    _positionOffset = positionOffset;
    _rotationAxis = rotationAxis;

    vertexCount = modelData.verticesStruct.size();
    index = modelData.indices.size();
    verticesTras = modelData.verticesStruct;
    IsSkinnedMesh = isSkinnedMesh;
    ifLight = ifLightIn;
    _ifShadow = ifShadow;
    shaderProgram = ShaderManager::GetInstance()->GetShader(name);

    GLuint useageBuffer = isSkinnedMesh ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;

    // 调用生成实例化矩阵的方法
    GenerateInstanceMatrices();
    // 创建 VAO, VBO, EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &_instanceBuffer);  // 实例化缓冲区，用来存储每个实例的变换数据

    // 绑定 VAO
    glBindVertexArray(VAO);

    // VBO: 传入所有顶点
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, modelData.verticesStruct.size() * sizeof(Vertex), modelData.verticesStruct.data(), useageBuffer);

    // EBO: 传入索引
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, modelData.indices.size() * sizeof(unsigned int), modelData.indices.data(), useageBuffer);

    // 设置顶点属性指针
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    

    // 将实例数据传递给 GPU
    glBindBuffer(GL_ARRAY_BUFFER, _instanceBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * _modelMatrices.size(), _modelMatrices.data(), useageBuffer);

    // 为每个实例化数据设置属性指针（每一列矩阵数据传入）
    for (GLuint i = 0; i < 4; i++) {
        glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * i));
        glEnableVertexAttribArray(3 + i);
        glVertexAttribDivisor(3 + i, 1);  // 告诉 OpenGL 每个实例的数据是独立的
    }

    // 解绑 VAO
    glBindVertexArray(0);
}

void Game::CustomModelInstance::GenerateInstanceMatrices()

{
    _modelMatrices.resize(_instanceCount);  // 根据实例数量调整 vector 大小

   int gridSize = std::cbrt(_instanceCount); // 计算立方体的边长
    for (GLuint i = 0; i < _modelMatrices.size(); i++) {
        glm::mat4 model = glm::mat4(1.0f);

        // 计算平移：将实例沿 X, Y, Z 轴等间距分布
        float xOffset = (i % gridSize) * _positionOffset.x;  // 沿X轴平移
        float yOffset = ((i / gridSize) % gridSize) * _positionOffset.y;  // 沿Y轴平移
        float zOffset = (i / (gridSize * gridSize)) * _positionOffset.z;  // 沿Z轴平移

        // 使用计算出的平移量进行平移
        model = glm::translate(model, glm::vec3(xOffset, yOffset, zOffset));  // 形成规则立方体

        // 旋转：绕指定的轴旋转，旋转增量由 rotationAxis 控制
        model = glm::rotate(model, glm::radians(i * _rotationAxis.x), glm::vec3(1.0f, 0.0f, 0.0f));  // 绕 X 轴旋转
        model = glm::rotate(model, glm::radians(i * _rotationAxis.y), glm::vec3(0.0f, 1.0f, 0.0f));  // 绕 Y 轴旋转
        model = glm::rotate(model, glm::radians(i * _rotationAxis.z), glm::vec3(0.0f, 0.0f, 1.0f));  // 绕 Z 轴旋转

        _modelMatrices[i] = model;


    }
}


// 需要保留原有的方法签名
bool Game::CustomModelInstance::Draw(glm::mat4 view, glm::mat4 projection)
{
    // 直接调用渲染纹理方法，渲染纹理数据
    RenderingTexture();

    // 绑定VAO
    glBindVertexArray(VAO);

    GLuint modelLoc = glGetUniformLocation(shaderProgram, "transform");

    // 这里可以传入变换 控制实例
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));

    // 使用实例化绘制
    justDrawVerteies == true
        ? glDrawArraysInstanced(GL_TRIANGLES, 0, index, _modelMatrices.size())  // 使用 glDrawArraysInstanced
        : glDrawElementsInstanced(GL_TRIANGLES, index, GL_UNSIGNED_INT, 0, _modelMatrices.size());  // 使用 glDrawElementsInstanced

    // 解绑VAO
    glBindVertexArray(0);

    return true;
}

// DrawDynamical 方法
bool Game::CustomModelInstance::DrawDynamical(glm::mat4 view, glm::mat4 projection)
{
    // 直接调用渲染纹理方法，渲染纹理数据
    RenderingTexture();

    // 获取模型矩阵的uniform位置
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");

    // 设置模型矩阵
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));

    // 绑定VAO
    glBindVertexArray(VAO);

    // 更新顶点数据（如果有动态数据）
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesTras.size() * sizeof(Vertex), verticesTras.data(), GL_DYNAMIC_DRAW);

    // 绑定实例化数据的VBO（如果有）
    glBindBuffer(GL_ARRAY_BUFFER, _instanceBuffer); // _instanceBuffer 是实例化数据的缓冲区
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * _modelMatrices.size(), _modelMatrices.data(), GL_DYNAMIC_DRAW);

    // 为每个实例化数据设置属性指针（每一列矩阵数据传入）
    for (GLuint i = 0; i < 4; i++) {
        glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * i));
        glEnableVertexAttribArray(3 + i);
        glVertexAttribDivisor(3 + i, 1);  // 每个实例的数据是独立的
    }

    // 使用实例化绘制
    justDrawVerteies == true
        ? glDrawArraysInstanced(GL_TRIANGLES, 0, index, _modelMatrices.size())  // 使用 glDrawArraysInstanced
        : glDrawElementsInstanced(GL_TRIANGLES, index, GL_UNSIGNED_INT, 0, _modelMatrices.size());  // 使用 glDrawElementsInstanced

    // 解绑VAO
    glBindVertexArray(0);

    return true;
}

// RenderingTexture方法
void Game::CustomModelInstance::RenderingTexture()
{
    // 这里调用引用的shader避免重复计算
    glUseProgram(shaderProgram);

    if (_drawTexture)
    {
        // 传入纹理缩放因子
        GLuint textureScaleLoc = glGetUniformLocation(shaderProgram, "textureScale");
        glUniform2f(textureScaleLoc, _textureScale.x, _textureScale.y); // 设置纹理缩放因子

        GLuint picData = glGetUniformLocation(shaderProgram, "baseTexture");
        glActiveTexture(GL_TEXTURE0 + textureOrder); // 激活纹理单元 0+order
        glBindTexture(GL_TEXTURE_2D, texture);      // 绑定纹理对象到纹理单元 0+order
        glUniform1i(picData, textureOrder);         // 将纹理单元绑定到着色器的“baseTexture” uniform
    }
}

#pragma endregion
#pragma region 蝴蝶测试新动画模型

void ButterflyScriptShader::UpdateVariant(glm::mat4 view, glm::mat4 projection)
{
    position += glm::vec3(0.0f, 0.01f, 0.00f);
}

#pragma endregion