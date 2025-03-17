#include <iostream>
#include "Monster.h"
#include <vector>
#include <GL/glew.h>//����Opengl��չ��
#include <GLFW/glfw3.h>//����Opengl��չ���Լ�����Opengl������
#include "Cube.h"
#include "Controller.h"
#include "LifecycleManager.h"
#include "IntergratedScripts.h"
#include <list>  // ���� list ����
#include "MeshDataManager.h"
#include "CustomModel.h"
#include "Light.h"
#include "FileLoadIO.h"
#include "TextRender.h"
#include "CoroutineMethod.h"
#include "ScriptModel.h"
#include <chrono>
#include <thread>
#include "ShaderManager.h"
using namespace Game;

//���������ʶ
extern Controller* controller;
extern LifecycleManager<CustomModel>* manager;
extern IntergratedScripts* scripts;
extern MeshDataManager* meshData;
extern TextRender* cusText;
extern CoroutineMethod* coroutine;
extern LightSpawner* lightSpawner;
extern LightRender* lightRender;
extern ShaderManager* shaderManager;

std::vector<CustomModel*> toActiveFalse;
std::vector<CustomModel*> toActiveTrue;
std::vector<CustomModel*> toDestory;
void GameUpdateShadowRenderT(const glm::mat4 &view,CustomModel* player,glm::vec3 offset)
{


    //--ȫ��ִ��������Ӱ
//ʹ��Controller�ڲ�ȫ����ɫ��
    lightRender->RenderDepthMapForParallelLight(lightSpawner->GetParallelLight().direction,view,player,offset);//��Ⱦ��Ȼ���ͼ��������Ӱ���ں���_paralleLightDepthMapFBO
    lightRender->BindFramebuffer();
    shaderManager->SetMat4("depthCal", "lightSpaceMatrix", lightRender->GetLightMatrix());
    manager->UpdateDepthPic(lightRender->GetLightMatrix(), shaderManager->GetShader("depthCal"));//��ȡlight�Ĺ�Դ���󣬲�Ϊ�����е�ÿ��������ƽ���_paralleLightDepthMapFBO,��������Ļ��Ʋ���ģ�巽��
    lightRender->UnbindFramebuffer();//�������֮�󣬽������Ӱ��Ⱦ�������ͼ
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // �����ɫ�������Ȼ���
    shaderManager->UseShader("depthVisual");
    lightRender->RenderShadowTexture(shaderManager->GetShader("depthVisual"));
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     //--����ƽ�й����ת
  lightSpawner->ParalletLightController(glm::vec3(0, 0.01F, 0.0f));


}
void GameUpdateShadowRenderSpecial()
{

    manager->GetSpecialObjects()["player"]->UpdateDepthPic(lightRender->GetLightMatrix(), shaderManager->GetShader("depthCal"));
}
/// <summary>
/// ��Ⱦ�ӿ����ͼ��Ϊ��ȡ��Ƚ��к���������Է�ʵ��������
/// </summary>
/// <param name="view"></param>
/// <param name="projection"></param>
void ShderViewPortRenderingT(const glm::mat4& view, const glm::mat4& projection,CustomModel* player)
{
 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // �����ɫ�������Ȼ���
    lightRender->BindDepthTestBuffer();
    shaderManager->SetMat4("depthViewPortCal", "view", view);
    shaderManager->SetMat4("depthViewPortCal", "projection", projection);
    manager->UpdateDepthViewPortPic(view, projection, shaderManager->GetShader("depthViewPortCal"));
    lightRender->UnbindDepthTestBuffer();//�������֮�󣬽������Ȳ��Ե��ӿ���ͼ
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // �����ɫ�������Ȼ���
    shaderManager->UseShader("depthLinerVisual");
    lightRender->RenderDepthTestTexture(shaderManager->GetShader("depthLinerVisual"));//�����ӿڵ����ͼ


}
//ȫ�ֺ���
void PostProcessingT(const glm::mat4& view, const glm::mat4& projection, GLFWwindow* window, CustomModel* player)
{
    lightRender->UbindPostProcessingBuffer();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);  // �����ɫ�������Ȼ����ģ�建��
    shaderManager->UseShader("postProcessingBloom");
    if (player->GetComponent<CollisionBody>()->GetCollisionProperties().gameProperties.underAttack == true)
    {
        player->timeAccumulator += 0.0167f;
        shaderManager->SetBool("postProcessingBloom", "attacked", true);

    }
    if (player->timeAccumulator > 0.5f)
    {
       player-> GetComponent<CollisionBody>()->GetCollisionProperties().gameProperties.underAttack = false;
        shaderManager->SetBool("postProcessingBloom", "attacked", false);
        player->timeAccumulator = 0;
    }
    if (player->GetComponent<CollisionBody>()->GetCollisionProperties().gameProperties.health <= 0)
    {
        auto& timer = player->GetComponent<CollisionBody>()->GetCollisionProperties().gameProperties.timer;
        timer += 0.0167f*0.5F;
        player->GetComponent<CollisionBody>()->GetCollisionProperties().gameProperties.death = true;
        player->GetComponent<CollisionBody>()->GetCollisionProperties().isActive = false; //�����ײ��������
        shaderManager->SetBool("postProcessingBloom", "death", true);//����shader��ҵ�����Ч��
        if (timer>0.9f)
        {
            timer = 0.9f;
        }
        shaderManager->SetFloat("postProcessingBloom", "deathProgress", timer);//����shader��ҵ�����Ч��
    }
    lightRender->RenderPostProcessingTexture(shaderManager->GetShader("postProcessingBloom"));
    
}
/// <summary>
/// �ڲ���������,���ڸ��õĵĶ���shader����ʹ�ô˷�������������������дAttachTexture�շ���
/// </summary>
/// <param name="shaderProgram"></param>
/// <param name="textures"></param>
/// <param name="textureOrder"></param>
void RenderingTextureGlobal(GLuint shaderProgram, const std::unordered_map<PictureTpye, GLuint>& textures, glm::vec2 textureScale = glm::vec2(1, 1), int textureOrder = 0)
{
    // ����������������
    GLuint textureScaleLoc = glGetUniformLocation(shaderProgram, "textureScale");
    if (textureScaleLoc != -1)
    {
        glUniform2f(textureScaleLoc, textureScale.x, textureScale.y);
    }

    int textureUnit = textureOrder; // ��ָ��������Ԫ��ʼ
    for (const auto& pair : textures) {
        PictureTpye type = pair.first;
        GLuint textureID = pair.second;

        switch (type) {
        case BaseP: {
            GLuint baseTextureLoc = glGetUniformLocation(shaderProgram, "baseTexture");
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            glBindTexture(GL_TEXTURE_2D, textureID);
            glUniform1i(baseTextureLoc, textureUnit);
            break;
        }
        case NormalP: {
            GLuint normalTextureLoc = glGetUniformLocation(shaderProgram, "normalTexture");
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            glBindTexture(GL_TEXTURE_2D, textureID);
            glUniform1i(normalTextureLoc, textureUnit);
            break;
        }
        case SpecularP: {
            GLuint specularTextureLoc = glGetUniformLocation(shaderProgram, "specularTexture");
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            glBindTexture(GL_TEXTURE_2D, textureID);
            glUniform1i(specularTextureLoc, textureUnit);
            break;
        }
        case HightP: { 
            GLuint heightTextureLoc = glGetUniformLocation(shaderProgram, "heightTexture");
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            glBindTexture(GL_TEXTURE_2D, textureID);
            glUniform1i(heightTextureLoc, textureUnit);
            break;
        }
        case RoughnessP: {
            GLuint roughnessTextureLoc = glGetUniformLocation(shaderProgram, "roughnessTexture");
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            glBindTexture(GL_TEXTURE_2D, textureID);
            glUniform1i(roughnessTextureLoc, textureUnit);
            break;
        }
        case AOP: {
            GLuint aoTextureLoc = glGetUniformLocation(shaderProgram, "aoTexture");
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            glBindTexture(GL_TEXTURE_2D, textureID);
            glUniform1i(aoTextureLoc, textureUnit);
            break;
        }
        case OpacityP: {
            // �޸ı�����Ϊ opacityTextureLoc ������
            GLuint opacityTextureLoc = glGetUniformLocation(shaderProgram, "opacityTexture");
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            glBindTexture(GL_TEXTURE_2D, textureID);
            glUniform1i(opacityTextureLoc, textureUnit);
            break;
        }
        case SpecularCubeP: {
            GLuint specularCubeTextureLoc = glGetUniformLocation(shaderProgram, "specularCubeTexture");
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
            glUniform1i(specularCubeTextureLoc, textureUnit);
            break;
        }
        default:
            break;
        }
        textureUnit++;
    }
}

/// <summary>
/// �ڲ��������������ڸ��õĶ���shader����ʹ�ô˷�������
/// </summary>
/// <param name="shaderProgram"></param>
/// <param name="baseColor"></param>
/// <param name="emission"></param>
/// <param name="metallic"></param>
/// <param name="roughness"></param>
/// <param name="opacity"></param>
/// <param name="IOR"></param>
/// <param name="ao"></param>
/// <param name="dissolveThreshold"></param>
void PassUniformParametersGlobal(GLuint shaderProgram, glm::vec3 baseColor, glm::vec3 emission, float metallic, float roughness, float opacity, float IOR, float ao, float dissolveThreshold = 0)
{

    //������
    GLuint metallicLoc = glGetUniformLocation(shaderProgram, "metallic");
    glUniform1f(metallicLoc, metallic);

    //�ڶ�
    GLuint roughnessLoc = glGetUniformLocation(shaderProgram, "roughness");
    glUniform1f(roughnessLoc, roughness);
    //͸����
    GLuint opacityLoc = glGetUniformLocation(shaderProgram, "opacity");
    glUniform1f(opacityLoc, opacity);
    //�������������ʣ������������ʲ���Ӱ��
    GLuint IORLoc = glGetUniformLocation(shaderProgram, "IOR");
    glUniform1f(IORLoc, IOR);
    //�����⹱����
    GLuint aoLoc = glGetUniformLocation(shaderProgram, "ao");
    glUniform1f(aoLoc, ao);

    // �Է���
    GLuint emissionLoc = glGetUniformLocation(shaderProgram, "emission");
    glUniform3f(emissionLoc, emission.x,emission.y,emission.z); // �����Է�����ɫ

    // ����ɫ
    GLuint baseColorLoc = glGetUniformLocation(shaderProgram, "baseColor");
    glUniform3f(baseColorLoc, baseColor.x,baseColor.y,baseColor.z); // �������ɫ����ɫ��


}
//�ӿھ���������벿��
void StaticShaderGlobalParametersPassingT(const glm::mat4& view, const glm::mat4& projection)
{
    //������Բ�ȡUBOģʽ�����ݾ�����Ⱦ��Ϊ���� 
    //Ϊͨ��shader ���빲����ͼ��͸�Ӿ���,��������˷���������ʱֱ�Ӵ������ֵ������shader�����ڿ��ۺ�
    shaderManager->SetMat4("commonLight", "view", view);
    shaderManager->SetMat4("commonLight", "projection", projection);
    //����ʵ����ͨ����ɫ��
    shaderManager->SetMat4("noneLightInstancer", "view", view);
    shaderManager->SetMat4("noneLightInstancer", "projection", projection);
    //������ʵ���������ӿ���ȼ�����ɫ��
    shaderManager->SetMat4("noneLightDepthCalInstancer", "view", view);
    shaderManager->SetMat4("noneLightDepthCalInstancer", "projection", projection);
    //�����޹��ջ�����ɫ��(һ�Ż�������)
    shaderManager->SetMat4("noneLight", "view", view);
    shaderManager->SetMat4("noneLight", "projection", projection);
    //�����޹���ͨ����ɫ��
    shaderManager->SetMat4("commonNoneLight", "view", view);
    shaderManager->SetMat4("commonNoneLight", "projection", projection);
    //���벨��ͨ�ù�����ɫ��(�����������ݵķ��е���)
    shaderManager->SetMat4("waveShader", "view", view);
    shaderManager->SetMat4("waveShader", "projection", projection);
    //�������ģ�������ɫ��
    shaderManager->SetMat4("stencilTestShader", "view", view);
    shaderManager->SetMat4("stencilTestShader", "projection", projection);
    //����ʵ����ӫ����ɫ��
    shaderManager->SetMat4("fireflyInstanceShader", "view", view);
    shaderManager->SetMat4("fireflyInstanceShader", "projection", projection);
    //����ذ������ɫ��
    shaderManager->SetMat4("planeCommonLight", "view", view);
    shaderManager->SetMat4("planeCommonLight", "projection", projection);
    //������ҹ�����ɫ��
    shaderManager->SetMat4("playerCommonLight", "view", view);
    shaderManager->SetMat4("playerCommonLight", "projection", projection);
    //����ڶ��޹�����ɫ��
    shaderManager->SetMat4("blackHoleCommonNoneLight", "view", view);
    shaderManager->SetMat4("blackHoleCommonNoneLight", "projection", projection);
    //����ʯͷ�ֹ�����ɫ��
    shaderManager->SetMat4("stoneMonsterCommonLight", "view", view);
    shaderManager->SetMat4("stoneMonsterCommonLight", "projection", projection);
    //����ʯͷ������ɫ��
    shaderManager->SetMat4("stoneCommonLight", "view", view);
    shaderManager->SetMat4("stoneCommonLight", "projection", projection);
    //�������������ɫ��
    shaderManager->SetMat4("butterflyCommonLight", "view", view);
    shaderManager->SetMat4("butterflyCommonLight", "projection", projection);
    //���뱦�������ɫ��
    shaderManager->SetMat4("chestCommonLight", "view", view);
    shaderManager->SetMat4("chestCommonLight", "projection", projection);
    //������������ɫ��
    shaderManager->SetMat4("treeCommonLight", "view", view);
    shaderManager->SetMat4("treeCommonLight", "projection", projection);
    //���뱦���޹�����ɫ��
    shaderManager->SetMat4("chestCommonNoneLight", "view", view);
    shaderManager->SetMat4("chestCommonNoneLight", "projection", projection);
    //����ʵ��������ɫ��
    shaderManager->SetMat4("grassNoneLightInstancer", "view", view);
    shaderManager->SetMat4("grassNoneLightInstancer", "projection", projection);
    //����ʵ����ʯ��ɫ��
    shaderManager->SetMat4("stoneNoneLightInstancer", "view", view);
    shaderManager->SetMat4("stoneNoneLightInstancer", "projection", projection);
}

//ƽ�й�ͳһ�����������Ӱ���벿��
void LightShaderGlobalParamerersCalculate()
{
    //Ϊ��Ⱦ��Ӱ������Ӱ��Դ����
    shaderManager->SetMat4("commonLight", "lightSpaceMatrix", lightRender->GetLightMatrix());
    shaderManager->SetMat4("commonNoneLight", "lightSpaceMatrix", lightRender->GetLightMatrix());
    shaderManager->SetMat4("waveShader", "lightSpaceMatrix", lightRender->GetLightMatrix());
    shaderManager->SetMat4("planeCommonLight", "lightSpaceMatrix", lightRender->GetLightMatrix());
    shaderManager->SetMat4("playerCommonLight", "lightSpaceMatrix", lightRender->GetLightMatrix());
    shaderManager->SetMat4("stoneMonsterCommonLight", "lightSpaceMatrix", lightRender->GetLightMatrix());
    shaderManager->SetMat4("stoneCommonLight", "lightSpaceMatrix", lightRender->GetLightMatrix());
    shaderManager->SetMat4("butterflyCommonLight", "lightSpaceMatrix", lightRender->GetLightMatrix());
    shaderManager->SetMat4("chestCommonLight", "lightSpaceMatrix", lightRender->GetLightMatrix());
    shaderManager->SetMat4("treeCommonLight", "lightSpaceMatrix", lightRender->GetLightMatrix());
    shaderManager->SetMat4("chestCommonNoneLight", "lightSpaceMatrix", lightRender->GetLightMatrix());
    shaderManager->SetMat4("blackHoleCommonNoneLight", "lightSpaceMatrix", lightRender->GetLightMatrix());
    //������Ӱ����Ԫ,������������Ԫ10�����ͻ
    shaderManager->SetTexture("commonLight", "autoParallelShadowMap", lightRender->GetDepthShaderProgram(ShaderClass::DepthMapParallel), 10);
    shaderManager->SetTexture("commonNoneLight", "autoParallelShadowMap", lightRender->GetDepthShaderProgram(ShaderClass::DepthMapParallel), 10);
    shaderManager->SetTexture("waveShader", "autoParallelShadowMap", lightRender->GetDepthShaderProgram(ShaderClass::DepthMapParallel), 10);
    shaderManager->SetTexture("planeCommonLight", "autoParallelShadowMap", lightRender->GetDepthShaderProgram(ShaderClass::DepthMapParallel), 10);
    shaderManager->SetTexture("playerCommonLight", "autoParallelShadowMap", lightRender->GetDepthShaderProgram(ShaderClass::DepthMapParallel), 10);
    shaderManager->SetTexture("stoneMonsterCommonLight", "autoParallelShadowMap", lightRender->GetDepthShaderProgram(ShaderClass::DepthMapParallel), 10);
    shaderManager->SetTexture("stoneCommonLight", "autoParallelShadowMap", lightRender->GetDepthShaderProgram(ShaderClass::DepthMapParallel), 10);
    shaderManager->SetTexture("butterflyCommonLight", "autoParallelShadowMap", lightRender->GetDepthShaderProgram(ShaderClass::DepthMapParallel), 10);
    shaderManager->SetTexture("chestCommonLight", "autoParallelShadowMap", lightRender->GetDepthShaderProgram(ShaderClass::DepthMapParallel), 10);
    shaderManager->SetTexture("treeCommonLight", "autoParallelShadowMap", lightRender->GetDepthShaderProgram(ShaderClass::DepthMapParallel), 10);
    shaderManager->SetTexture("chestCommonNoneLight", "autoParallelShadowMap", lightRender->GetDepthShaderProgram(ShaderClass::DepthMapParallel), 10);
    shaderManager->SetTexture("blackHoleCommonNoneLight", "autoParallelShadowMap", lightRender->GetDepthShaderProgram(ShaderClass::DepthMapParallel), 10);
    //ƽ�й�ǿ����Ⱦ,ƽ�й�Ĳ�����һ�µ�
    shaderManager->SetVec3("commonLight", "parallelLightDirection", lightSpawner->GetParallelLight().direction);
    shaderManager->SetVec3("commonNoneLight", "parallelLightDirection", lightSpawner->GetParallelLight().direction);
    shaderManager->SetVec3("waveShader", "parallelLightDirection", lightSpawner->GetParallelLight().direction);
    shaderManager->SetVec3("planeCommonLight", "parallelLightDirection", lightSpawner->GetParallelLight().direction);
    shaderManager->SetVec3("playerCommonLight", "parallelLightDirection", lightSpawner->GetParallelLight().direction);
    shaderManager->SetVec3("stoneMonsterCommonLight", "parallelLightDirection", lightSpawner->GetParallelLight().direction);
    shaderManager->SetVec3("stoneCommonLight", "parallelLightDirection", lightSpawner->GetParallelLight().direction);
    shaderManager->SetVec3("butterflyCommonLight", "parallelLightDirection", lightSpawner->GetParallelLight().direction);
    shaderManager->SetVec3("chestCommonLight", "parallelLightDirection", lightSpawner->GetParallelLight().direction);
    shaderManager->SetVec3("treeCommonLight", "parallelLightDirection", lightSpawner->GetParallelLight().direction);
    shaderManager->SetVec3("chestCommonNoneLight", "parallelLightDirection", lightSpawner->GetParallelLight().direction);
    shaderManager->SetVec3("blackHoleCommonNoneLight", "parallelLightDirection", lightSpawner->GetParallelLight().direction);
    //��������ͨ����ɫ��������ͬ���Ĳ�������
    shaderManager->SetVec3("commonLight", "parallelLightColor", lightSpawner->GetParallelLight().color);
    shaderManager->SetFloat("commonLight", "parallelLightIntensity", lightSpawner->GetParallelLight().intensity);
    shaderManager->SetVec3("waveShader", "parallelLightColor", lightSpawner->GetParallelLight().color);
    shaderManager->SetFloat("waveShader", "parallelLightIntensity", lightSpawner->GetParallelLight().intensity);
    shaderManager->SetVec3("planeCommonLight", "parallelLightColor", lightSpawner->GetParallelLight().color);
    shaderManager->SetFloat("planeCommonLight", "parallelLightIntensity", lightSpawner->GetParallelLight().intensity);
    shaderManager->SetVec3("playerCommonLight", "parallelLightColor", lightSpawner->GetParallelLight().color);
    shaderManager->SetFloat("playerCommonLight", "parallelLightIntensity", lightSpawner->GetParallelLight().intensity);
    shaderManager->SetVec3("stoneMonsterCommonLight", "parallelLightColor", lightSpawner->GetParallelLight().color);
    shaderManager->SetFloat("stoneMonsterCommonLight", "parallelLightIntensity", lightSpawner->GetParallelLight().intensity);
    shaderManager->SetVec3("stoneCommonLight", "parallelLightColor", lightSpawner->GetParallelLight().color);
    shaderManager->SetFloat("stoneCommonLight", "parallelLightIntensity", lightSpawner->GetParallelLight().intensity);
    shaderManager->SetVec3("butterflyCommonLight", "parallelLightColor", lightSpawner->GetParallelLight().color);
    shaderManager->SetFloat("butterflyCommonLight", "parallelLightIntensity", lightSpawner->GetParallelLight().intensity);
    shaderManager->SetVec3("chestCommonLight", "parallelLightColor", lightSpawner->GetParallelLight().color);
    shaderManager->SetFloat("chestCommonLight", "parallelLightIntensity", lightSpawner->GetParallelLight().intensity);
    shaderManager->SetVec3("treeCommonLight", "parallelLightColor", lightSpawner->GetParallelLight().color);
    shaderManager->SetFloat("treeCommonLight", "parallelLightIntensity", lightSpawner->GetParallelLight().intensity);
}

void GameUpdateMainLogicT(const glm::mat4& view, const glm::mat4& projection, GLFWwindow* window, CustomModel* player)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);  // �����ɫ�������Ȼ����ģ�建��
    //ִ�й�Դ������ȫ������
    LightShaderGlobalParamerersCalculate();
    //ÿ֡��տ��Ƴ�����
    toActiveFalse.clear();
    toActiveTrue.clear();
    toDestory.clear();
    //2.ʹ���ۺϽű����п��ƣ�������������ۺ��Եķ���,�������Ҳ����ͨ����������int������ִ�в�ͬ�Ľű�
    //����ִ�г�
    for (auto& item : manager->GetNativeObjects()) {
        switch (item.second->GetVariant()) {

        case ModelClass::BlackHoleE: // ��Ӧ else if (item.second->GetVariant() == ModelClass::BlackHoleE)
            scripts->TestUpdateFun(item.second); // �ű�������ִ���ۺϷ���
            break;

        case ModelClass::ParallelLight: // ��Ӧ else if (item.second->GetVariant() == ModelClass::ParallelLight)
            scripts->TParallelLightRotation(item.second); // ƽ�й���ת
            break;

        case ModelClass::StoneMonser: // ��Ӧ else if (item.second->GetVariant() == ModelClass::StoneMonser)
            if (item.second->GetComponent<CollisionBody>()->GetCollisionProperties().gameProperties.death == true) {
                toActiveFalse.push_back(item.second); // �����������
            }
            break;

        case ModelClass::TsetButterfly: // ��Ӧ else if (item.second->GetVariant() == ModelClass::TsetButterfly)
            item.second->PlayAnimation(0, 0.1f); // ���ż��׶��㶯��
            if (item.second->timeAccumulator >= 120) {
                toDestory.push_back(item.second); // ���Ժ�������120����ʧ
            }
            break;

        case ModelClass::TestPhysics: // ��Ӧ else if (item.second->GetVariant() == ModelClass::TestPhysics)
            if (item.second->GetComponent<CollisionBody>()->GetCollisionProperties().gameProperties.health <= 0) {
                toActiveFalse.push_back(item.second); // ��������ֵС��0ʱ��������ʧ
            }
            break;

        case ModelClass::PlayerBullet: // ��Ӧ else if (item.second->GetVariant() == ModelClass::PlayerBullet)
            if (item.second->timeAccumulator >= 30) {
                toDestory.push_back(item.second); // ����ӵ�����30������
            }
            break;

        default: 
            break;
        }
    }

    //���������
    for (auto& item : manager->GetCacheObjects())
    {
        //������Ӽ�������
        if (false)
        {
            if (item.second->GetVariant() == ModelClass::TestPhysics)
            {
                toActiveTrue.push_back(item.second);
            }
        }
    }
    // ��ʱ��ȡʧ�����
    for (auto* obj : toActiveFalse) {
        manager->SetActive(obj,false);
    }
    //��ʱ��ȡ�������
    for (auto* obj : toActiveTrue) {
        obj->position += glm::vec3(0, 10, 0);
        obj->GetComponent<PhysicalEngine>()->SetVelocity(glm::vec3(0));
        manager->SetActive(obj, true);
    }
    //��ʱ���ٶ���
    for (auto* obj : toDestory) {
        manager->DestroyObject(obj);
    }
#pragma region �ָ��߼�


#pragma endregion


}
/// <summary>
/// ִ�в��Ե�����ṹ
/// </summary>
/// <param name="view"></param>
/// <param name="projection"></param>
/// <param name="window"></param>
/// <param name="player"></param>
void GameUpdateBufferTestT(const glm::mat4& view, const glm::mat4& projection, GLFWwindow* window, CustomModel* player)
{

    //����ۺ��Խű��ٿ�
    if (!player->GetComponent<CollisionBody>()->GetCollisionProperties().gameProperties.death)
    {
        scripts->PlayerControl(window, manager->GetSpecialObjects()["player"]);
    }
    //ʵ������ľ�����ӿ����ͼ����͸���ȶ������
    manager->GetSpecialObjects()["treeInstance"]->Update(view, projection);
    //���ģ�����
    glClear(GL_STENCIL_BUFFER_BIT);
    // ����ģ�����
    glEnable(GL_STENCIL_TEST);
    // ���ģ�建����
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    manager->GetSpecialObjects()["player"]->Update(view, projection);
    manager->GetSpecialObjects()["player"]->UpdateVariant(view, projection);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glDepthFunc(GL_GREATER); // ���ڵ�ǰƬ�����ֵ������Ȼ������е�ֵʱͨ������
   // glBlendFunc(GL_ONE, GL_ONE);//
   // glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    shaderManager->SetMat4("stencilTestShader", "model", manager->GetSpecialObjects()["player"]->transform);
    //���������ִ����ģ����Ե����ⷽ��
    manager->GetSpecialObjects()["player"]->SpecialMethod();
    glDepthFunc(GL_LESS); // �ָ�Ĭ����Ȳ��Ժ���
    glDisable(GL_STENCIL_TEST);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

