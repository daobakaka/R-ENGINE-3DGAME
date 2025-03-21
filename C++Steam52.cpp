﻿#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32sd.lib")  // 调试版本
//#pragma comment(lib, "freetype.lib")  // 确保 FreeType 库被链接
#include <iostream>
#include "Monster.h"
#include <vector>
#include <GL/glew.h>//加载Opengl扩展，
#include <GLFW/glfw3.h>//创建Opengl扩展，以及管理Opengl上下文
#include "Cube.h"
#include "Test.h"
#include "MicroCpp.h"
#include "Controller.h"
#include "LifecycleManager.h"
#include "IntegratedScripts.h"
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
//调试宏
#define DEBUGLOG(msg) std::cout << "DEBUG: " << msg << std::endl;
//末尾查看有无追踪提示

using namespace Game;
#pragma region 前向声明区域
//内部方法
void CheckOpenGLError();
//着色器名称
extern const char* colorlightsArrayVertexShaderSource;
extern const char* colorlightsArraySourceFragmentShaderSource;

//场景方法声明
extern GLFWwindow* GLinitializeT();
extern void GameAwakeT();
extern CustomModel* GameStartT();
extern void LightInitialization();
extern void SourceInitialize();
extern void GameUpdateShadowRenderT(const glm::mat4& view, CustomModel* player, glm::vec3 offset);
extern void GameUpdateMainLogicT(const glm::mat4& view, const glm::mat4& projection, GLFWwindow* window, CustomModel* player);
extern void GameUpdateBufferTestT(const glm::mat4& view, const glm::mat4& projection, GLFWwindow* window, CustomModel* player);
extern void ShderViewPortRenderingT(const glm::mat4& view, const glm::mat4& projection,  CustomModel* player);
extern void PostProcessingT(const glm::mat4& view, const glm::mat4& projectionn, GLFWwindow* window, CustomModel* player);
extern void StaticShaderGlobalParametersPassingT(const glm::mat4& view, const glm::mat4& projection);
//控制组件标识
extern Controller* controller;
extern LifecycleManager<CustomModel>* manager;
extern IntegratedScripts* scripts;
extern MeshDataManager* meshData;
extern TextRender* cusText;
extern CoroutineMethod* coroutine;
extern LightSpawner* lightSpawner;
extern LightRender* lightRender;
extern ShaderManager* shaderManager;

#pragma endregion


int GLins() {
#pragma region GameStart

    if (!glfwInit()) {
        std::cerr << "GLFW Initialization failed!" << std::endl;
        return -1;
    }
    // 控制器初始化，全局控制器获取
    GameAwakeT();
    //--图形窗口初始化阶段,OPENGL 窗口初始化 深度 模板 alpha 设置
    GLFWwindow* window = GLinitializeT();

#pragma region 游戏初始化区域，包含灯光创建，游戏对象创建，特殊构建等
    DEBUGLOG("开始预初始化");
    //资源预初始化
    SourceInitialize();
    //特殊构建,综合类构建方法，这里构建坐标系，编译通用着色器,天空盒的渲染是特殊的立方体贴图，并且去除摄像机平移，在这里单独声明
    controller->BuildWidgetShader(meshData, manager);
    shaderManager->IntegratedShaderCompile(); 
    auto* skybox = new Cube();

    //对象创建
  auto* gamePlayer = GameStartT();
    //灯光初始化
    LightInitialization();
    DEBUGLOG("开始进入GL");
    //调用对象开始方法
    manager->StartAll();
#pragma endregion

#pragma region 游戏进行时


    while (!glfwWindowShouldClose(window)) {
#pragma region 进行时参数，设置shader 等参数
        // 渲染代码
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // 清除颜色缓冲和深度缓冲
        // 调用函数处理键盘输入并更新物体位置,这里采用自定以的帧综合控制方法
        controller->FrameControlMethodPlayer(window,gamePlayer);
        // 获取视图矩阵和投影矩阵
        glm::mat4 view = controller->GetPlayerViewMatrix(gamePlayer, glm::vec3(0, 30, 100));//摄像机的朝向
        glm::mat4 projection = controller->GetProjectionMatrix();//摄像机的裁剪方向
        //静态shader全局参数传入
        StaticShaderGlobalParametersPassingT(view,projection);
#pragma endregion
        //多通道渲染
        //通道1:这里渲染平行光阴影正交深度图
        if (true)
        {
            GameUpdateShadowRenderT(view,gamePlayer, glm::vec3(0, 30, 100));
        }
        //通道2：透视视口深度图渲染
        if (true)
        {
             ShderViewPortRenderingT(view, projection,gamePlayer);
        }
        //通道3：离屏渲染和主逻辑
        if (true)
        {
            //绑定后处理的离屏帧缓冲对象，这里用于后处理，不需要后处理这里可以注释掉
            lightRender->BindPostProcessingBuffer();
            //保持天空盒在其他物体之前渲染,渲染天空盒
            skybox->Draw(view, projection);
            //基本逻辑，批处理方法
            GameUpdateMainLogicT(view, projection,window,gamePlayer);
            //控制器更新方法1、基本更新2、变体更新
            manager->UpdateAll(view, projection);
            //执行三种渲染测试方法的特殊区域，模板、深度
            GameUpdateBufferTestT(view, projection, window, gamePlayer);
        }
        //通道4：后处理渲染
        if (true)
        {
            PostProcessingT(view, projection, window, gamePlayer);
        }
        //开启字体渲染
       // cusText->RenderText("FPS :" + std::to_string(scripts->TUpdateFPS()), 2200.0f, 1150.0f, .70f, glm::vec3(1.0f, .50f, 1.0f));//帧率渲染
        //设置时间控制器，将协程的方法在主循环中进行时间控制
        coroutine->CoroutineUpdate();
        //帧率控制，目前是使用比较笨的阻塞方法
        scripts->TControlFrameRate(120);
        // 测试收尾交换缓冲区
        CheckOpenGLError();
        // 交换缓冲区
        glfwSwapBuffers(window);
        // 处理事件
        glfwPollEvents();
    }
#pragma region 退出游戏 删除对象 清空内存
    // 销毁窗口和退出
    glfwDestroyWindow(window);
    glfwTerminate();
    // 删除 Cube 对象
    manager->DestroyAll();
    delete skybox;//释放天空盒内存
    //delete animator;
    return 0;
#pragma endregion

}

#pragma endregion
int main() {

    //GL图形窗口主逻辑
    GLins();
    return 0;
}

#pragma region  附属方法
void CheckOpenGLError() {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << err << std::endl;
    }
}

#pragma endregion

