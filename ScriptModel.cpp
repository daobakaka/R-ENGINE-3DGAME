#include "ScriptModel.h"
#include "ShaderManager.h"
using namespace Game;
#pragma region ���Ժ���
void ButterflyScript::UpdateVariant(glm::mat4 view, glm::mat4 projection)
{

    //����Ϳ���д���������֡�任����

    position += glm::vec3(0.0f, 0.01f, 0.00f);

}
#pragma endregion
#pragma region �ο�����
void CoordinateSystemCus::Update(glm::mat4 view, glm::mat4 projection)
{
    //UpdateTransform(); ������Բ��ø���λ����Ϣ�ˣ� �ο�����ϵ����Ҫ�ƶ� 
    DrawLineFixedWidget(view, projection);
}
#pragma endregion
#pragma region ��Դģ��

void LightModel::SteLightParameters(glm::vec3 color, float intensity, glm::vec3 dirction)
{
    myColor = color;
    myIntensity = intensity;
    myDirection = dirction;
}

void LightModel::RenderingTexture()
{
    // 1) ʹ�õ�ǰ LightModel ����ɫ��
    glUseProgram(shaderProgram);

    // 2) ���� baseColor
    GLint baseColorLoc = glGetUniformLocation(shaderProgram, "baseColor");
    glUniform3fv(baseColorLoc, 1, glm::value_ptr(myColor));
    // 3) ���� lightIntensity
    GLint intensityLoc = glGetUniformLocation(shaderProgram, "lightIntensity");
    glUniform1f(intensityLoc, myIntensity);
    //3.1)���� lightDirection
    GLuint dirtionLoc = glGetUniformLocation(shaderProgram, "lightDirction");
    glUniform3fv(dirtionLoc, 1, glm::value_ptr(myDirection));

    // 4) �ҵ� sampler uniform "texture1"
    GLint samplerLoc = glGetUniformLocation(shaderProgram, "texture1");


    // 5) ��������Ԫ (GL_TEXTURE0 + textureOrder)
    glActiveTexture(GL_TEXTURE0 + textureOrder);

    // 6) ������������ (texture) ���õ�Ԫ
    glBindTexture(GL_TEXTURE_2D, texture);

    // 7) ���� sampler uniform "texture1" ʹ�õ� textureOrder ������Ԫ
    glUniform1i(samplerLoc, textureOrder);

}
#pragma endregion
#pragma region �Զ�������

void CustomizeRay::Update(glm::mat4 view, glm::mat4 projection)
{
    UpdateTransform();
    DrawLine(view,projection);
}

bool CustomizeRay::DrawLine(glm::mat4 view, glm::mat4 projection)
{
    
    // ���ǰ����ɫ������
    glUseProgram(shaderProgram);


    // 2) ����������ɫ,�����Ƕ�drawline�ĸ�������������д
    GLint baseColorLoc = glGetUniformLocation(shaderProgram, "baseColor");
    glUniform3fv(baseColorLoc, 1, glm::value_ptr(myColor));

    // ��ȡ shader �� "model"��"view" �� "projection" uniform ������λ��
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");

    // ����任���� shader������������Ϊ GL_FALSE ��ʾ������Ҫת�ã�
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // �󶨵�ǰģ�͵� VAO
    glBindVertexArray(VAO);

    // ֱ��ʹ�� GL_LINES ģʽ�����߶�
    glDrawElements(GL_LINES, index, GL_UNSIGNED_INT, 0);

    // ��� VAO
    glBindVertexArray(0);


    return true;
}

void CustomizeRay::SetRayPar(glm::vec3 color,float intensity)
{
    myColor = color;
    myIntensity = intensity;
}

#pragma endregion
#pragma region  ������Ӱ��ͼ
void ShadowTexture::DrawDepthPic(glm::mat4 lightSpaceMatrix, GLuint shader)
{
  
    //����ģ���������꣬��ͼ��͸�Ӿ�����lightRender������
    GLuint modelLoc = glGetUniformLocation(shader, "model");
    glUniformMatrix4fv(modelLoc, 1, 0, glm::value_ptr(transform));


    glBindVertexArray(VAO);
    justDrawVerteies == true ? glDrawArrays(GL_TRIANGLES, 0, index) : glDrawElements(GL_TRIANGLES, index, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);


}
void ShadowTexture::DrawDepthPicDynamical(glm::mat4 lightSpaceMatrix, GLuint shader)
{



    //����ģ���������꣬��ͼ��͸�Ӿ�����lightRender������
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
#pragma region ͨ��shaderģ�� 

CustomModelShader::CustomModelShader(const std::string& name, const ModelData& modelData, bool isSkinnedMesh, bool ifLightIn, bool ifShadow)
{
   
    vertexCount = modelData.verticesStruct.size();
    index = modelData.indices.size();
    verticesTras = modelData.verticesStruct;
    IsSkinnedMesh = isSkinnedMesh;
    ifLight = ifLightIn;
    _ifShadow = ifShadow;
    
    // ��ȡ��ɫ������
    shaderProgram = ShaderManager::GetInstance()->GetShader(name);

    // ���� VAO, VBO, EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // ��VAO
    glBindVertexArray(VAO);

    // �����Ƿ�Ϊ��̬������������������񣩾���ʹ�ö�̬��̬������
    GLuint bufferUsage = isSkinnedMesh ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;

    // VBO: �������ж���
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
        modelData.verticesStruct.size() * sizeof(Vertex),
        modelData.verticesStruct.data(),
        bufferUsage);  // �����Ƿ�Ϊ��̬����ѡ�񻺳�������

    // EBO: ��������
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        modelData.indices.size() * sizeof(unsigned int),
        modelData.indices.data(),
        bufferUsage);  // ͬ�������Ƿ�Ϊ��̬����ѡ�񻺳�������

    // ���ö�������ָ��
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

    // ���VAO
    glBindVertexArray(0);
}



CustomModelShader::CustomModelShader()
{
}

bool CustomModelShader::Draw(glm::mat4 view, glm::mat4 projection)
{
    //����������Ⱦ����,Ҫ��draw֮ǰ���ã�������Ч
    RenderingTexture();

    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, 0, glm::value_ptr(transform));
    glBindVertexArray(VAO);
    justDrawVerteies == true ? glDrawArrays(GL_TRIANGLES, 0, index) : glDrawElements(GL_TRIANGLES, index, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    //����������Ⱦ����
  // RenderingTexture();
    return true;
}

bool Game::CustomModelShader::DrawDynamical(glm::mat4 view, glm::mat4 projection)
{

    //����������Ⱦ����,Ҫ��draw֮ǰ���ã�������Ч
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
    //����������õ�shader�����ظ�����
    glUseProgram(shaderProgram);
    if (_drawTexture)
    {

        // ����������������
        GLuint textureScaleLoc = glGetUniformLocation(shaderProgram, "textureScale");
        glUniform2f(textureScaleLoc, _textureScale.x, _textureScale.y); // ����������������

        GLuint picData = glGetUniformLocation(shaderProgram, "baseTexture");//Ԥд��ͼ���shader��������
        glActiveTexture(GL_TEXTURE0 + textureOrder);          // ��������Ԫ 0+order
        glBindTexture(GL_TEXTURE_2D, texture);  // �������������Ԫ 0+order,�������DicTexture���ϵ��������
        // ����������Ԫ 0+order�����˳�����������Ԫ��������һ��
        glUniform1i(picData, textureOrder);
    }

}
#pragma endregion
#pragma region ʵ����ģ��
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

    // ��������ʵ��������ķ���
    GenerateInstanceMatrices();
    // ���� VAO, VBO, EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &_instanceBuffer);  // ʵ�����������������洢ÿ��ʵ���ı任����

    // �� VAO
    glBindVertexArray(VAO);

    // VBO: �������ж���
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, modelData.verticesStruct.size() * sizeof(Vertex), modelData.verticesStruct.data(), useageBuffer);

    // EBO: ��������
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, modelData.indices.size() * sizeof(unsigned int), modelData.indices.data(), useageBuffer);

    // ���ö�������ָ��
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    

    // ��ʵ�����ݴ��ݸ� GPU
    glBindBuffer(GL_ARRAY_BUFFER, _instanceBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * _modelMatrices.size(), _modelMatrices.data(), useageBuffer);

    // Ϊÿ��ʵ����������������ָ�루ÿһ�о������ݴ��룩
    for (GLuint i = 0; i < 4; i++) {
        glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * i));
        glEnableVertexAttribArray(3 + i);
        glVertexAttribDivisor(3 + i, 1);  // ���� OpenGL ÿ��ʵ���������Ƕ�����
    }

    // ��� VAO
    glBindVertexArray(0);
}

void Game::CustomModelInstance::GenerateInstanceMatrices()

{
    _modelMatrices.resize(_instanceCount);  // ����ʵ���������� vector ��С

   int gridSize = std::cbrt(_instanceCount); // ����������ı߳�
    for (GLuint i = 0; i < _modelMatrices.size(); i++) {
        glm::mat4 model = glm::mat4(1.0f);

        // ����ƽ�ƣ���ʵ���� X, Y, Z ��ȼ��ֲ�
        float xOffset = (i % gridSize) * _positionOffset.x;  // ��X��ƽ��
        float yOffset = ((i / gridSize) % gridSize) * _positionOffset.y;  // ��Y��ƽ��
        float zOffset = (i / (gridSize * gridSize)) * _positionOffset.z;  // ��Z��ƽ��

        // ʹ�ü������ƽ��������ƽ��
        model = glm::translate(model, glm::vec3(xOffset, yOffset, zOffset));  // �γɹ���������

        // ��ת����ָ��������ת����ת������ rotationAxis ����
        model = glm::rotate(model, glm::radians(i * _rotationAxis.x), glm::vec3(1.0f, 0.0f, 0.0f));  // �� X ����ת
        model = glm::rotate(model, glm::radians(i * _rotationAxis.y), glm::vec3(0.0f, 1.0f, 0.0f));  // �� Y ����ת
        model = glm::rotate(model, glm::radians(i * _rotationAxis.z), glm::vec3(0.0f, 0.0f, 1.0f));  // �� Z ����ת

        _modelMatrices[i] = model;


    }
}


// ��Ҫ����ԭ�еķ���ǩ��
bool Game::CustomModelInstance::Draw(glm::mat4 view, glm::mat4 projection)
{
    // ֱ�ӵ�����Ⱦ����������Ⱦ��������
    RenderingTexture();

    // ��VAO
    glBindVertexArray(VAO);

    GLuint modelLoc = glGetUniformLocation(shaderProgram, "transform");

    // ������Դ���任 ����ʵ��
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));

    // ʹ��ʵ��������
    justDrawVerteies == true
        ? glDrawArraysInstanced(GL_TRIANGLES, 0, index, _modelMatrices.size())  // ʹ�� glDrawArraysInstanced
        : glDrawElementsInstanced(GL_TRIANGLES, index, GL_UNSIGNED_INT, 0, _modelMatrices.size());  // ʹ�� glDrawElementsInstanced

    // ���VAO
    glBindVertexArray(0);

    return true;
}

// DrawDynamical ����
bool Game::CustomModelInstance::DrawDynamical(glm::mat4 view, glm::mat4 projection)
{
    // ֱ�ӵ�����Ⱦ����������Ⱦ��������
    RenderingTexture();

    // ��ȡģ�;����uniformλ��
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");

    // ����ģ�;���
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));

    // ��VAO
    glBindVertexArray(VAO);

    // ���¶������ݣ�����ж�̬���ݣ�
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesTras.size() * sizeof(Vertex), verticesTras.data(), GL_DYNAMIC_DRAW);

    // ��ʵ�������ݵ�VBO������У�
    glBindBuffer(GL_ARRAY_BUFFER, _instanceBuffer); // _instanceBuffer ��ʵ�������ݵĻ�����
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * _modelMatrices.size(), _modelMatrices.data(), GL_DYNAMIC_DRAW);

    // Ϊÿ��ʵ����������������ָ�루ÿһ�о������ݴ��룩
    for (GLuint i = 0; i < 4; i++) {
        glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * i));
        glEnableVertexAttribArray(3 + i);
        glVertexAttribDivisor(3 + i, 1);  // ÿ��ʵ���������Ƕ�����
    }

    // ʹ��ʵ��������
    justDrawVerteies == true
        ? glDrawArraysInstanced(GL_TRIANGLES, 0, index, _modelMatrices.size())  // ʹ�� glDrawArraysInstanced
        : glDrawElementsInstanced(GL_TRIANGLES, index, GL_UNSIGNED_INT, 0, _modelMatrices.size());  // ʹ�� glDrawElementsInstanced

    // ���VAO
    glBindVertexArray(0);

    return true;
}

// RenderingTexture����
void Game::CustomModelInstance::RenderingTexture()
{
    // ����������õ�shader�����ظ�����
    glUseProgram(shaderProgram);

    if (_drawTexture)
    {
        // ����������������
        GLuint textureScaleLoc = glGetUniformLocation(shaderProgram, "textureScale");
        glUniform2f(textureScaleLoc, _textureScale.x, _textureScale.y); // ����������������

        GLuint picData = glGetUniformLocation(shaderProgram, "baseTexture");
        glActiveTexture(GL_TEXTURE0 + textureOrder); // ��������Ԫ 0+order
        glBindTexture(GL_TEXTURE_2D, texture);      // �������������Ԫ 0+order
        glUniform1i(picData, textureOrder);         // ������Ԫ�󶨵���ɫ���ġ�baseTexture�� uniform
    }
}

#pragma endregion
#pragma region ���������¶���ģ��

void ButterflyScriptShader::UpdateVariant(glm::mat4 view, glm::mat4 projection)
{
    position += glm::vec3(0.0f, 0.01f, 0.00f);
}

#pragma endregion