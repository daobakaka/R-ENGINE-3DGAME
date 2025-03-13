#include <iostream>
#include "Monster.h"
#include <vector>
#include <GL/glew.h>//加载Opengl扩展，
#include <GLFW/glfw3.h>//创建Opengl扩展，以及管理Opengl上下文
#include "Cube.h"
#include "Controller.h"
#include "LifecycleManager.h"
#include "IntergratedScripts.h"
#include <list>  // 包含 list 容器
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

//控制组件标识
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


    //--全局执行区域阴影
//使用Controller内部全局着色器
    lightRender->RenderDepthMapForParallelLight(lightSpawner->GetParallelLight().direction,view,player,offset);//渲染深度缓冲图，用于阴影，内含绑定_paralleLightDepthMapFBO
    lightRender->BindFramebuffer();
    shaderManager->SetMat4("depthCal", "lightSpaceMatrix", lightRender->GetLightMatrix());
    manager->UpdateDepthPic(lightRender->GetLightMatrix(), shaderManager->GetShader("depthCal"));//获取light的光源矩阵，并为场景中的每个对象绘制进入_paralleLightDepthMapFBO,独立对象的绘制采用模板方法
    lightRender->UnbindFramebuffer();//绘制完成之后，解除绑定阴影渲染的深度贴图
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // 清除颜色缓冲和深度缓冲
    shaderManager->UseShader("depthVisual");
    lightRender->RenderShadowTexture(shaderManager->GetShader("depthVisual"));
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     //--控制平行光的旋转
  lightSpawner->ParalletLightController(glm::vec3(0, 0.01F, 0.0f));


}


void GameUpdateShadowRenderSpecial()
{


    manager->GetspecialObjects()["player"]->UpdateDepthPic(lightRender->GetLightMatrix(), shaderManager->GetShader("depthCal"));

}


/// <summary>
/// 渲染视口深度图，为获取深度进行后处理，这里针对非实例化对象
/// </summary>
/// <param name="view"></param>
/// <param name="projection"></param>
void ShderViewPortRenderingT(const glm::mat4& view, const glm::mat4& projection,CustomModel* player)
{
 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // 清除颜色缓冲和深度缓冲
    lightRender->BindDepthTestBuffer();
    shaderManager->SetMat4("depthViewPortCal", "view", view);
   // shaderManager->SetMat4("depthViewPortCal", "projection", lightRender->GetDepthMapCrossView(view,player,glm::vec3(0,0,100)));
    shaderManager->SetMat4("depthViewPortCal", "projection", projection);
    manager->UpdateDepthViewPortPic(view, projection, shaderManager->GetShader("depthViewPortCal"));
    lightRender->UnbindDepthTestBuffer();//绘制完成之后，解除绑定深度测试的视口贴图
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // 清除颜色缓冲和深度缓冲
    shaderManager->UseShader("depthLinerVisual");
    lightRender->RenderDopthTestTexture(shaderManager->GetShader("depthLinerVisual"));//计算视口的深度图

}


//平行光统一参数传入和阴影传入部分
void LightGlobalCalculate()
{

    //为渲染阴影设置阴影光源矩阵
    shaderManager->SetMat4("commonLight", "lightSpaceMatrix", lightRender->GetLightMatrix());
    shaderManager->SetMat4("commonNoneLight", "lightSpaceMatrix", lightRender->GetLightMatrix());
    shaderManager->SetMat4("waveShader", "lightSpaceMatrix", lightRender->GetLightMatrix());
    //激活阴影纹理单元,这里设置纹理单元9避免冲突
    shaderManager->SetTexture("commonLight", "autoParallelShadowMap", lightRender->GetDepthShaderProgram(ShaderClass::DepthMapParallel), 9);
    shaderManager->SetTexture("commonNoneLight", "autoParallelShadowMap", lightRender->GetDepthShaderProgram(ShaderClass::DepthMapParallel), 9);
    shaderManager->SetTexture("waveShader", "autoParallelShadowMap", lightRender->GetDepthShaderProgram(ShaderClass::DepthMapParallel), 9);
    //平行光强度渲染,平行光的参数是一致的
    shaderManager->SetVec3("commonLight", "parallelLightDirection", lightSpawner->GetParallelLight().direction);
    shaderManager->SetVec3("commonNoneLight", "parallelLightDirection", lightSpawner->GetParallelLight().direction);
    shaderManager->SetVec3("waveShader", "parallelLightDirection", lightSpawner->GetParallelLight().direction);
    //以上两个通用着色器，采用同样的参数传入
    shaderManager->SetVec3("commonLight", "parallelLightColor", lightSpawner->GetParallelLight().color);
    shaderManager->SetFloat("commonLight", "parallelLightIntensity", lightSpawner->GetParallelLight().intensity);
    shaderManager->SetVec3("waveShader", "parallelLightColor", lightSpawner->GetParallelLight().color);
    shaderManager->SetFloat("waveShader", "parallelLightIntensity", lightSpawner->GetParallelLight().intensity);
}

void GameUpdateMainLogicT(glm::mat4 view, glm::mat4 projection, GLFWwindow* window, CustomModel* player)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // 清除颜色缓冲和深度缓冲
    //执行光源参数的全局输入
    LightGlobalCalculate();
    //每帧清空可移除对象
    toActiveFalse.clear();
    toActiveTrue.clear();
    toDestory.clear();
    //2.使用综合脚本进行控制，场景类独立性综合性的方法,这个方法也可以通过变体种子int参数来执行不同的脚本
    //遍历执行池

    for (auto &item : manager->GetNativeObjects()) {
        //现在更改为使用构造化方式，统一使用
        if (item.second->GetVariant() == 0)
        {

            scripts->TChangeRandom(-0.01f, 0.01f);//改变构造随机种子
            // scripts->CubeUpdateFun(item); // 使用迭代器遍历链表并调用每个,暂时理解为一个遍历语法糖
        }
        else if (item.second->GetVariant() == ModelClass::BlackHoleE)
        {
            //脚本方法，执行综合方法
          scripts->TestUpdateFun(item.second);
         
        }
        else if (item.second->GetVariant() == ModelClass::ParallelLight)//平行光旋转，后面增加其他逻辑
        {

            scripts->TParallelLightRotation(item.second);

        }
        else if (item.second->GetVariant() == ModelClass::StoneMonser)
        {
           
            // 播放动画
            item.second->PlayAnimation(0, 0.1f);
        }
        else if (item.second->GetVariant() == ModelClass::TsetButterfly)
        {
            item.second->PlayAnimation(0, 0.1f);//播放简易顶点动画
            if (item.second->_timeAccumulator>=120)
            {
                toDestory.push_back(item.second);//测试蝴蝶超过120秒消失，不用重新加载的对象，放入销毁列表
            }
        }
        else if (item.second->GetVariant() == ModelClass::TestPhysics)
        {
            //设置生命值小于0时，物体消失，需要重复加载的对象，放入对象池以复用
            if(item.second->GetComponent<CollisionBody>()->GetCollisionProperties().gameProperties.health<=0)
            toActiveFalse .push_back(item.second);
        }
        else if (item.second->GetVariant() == ModelClass::InstanceSphere)
        {
        }
       
    }


    //遍历缓存池
    for (auto& item : manager->GetCacheObjects())
    {

        if (item.second->GetVariant() == ModelClass::TestPhysics)
        {
            toActiveTrue.push_back(item.second);
        }

    }

    // 临时存取失活对象
    for (auto* obj : toActiveFalse) {
        manager->SetActive(obj,false);
    }
    //临时存取激活对像
    for (auto* obj : toActiveTrue) {
        obj->position += glm::vec3(0, 50, 0);
        obj->GetComponent<PhysicalEngine>()->SetVelocity(glm::vec3(0));
        manager->SetActive(obj, true);
    }
    //临时销毁对象
    for (auto* obj : toDestory) {
        manager->DestroyObject(obj);
    }
#pragma region 恢复逻辑


#pragma endregion


}
/// <summary>
/// 执行测试的特殊结构
/// </summary>
/// <param name="view"></param>
/// <param name="projection"></param>
/// <param name="window"></param>
/// <param name="player"></param>
void GameUpdateBufferTestT(glm::mat4 view, glm::mat4 projection, GLFWwindow* window, CustomModel* player)
{

    //玩家综合性脚本操控
    scripts->PlayerControl(window, manager->GetspecialObjects()["player"]);
    //测试执行区域
    manager->GetspecialObjects()["treeInstance"]->Update(view, projection);
    //玩家模板测试
    glClear(GL_STENCIL_BUFFER_BIT);
    // 启用模板测试
    glEnable(GL_STENCIL_TEST);
    // 清除模板缓冲区
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    manager->GetspecialObjects()["player"]->Update(view, projection);
    manager->GetspecialObjects()["player"]->UpdateVariant(view, projection);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glDepthFunc(GL_GREATER); // 仅在当前片段深度值大于深度缓冲区中的值时通过测试
   // glBlendFunc(GL_ONE, GL_ONE);//
   // glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    shaderManager->SetMat4("stencilTestShader", "model", manager->GetspecialObjects()["player"]->transform);
    manager->GetspecialObjects()["player"]->SpecialMethod();
    glDepthFunc(GL_LESS); // 恢复默认深度测试函数
    glDisable(GL_STENCIL_TEST);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

