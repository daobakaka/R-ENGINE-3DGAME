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

    manager->GetSpecialObjects()["player"]->UpdateDepthPic(lightRender->GetLightMatrix(), shaderManager->GetShader("depthCal"));
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
    shaderManager->SetMat4("depthViewPortCal", "projection", projection);
    manager->UpdateDepthViewPortPic(view, projection, shaderManager->GetShader("depthViewPortCal"));
    lightRender->UnbindDepthTestBuffer();//绘制完成之后，解除绑定深度测试的视口贴图
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // 清除颜色缓冲和深度缓冲
    shaderManager->UseShader("depthLinerVisual");
    lightRender->RenderDepthTestTexture(shaderManager->GetShader("depthLinerVisual"));//计算视口的深度图


}
//全局后处理
void PostProcessingT(const glm::mat4& view, const glm::mat4& projection, GLFWwindow* window, CustomModel* player)
{
    lightRender->UbindPostProcessingBuffer();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);  // 清除颜色缓冲和深度缓冲和模板缓冲
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
        player->GetComponent<CollisionBody>()->GetCollisionProperties().isActive = false; //解除碰撞及物理检测
        shaderManager->SetBool("postProcessingBloom", "death", true);//设置shader玩家的死亡效果
        if (timer>0.9f)
        {
            timer = 0.9f;
        }
        shaderManager->SetFloat("postProcessingBloom", "deathProgress", timer);//设置shader玩家的死亡效果
    }
    lightRender->RenderPostProcessingTexture(shaderManager->GetShader("postProcessingBloom"));
    
}
/// <summary>
/// 内部辅助函数,对于复用的的独立shader可以使用此方法传纹理，需在子类重写AttachTexture空方法
/// </summary>
/// <param name="shaderProgram"></param>
/// <param name="textures"></param>
/// <param name="textureOrder"></param>
void RenderingTextureGlobal(GLuint shaderProgram, const std::unordered_map<PictureTpye, GLuint>& textures, glm::vec2 textureScale = glm::vec2(1, 1), int textureOrder = 0)
{
    // 传入纹理缩放因子
    GLuint textureScaleLoc = glGetUniformLocation(shaderProgram, "textureScale");
    if (textureScaleLoc != -1)
    {
        glUniform2f(textureScaleLoc, textureScale.x, textureScale.y);
    }

    int textureUnit = textureOrder; // 从指定的纹理单元开始
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
            // 修改变量名为 opacityTextureLoc 更清晰
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
/// 内部辅助函数，对于复用的独立shader可以使用此方法传参
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

    //金属度
    GLuint metallicLoc = glGetUniformLocation(shaderProgram, "metallic");
    glUniform1f(metallicLoc, metallic);

    //糙度
    GLuint roughnessLoc = glGetUniformLocation(shaderProgram, "roughness");
    glUniform1f(roughnessLoc, roughness);
    //透明度
    GLuint opacityLoc = glGetUniformLocation(shaderProgram, "opacity");
    glUniform1f(opacityLoc, opacity);
    //基础材料折射率，纯金属折射率不受影响
    GLuint IORLoc = glGetUniformLocation(shaderProgram, "IOR");
    glUniform1f(IORLoc, IOR);
    //环境光贡献率
    GLuint aoLoc = glGetUniformLocation(shaderProgram, "ao");
    glUniform1f(aoLoc, ao);

    // 自发光
    GLuint emissionLoc = glGetUniformLocation(shaderProgram, "emission");
    glUniform3f(emissionLoc, emission.x,emission.y,emission.z); // 传入自发光颜色

    // 基本色
    GLuint baseColorLoc = glGetUniformLocation(shaderProgram, "baseColor");
    glUniform3f(baseColorLoc, baseColor.x,baseColor.y,baseColor.z); // 传入基本色（暗色）


}
//视口矩阵参数传入部分
void StaticShaderGlobalParametersPassingT(const glm::mat4& view, const glm::mat4& projection)
{
    //这里可以采取UBO模式，根据具体渲染行为而定 
    //为通用shader 传入共用视图及透视矩阵,这里改善了方法，设置时直接传入名字调用相关shader，后期可综合
    shaderManager->SetMat4("commonLight", "view", view);
    shaderManager->SetMat4("commonLight", "projection", projection);
    //传入实例化通用着色器
    shaderManager->SetMat4("noneLightInstancer", "view", view);
    shaderManager->SetMat4("noneLightInstancer", "projection", projection);
    //传入树实例化运用视口深度计算着色器
    shaderManager->SetMat4("noneLightDepthCalInstancer", "view", view);
    shaderManager->SetMat4("noneLightDepthCalInstancer", "projection", projection);
    //传入无光照基础着色器(一张基础纹理)
    shaderManager->SetMat4("noneLight", "view", view);
    shaderManager->SetMat4("noneLight", "projection", projection);
    //传入无光照通用着色器
    shaderManager->SetMat4("commonNoneLight", "view", view);
    shaderManager->SetMat4("commonNoneLight", "projection", projection);
    //传入波浪通用光照着色器(构建类似泡泡的飞行道具)
    shaderManager->SetMat4("waveShader", "view", view);
    shaderManager->SetMat4("waveShader", "projection", projection);
    //传入玩家模板测试着色器
    shaderManager->SetMat4("stencilTestShader", "view", view);
    shaderManager->SetMat4("stencilTestShader", "projection", projection);
    //传入实例化荧光着色器
    shaderManager->SetMat4("fireflyInstanceShader", "view", view);
    shaderManager->SetMat4("fireflyInstanceShader", "projection", projection);
    //传入地板光照着色器
    shaderManager->SetMat4("planeCommonLight", "view", view);
    shaderManager->SetMat4("planeCommonLight", "projection", projection);
    //传入玩家光照着色器
    shaderManager->SetMat4("playerCommonLight", "view", view);
    shaderManager->SetMat4("playerCommonLight", "projection", projection);
    //传入黑洞无光照着色器
    shaderManager->SetMat4("blackHoleCommonNoneLight", "view", view);
    shaderManager->SetMat4("blackHoleCommonNoneLight", "projection", projection);
    //传入石头怪光照着色器
    shaderManager->SetMat4("stoneMonsterCommonLight", "view", view);
    shaderManager->SetMat4("stoneMonsterCommonLight", "projection", projection);
    //传入石头光照着色器
    shaderManager->SetMat4("stoneCommonLight", "view", view);
    shaderManager->SetMat4("stoneCommonLight", "projection", projection);
    //传入蝴蝶光照着色器
    shaderManager->SetMat4("butterflyCommonLight", "view", view);
    shaderManager->SetMat4("butterflyCommonLight", "projection", projection);
    //传入宝箱光照着色器
    shaderManager->SetMat4("chestCommonLight", "view", view);
    shaderManager->SetMat4("chestCommonLight", "projection", projection);
    //传入树光照着色器
    shaderManager->SetMat4("treeCommonLight", "view", view);
    shaderManager->SetMat4("treeCommonLight", "projection", projection);
    //传入宝箱无光照着色器
    shaderManager->SetMat4("chestCommonNoneLight", "view", view);
    shaderManager->SetMat4("chestCommonNoneLight", "projection", projection);
    //传入实例化草着色器
    shaderManager->SetMat4("grassNoneLightInstancer", "view", view);
    shaderManager->SetMat4("grassNoneLightInstancer", "projection", projection);
    //传入实例化石着色器
    shaderManager->SetMat4("stoneNoneLightInstancer", "view", view);
    shaderManager->SetMat4("stoneNoneLightInstancer", "projection", projection);
}

//平行光统一参数传入和阴影传入部分
void LightShaderGlobalParamerersCalculate()
{
    //为渲染阴影设置阴影光源矩阵
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
    //激活阴影纹理单元,这里设置纹理单元10避免冲突
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
    //平行光强度渲染,平行光的参数是一致的
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
    //以上两个通用着色器，采用同样的参数传入
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

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);  // 清除颜色缓冲和深度缓冲和模板缓冲
    //执行光源参数的全局输入
    LightShaderGlobalParamerersCalculate();
    //每帧清空可移除对象
    toActiveFalse.clear();
    toActiveTrue.clear();
    toDestory.clear();
    //2.使用综合脚本进行控制，场景类独立性综合性的方法,这个方法也可以通过变体种子int参数来执行不同的脚本
    //遍历执行池
    for (auto& item : manager->GetNativeObjects()) {
        switch (item.second->GetVariant()) {

        case ModelClass::BlackHoleE: // 对应 else if (item.second->GetVariant() == ModelClass::BlackHoleE)
            scripts->TestUpdateFun(item.second); // 脚本方法，执行综合方法
            break;

        case ModelClass::ParallelLight: // 对应 else if (item.second->GetVariant() == ModelClass::ParallelLight)
            scripts->TParallelLightRotation(item.second); // 平行光旋转
            break;

        case ModelClass::StoneMonser: // 对应 else if (item.second->GetVariant() == ModelClass::StoneMonser)
            if (item.second->GetComponent<CollisionBody>()->GetCollisionProperties().gameProperties.death == true) {
                toActiveFalse.push_back(item.second); // 怪物生命监测
            }
            break;

        case ModelClass::TsetButterfly: // 对应 else if (item.second->GetVariant() == ModelClass::TsetButterfly)
            item.second->PlayAnimation(0, 0.1f); // 播放简易顶点动画
            if (item.second->timeAccumulator >= 120) {
                toDestory.push_back(item.second); // 测试蝴蝶超过120秒消失
            }
            break;

        case ModelClass::TestPhysics: // 对应 else if (item.second->GetVariant() == ModelClass::TestPhysics)
            if (item.second->GetComponent<CollisionBody>()->GetCollisionProperties().gameProperties.health <= 0) {
                toActiveFalse.push_back(item.second); // 设置生命值小于0时，物体消失
            }
            break;

        case ModelClass::PlayerBullet: // 对应 else if (item.second->GetVariant() == ModelClass::PlayerBullet)
            if (item.second->timeAccumulator >= 30) {
                toDestory.push_back(item.second); // 玩家子弹超过30秒销毁
            }
            break;

        default: 
            break;
        }
    }

    //遍历缓存池
    for (auto& item : manager->GetCacheObjects())
    {
        //这里添加激活条件
        if (false)
        {
            if (item.second->GetVariant() == ModelClass::TestPhysics)
            {
                toActiveTrue.push_back(item.second);
            }
        }
    }
    // 临时存取失活对象
    for (auto* obj : toActiveFalse) {
        manager->SetActive(obj,false);
    }
    //临时存取激活对像
    for (auto* obj : toActiveTrue) {
        obj->position += glm::vec3(0, 10, 0);
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
void GameUpdateBufferTestT(const glm::mat4& view, const glm::mat4& projection, GLFWwindow* window, CustomModel* player)
{

    //玩家综合性脚本操控
    if (!player->GetComponent<CollisionBody>()->GetCollisionProperties().gameProperties.death)
    {
        scripts->PlayerControl(window, manager->GetSpecialObjects()["player"]);
    }
    //实例化树木进行视口深度图计算透明等多种情况
    manager->GetSpecialObjects()["treeInstance"]->Update(view, projection);
    //玩家模板测试
    glClear(GL_STENCIL_BUFFER_BIT);
    // 启用模板测试
    glEnable(GL_STENCIL_TEST);
    // 清除模板缓冲区
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    manager->GetSpecialObjects()["player"]->Update(view, projection);
    manager->GetSpecialObjects()["player"]->UpdateVariant(view, projection);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glDepthFunc(GL_GREATER); // 仅在当前片段深度值大于深度缓冲区中的值时通过测试
   // glBlendFunc(GL_ONE, GL_ONE);//
   // glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    shaderManager->SetMat4("stencilTestShader", "model", manager->GetSpecialObjects()["player"]->transform);
    //这个方法是执行深模板测试的特殊方法
    manager->GetSpecialObjects()["player"]->SpecialMethod();
    glDepthFunc(GL_LESS); // 恢复默认深度测试函数
    glDisable(GL_STENCIL_TEST);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

