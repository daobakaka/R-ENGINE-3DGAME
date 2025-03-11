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
/// <summary>
/// ��Ⱦ�ӿ����ͼ��Ϊ��ȡ��Ƚ��к���������Է�ʵ��������
/// </summary>
/// <param name="view"></param>
/// <param name="projection"></param>

void ShderTestT(const glm::mat4& view, const glm::mat4& projection)
{
 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // �����ɫ�������Ȼ���
    lightRender->BindDepthTestBuffer();
    shaderManager->SetMat4("depthViewPortCal", "view", view);
    shaderManager->SetMat4("depthViewPortCal", "projection", projection);
    manager->UpdateDepthViewPortPic(view, projection, shaderManager->GetShader("depthViewPortCal"));
    lightRender->UnbindDepthTestBuffer();//�������֮�󣬽������Ȳ��Ե��ӿ���ͼ
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // �����ɫ�������Ȼ���
    shaderManager->UseShader("depthVisual");
    lightRender->RenderDopthTestTexture(shaderManager->GetShader("depthVisual"));//�����ӿڵ����ͼ



}


//ƽ�й�ͳһ�����������Ӱ���벿��
void LightGlobalCalculate()
{

    //Ϊ��Ⱦ��Ӱ������Ӱ��Դ����
    shaderManager->SetMat4("commonLight", "lightSpaceMatrix", lightRender->GetLightMatrix());
    shaderManager->SetMat4("commonNoneLight", "lightSpaceMatrix", lightRender->GetLightMatrix());
    shaderManager->SetMat4("waveShader", "lightSpaceMatrix", lightRender->GetLightMatrix());
    //������Ӱ����Ԫ,������������Ԫ9�����ͻ
    shaderManager->SetTexture("commonLight", "autoParallelShadowMap", lightRender->GetDepthShaderProgram(ShaderClass::DepthMapParallel), 9);
    shaderManager->SetTexture("commonNoneLight", "autoParallelShadowMap", lightRender->GetDepthShaderProgram(ShaderClass::DepthMapParallel), 9);
    shaderManager->SetTexture("waveShader", "autoParallelShadowMap", lightRender->GetDepthShaderProgram(ShaderClass::DepthMapParallel), 9);
    //ƽ�й�ǿ����Ⱦ,ƽ�й�Ĳ�����һ�µ�
    shaderManager->SetVec3("commonLight", "parallelLightDirection", lightSpawner->GetParallelLight().direction);
    shaderManager->SetVec3("commonNoneLight", "parallelLightDirection", lightSpawner->GetParallelLight().direction);
    shaderManager->SetVec3("waveShader", "parallelLightDirection", lightSpawner->GetParallelLight().direction);
    //��������ͨ����ɫ��������ͬ���Ĳ�������
    shaderManager->SetVec3("commonLight", "parallelLightColor", lightSpawner->GetParallelLight().color);
    shaderManager->SetFloat("commonLight", "parallelLightIntensity", lightSpawner->GetParallelLight().intensity);
    shaderManager->SetVec3("waveShader", "parallelLightColor", lightSpawner->GetParallelLight().color);
    shaderManager->SetFloat("waveShader", "parallelLightIntensity", lightSpawner->GetParallelLight().intensity);
}

void GameUpdateMainLogicT(glm::mat4 view, glm::mat4 projection, GLFWwindow* window, CustomModel* player)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // �����ɫ�������Ȼ���
    //ִ�й�Դ������ȫ������
    LightGlobalCalculate();
    //ÿ֡��տ��Ƴ�����
    toActiveFalse.clear();
    toActiveTrue.clear();
    toDestory.clear();
    //2.ʹ���ۺϽű����п��ƣ�������������ۺ��Եķ���,�������Ҳ����ͨ����������int������ִ�в�ͬ�Ľű�
    //����ִ�г�

    for (auto &item : manager->GetNativeObjects()) {
        //���ڸ���Ϊʹ�ù��컯��ʽ��ͳһʹ��
        if (item.second->GetVariant() == 0)
        {

            scripts->TChangeRandom(-0.01f, 0.01f);//�ı乹���������
            // scripts->CubeUpdateFun(item); // ʹ�õ�����������������ÿ��,��ʱ���Ϊһ�������﷨��
        }
        else if (item.second->GetVariant() == ModelClass::CubeTestE)
        {
            //�ű�������ִ���ۺϷ���
          scripts->TestUpdateFun(item.second);
            //  baseSphere->AttachTexture(TextureDic["butterfly"][0], 1);        
        }
        else if (item.second->GetVariant() == ModelClass::ParallelLight)//ƽ�й���ת���������������߼�
        {

            scripts->TParallelLightRotation(item.second);

        }
        else if (item.second->GetVariant() == ModelClass::StoneMonser)
        {
           
            // ���Ŷ���
            item.second->PlayAnimation(0, 0.1f);
        }
        else if (item.second->GetVariant() == ModelClass::TsetButterfly)
        {
            item.second->PlayAnimation(0, 0.1f);//���ż��׶��㶯��
            if (item.second->_timeAccumulator>=120)
            {
                toDestory.push_back(item.second);//���Ժ�������120����ʧ���������¼��صĶ��󣬷��������б�
            }
        }
        else if (item.second->GetVariant() == ModelClass::TestPhysics)
        {
            //��������ֵС��0ʱ��������ʧ����Ҫ�ظ����صĶ��󣬷��������Ը���
            if(item.second->GetComponent<CollisionBody>()->GetCollisionProperties().gameProperties.health<=0)
            toActiveFalse .push_back(item.second);
        }
        else if (item.second->GetVariant()==ModelClass::Player)
        { 
            //��Ұ��������ӵ�
            scripts->PlayerControl(window, item.second);
        }
       
    }


    //���������
    for (auto& item : manager->GetCacheObjects())
    {

        if (item.second->GetVariant() == ModelClass::TestPhysics)
        {
            toActiveTrue.push_back(item.second);
        }

    }


    // ��ʱ��ȡʧ�����
    for (auto* obj : toActiveFalse) {
        manager->SetActive(obj,false);
    }
    //��ʱ��ȡ�������
    for (auto* obj : toActiveTrue) {
        obj->position += glm::vec3(0, 50, 0);
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
void GameUpdateBufferTestT(glm::mat4 view, glm::mat4 projection, GLFWwindow* window, CustomModel* player)
{


    //����ִ������
    manager->GetspecialObjects()["treeInstance"]->Update(view, projection);



}

