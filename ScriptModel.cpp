#include "ScriptModel.h"
#include "ShaderManager.h"
#include "LifecycleManager.h"
#include "light.h"
using namespace Game;
#pragma region ���Ժ���
//extern LifecycleManager<CustomModel>* manager;//��ȡ������������


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
    glActiveTexture(GL_TEXTURE0 + _textureOrder);

    // 6) ������������ (texture) ���õ�Ԫ
    glBindTexture(GL_TEXTURE_2D, _textures[PictureTpye::BaseP]);

    // 7) ���� sampler uniform "texture1" ʹ�õ� textureOrder ������Ԫ
    glUniform1i(samplerLoc, _textureOrder);

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
    
    lightSpawner = LightSpawner::GetInstance();
    // ��ȡ��ɫ������
    shaderProgram = ShaderManager::GetInstance()->GetShader(name);
    //��������C++�в��ܲ��ô��ַ�ʽ���й��죬��Ϊ������ĳ�ʼ����δ�������
    SelfIns();

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
    UniformParametersInput();//������������ÿշ���
    RenderingLight();//ͨ��shader�������Ⱦ���������

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
    UniformParametersInput();//������������ÿշ���
    RenderingLight();//ͨ��shader�������Ⱦ���������
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
        glActiveTexture(GL_TEXTURE0 + _textureOrder);          // ��������Ԫ 0+order
        glBindTexture(GL_TEXTURE_2D, _textures[PictureTpye::BaseP]);  // �������������Ԫ 0+order,�������DicTexture���ϵ��������
        // ����������Ԫ 0+order�����˳�����������Ԫ��������һ��
        glUniform1i(picData, _textureOrder);


        RenderingTextureAdditional();
    }

}
void Game::CustomModelShader::RenderingTextureAdditional()
{
    int textureUnit = _textureOrder + 1; // �� textureOrder + 1 ��ʼ

    for (const auto& pair : _textures) {
        PictureTpye type = pair.first;
        GLuint textureID = pair.second;

        // �������������Ѿ��� RenderingTexture �м��أ�
        if (type == BaseP) {
            continue;
        }

        // �����������Ͱ󶨵���Ӧ������Ԫ
        switch (type) {
        case NormalP: {
            GLuint normalTextureLoc = glGetUniformLocation(shaderProgram, "normalTexture");
            glActiveTexture(GL_TEXTURE0 + textureUnit); // ��������Ԫ
            glBindTexture(GL_TEXTURE_2D, textureID); // �󶨷�������
            glUniform1i(normalTextureLoc, textureUnit); // ��������Ԫ
            break;
        }
        case SpecularP: {
            GLuint specularTextureLoc = glGetUniformLocation(shaderProgram, "specularTexture");
            glActiveTexture(GL_TEXTURE0 + textureUnit); // ��������Ԫ
            glBindTexture(GL_TEXTURE_2D, textureID); // �󶨸߹ⷴ������
            glUniform1i(specularTextureLoc, textureUnit); // ��������Ԫ
            break;
        }
        case HightP: {
            GLuint heightTextureLoc = glGetUniformLocation(shaderProgram, "heightTexture");
            glActiveTexture(GL_TEXTURE0 + textureUnit); // ��������Ԫ
            glBindTexture(GL_TEXTURE_2D, textureID); // �󶨸߶�����
            glUniform1i(heightTextureLoc, textureUnit); // ��������Ԫ
            break;
        }
        case RoughnessP: {
            GLuint roughnessTextureLoc = glGetUniformLocation(shaderProgram, "roughnessTexture");
            glActiveTexture(GL_TEXTURE0 + textureUnit); // ��������Ԫ
            glBindTexture(GL_TEXTURE_2D, textureID); // �󶨴ֲڶ�����
            glUniform1i(roughnessTextureLoc, textureUnit); // ��������Ԫ
            break;
        }
        case AOP: {
            GLuint aoTextureLoc = glGetUniformLocation(shaderProgram, "aoTexture");
            glActiveTexture(GL_TEXTURE0 + textureUnit); // ��������Ԫ
            glBindTexture(GL_TEXTURE_2D, textureID); // �� AO ����
            glUniform1i(aoTextureLoc, textureUnit); // ��������Ԫ
            break;
        }
        default:
            break;
        }

        textureUnit++; // ��������Ԫ
    }

}
void Game::CustomModelShader::RenderingLight()
{

    if (ifLight)
    {  // 2. ���Դ����
        const auto& pointLights = lightSpawner->GetPointLights();
        std::vector<std::pair<float, int>> pointLightDistances;  // �洢ÿ�����Դ������ľ���ƽ��������,�������Ľṹ
        glm::vec3 objectPosition = position;  // ʹ������ĵ�ǰλ����Ϊ�ο�

        // �������Դ�������ƽ��������ɸѡ������С�ڵ��� 1000 �Ĺ�Դ
        for (int i = 0; i < pointLights.size(); i++) {
            const auto& light = pointLights[i];
            float distanceSquared = glm::dot(objectPosition - light.position, objectPosition - light.position);
            if (distanceSquared <= 2000.0f) {  // ֻѡ�����ƽ��С�ڵ��� 100 �Ĺ�Դ
                pointLightDistances.push_back({ distanceSquared, i });
               // std::cout << i << "����" << distanceSquared << std::endl;
            }
        }

        // ���վ���ƽ����������
        std::sort(pointLightDistances.begin(), pointLightDistances.end(), [](const std::pair<float, int>& a, const std::pair<float, int>& b) {
            return a.first < b.first;  // ���վ���ƽ����������
            });
        //���ܶ�̬��������������shader���治֧�֣�����������д�������ǿ���ͨ��������Լ�� 
        GLuint numPointLightsLoc = glGetUniformLocation(shaderProgram, "numPointLights");
        glUniform1i(numPointLightsLoc, std::min(static_cast<int>(pointLightDistances.size()), 4));

        // ֻѡ������� 4 �����Դ
        for (int i = 0; i < std::min(4, static_cast<int>(pointLightDistances.size())); i++) {
            int index = pointLightDistances[i].second;  // ��ȡ����ĵ��Դ����������һ��ֵ�Ǿ��룬�ڶ���ֵ�Ǿ���
            auto light = pointLights[index];
            //  std::cout << "���Դǿ��" << light.intensity << std::endl;
              // �������ƽ�������ݵ� shader
            float distanceSquared = pointLightDistances[i].first;
            //  std::cout <<i<< "����" << distanceSquared << std::endl;
            std::string posName = "lightPos[" + std::to_string(i) + "]";
            std::string colorName = "lightColor[" + std::to_string(i) + "]";
            std::string intensityName = "lightIntensity[" + std::to_string(i) + "]";
            std::string distanceSquaredName = "lightDistanceSquared[" + std::to_string(i) + "]"; // ��������

            GLuint posLoc = glGetUniformLocation(shaderProgram, posName.c_str());
            GLuint colorLoc = glGetUniformLocation(shaderProgram, colorName.c_str());
            GLuint intensityLoc = glGetUniformLocation(shaderProgram, intensityName.c_str());
            GLuint distanceSquaredLoc = glGetUniformLocation(shaderProgram, distanceSquaredName.c_str()); // ��ȡ����ƽ��λ��

            glUniform3fv(posLoc, 1, glm::value_ptr(light.position));
            glUniform3fv(colorLoc, 1, glm::value_ptr(light.color));
            glUniform1f(intensityLoc, light.intensity);
            glUniform1f(distanceSquaredLoc, distanceSquared);  // ���ݾ���ƽ��
        }


        // 4. �ֵ�Ͳ���۹�ƣ�����
        auto flashLights = lightSpawner->GetFlashLights();
        std::vector<std::pair<float, int>> flashLightDistances;  // �洢ÿ���۹�Ƶ�����ľ���ƽ��������

        // �����۹�Ƽ������ƽ��������ɸѡ������С�ڵ��� 100 �ľ۹��
        for (int j = 0; j < flashLights.size(); j++) {
            const auto& flash = flashLights[j];
            float distanceSquared = glm::dot(objectPosition - flash.position, objectPosition - flash.position);
            if (distanceSquared <= 2000.0f) {  // ֻѡ�����С�ڵ��� 1000 �ľ۹��
                flashLightDistances.push_back({ distanceSquared, j });
            }
        }

        // ���վ���ƽ����������
        std::sort(flashLightDistances.begin(), flashLightDistances.end(), [](const std::pair<float, int>& a, const std::pair<float, int>& b) {
            return a.first < b.first;  // ���վ���ƽ����������
            });

        GLuint numFlashLightsLoc = glGetUniformLocation(shaderProgram, "numFlashLights");
        glUniform1i(numFlashLightsLoc, std::min(static_cast<int>(flashLightDistances.size()), 4));

        // ֻѡ������� 4 ���۹��
        for (int j = 0; j < std::min(4, static_cast<int>(flashLightDistances.size())); j++) {
            int index = flashLightDistances[j].second;  // ��ȡ����ľ۹�Ƶ�����
            auto flash = flashLights[index];

            // �������ƽ�������ݵ� shader
            float distanceSquared = flashLightDistances[j].first;

            std::string flashPosName = "flashLightPos[" + std::to_string(j) + "]";
            std::string flashDirName = "flashLightDirection[" + std::to_string(j) + "]";
            std::string flashColorName = "flashLightColor[" + std::to_string(j) + "]";
            std::string flashIntensityName = "flashLightIntensity[" + std::to_string(j) + "]";
            std::string flashCutoffName = "flashLightCutoff[" + std::to_string(j) + "]";
            std::string flashDistanceSquaredName = "flashLightDistanceSquared[" + std::to_string(j) + "]"; // ��������

            GLuint flashPosLoc = glGetUniformLocation(shaderProgram, flashPosName.c_str());
            GLuint flashDirLoc = glGetUniformLocation(shaderProgram, flashDirName.c_str());
            GLuint flashColorLoc = glGetUniformLocation(shaderProgram, flashColorName.c_str());
            GLuint flashIntensityLoc = glGetUniformLocation(shaderProgram, flashIntensityName.c_str());
            GLuint flashCutoffLoc = glGetUniformLocation(shaderProgram, flashCutoffName.c_str());
            GLuint flashDistanceSquaredLoc = glGetUniformLocation(shaderProgram, flashDistanceSquaredName.c_str()); // ��ȡ����ƽ��λ��

            glUniform3fv(flashPosLoc, 1, glm::value_ptr(flash.position));
            glUniform3fv(flashDirLoc, 1, glm::value_ptr(flash.direction));
            glUniform3fv(flashColorLoc, 1, glm::value_ptr(flash.color));
            glUniform1f(flashIntensityLoc, flash.intensity);
            glUniform1f(flashCutoffLoc, flash.cutoff);
            glUniform1f(flashDistanceSquaredLoc, distanceSquared);  // ���ݾ���ƽ��
        }

    }
}
void Game::CustomModelShader::SelfIns()
{
   
}
#pragma endregion
#pragma region ʵ����ģ��
CustomModelInstance::CustomModelInstance()
{
}

Game::CustomModelInstance::~CustomModelInstance()
{
    if (_lightRender!=nullptr)
    {
          //�������ɷ��ʣ�  
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

    // ��������ʵ��������ķ���
    GenerateInstanceMatrices(type);
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

void Game::CustomModelInstance::GenerateInstanceMatrices(ModelClass type)
{
    _modelMatrices.resize(_instanceCount);  // ����ʵ���������� vector ��С

    switch (type)
    {
    case Game::InstanceCube:
    {
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
        break;
    }

    case Game::InstanceCircle:
    {
        // Բ��ʵ����������Ч����
        float baseRadius = 20.0f; // �����뾶
        int layers = 100; // ���Ĳ���
        int remainingInstances = _instanceCount; // ʣ���ʵ������

        // ��������ƫ�Ʋ���
        float offsetX = 0.0f; // X ������ƫ��
        float offsetZ = 20.0f; // Z ������ƫ��

        for (int layer = 0; layer < layers; layer++) {
            // ���㵱ǰ��İ뾶
            float radius = baseRadius * (static_cast<float>(layer) / layers) * 100.0f;

            // ȷ�������뾶��Χ��û����
            if (radius < baseRadius) {
                continue; // ���������뾶��Χ�ڵĲ�
            }

            // ���㵱ǰ���ʵ������
            int instancesInLayer = (layer == layers - 1) ? remainingInstances : remainingInstances / (layers - layer);
            remainingInstances -= instancesInLayer;

            for (int i = 0; i < instancesInLayer; i++) {
                glm::mat4 model = glm::mat4(1.0f);

                // ����Բ���ϵ�λ��
                float angle = static_cast<float>(i) / instancesInLayer * 2.0f * glm::pi<float>(); // ��ǰʵ���ĽǶ�
                float x = radius * cos(angle); // X ����
                float z = radius * sin(angle); // Z ����

                // ������ƫ��
                float randomOffsetX = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * _positionOffset.x * 0.5f;
                float randomOffsetZ = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * _positionOffset.z * 0.5f;

                // ƽ�ƣ���ʵ��������Բ���ϣ���Ӧ�����ƫ�ƺ�����ƫ��
                model = glm::translate(model, glm::vec3(x + randomOffsetX + offsetX, 0.0f, z + randomOffsetZ + offsetZ));

                // ���������ţ����Ĵ�С�������
                float randomScale = 0.5f + static_cast<float>(rand()) / RAND_MAX * 1.5f; // ������ŷ�Χ��0.5 �� 2.0
                model = glm::scale(model, glm::vec3(randomScale));

                // ��ת��ʹʵ������Բ�����ģ���Ӧ�� _rotationAxis
                float rotationAngle = glm::degrees(angle) + 90.0f; // ��������
                model = glm::rotate(model, glm::radians(rotationAngle * _rotationAxis.y), glm::vec3(0.0f, 1.0f, 0.0f)); // �� Y ����ת
                model = glm::rotate(model, glm::radians(i * _rotationAxis.x), glm::vec3(1.0f, 0.0f, 0.0f)); // �� X ����ת
                model = glm::rotate(model, glm::radians(i * _rotationAxis.z), glm::vec3(0.0f, 0.0f, 1.0f)); // �� Z ����ת

                _modelMatrices.push_back(model);
            }
        }
        break;
    }

    case Game::InstanceSphere:
    {
        // ����ʵ����
        float radius = 5.0f; // ����뾶
        int rows = static_cast<int>(std::sqrt(_instanceCount)); // ���������
        int cols = rows; // ���������

        for (GLuint i = 0; i < _modelMatrices.size(); i++) {
            glm::mat4 model = glm::mat4(1.0f);

            // ���������ϵ�λ�ã����ڼ����꣩
            float phi = static_cast<float>(i / cols) / rows * glm::pi<float>(); // ���� (0 �� ��)
            float theta = static_cast<float>(i % cols) / cols * 2.0f * glm::pi<float>(); // ��λ�� (0 �� 2��)

            float x = radius * sin(phi) * cos(theta); // X ����
            float y = radius * cos(phi);             // Y ����
            float z = radius * sin(phi) * sin(theta); // Z ����

            // ƽ�ƣ���ʵ�������������ϣ���Ӧ�� _positionOffset
            model = glm::translate(model, glm::vec3(x * _positionOffset.x, y * _positionOffset.y, z * _positionOffset.z));

            // ��ת��ʹʵ�������������ģ���Ӧ�� _rotationAxis
            float rotationAngleX = glm::degrees(phi) * _rotationAxis.x; // �� X ����ת
            float rotationAngleY = glm::degrees(theta) * _rotationAxis.y; // �� Y ����ת
            float rotationAngleZ = i * _rotationAxis.z; // �� Z ����ת
            model = glm::rotate(model, glm::radians(rotationAngleX), glm::vec3(1.0f, 0.0f, 0.0f)); // �� X ����ת
            model = glm::rotate(model, glm::radians(rotationAngleY), glm::vec3(0.0f, 1.0f, 0.0f)); // �� Y ����ת
            model = glm::rotate(model, glm::radians(rotationAngleZ), glm::vec3(0.0f, 0.0f, 1.0f)); // �� Z ����ת

            _modelMatrices[i] = model;
        }
        break;
    }
    case Game::InstanceRound:
    {
        // ����Բ����������뾶
        float maxRadius = 300.0f; // �����������Բ������İ뾶
        // ȫ��ʵ������ _instanceCount ����
        for (int i = 0; i < _instanceCount; i++) {
            glm::mat4 model = glm::mat4(1.0f);

            // �������һ�����ȷֲ���Բ���ڵĵ�
            // Ϊ�˾��ȷֲ���ʹ�� sqrt(random) �õ��뾶 r
            float r = sqrt(static_cast<float>(rand()) / RAND_MAX) * maxRadius;
            float theta = static_cast<float>(rand()) / RAND_MAX * 2.0f * glm::pi<float>();
            float x = r * cos(theta);
            float z = r * sin(theta);

            // ������ڰ뾶�Ķ������ƫ��
            // �� r �ϴ󣨿�����Ե��ʱ��ƫ�Ʒ��ȸ���
            float factor = r / maxRadius; // ��Χ [0, 1]
            float randomOffsetX = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * _positionOffset.x * (1.0f + factor);
            float randomOffsetZ = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * _positionOffset.z * (1.0f + factor);

            // ƽ�ƣ���ʵ�����õ�Բ�����λ�ã���Ӧ�����ƫ��
            model = glm::translate(model, glm::vec3(x + randomOffsetX, 0.0f, z + randomOffsetZ));

            // ���������ţ��������Ĵ�С�������
            float randomScale = 0.5f + static_cast<float>(rand()) / RAND_MAX * 1.5f; // ���ŷ�Χ��0.5 �� 2.0
            model = glm::scale(model, glm::vec3(randomScale));

            // ��ת��ʹʵ��������Բ�ĵķ����ټ��϶���������ת
            float baseRotation = glm::degrees(theta) + 90.0f; // ʹ������
            model = glm::rotate(model, glm::radians(baseRotation * _rotationAxis.y), glm::vec3(0.0f, 1.0f, 0.0f));
            // ����Ҳ��������� X/Z ��Ķ�����ת
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

    //�շ��� ʵ�������岻���ڼ���
}

void Game::CustomModelInstance::EnableDepthcal()
{

    _useViewPortDepthMap = true;
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
        glActiveTexture(GL_TEXTURE0 + _textureOrder); // ��������Ԫ 0+order
        glBindTexture(GL_TEXTURE_2D, _textures[PictureTpye::BaseP]);      // �������������Ԫ 0+order
        glUniform1i(picData, _textureOrder);         // ������Ԫ�󶨵���ɫ���ġ�baseTexture�� uniform
               
    }
    //����ͨ��enable��������,ʹ���ӿ����ͼ���д���
    if (_useViewPortDepthMap)
    {
        //�����������
        GLuint picDataDepth = glGetUniformLocation(shaderProgram, "depthTexture");
        glActiveTexture(GL_TEXTURE0 + _textureOrder + 1); // ��������Ԫ 0+order
        glBindTexture(GL_TEXTURE_2D, _lightRender->GetDepthShaderProgram(ShaderClass::DepthMapTest));      // �������������Ԫ 0+order
        glUniform1i(picDataDepth, _textureOrder + 1);         // ������Ԫ�󶨵���ɫ���ġ�baseTexture�� uniform

    }

}

#pragma endregion
#pragma region ���������¶���ģ��

void ButterflyScriptShader::SelfIns()
{
    // �������������Χ [0,1]
    float r0 = static_cast<float>(rand()) / RAND_MAX;
    float r1 = static_cast<float>(rand()) / RAND_MAX;
    float r2 = static_cast<float>(rand()) / RAND_MAX;
    float r3 = static_cast<float>(rand()) / RAND_MAX;
    float r4 = static_cast<float>(rand()) / RAND_MAX;
    float r5 = static_cast<float>(rand()) / RAND_MAX;
    float r6 = static_cast<float>(rand()) / RAND_MAX;
    _dt = 0.0167f;
    // �������ˮƽԲ���˶�������
    // _circleRadius �� [5,15]
    _circleRadius = 5.0f + r0 * 10.0f;
    // _horizontalSpeed �� [3,6]
    float sign = (rand() % 2 == 0) ? 1.0f : -1.0f;//��һ���������ֵ
    _horizontalSpeed = sign * (3.0f + r1 * 3.0f);
    // ��ʼ�Ƕ��� [0, 2��)
    _xzStartAngle = r2 * 2.0f * glm::pi<float>();

    // ������ɴ�ֱ����������
    // _verticalSpeed �� [0.5, 2.0]
    _verticalSpeed = 0.5f + r3 * 1.5f;
    _baseY = 0.0f; // ��׼ Y ����

    // ��������񵴲�����
    // �񵴿�ʼʱ���� [10, 20] ��
    _oscStartTime = 10.0f + r4 * 10.0f;
    // ������� [0.5, 2.0]
    _oscillationAmplitude = 0.5f + r5 * 1.5f;
    // ��Ƶ���� [1, 3] ����/��
    _oscillationFrequency = 1.0f + r6 * 2.0f;

    _timeAccumulator = 0.0f;

    // ��ʼ��λ�ã����ݳ�ʼ�Ƕ�ȷ���� XZ ƽ��ĳ�ʼλ�ã�Y ȡ��׼ֵ
    position = glm::vec3(_circleRadius * cos(_xzStartAngle),
        _baseY,
        _circleRadius * sin(_xzStartAngle));
    // ��ʼ����תΪĬ�ϣ��ٶ�Ĭ�ϳ���Ϊ�� Z �ᣩ
    rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    std::cout << "��ʼ����������" << std::endl;
}

void ButterflyScriptShader::UpdateVariant(glm::mat4 view, glm::mat4 projection)
{
  
    _timeAccumulator += _dt;

    // Ϊ����ˮƽ���ٶȹ̶���
    // ˮƽ���ٶ� = _horizontalSpeed (��λ/��)
    // Բ���˶��Ľ��ٶ� = _horizontalSpeed / _circleRadius
    float angle = _xzStartAngle + (_horizontalSpeed / _circleRadius) * _timeAccumulator;
    angle = fmod(angle, 2.0f * glm::pi<float>()); // ��֤�� [0,2��) ��

    // ����λ�ã�
    // XZ ƽ�棺��Բ���˶�
    float x = _circleRadius * cos(angle);
    float z = _circleRadius * sin(angle);
    // Y �᣺�������� + �񵴣���ʱ�䳬���񵴿�ʼʱ���
    float y = _baseY + _verticalSpeed * _timeAccumulator;
    if (_timeAccumulator > _oscStartTime)
    {
        float oscTime = _timeAccumulator - _oscStartTime;
        y += _oscillationAmplitude * sin(_oscillationFrequency * oscTime);
    }
    position = glm::vec3(x, y, z);

    // ������ת��ʹ����ʼ������ˮƽ�˶������߷��򣨽��� Y ����ת��
    // ����Բ�ܹ켣 P(angle) = (R*cos(angle), y, R*sin(angle))
    // ˮƽ���� T = (-sin(angle), 0, cos(angle))
    glm::vec3 tangent = glm::vec3(-sin(angle), 0.0f, cos(angle));
    tangent = glm::normalize(tangent);

    // ����Ŀ�� yaw �ǣ�ʹ�� atan2 �õ�ˮƽ���߷���ĽǶ�
    float desiredYaw = atan2(tangent.x, tangent.z);

    // ���ڵ�ǰ����õ��ķ���ʵ�����������෴���������峯��Բ�ģ���
    // ���� �� ������ʹ�䳯�����߷���
    desiredYaw -= glm::pi<float>()/2;

    // ������� Y ����ת����Ԫ��
    rotation = glm::angleAxis(desiredYaw, glm::vec3(0.0f, 1.0f, 0.0f));
}

#pragma endregion

#pragma region ���ģ��

void GamePlayer::UpdateVariant(glm::mat4 view, glm::mat4 projection)
{




    


}
void Game::GamePlayer::Start()
{

    //���ù�����ʼ����������ʼ�����̿���������
    _manager= LifecycleManager<CustomModel>::GetInstance();
    //��ȡ���������
    _controller = Controller::GetInstance();
}

void Game::GamePlayer::UniformParametersInput()
{
    
 
    
    //������
    GLuint metallicLoc = glGetUniformLocation(shaderProgram, "metallic");
    glUniform1f(metallicLoc,0.9f);

    //�ڶ�
    GLuint roughnessLoc = glGetUniformLocation(shaderProgram, "roughness");
    glUniform1f(roughnessLoc, .5f);
    //͸����
    GLuint opacityLoc = glGetUniformLocation(shaderProgram, "opacity");
    glUniform1f(opacityLoc, 1.0f);
    //������
    GLuint IORLoc = glGetUniformLocation(shaderProgram, " IOR");
    glUniform1f(IORLoc, 1.330f);
    //�����⹱����
    GLuint aoLoc = glGetUniformLocation(shaderProgram, "ao");
    glUniform1f(aoLoc, 1.5f);


    // �Է���
    GLuint emissionLoc = glGetUniformLocation(shaderProgram, "emission");
    glUniform3f(emissionLoc, .05f, .05f, .05f); // �����Է�����ɫ

    // ����ɫ
    GLuint baseColorLoc = glGetUniformLocation(shaderProgram, "baseColor");
    glUniform3f(baseColorLoc, 0.9f, 0.9f, 0.9f); // �������ɫ����ɫ��
}



#pragma endregion
#pragma region ��Ϸ���߰��


void Game::GameBullet::UpdateVariant(glm::mat4 view, glm::mat4 projection)
{
    //�����ӵ�10 �ٶ�
   // this->GetComponent<PhysicalEngine>()->SetVelocity(glm::vec3(0, 0, 10));
    _waveTime += 0.0167f;

}
void Game::GameBullet::UniformParametersInput()
{

    // ��ȡ uniform ������λ��
    GLuint timeLoc = glGetUniformLocation(shaderProgram, "waveTime");
    GLuint waveAmplitudeLoc = glGetUniformLocation(shaderProgram, "waveAmplitude");
    GLuint waveFrequencyLoc = glGetUniformLocation(shaderProgram, "waveFrequency");
    GLuint waveSpeedLoc = glGetUniformLocation(shaderProgram, "waveSpeed");

    glUniform1f(timeLoc, _waveTime);
    glUniform1f(waveAmplitudeLoc, _waveAmplitude);
    glUniform1f(waveFrequencyLoc, _waveFrequency);
    glUniform1f(waveSpeedLoc, _waveSpeed);


    //������
    GLuint metallicLoc = glGetUniformLocation(shaderProgram, "metallic");
    glUniform1f(metallicLoc, 0.3f);

    //�ڶ�
    GLuint roughnessLoc = glGetUniformLocation(shaderProgram, "roughness");
    glUniform1f(roughnessLoc, .1f);
    //͸����
    GLuint opacityLoc = glGetUniformLocation(shaderProgram, "opacity");
    glUniform1f(opacityLoc, 0.4f);
    //������
    GLuint IORLoc = glGetUniformLocation(shaderProgram, " IOR");
    glUniform1f(IORLoc, 1.330f);
    //�����⹱����
    GLuint aoLoc = glGetUniformLocation(shaderProgram, "ao");
    glUniform1f(aoLoc, 1.5f);



    // �Է���
    GLuint emissionLoc = glGetUniformLocation(shaderProgram, "emission");
    glUniform3f(emissionLoc, .05f, .05f, .05f); // �����Է�����ɫ

    // ����ɫ
    GLuint baseColorLoc = glGetUniformLocation(shaderProgram, "baseColor");
    glUniform3f(baseColorLoc, 0.9f, 0.9f, 0.9f); // �������ɫ����ɫ��

}
void Game::GameBullet::SelfIns()
{

    // �������������Χ [0,1]
    float r0 = static_cast<float>(rand()) / RAND_MAX;
    float r1 = static_cast<float>(rand()) / RAND_MAX;
    float r2 = static_cast<float>(rand()) / RAND_MAX;

    // ������ɲ��˷��� _waveAmplitude����Χ [0.1, 0.5]
    _waveAmplitude = 0.1f + r0 * 0.4f;

    // ������ɲ���Ƶ�� _waveFrequency����Χ [1.0, 3.0]
    _waveFrequency = 1.0f + r1 * 2.0f;

    // ������ɲ����ٶ� _waveSpeed����Χ [0.5, 2.0]
    _waveSpeed = 0.5f + r2 * 1.5f;
}
#pragma endregion

void Game::NoneLightModel::UniformParametersInput()
{
    //������
    GLuint metallicLoc = glGetUniformLocation(shaderProgram, "metallic");
    glUniform1f(metallicLoc, 0.0f);

    //�ڶ�
    GLuint roughnessLoc = glGetUniformLocation(shaderProgram, "roughness");
    glUniform1f(roughnessLoc, 1.0f);
    //͸����
    GLuint opacityLoc = glGetUniformLocation(shaderProgram, "opacity");
    glUniform1f(opacityLoc, 1.0f);
    //������
    GLuint IORLoc = glGetUniformLocation(shaderProgram, " IOR");
    glUniform1f(IORLoc, 1.330f);
    //�����⹱����
    GLuint aoLoc = glGetUniformLocation(shaderProgram, "ao");
    glUniform1f(aoLoc, 2.0f);


    // �Է���
    GLuint emissionLoc = glGetUniformLocation(shaderProgram, "emission");
    glUniform3f(emissionLoc, .05f, .05f, .05f); // �����Է�����ɫ

    // ����ɫ
    GLuint baseColorLoc = glGetUniformLocation(shaderProgram, "baseColor");
    glUniform3f(baseColorLoc, 0.9f, 0.9f, 0.9f); // �������ɫ����ɫ��

}
