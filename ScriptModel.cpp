#include "ScriptModel.h"
#include "ShaderManager.h"
#include "LifecycleManager.h"
#include "light.h"
using namespace Game;
#pragma region 测试蝴蝶
//extern LifecycleManager<CustomModel>* manager;//获取管理器控制器


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
    glActiveTexture(GL_TEXTURE0 + _textureOrder);

    // 6) 绑定你的纹理对象 (texture) 到该单元
    glBindTexture(GL_TEXTURE_2D, _textures[PictureTpye::BaseP]);

    // 7) 告诉 sampler uniform "texture1" 使用第 textureOrder 号纹理单元
    glUniform1i(samplerLoc, _textureOrder);

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
    
    lightSpawner = LightSpawner::GetInstance();
    // 获取着色器程序
    shaderProgram = ShaderManager::GetInstance()->GetShader(name);
    //用于子类C++中不能采用此种方式进行构造，因为派生类的初始化还未构造完毕
    SelfIns();

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
    UniformParametersInput();//父类在这里调用空方法
    RenderingLight();//通用shader，光的渲染在这里进行

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
    UniformParametersInput();//父类在这里调用空方法
    RenderingLight();//通用shader，光的渲染在这里进行
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
        glActiveTexture(GL_TEXTURE0 + _textureOrder);          // 激活纹理单元 0+order
        glBindTexture(GL_TEXTURE_2D, _textures[PictureTpye::BaseP]);  // 绑定纹理对象到纹理单元 0+order,这里添加DicTexture集合的纹理对象
        // 绑定纹理到纹理单元 0+order，这个顺序的所有纹理单元都遍历绑定一次
        glUniform1i(picData, _textureOrder);


        RenderingTextureAdditional();
    }

}
void Game::CustomModelShader::RenderingTextureAdditional()
{
    int textureUnit = _textureOrder + 1; // 从 textureOrder + 1 开始

    for (const auto& pair : _textures) {
        PictureTpye type = pair.first;
        GLuint textureID = pair.second;

        // 跳过基础纹理（已经在 RenderingTexture 中加载）
        if (type == BaseP) {
            continue;
        }

        // 根据纹理类型绑定到对应的纹理单元
        switch (type) {
        case NormalP: {
            GLuint normalTextureLoc = glGetUniformLocation(shaderProgram, "normalTexture");
            glActiveTexture(GL_TEXTURE0 + textureUnit); // 激活纹理单元
            glBindTexture(GL_TEXTURE_2D, textureID); // 绑定法线纹理
            glUniform1i(normalTextureLoc, textureUnit); // 传递纹理单元
            break;
        }
        case SpecularP: {
            GLuint specularTextureLoc = glGetUniformLocation(shaderProgram, "specularTexture");
            glActiveTexture(GL_TEXTURE0 + textureUnit); // 激活纹理单元
            glBindTexture(GL_TEXTURE_2D, textureID); // 绑定高光反射纹理
            glUniform1i(specularTextureLoc, textureUnit); // 传递纹理单元
            break;
        }
        case HightP: {
            GLuint heightTextureLoc = glGetUniformLocation(shaderProgram, "heightTexture");
            glActiveTexture(GL_TEXTURE0 + textureUnit); // 激活纹理单元
            glBindTexture(GL_TEXTURE_2D, textureID); // 绑定高度纹理
            glUniform1i(heightTextureLoc, textureUnit); // 传递纹理单元
            break;
        }
        case RoughnessP: {
            GLuint roughnessTextureLoc = glGetUniformLocation(shaderProgram, "roughnessTexture");
            glActiveTexture(GL_TEXTURE0 + textureUnit); // 激活纹理单元
            glBindTexture(GL_TEXTURE_2D, textureID); // 绑定粗糙度纹理
            glUniform1i(roughnessTextureLoc, textureUnit); // 传递纹理单元
            break;
        }
        case AOP: {
            GLuint aoTextureLoc = glGetUniformLocation(shaderProgram, "aoTexture");
            glActiveTexture(GL_TEXTURE0 + textureUnit); // 激活纹理单元
            glBindTexture(GL_TEXTURE_2D, textureID); // 绑定 AO 纹理
            glUniform1i(aoTextureLoc, textureUnit); // 传递纹理单元
            break;
        }
        default:
            break;
        }

        textureUnit++; // 递增纹理单元
    }

}
void Game::CustomModelShader::RenderingLight()
{

    if (ifLight)
    {  // 2. 点光源数据
        const auto& pointLights = lightSpawner->GetPointLights();
        std::vector<std::pair<float, int>> pointLightDistances;  // 存储每个点光源到物体的距离平方和索引,新声明的结构
        glm::vec3 objectPosition = position;  // 使用物体的当前位置作为参考

        // 遍历点光源计算距离平方，并且筛选出距离小于等于 1000 的光源
        for (int i = 0; i < pointLights.size(); i++) {
            const auto& light = pointLights[i];
            float distanceSquared = glm::dot(objectPosition - light.position, objectPosition - light.position);
            if (distanceSquared <= 2000.0f) {  // 只选择距离平方小于等于 100 的光源
                pointLightDistances.push_back({ distanceSquared, i });
               // std::cout << i << "距离" << distanceSquared << std::endl;
            }
        }

        // 按照距离平方升序排序
        std::sort(pointLightDistances.begin(), pointLightDistances.end(), [](const std::pair<float, int>& a, const std::pair<float, int>& b) {
            return a.first < b.first;  // 按照距离平方升序排序
            });
        //不能动态传入数组数量，shader里面不支持，数组索引需写死，但是可以通过常量来约束 
        GLuint numPointLightsLoc = glGetUniformLocation(shaderProgram, "numPointLights");
        glUniform1i(numPointLightsLoc, std::min(static_cast<int>(pointLightDistances.size()), 4));

        // 只选择最近的 4 个点光源
        for (int i = 0; i < std::min(4, static_cast<int>(pointLightDistances.size())); i++) {
            int index = pointLightDistances[i].second;  // 获取最近的点光源的索引，第一个值是距离，第二个值是距离
            auto light = pointLights[index];
            //  std::cout << "点光源强度" << light.intensity << std::endl;
              // 计算距离平方并传递到 shader
            float distanceSquared = pointLightDistances[i].first;
            //  std::cout <<i<< "距离" << distanceSquared << std::endl;
            std::string posName = "lightPos[" + std::to_string(i) + "]";
            std::string colorName = "lightColor[" + std::to_string(i) + "]";
            std::string intensityName = "lightIntensity[" + std::to_string(i) + "]";
            std::string distanceSquaredName = "lightDistanceSquared[" + std::to_string(i) + "]"; // 新增变量

            GLuint posLoc = glGetUniformLocation(shaderProgram, posName.c_str());
            GLuint colorLoc = glGetUniformLocation(shaderProgram, colorName.c_str());
            GLuint intensityLoc = glGetUniformLocation(shaderProgram, intensityName.c_str());
            GLuint distanceSquaredLoc = glGetUniformLocation(shaderProgram, distanceSquaredName.c_str()); // 获取距离平方位置

            glUniform3fv(posLoc, 1, glm::value_ptr(light.position));
            glUniform3fv(colorLoc, 1, glm::value_ptr(light.color));
            glUniform1f(intensityLoc, light.intensity);
            glUniform1f(distanceSquaredLoc, distanceSquared);  // 传递距离平方
        }


        // 4. 手电筒（聚光灯）数据
        auto flashLights = lightSpawner->GetFlashLights();
        std::vector<std::pair<float, int>> flashLightDistances;  // 存储每个聚光灯到物体的距离平方和索引

        // 遍历聚光灯计算距离平方，并且筛选出距离小于等于 100 的聚光灯
        for (int j = 0; j < flashLights.size(); j++) {
            const auto& flash = flashLights[j];
            float distanceSquared = glm::dot(objectPosition - flash.position, objectPosition - flash.position);
            if (distanceSquared <= 2000.0f) {  // 只选择距离小于等于 1000 的聚光灯
                flashLightDistances.push_back({ distanceSquared, j });
            }
        }

        // 按照距离平方升序排序
        std::sort(flashLightDistances.begin(), flashLightDistances.end(), [](const std::pair<float, int>& a, const std::pair<float, int>& b) {
            return a.first < b.first;  // 按照距离平方升序排序
            });

        GLuint numFlashLightsLoc = glGetUniformLocation(shaderProgram, "numFlashLights");
        glUniform1i(numFlashLightsLoc, std::min(static_cast<int>(flashLightDistances.size()), 4));

        // 只选择最近的 4 个聚光灯
        for (int j = 0; j < std::min(4, static_cast<int>(flashLightDistances.size())); j++) {
            int index = flashLightDistances[j].second;  // 获取最近的聚光灯的索引
            auto flash = flashLights[index];

            // 计算距离平方并传递到 shader
            float distanceSquared = flashLightDistances[j].first;

            std::string flashPosName = "flashLightPos[" + std::to_string(j) + "]";
            std::string flashDirName = "flashLightDirection[" + std::to_string(j) + "]";
            std::string flashColorName = "flashLightColor[" + std::to_string(j) + "]";
            std::string flashIntensityName = "flashLightIntensity[" + std::to_string(j) + "]";
            std::string flashCutoffName = "flashLightCutoff[" + std::to_string(j) + "]";
            std::string flashDistanceSquaredName = "flashLightDistanceSquared[" + std::to_string(j) + "]"; // 新增变量

            GLuint flashPosLoc = glGetUniformLocation(shaderProgram, flashPosName.c_str());
            GLuint flashDirLoc = glGetUniformLocation(shaderProgram, flashDirName.c_str());
            GLuint flashColorLoc = glGetUniformLocation(shaderProgram, flashColorName.c_str());
            GLuint flashIntensityLoc = glGetUniformLocation(shaderProgram, flashIntensityName.c_str());
            GLuint flashCutoffLoc = glGetUniformLocation(shaderProgram, flashCutoffName.c_str());
            GLuint flashDistanceSquaredLoc = glGetUniformLocation(shaderProgram, flashDistanceSquaredName.c_str()); // 获取距离平方位置

            glUniform3fv(flashPosLoc, 1, glm::value_ptr(flash.position));
            glUniform3fv(flashDirLoc, 1, glm::value_ptr(flash.direction));
            glUniform3fv(flashColorLoc, 1, glm::value_ptr(flash.color));
            glUniform1f(flashIntensityLoc, flash.intensity);
            glUniform1f(flashCutoffLoc, flash.cutoff);
            glUniform1f(flashDistanceSquaredLoc, distanceSquared);  // 传递距离平方
        }

    }
}
void Game::CustomModelShader::SelfIns()
{
   
}
#pragma endregion
#pragma region 实例化模型
CustomModelInstance::CustomModelInstance()
{
}

Game::CustomModelInstance::~CustomModelInstance()
{
    if (_lightRender!=nullptr)
    {
          //单例不可访问？  
    }
}

CustomModelInstance::CustomModelInstance(const std::string& name, const ModelData& modelData, bool isSkinnedMesh, bool ifLightIn, bool ifShadow, int instanceCount, glm::vec3 positionOffset,
    glm::vec3 rotationAxis,ModelClass type)
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
    _lightRender = LightRender::GetInstance();

    GLuint useageBuffer = isSkinnedMesh ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;

    // 调用生成实例化矩阵的方法
    GenerateInstanceMatrices(type);
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

void Game::CustomModelInstance::GenerateInstanceMatrices(ModelClass type)
{
    _modelMatrices.resize(_instanceCount);  // 根据实例数量调整 vector 大小

    switch (type)
    {
    case Game::InstanceCube:
    {
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
        break;
    }

    case Game::InstanceCircle:
    {
        // 圆环实例化（树的效果）
        float baseRadius = 20.0f; // 基础半径
        int layers = 100; // 树的层数
        int remainingInstances = _instanceCount; // 剩余的实例数量

        // 设置整体偏移参数
        float offsetX = 0.0f; // X 轴整体偏移
        float offsetZ = 20.0f; // Z 轴整体偏移

        for (int layer = 0; layer < layers; layer++) {
            // 计算当前层的半径
            float radius = baseRadius * (static_cast<float>(layer) / layers) * 100.0f;

            // 确保基础半径范围内没有树
            if (radius < baseRadius) {
                continue; // 跳过基础半径范围内的层
            }

            // 计算当前层的实例数量
            int instancesInLayer = (layer == layers - 1) ? remainingInstances : remainingInstances / (layers - layer);
            remainingInstances -= instancesInLayer;

            for (int i = 0; i < instancesInLayer; i++) {
                glm::mat4 model = glm::mat4(1.0f);

                // 计算圆环上的位置
                float angle = static_cast<float>(i) / instancesInLayer * 2.0f * glm::pi<float>(); // 当前实例的角度
                float x = radius * cos(angle); // X 坐标
                float z = radius * sin(angle); // Z 坐标

                // 添加随机偏移
                float randomOffsetX = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * _positionOffset.x * 0.5f;
                float randomOffsetZ = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * _positionOffset.z * 0.5f;

                // 平移：将实例放置在圆环上，并应用随机偏移和整体偏移
                model = glm::translate(model, glm::vec3(x + randomOffsetX + offsetX, 0.0f, z + randomOffsetZ + offsetZ));

                // 添加随机缩放（树的大小随机化）
                float randomScale = 0.5f + static_cast<float>(rand()) / RAND_MAX * 1.5f; // 随机缩放范围：0.5 到 2.0
                model = glm::scale(model, glm::vec3(randomScale));

                // 旋转：使实例朝向圆环中心，并应用 _rotationAxis
                float rotationAngle = glm::degrees(angle) + 90.0f; // 调整朝向
                model = glm::rotate(model, glm::radians(rotationAngle * _rotationAxis.y), glm::vec3(0.0f, 1.0f, 0.0f)); // 绕 Y 轴旋转
                model = glm::rotate(model, glm::radians(i * _rotationAxis.x), glm::vec3(1.0f, 0.0f, 0.0f)); // 绕 X 轴旋转
                model = glm::rotate(model, glm::radians(i * _rotationAxis.z), glm::vec3(0.0f, 0.0f, 1.0f)); // 绕 Z 轴旋转

                _modelMatrices.push_back(model);
            }
        }
        break;
    }

    case Game::InstanceSphere:
    {
        // 球形实例化
        float radius = 5.0f; // 球体半径
        int rows = static_cast<int>(std::sqrt(_instanceCount)); // 球体的行数
        int cols = rows; // 球体的列数

        for (GLuint i = 0; i < _modelMatrices.size(); i++) {
            glm::mat4 model = glm::mat4(1.0f);

            // 计算球体上的位置（基于极坐标）
            float phi = static_cast<float>(i / cols) / rows * glm::pi<float>(); // 极角 (0 到 π)
            float theta = static_cast<float>(i % cols) / cols * 2.0f * glm::pi<float>(); // 方位角 (0 到 2π)

            float x = radius * sin(phi) * cos(theta); // X 坐标
            float y = radius * cos(phi);             // Y 坐标
            float z = radius * sin(phi) * sin(theta); // Z 坐标

            // 平移：将实例放置在球体上，并应用 _positionOffset
            model = glm::translate(model, glm::vec3(x * _positionOffset.x, y * _positionOffset.y, z * _positionOffset.z));

            // 旋转：使实例朝向球体中心，并应用 _rotationAxis
            float rotationAngleX = glm::degrees(phi) * _rotationAxis.x; // 绕 X 轴旋转
            float rotationAngleY = glm::degrees(theta) * _rotationAxis.y; // 绕 Y 轴旋转
            float rotationAngleZ = i * _rotationAxis.z; // 绕 Z 轴旋转
            model = glm::rotate(model, glm::radians(rotationAngleX), glm::vec3(1.0f, 0.0f, 0.0f)); // 绕 X 轴旋转
            model = glm::rotate(model, glm::radians(rotationAngleY), glm::vec3(0.0f, 1.0f, 0.0f)); // 绕 Y 轴旋转
            model = glm::rotate(model, glm::radians(rotationAngleZ), glm::vec3(0.0f, 0.0f, 1.0f)); // 绕 Z 轴旋转

            _modelMatrices[i] = model;
        }
        break;
    }
    case Game::InstanceRound:
    {
        // 定义圆形区域的最大半径
        float maxRadius = 300.0f; // 根据需求调整圆形区域的半径
        // 全部实例数由 _instanceCount 决定
        for (int i = 0; i < _instanceCount; i++) {
            glm::mat4 model = glm::mat4(1.0f);

            // 随机生成一个均匀分布在圆形内的点
            // 为了均匀分布，使用 sqrt(random) 得到半径 r
            float r = sqrt(static_cast<float>(rand()) / RAND_MAX) * maxRadius;
            float theta = static_cast<float>(rand()) / RAND_MAX * 2.0f * glm::pi<float>();
            float x = r * cos(theta);
            float z = r * sin(theta);

            // 计算基于半径的额外随机偏移
            // 当 r 较大（靠近边缘）时，偏移幅度更大
            float factor = r / maxRadius; // 范围 [0, 1]
            float randomOffsetX = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * _positionOffset.x * (1.0f + factor);
            float randomOffsetZ = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * _positionOffset.z * (1.0f + factor);

            // 平移：将实例放置到圆内随机位置，并应用随机偏移
            model = glm::translate(model, glm::vec3(x + randomOffsetX, 0.0f, z + randomOffsetZ));

            // 添加随机缩放（例如树的大小随机化）
            float randomScale = 0.5f + static_cast<float>(rand()) / RAND_MAX * 1.5f; // 缩放范围：0.5 到 2.0
            model = glm::scale(model, glm::vec3(randomScale));

            // 旋转：使实例面向离圆心的方向，再加上额外的随机旋转
            float baseRotation = glm::degrees(theta) + 90.0f; // 使对象朝外
            model = glm::rotate(model, glm::radians(baseRotation * _rotationAxis.y), glm::vec3(0.0f, 1.0f, 0.0f));
            // 这里也可以添加绕 X/Z 轴的额外旋转
            model = glm::rotate(model, glm::radians(i * _rotationAxis.x), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(i * _rotationAxis.z), glm::vec3(0.0f, 0.0f, 1.0f));

            _modelMatrices.push_back(model);
        }
        break;
    }



    default:
        break;
    }
}

void Game::CustomModelInstance::UpdateDepthViewPortPic(glm::mat4 view, glm::mat4 projection, GLuint shader)
{

    //空方法 实例化物体不参于计算
}

void Game::CustomModelInstance::EnableDepthcal()
{

    _useViewPortDepthMap = true;
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
        glActiveTexture(GL_TEXTURE0 + _textureOrder); // 激活纹理单元 0+order
        glBindTexture(GL_TEXTURE_2D, _textures[PictureTpye::BaseP]);      // 绑定纹理对象到纹理单元 0+order
        glUniform1i(picData, _textureOrder);         // 将纹理单元绑定到着色器的“baseTexture” uniform
               
    }
    //这里通过enable方法激活,使用视口深度图进行处理
    if (_useViewPortDepthMap)
    {
        //传入深度纹理
        GLuint picDataDepth = glGetUniformLocation(shaderProgram, "depthTexture");
        glActiveTexture(GL_TEXTURE0 + _textureOrder + 1); // 激活纹理单元 0+order
        glBindTexture(GL_TEXTURE_2D, _lightRender->GetDepthShaderProgram(ShaderClass::DepthMapTest));      // 绑定纹理对象到纹理单元 0+order
        glUniform1i(picDataDepth, _textureOrder + 1);         // 将纹理单元绑定到着色器的“baseTexture” uniform

    }

}

#pragma endregion
#pragma region 蝴蝶测试新动画模型

void ButterflyScriptShader::SelfIns()
{
    // 生成随机数，范围 [0,1]
    float r0 = static_cast<float>(rand()) / RAND_MAX;
    float r1 = static_cast<float>(rand()) / RAND_MAX;
    float r2 = static_cast<float>(rand()) / RAND_MAX;
    float r3 = static_cast<float>(rand()) / RAND_MAX;
    float r4 = static_cast<float>(rand()) / RAND_MAX;
    float r5 = static_cast<float>(rand()) / RAND_MAX;
    float r6 = static_cast<float>(rand()) / RAND_MAX;
    _dt = 0.0167f;
    // 随机生成水平圆周运动参数：
    // _circleRadius 在 [5,15]
    _circleRadius = 5.0f + r0 * 10.0f;
    // _horizontalSpeed 在 [3,6]
    float sign = (rand() % 2 == 0) ? 1.0f : -1.0f;//给一个随机正复值
    _horizontalSpeed = sign * (3.0f + r1 * 3.0f);
    // 起始角度在 [0, 2π)
    _xzStartAngle = r2 * 2.0f * glm::pi<float>();

    // 随机生成垂直上升参数：
    // _verticalSpeed 在 [0.5, 2.0]
    _verticalSpeed = 0.5f + r3 * 1.5f;
    _baseY = 0.0f; // 基准 Y 坐标

    // 随机生成振荡参数：
    // 振荡开始时间在 [10, 20] 秒
    _oscStartTime = 10.0f + r4 * 10.0f;
    // 振荡振幅在 [0.5, 2.0]
    _oscillationAmplitude = 0.5f + r5 * 1.5f;
    // 振荡频率在 [1, 3] 弧度/秒
    _oscillationFrequency = 1.0f + r6 * 2.0f;

    _timeAccumulator = 0.0f;

    // 初始化位置：根据初始角度确定在 XZ 平面的初始位置，Y 取基准值
    position = glm::vec3(_circleRadius * cos(_xzStartAngle),
        _baseY,
        _circleRadius * sin(_xzStartAngle));
    // 初始化旋转为默认（假定默认朝向为正 Z 轴）
    rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    std::cout << "初始化蝴蝶参数" << std::endl;
}

void ButterflyScriptShader::UpdateVariant(glm::mat4 view, glm::mat4 projection)
{
  
    _timeAccumulator += _dt;

    // 为保持水平线速度固定：
    // 水平线速度 = _horizontalSpeed (单位/秒)
    // 圆周运动的角速度 = _horizontalSpeed / _circleRadius
    float angle = _xzStartAngle + (_horizontalSpeed / _circleRadius) * _timeAccumulator;
    angle = fmod(angle, 2.0f * glm::pi<float>()); // 保证在 [0,2π) 内

    // 更新位置：
    // XZ 平面：沿圆周运动
    float x = _circleRadius * cos(angle);
    float z = _circleRadius * sin(angle);
    // Y 轴：均匀上升 + 振荡（当时间超过振荡开始时间后）
    float y = _baseY + _verticalSpeed * _timeAccumulator;
    if (_timeAccumulator > _oscStartTime)
    {
        float oscTime = _timeAccumulator - _oscStartTime;
        y += _oscillationAmplitude * sin(_oscillationFrequency * oscTime);
    }
    position = glm::vec3(x, y, z);

    // 更新旋转：使蝴蝶始终面向水平运动的切线方向（仅绕 Y 轴旋转）
    // 对于圆周轨迹 P(angle) = (R*cos(angle), y, R*sin(angle))
    // 水平切线 T = (-sin(angle), 0, cos(angle))
    glm::vec3 tangent = glm::vec3(-sin(angle), 0.0f, cos(angle));
    tangent = glm::normalize(tangent);

    // 计算目标 yaw 角：使用 atan2 得到水平切线方向的角度
    float desiredYaw = atan2(tangent.x, tangent.z);

    // 由于当前计算得到的方向实际上与切线相反（导致物体朝向圆心），
    // 加上 π 补偿，使其朝向切线方向
    desiredYaw -= glm::pi<float>()/2;

    // 构造仅绕 Y 轴旋转的四元数
    rotation = glm::angleAxis(desiredYaw, glm::vec3(0.0f, 1.0f, 0.0f));
}

#pragma endregion

#pragma region 玩家模型

void GamePlayer::UpdateVariant(glm::mat4 view, glm::mat4 projection)
{




    


}
void Game::GamePlayer::Start()
{

    //调用公共初始化函数，初始化流程控制器变量
    _manager= LifecycleManager<CustomModel>::GetInstance();
    //获取控制器组件
    _controller = Controller::GetInstance();
}

void Game::GamePlayer::UniformParametersInput()
{
    
 
    
    //金属度
    GLuint metallicLoc = glGetUniformLocation(shaderProgram, "metallic");
    glUniform1f(metallicLoc,0.9f);

    //糙度
    GLuint roughnessLoc = glGetUniformLocation(shaderProgram, "roughness");
    glUniform1f(roughnessLoc, .5f);
    //透明度
    GLuint opacityLoc = glGetUniformLocation(shaderProgram, "opacity");
    glUniform1f(opacityLoc, 1.0f);
    //折射率
    GLuint IORLoc = glGetUniformLocation(shaderProgram, " IOR");
    glUniform1f(IORLoc, 1.330f);
    //环境光贡献率
    GLuint aoLoc = glGetUniformLocation(shaderProgram, "ao");
    glUniform1f(aoLoc, 1.5f);


    // 自发光
    GLuint emissionLoc = glGetUniformLocation(shaderProgram, "emission");
    glUniform3f(emissionLoc, .05f, .05f, .05f); // 传入自发光颜色

    // 基本色
    GLuint baseColorLoc = glGetUniformLocation(shaderProgram, "baseColor");
    glUniform3f(baseColorLoc, 0.9f, 0.9f, 0.9f); // 传入基本色（暗色）
}



#pragma endregion
#pragma region 游戏道具板块


void Game::GameBullet::UpdateVariant(glm::mat4 view, glm::mat4 projection)
{
    //给予子弹10 速度
   // this->GetComponent<PhysicalEngine>()->SetVelocity(glm::vec3(0, 0, 10));
    _waveTime += 0.0167f;

}
void Game::GameBullet::UniformParametersInput()
{

    // 获取 uniform 变量的位置
    GLuint timeLoc = glGetUniformLocation(shaderProgram, "waveTime");
    GLuint waveAmplitudeLoc = glGetUniformLocation(shaderProgram, "waveAmplitude");
    GLuint waveFrequencyLoc = glGetUniformLocation(shaderProgram, "waveFrequency");
    GLuint waveSpeedLoc = glGetUniformLocation(shaderProgram, "waveSpeed");

    glUniform1f(timeLoc, _waveTime);
    glUniform1f(waveAmplitudeLoc, _waveAmplitude);
    glUniform1f(waveFrequencyLoc, _waveFrequency);
    glUniform1f(waveSpeedLoc, _waveSpeed);


    //金属度
    GLuint metallicLoc = glGetUniformLocation(shaderProgram, "metallic");
    glUniform1f(metallicLoc, 0.3f);

    //糙度
    GLuint roughnessLoc = glGetUniformLocation(shaderProgram, "roughness");
    glUniform1f(roughnessLoc, .1f);
    //透明度
    GLuint opacityLoc = glGetUniformLocation(shaderProgram, "opacity");
    glUniform1f(opacityLoc, 0.4f);
    //折射率
    GLuint IORLoc = glGetUniformLocation(shaderProgram, " IOR");
    glUniform1f(IORLoc, 1.330f);
    //环境光贡献率
    GLuint aoLoc = glGetUniformLocation(shaderProgram, "ao");
    glUniform1f(aoLoc, 1.5f);



    // 自发光
    GLuint emissionLoc = glGetUniformLocation(shaderProgram, "emission");
    glUniform3f(emissionLoc, .05f, .05f, .05f); // 传入自发光颜色

    // 基本色
    GLuint baseColorLoc = glGetUniformLocation(shaderProgram, "baseColor");
    glUniform3f(baseColorLoc, 0.9f, 0.9f, 0.9f); // 传入基本色（暗色）

}
void Game::GameBullet::SelfIns()
{

    // 生成随机数，范围 [0,1]
    float r0 = static_cast<float>(rand()) / RAND_MAX;
    float r1 = static_cast<float>(rand()) / RAND_MAX;
    float r2 = static_cast<float>(rand()) / RAND_MAX;

    // 随机生成波浪幅度 _waveAmplitude，范围 [0.1, 0.5]
    _waveAmplitude = 0.1f + r0 * 0.4f;

    // 随机生成波浪频率 _waveFrequency，范围 [1.0, 3.0]
    _waveFrequency = 1.0f + r1 * 2.0f;

    // 随机生成波浪速度 _waveSpeed，范围 [0.5, 2.0]
    _waveSpeed = 0.5f + r2 * 1.5f;
}
#pragma endregion

void Game::NoneLightModel::UniformParametersInput()
{
    //金属度
    GLuint metallicLoc = glGetUniformLocation(shaderProgram, "metallic");
    glUniform1f(metallicLoc, 0.0f);

    //糙度
    GLuint roughnessLoc = glGetUniformLocation(shaderProgram, "roughness");
    glUniform1f(roughnessLoc, 1.0f);
    //透明度
    GLuint opacityLoc = glGetUniformLocation(shaderProgram, "opacity");
    glUniform1f(opacityLoc, 1.0f);
    //折射率
    GLuint IORLoc = glGetUniformLocation(shaderProgram, " IOR");
    glUniform1f(IORLoc, 1.330f);
    //环境光贡献率
    GLuint aoLoc = glGetUniformLocation(shaderProgram, "ao");
    glUniform1f(aoLoc, 2.0f);


    // 自发光
    GLuint emissionLoc = glGetUniformLocation(shaderProgram, "emission");
    glUniform3f(emissionLoc, .05f, .05f, .05f); // 传入自发光颜色

    // 基本色
    GLuint baseColorLoc = glGetUniformLocation(shaderProgram, "baseColor");
    glUniform3f(baseColorLoc, 0.9f, 0.9f, 0.9f); // 传入基本色（暗色）

}
