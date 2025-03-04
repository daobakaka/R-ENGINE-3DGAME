#include <iostream>
#include "Monster.h"
#include <vector>
#include <GL/glew.h>//����Opengl��չ��
#include <GLFW/glfw3.h>//����Opengl��չ���Լ�����Opengl������
#include "Cube.h"
//#include "shader.h"  // ����һ����������ɫ���ļ�����ֻ�ܱ�����һ��
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

std::vector<CustomModel*> toRemove;
void GameUpdateShadowRenderT()
{


    //--ȫ��ִ��������Ӱ
//ʹ��Controller�ڲ�ȫ����ɫ��
    lightRender->RenderDepthMapForParallelLight(lightSpawner->GetParallelLight().direction);//��Ⱦ��Ȼ���ͼ��������Ӱ���ں���_paralleLightDepthMapFBO
    lightRender->BindFramebuffer();
    shaderManager->SetMat4("depthCal", "lightSpaceMatrix", lightRender->GetLightMatrix());
    manager->UpdateDepthPic(lightRender->GetLightMatrix(), shaderManager->GetShader("depthCal"));//��ȡlight�Ĺ�Դ���󣬲�Ϊ�����е�ÿ��������ƽ���_paralleLightDepthMapFBO,��������Ļ��Ʋ���ģ�巽��
    lightRender->UnbindFramebuffer();//�������֮�󣬽������Ӱ��Ⱦ�������ͼ
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // �����ɫ�������Ȼ���
    shaderManager->UseShader("depthVisual");
    lightRender->RenderShadowTexture(shaderManager->GetShader("depthVisual"));
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     //--����ƽ�й����ת
  //  lightSpawner->ParalletLightController(glm::vec3(0, 1, 0.0f));


}



void GameUpdateMainLogicT(glm::mat4 view,glm::mat4 projection)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // �����ɫ�������Ȼ���
    //ÿ֡��տ��Ƴ�����
     toRemove.clear();
    //2.ʹ���ۺϽű����п��ƣ�������������ۺ��Եķ���,�������Ҳ����ͨ����������int������ִ�в�ͬ�Ľű�
    //����ִ������
    for (auto &item : manager->GetNativeObjects()) {
        //�����Դ����Ч����װ�� �ƹ���Ⱦ����.����ǹ���shader������Ҫ������һ�δ��룬���������Ⱦ�������������Ҫ
        //���ڸ���Ϊʹ�ù��컯��ʽ��ͳһʹ��
       //--�Ƿ����ģ���ж�
        if (item.second->ifLight)
        {
            //����ȿ���ʹ�ö���shader Ҳ����ʹ�ù���shader����ʹ��ʲô��ȥ����
            lightRender->RenderLights(item.second->shaderProgram, controller, lightSpawner, item.second->position);//������Ⱦ�߼������ٷ��ʴ���
        }

        if (item.second->GetVariant() == 0)
        {

            scripts->TChangeRandom(-0.01f, 0.01f);//�ı乹���������
            // scripts->CubeUpdateFun(item); // ʹ�õ�����������������ÿ��,��ʱ���Ϊһ�������﷨��
        }
        else if (item.second->GetVariant() == ModelClass::CubeTestE)
        {
          scripts->TestUpdateFun(item.second);
            //  baseSphere->AttachTexture(TextureDic["butterfly"][0], 1);        
        }
        else if (item.second->GetVariant() == ModelClass::ParallelLight)//ƽ�й���ת���������������߼�
        {

            scripts->TParallelLightRotation(item.second);

        }
        else if (item.second->GetVariant() == ModelClass::ActorButterfly)
        {
            //Ŀǰ����ͳһ�ű��ķ�ʽ����������ȫ�����ڶ���ű���ֻ����һ���̶��Ͻ��ж���
            scripts->ActorButtfly(item.second);
            item.second->PlayAnimation(0, 0.1f);


        }
        else if (item.second->GetVariant() == ModelClass::TsetButterfly)
        {

            //����Ҳ����ִ�б��巽������������֤
         //   item->UpdateVariant(view, projection);
            item.second->PlayAnimation(0, 0.1f);

        }
        else if (item.second->GetVariant() == ModelClass::TestPhysics)
        {
           // scripts->TPosition(item);
          //  item.second->DestroySelf();
            if(item.second->position.y<-10)
            toRemove.push_back(item.second);
            //���ù����������Ƴ��������б�ָ�룬���������
           // manager->RemoveObject(item.second);
        }
    }
    // �������������Ƴ�
    for (auto* obj : toRemove) {
        manager->SetActive(obj,false);
    }
    //3.���巽�����������ֶ�׫д�ű���ʵ�֣����ڳ�ʼ��ʱ��ע��ű�����,ͨ�ýű�������һ�������ԣ����巽����������ʹ��
    //���弴��ģ������ע�᲻Ϊ0����Ҫ���ڳ����е���Ҫ����������ر任�Ķ���
    for (auto& item : manager->GetVariantObjects()) {
        // item->UpdateVariant(view, projection);
    }

}


