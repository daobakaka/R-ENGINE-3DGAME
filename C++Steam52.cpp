﻿#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32sd.lib")  // 调试版本
//#pragma comment(lib, "freetype.lib")  // 确保 FreeType 库被链接
#include <iostream>
#include "Monster.h"
#include <vector>
#include <GL/glew.h>//加载Opengl扩展，
#include <GLFW/glfw3.h>//创建Opengl扩展，以及管理Opengl上下文
#include "Cube.h"
//#include "shader.h"  // 这是一个独立的着色器文件，且只能被引用一次
#include "Test.h"
#include "MicroCpp.h"
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
//调试宏
#define DEBUGLOG(msg) std::cout << "DEBUG: " << msg << std::endl;
//末尾查看有无追踪提示

using namespace Game;
#pragma region 前向声明区域

#pragma endregion

#pragma region  脚本全局变量
bool iftest = false;
bool gameStart = true;
bool gameEnd = true;
float zRotation;
//裸指针
std::vector<GameObject*> nativeObjects;
std::vector<GameObject*> variantObjects;



//智能指针，可不用手动管理内存，里面储存的是对象
//std::vector<std::shared_ptr<GameObject>> variantObjects;
void  CallbackMouseScrollGlobal(GLFWwindow* window, double xoffset, double yoffset)
{

    
    Controller::GetInstance()->ProcessMouseScrollGlobal(static_cast<float>(yoffset));
}

#pragma endregion

#pragma region The module of Test
//帧数
// OpenGL 错误检查函数
void CheckOpenGLError() {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << err << std::endl;
    }
}
void PrintVectors(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec2>& uvs, const std::vector<glm::vec3>& normals) {
    std::cerr << "Vertices:\n";
    for (const auto& vertex : vertices) {
        std::cerr << "  " << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
    }

    std::cerr << "UVs:\n";
    for (const auto& uv : uvs) {
        std::cerr << "  " << uv.x << " " << uv.y << std::endl;
    }

    std::cerr << "Normals:\n";
    for (const auto& normal : normals) {
        std::cerr << "  " << normal.x << " " << normal.y << " " << normal.z << std::endl;
    }
}
//预初始化区域，加载模型 动画文件 纹理等资源，从主函数中完全解耦
void PreInitialize()
{
    //加载模型区域
    MakeModel();
    // 动画控制模块，这个方法动态拓展，用于添加不同的动画，接下来可以高度封装的动画声明及添加
    MakeAnimation();
    // 加载纹理
    MakeTxture();
    //加载字体资源
    MakeFronts();
   
}

int GLins() {
#pragma region GameStart

        if (!glfwInit()) {
            std::cerr << "GLFW Initialization failed!" << std::endl;
            return -1;
        }
#pragma region 基本控制器初始化

        // 获取 Controller 实例,自定义类用于初始化主摄像机，及相关的图形窗口参数
        Controller* controller = Controller::GetInstance();
        //获取生命周期管理器,内含动画控制等
        LifecycleManager<CustomModel>* manager = LifecycleManager<CustomModel>::GetInstance();//管理所有继承了MonoBehaviour的类,这里实例化为了CustomModel
        //获取综合脚本控制器
        IntergratedScripts* scripts = IntergratedScripts::GetInstance();
        //获取网格管理器
        MeshDataManager* meshData = MeshDataManager::GetInstance();
        //获取字体渲染器
        auto* cusText = TextRender::GetInstance();
        //获取协程控制器
        auto* coroutine = CoroutineMethod::GetInstance();
        //获取灯光生成器
        auto* lightSpawner = LightSpawner::GetInstance();
        //开启灯光标识
        lightSpawner->modelIdentification = false;
        //获取灯光渲染器
        auto* lightRender = LightRender::GetInstance();
        //获取着色器控制器,构造添加默认编译
        auto* shaderManager = ShaderManager::GetInstance();
        

       

#pragma endregion
#pragma region OPENGL 窗口初始化 深度 模板 alpha 设置

        //--图形窗口初始化阶段
        GLFWwindow* window = glfwCreateWindow(controller->windowWidth, controller->windowHeight, "OpenGL Window", nullptr, nullptr);
        if (!window) {
            glfwTerminate();
            std::cerr << "Failed to create OpenGL window!" << std::endl;
            return -1;
        }
        //初始化
        glfwMakeContextCurrent(window);//设置当前绘制的上下文环境，即当前窗体
        glfwSetScrollCallback(window, CallbackMouseScrollGlobal);//鼠标滚轮回调
        GLenum glewInitResult = glewInit();

    
        //设置Opengl使用版本，4.5
       glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
       glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        //设置OpenGL渲染模式，核心模式，非立即渲染,这个添加了之后貌似不卡了
       glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        if (GLEW_OK != glewInitResult) {
            std::cerr << "Error initializing GLEW: " << glewGetErrorString(glewInitResult) << std::endl;
            return -1;
        }
      

        // 获取窗口尺寸
        glfwGetWindowSize(window, &controller->windowWidth, &controller->windowHeight);

        // 启用深度测试等
        glEnable(GL_DEPTH_TEST);//深度测试
        glEnable(GL_ALPHA_TEST);//alpha测试
        glEnable(GL_STENCIL_TEST);//模板测试
        glDepthFunc(GL_LESS); // 设置深度函数，通常使用GL_LESS
        //启用混合
       // glEnable(GL_BLEND);
       // glDisable(GL_BLEND);  // 禁用混合
        // glDepthFunc(GL_LESS);
        // glEnable(GL_CULL_FACE);
       // glDisable(GL_CULL_FACE);
        // glCullFace(GL_FRONT);   // 只剔除背面//背面剔除
#pragma endregion


#pragma region 对象创建区域
        DEBUGLOG("开始预初始化");
        //资源预初始化
        PreInitialize();
        //获取文字渲染器,制作字体
        DEBUGLOG("开始进入GL");

        //特殊构建,综合类构建方法，这里构建坐标系，编译通用着色器
       controller->BuildWidgetShader(meshData, manager);
       shaderManager->IntergratedShaderCompile();


        // Main render loop
 // 批量创建 对象
        //批量创建对象可以并入这个协程类，只需传入注册控制器即可
       // coroutine->StartSpawnButterfliesAsync(manager);
     //std::thread spawnThread(coroutine->StartSpawnButterfliesAsync);
     // 
        //天空盒的渲染是特殊的立方体贴图，并且去除摄像机平移，所以在这里单独声明
        auto* skybox = new Cube();
           
        auto* baseSphere = new CustomModelShader("commonLight", ModelDic["baseSphere"], false, true, true);
        baseSphere->SetVariant(ModelClass::CubeTestE);
        baseSphere->Initialize(glm::vec3(-3.0f, -2.0f, -2.0f), glm::quat(glm::vec3(0.0f, 45.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f));
        manager->RegisterObject(baseSphere);
        baseSphere->AttachTexture(TextureDic["butterfly"][0], 0);


        auto* baseCylinder = new CustomModelShader("commonLight", ModelDic["baseCylinder"], false, true, true);
        baseCylinder->SetVariant(ModelClass::CubeTestE);
        baseCylinder->Initialize(glm::vec3(-0.0f, 1.0f, -2.0f), glm::quat(glm::vec3(0.0f, 45.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f));
        manager->RegisterObject(baseCylinder);
        baseCylinder->AttachTexture(TextureDic["default"][0], 0);

        auto* basePlane = new  CustomModelShader("commonLight",  ModelDic["basePlane"], false, true, false);
        basePlane->SetVariant(ModelClass::StaticPlane);
        basePlane->Initialize(glm::vec3(0.0f, -5.0f, 0.0f), glm::quat(glm::vec3(0.0f, .0f, 0.0f)), glm::vec3(50.0f, 0.1f, 50.0f));
        manager->RegisterObject(basePlane);
        basePlane->AttachTexture(TextureDic["stone"][0], 0, glm::vec2(1, 1));


        auto* butterflyInstance = new  CustomModelInstance("noneLightInstancer", ModelDic["butterfly"], false, false, false,100000,glm::vec3(100,100,-100));
        butterflyInstance->SetVariant(ModelClass::CubeE);
        butterflyInstance->Initialize(glm::vec3(0.0f, -5.0f, 0.0f), glm::quat(glm::vec3(0.0f, .0f, 0.0f)), glm::vec3(0.3f, 0.3f, 0.30f));
        manager->RegisterObject(butterflyInstance);
        butterflyInstance->AttachTexture(TextureDic["butterfly"][0], 0, glm::vec2(1, 1));




        //CustomModel* testMonkey = new CustomModel(colorlightsArrayVertexShaderSource, colorlightsArraySourceFragmentShaderSource, ModelDic["testMonkey"], false, true);
        //testMonkey->SetVariant(ModelClass::CubeTestE);
        //testMonkey->Initialize(glm::vec3(-0.0f, 3.0f, 2.0f), glm::quat(glm::vec3(0.0f, 45.0f, 0.0f)), glm::vec3(0.5f));
        //manager->RegisterObject(testMonkey);
        //testMonkey->AttachTexture(TextureDic["butterfly"][0], 0,glm::vec2(1,1));
        //测试用的方法，生成注册器中的ActorButterFly 方法
      // coroutine->StartSpawnButterfliesByTimer(manager, TextureDic["butterfly"][0],0);
        //综合性赋值方法泛型方法， 可以直接异步初始化各种继承customModel的对象

      // baseSphere->AttachTexture(TextureDic["butterfly"][0], 1);
        StepVector3 step;
        step.position = glm::vec3(0, -0.5f, 0);
        step.scale = glm::vec3(0.9f);
        //coroutine->StartSpawnByTimerAnimation<ButterflyScript>(
        //    manager,
        //    colorlightsArrayVertexShaderSource,
        //    colorlightsArraySourceFragmentShaderSource,
        //    true,
        //    ModelDic["butterfly"],
        //    AnimationDic["butterfly"]["fly"],
        //    TextureDic["default"][0],  
        //    ModelClass::TsetButterfly,
        //    2, 15,
        //    step,
        //    glm::vec3(0.0f, 0.0f, 0.0f),//赋默认值
        //    {},
        //    glm::vec3(1.1f)           
        //);
     // spawnThread.detach();  // 分离线程


        
      

#pragma endregion
#pragma region 光照区域
        //预定义最大光源数量，这里可以做性能限定，目前光照的实现均为实时光照，目前没有提供注入模式，目前没有限制
        //但通过灯光渲染逻辑，现在实现了类似游戏引擎的光线渲染逻辑判断
        //初始化用于渲染阴影的平行光深度图，可以在类中直接构造，编译阴影着色器
        lightRender->CreateShadowMapForParallelLight();
        //点光源生成使用灯光控制器完成,测试定义4个灯光，物体形态的变化
        auto pointLight2= lightSpawner->SpawPointLight(glm::vec3(2,2,2),glm::vec3(1,1,1),10);
        auto pointLight = lightSpawner->SpawPointLight(glm::vec3(3, 0, 0), glm::vec3(0, 1, 1), 10);
        auto pointLight3= lightSpawner->SpawPointLight(glm::vec3(0, 3, 0), glm::vec3(0, 1, 0), 20);
        auto pointLight4= lightSpawner->SpawPointLight(glm::vec3(-3, 0, -3), glm::vec3(0, 0, 1), 20);
        
        //平行光使用灯光生成器生成，默认一个
        auto parallelLight = lightSpawner->SpawParallelLight(glm::vec3(-1),glm::vec3(1,1,1),10);//使用默认值 强度10
        //手电筒光使用灯光生成器生成，默认支持4个
        auto splashLight = lightSpawner->SpawFlashLight(glm::vec3(0,5,-2),glm::vec3(0,-1,0));//使用默认值 强度10
#pragma endregion
        manager->StartAll();



    
#pragma endregion
#pragma region 游戏进行时


    while (!glfwWindowShouldClose(window)) {
#pragma region 初始化区域
        // 渲染代码
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // 清除颜色缓冲和深度缓冲
        // 调用函数处理键盘输入并更新物体位置,这里采用自定以的帧综合控制方法
        controller->FrameControlMethod(window);
        // 获取视图矩阵和投影矩阵
        glm::mat4 view = controller->GetViewMatrix();//摄像机的朝向
        glm::mat4 projection = controller->GetProjectionMatrix();//摄像机的裁剪方向

        //为通用shader 传入共用视图及透视矩阵,这里改善了方法，设置时直接传入名字调用相关shader，
        shaderManager->SetMat4("commonLight", "view", view);
        shaderManager->SetMat4("commonLight", "projection", projection);
        //传入实例化shader的渲染
        shaderManager->SetMat4("noneLightInstancer", "view", view);
        shaderManager->SetMat4("noneLightInstancer", "projection", projection);

        //--全局执行区域阴影
       // glViewport(0, 0, 2400, 1600);  // 为深度图设置正确的视口大小
        //使用Controller内部全局着色器
        lightRender->UseDepthShaderProgram(ShaderClass::DepthCalculate);
        lightRender->RenderDepthMapForParallelLight(lightSpawner->GetParallelLight().direction);//渲染深度缓冲图，用于阴影，内含绑定_paralleLightDepthMapFBO
        lightRender->BindFramebuffer();
        manager->UpdateDepthPic(lightRender->GetLightMatrix(), lightRender->GetDepthShaderProgram(ShaderClass::DepthCalculate));//获取light的光源矩阵，并为场景中的每个对象绘制进入_paralleLightDepthMapFBO,独立对象的绘制采用模板方法
        lightRender->UnbindFramebuffer();//绘制完成之后，解除绑定阴影渲染的深度贴图
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // 清除颜色缓冲和深度缓冲
        lightRender->UseDepthShaderProgram(ShaderClass::DepthRender);
        lightRender->RenderShadowTexture();
       //--
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // 清除颜色缓冲和深度缓冲
       // baseSphere->Draw(view,projection);
        //--全局执行区域光照，光源旋转不写在GameObject的逻辑里面，在灯光生成器里单独控制
        lightSpawner->ParalletLightController(glm::vec3(0, 1, 0.0f));
  
#pragma endregion

#pragma region  完成类似脚本功能的三种实现模式，根据需要选择   
        //1.简易对象动态参数设置区域
       // newCube1->position += glm::vec3(0.0f, 0.0f, -0.01f);
      //  newCube1->rotation *= glm::quat(glm::vec3(0.0f, 0.01f, 0.0f));  
       // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (true)
        {
           // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // 清除颜色缓冲和深度缓冲


            //2.使用综合脚本进行控制，场景类独立性综合性的方法,这个方法也可以通过变体种子int参数来执行不同的脚本
            //遍历执行区域
            for (CustomModel* item : manager->GetNativeObjects()) {
                //将多光源照射效果封装在 灯光渲染器中.如果是光照shader，则需要加入这一段代码，引入光照渲染，如果不是则不需要
                //现在更改为使用构造化方式，统一使用
               //--是否光照模型判断
                if (item->ifLight)
                {
                    //这里既可以使用独有shader 也可以使用共有shader，看使用什么类去构造
                    lightRender->RenderLights(item->shaderProgram, controller, lightSpawner, item->position);//更改渲染逻辑，减少访问次数

                }

                if (item->GetVariant() == 0)
                {

                    scripts->TChangeRandom(-0.01f, 0.01f);//改变构造随机种子
                    // scripts->CubeUpdateFun(item); // 使用迭代器遍历链表并调用每个,暂时理解为一个遍历语法糖
                }
                else if (item->GetVariant() == ModelClass::CubeTestE)
                {
                    scripts->TestUpdateFun(item);
                    //  baseSphere->AttachTexture(TextureDic["butterfly"][0], 1);        
                }
                else if (item->GetVariant() == ModelClass::ParallelLight)//平行光旋转，后面增加其他逻辑
                {

                    scripts->TParallelLightRotation(item);

                }
                else if (item->GetVariant() == ModelClass::ActorButterfly)
                {
                    //目前这种统一脚本的方式，并不能完全独立于对象脚本，只能在一定程度上进行独立
                    scripts->ActorButtfly(item);
                    item->PlayAnimation(0, 0.1f);


                }
                else if (item->GetVariant() == ModelClass::TsetButterfly)
                {

                    //这样也可以执行变体方法，待后期验证
                    item->UpdateVariant(view, projection);
                    item->PlayAnimation(0, 0.1f);

                }
                else if (item->GetVariant() == ModelClass::LightColorTestCube)
                {


                }
            }
            //3.变体方法，类似于手动撰写脚本来实现，须在初始化时，注册脚本变体,通用脚本方法有一定局限性，变体方法后续还会使用
            //变体即其模型类型注册不为0，主要用于场景中的需要批量设置相关变换的对象
            for (auto* item : manager->GetVariantObjects()) {
                // item->UpdateVariant(view, projection);
            }

            //  LDmodel->PlayAnimation(0,0.1f);
              //基础方法，运行场景的自动更新,包含transform更新和绘制更新
            skybox->Draw(view, projection);//保持天空盒在其他物体之前渲染,渲染天空盒
            manager->UpdateAll(view, projection);
            // skybox->Draw(view, projection);//保持天空盒在其他物体之前渲染,渲染天空盒
             //变体方法，自动化脚本，目前,同第三条一样暂时弃用之
        }
       //skybox->Draw(view, projection);//保持天空盒在其他物体之前渲染,渲染天空盒
      //  manager->UpdateAllVariant(view, projection);
       // cusText->RenderText("abcdefghijklmnopqrstuvwxyz1234567890 *+!@#$%^&*(){}[];:''<>", 100.0f,1500.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
        cusText->RenderText("FPS:" + std::to_string(scripts->TUpdateFPS()), 100.0f, 1400.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));//帧率渲染
        //设置时间控制器，将协程的方法在主循环中进行时间控制
        coroutine->CoroutineUpdate();
        scripts->TControlFrameRate(120);//帧率控制，目前是使用比较笨的阻塞方法

#pragma region 光照更新模块
             
       


#pragma endregion


#pragma endregion

#pragma region 收尾 缓冲区处理
        // 检查 OpenGL 错误
        CheckOpenGLError();
        // 交换缓冲区
        glfwSwapBuffers(window);
        // 处理事件
        glfwPollEvents();
#pragma endregion
    }
#pragma endregion
#pragma region 退出游戏 删除对象 清空内存
    // 销毁窗口和退出
    glfwDestroyWindow(window);
    glfwTerminate();
    // 删除 Cube 对象
    manager->DestroyAll();
    //delete animator;
    return 0;
#pragma endregion

}

void TestCpp()
{
    std::cout << "Fuck you bitch!\n";

    //栈创建，类似C#的普通变量，但没有new
    Monster monster("daobakaka", 100, 10);
    monster.Attack();
    monster.TakeDamage(10);
    //堆创建，以指针模式创建
    Monster* monsterPtr = new Monster("kakadaoba", 50, 5);
    monsterPtr->Attack();
    monsterPtr->TakeDamage(5);
    delete(monsterPtr);

    std::vector<Monster> monsters;
    monsters.push_back(Monster("Monster 1", 101, 10));
    monsters.push_back(Monster("Monster 2", 81, 8));
    monsters.push_back(Monster("Monster 3", 51, 5));
    monsters[1].Attack();

    Monster* monstersa = new Monster[3]{
        Monster("Monster a1", 101, 10),
        Monster("Monster a2", 81, 8),
        Monster("Monster a3", 51, 5),
    };
    monstersa[2].TakeDamage(3);
    monstersa->TakeDamage(1);
    delete[] monstersa;

    MonsterSon* monsterSonPtr = new MonsterSon("son", 888, 1);
    monsterSonPtr->TakeDamage(20);
    monsterSonPtr->MagicSon();
    delete(monsterSonPtr);

    MonsterGrandSon* monsterGrandSonPtr = new MonsterGrandSon("grandSon", 999, 2);
    monsterGrandSonPtr->MagicSon();
    monsterGrandSonPtr->TakeDamage(30);
    monsterGrandSonPtr->MagicGrandSon();
    delete(monsterGrandSonPtr);

    MonsterDeepSon* monsterDeepSon = new MonsterDeepSon("DeepSon", 111, 2);
    monsterDeepSon->TakeDamage(100);
    monsterDeepSon->AbstractFunc();
    monsterDeepSon->TestFuncinline();
    delete(monsterDeepSon);


}
//extern 关键字，告诉编译器 这个方法的编译从其他文件中来，因此不用引入头文件了
extern void TestGlobal();
//extern void SayHelloInl(); //这种扩展方法的实现需要在直接构造在 .cpp源文件中
void MicroTest()
{
#pragma region 宏定义模块,C++中的宏替换，内联函数等
    double x = 10;
    x = x + PICUS;
    std::cout << "hello world:" << PIConst << std::endl;
    SquareFun(PIConst);
    DEBUGLOG(SQUARE(x));
    TestOne();//仅仅在头文件中直接声明一个方法，未包含在类里面，独立的方法，且未包含作用域修饰符 public，但是源文件中有实现
    TestGlobal();//仅仅在源文件中有实现，也没有修饰符，应该是默认public,
    
    InlTest *obj=new InlTest(); //这种包含在类里面的内联函数，也必须通过对象来声明，而不能像上面的裸内联直接声明
    obj->SayHelloInl();
 

#pragma endregion

}
#pragma endregion
int main() {
    // 打印调试信息

   // MicroTest();
   // TestCpp();
  
    //GL图形窗口主逻辑
    GLins();
    return 0;
}



