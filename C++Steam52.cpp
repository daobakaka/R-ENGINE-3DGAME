#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32sd.lib")  // 调试版本
//#pragma comment(lib, "freetype.lib")  // 确保 FreeType 库被链接
#include <iostream>
#include "Monster.h"
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Cube.h"
#include "shader.h"  // 假设你有一个独立的着色器类
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
//调试宏，感觉这个挺有用
#define DEBUGLOG(msg) std::cout << "DEBUG: " << msg << std::endl;
//末尾查看有无追踪提示

using namespace Game;
#pragma region 前向声明区域

#pragma endregion

#pragma region  脚本全局变量
bool iftest = false;
bool gameStart = true;
bool gameEnd = true;
//裸指针
std::vector<GameObject*> nativeObjects;
std::vector<GameObject*> variantObjects;


//智能指针，可不用手动管理内存，里面储存的是对象
//std::vector<std::shared_ptr<GameObject>> variantObjects;


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
        LifecycleManager<CustomModel>* manager = LifecycleManager<CustomModel>::GetInstance();//管理所有继承了MonoBehaviour的类,这里实例化为了GameObject
        //获取综合脚本控制器
        IntergratedScripts* scripts = IntergratedScripts::GetInstance();
        //获取网格管理器
        MeshDataManager* meshData = MeshDataManager::GetInstance();
        //获取字体渲染器
        auto* cusText = TextRender::GetInstance();
        //获取携程控制器
        auto* coroutine = CoroutineMethod::GetInstance();

#pragma endregion
#pragma region OPENGL 窗口初始化 深度 模板 alpha 设置

        //--图形窗口初始化阶段
        GLFWwindow* window = glfwCreateWindow(controller->windowWidth, controller->windowHeight, "OpenGL Window", nullptr, nullptr);
        if (!window) {
            glfwTerminate();
            std::cerr << "Failed to create OpenGL window!" << std::endl;
            return -1;
        }
        glfwMakeContextCurrent(window);

        GLenum glewInitResult = glewInit();
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
        //启用混合
       // glEnable(GL_BLEND);
        glDisable(GL_BLEND);  // 禁用混合
        // glDepthFunc(GL_LESS);
        // glEnable(GL_CULL_FACE);
        glDisable(GL_CULL_FACE);
        // glCullFace(GL_FRONT);   // 只剔除背面//背面剔除
#pragma endregion


#pragma region 对象创建区域
        DEBUGLOG("开始预初始化");
        //资源预初始化
        PreInitialize();
        //获取文字渲染器,制作字体
        DEBUGLOG("开始进入GL");

        // Main render loop
 // 批量创建 对象
        //批量创建对象可以并入这个协程类，只需传入注册控制器即可
     // coroutine->StartSpawnButterfliesAsync(manager);
     // std::thread spawnThread(coroutine->StartSpawnButterfliesAsync);
        coroutine->StartSpawnButterfliesByTimer(manager);
        //综合性赋值方法泛型方法， 可以直接异步初始化各种继承customModel的对象
        StepVector3 step;
        step.position = glm::vec3(0, -0.5f, 0);
        step.scale = glm::vec3(0.9f);

        coroutine->StartSpawnByTimerAnimation<ButterflyScript>(
            manager,
            noneLightVertexShaderSource,
            noneLightcubeFragmentShaderSource,
            ModelDic["butterfly"],
            AnimationDic["butterfly"]["fly"],
            ModelClass::TsetButterfly,
            2, 15,
            step,
            {},//赋默认值
            {},
            glm::vec3(1.1f)           
        );
     // spawnThread.detach();  // 分离线程

//-- 这里就类似于游戏引擎的加载组件的方法，可以定义为具体的模板类，可采用GetComponenet<T>等泛型方式来解决，加载中每个模型涉及不同的情况，需要分散到每个具体的子类
       // 
        //这里利用多样化的子类继承CustomModel，来直接开展变体方法，模仿独立脚本，后期验证
      
       




        CustomModel* customCone = new CustomModel(lightSourceVertexShaderSource, lightSourceFragmentShaderSource, meshData->cylinderVertices, meshData->cylinderIndices, meshData->cylinderVertexCount, meshData->cylinderIndexCount);
        customCone->SetVariant(ModelClass::CubeTestE);
        customCone->Initialize(glm::vec3(2.0f, 0.0f, 1.0f), glm::quat(glm::vec3(0.0f, 45.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f));
        manager->RegisterObject(customCone);

        CustomModel* customSphere = new CustomModel(colorlightSourceVertexShaderSource, colorlightSourceFragmentShaderSource, meshData->colorCubeVertices, meshData->colorCubeIndices, meshData->colorCubeVertexCount, meshData->colorCubeIndexCount,true);
        customSphere->SetVariant(ModelClass::CubeTestE);
        customSphere->Initialize(glm::vec3(-2.0f, 0.0f, -1.0f), glm::quat(glm::vec3(0.0f, 45.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f));
        manager->RegisterObject(customSphere);

#pragma endregion
#pragma region 光照区域

        CustomPointLight pointLight;
        pointLight.position = glm::vec3(0.0f, 0.0f, 0.0f);
        pointLight.color = glm::vec3(1.0f, .0f,.0f);
        pointLight.intensity = 5.0f;

        //GLuint lightshader = customCone->shaderProgram;
        //GLuint lightPosLoc = glGetUniformLocation(lightshader, "lightPos");//获取shader中的相关的参数，这个参数可以自行设置，因为共用一个shader 所以改变一个等于改变全部
        //GLuint lightColorLoc = glGetUniformLocation(lightshader, "lightColor");
        //GLuint lightIntensityLoc = glGetUniformLocation(lightshader, "lightIntensity");



#pragma endregion
        manager->StartAll();


        // 2. 这里需要实时进行 每一帧，因为画面会切换
      //  cusText->RenderText("Hello World", 300.0f, 500.0f, 10.0f, glm::vec3(1.0f, 1.0f, 1.0f));

    
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
#pragma endregion

#pragma region  完成类似脚本功能的三种实现模式，根据需要选择   
        //1.简易对象动态参数设置区域
       // newCube1->position += glm::vec3(0.0f, 0.0f, -0.01f);
      //  newCube1->rotation *= glm::quat(glm::vec3(0.0f, 0.01f, 0.0f));  

        //2.使用综合脚本进行控制，场景类独立性综合性的方法,这个方法也可以通过变体种子int参数来执行不同的脚本
        for (CustomModel * item : manager->GetNativeObjects()) {
            if (item->GetVariant() == 0)
            {
               
                scripts->TChangeRandom(-0.01f, 0.01f);//改变构造随机种子
               // scripts->CubeUpdateFun(item); // 使用迭代器遍历链表并调用每个,暂时理解为一个遍历语法糖
            }
            else if (item->GetVariant()==ModelClass::CubeTestE)
            {
              scripts->TestUpdateFun(item);
              glUseProgram(item->shaderProgram);

              // 获取每个物体的光源参数位置
              GLuint lightPosLoc = glGetUniformLocation(item->shaderProgram, "lightPos");
              GLuint lightColorLoc = glGetUniformLocation(item->shaderProgram, "lightColor");
              GLuint lightIntensityLoc = glGetUniformLocation(item->shaderProgram, "lightIntensity");
              GLuint viewPos = glGetUniformLocation(item->shaderProgram, "viewPos");
              glm::vec3 viewP = glm::vec3(controller->front);

              // 设置光源参数
              glUniform3f(lightPosLoc, pointLight.position.x, pointLight.position.y, pointLight.position.z);
              glUniform3f(lightColorLoc, pointLight.color.x, pointLight.color.y, pointLight.color.z);
              glUniform3f(viewPos,viewP.x,viewP.y,viewP.z);
              glUniform1f(lightIntensityLoc, pointLight.intensity);
            }
            else if (item->GetVariant()==ModelClass::ActorButterfly)
            {
              //目前这种统一脚本的方式，并不能完全独立于对象脚本，只能在一定程度上进行独立
                scripts->ActorButtfly(item);
                item->PlayAnimation(0, 0.1f);
                
                
            }
            else if (item->GetVariant()==ModelClass::TsetButterfly)
            {
             
                //这样也可以执行变体方法，待后期验证
                item->UpdateVariant(view, projection);
                item->PlayAnimation(0, 0.1f);
            }
        }
        //3.变体方法，类似于手动撰写脚本来实现，须在初始化时，注册脚本变体,通用脚本方法有一定局限性，变体方法后续还会使用
        //变体即其模型类型注册不为0，主要用于场景中的需要批量设置相关变换的对象
        for (auto* item : manager->GetVariantObjects()) {
           // item->UpdateVariant(view, projection);
        }
      
      //  LDmodel->PlayAnimation(0,0.1f);
        //基础方法，运行场景的自动更新
        manager->UpdateAll(view, projection);
        //变体方法，自动化脚本，目前,同第三条一样暂时弃用之
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



