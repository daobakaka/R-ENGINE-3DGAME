
# R-ENGINE 3D Game Engine

**R-ENGINE** 是一个基于 C++ 和 OpenGL 构建的游戏引擎基础框架，提供渲染管线、物理模拟、动画控制、碰撞检测、资源管理、游戏逻辑支持等，附带完整的 3D 游戏示例 Demo。

框架集成了PBR渲染、动态阴影、GPU 实例化、八叉树空间划分、基础物理引擎、基础碰撞检测、对象生命周期管理、基础协程异步功能，能够满足基本的 3D 游戏开发需求。

框架适用于有一定图形学基础的人群，需对游戏引擎有一定的了解，需对图形学或者游戏开发有一定的程度的热爱，欢迎朋友们探讨。

---

## 目录

- [项目简介](#项目简介)
- [主要特性](#主要特性)
- [模块与组件](#模块与组件)
  - [基础库与程序入口](#基础库与程序入口)
  - [动画组件](#动画组件)
  - [碰撞组件](#碰撞组件)
  - [物理组件](#物理组件)
  - [资源管理逻辑](#资源管理逻辑)
  - [游戏逻辑](#游戏逻辑)
  - [基础逻辑](#基础逻辑)
  - [综合结构](#综合结构)
  - [模型管理](#模型管理)
  - [渲染逻辑](#渲染逻辑)
  - [Cpp 测试模块](#cpp-测试模块)
  - [其他扩展功能](#其他扩展功能)
- [优化技术说明](#优化技术说明)
- [构建与运行](#构建与运行)
- [使用说明](#使用说明)
- [贡献指南](#贡献指南)
- [后期改进](#后期改进)
- [许可证](#许可证)
- [Demo示例](#Demo示例)

## 项目简介

**R-ENGINE** 为开发者提供从基础图形渲染到通用游戏逻辑的底层基础解决方案，便于二次开发和项目扩展。项目结构较清晰，具有良好的扩展性，但整体框架较为简陋，存在较大改进空间。

---

## 主要特性

- 基于物理的渲染（PBR）
- 实时阴影与深度贴图
- GPU 实例化与动态实例化渲染
- 基础碰撞检测（AABB + 八叉树）
- 骨骼动画与顶点动画控制
- Shader 动态管理与后处理效果
- 协程管理与异步资源加载
- 对象生命周期与内存优化管理
- 模型、纹理资源完整导入与管理系统
- 渲染后处理

---

## 模块与组件

### 基础库与程序入口

- OpenGL 为核心图形渲染库
- 程序入口文件：`C++Steam52.cpp`，统一管理引擎启动与初始化

### 动画组件

- 动画控制器 `AnimationController`
  - 支持骨骼动画与顶点动画
  - 关键帧插值与状态控制
- 动画组件 `Animation`
### 碰撞组件

- 碰撞体 `CollisionBody`
  - AABB 碰撞检测
  - 八叉树（`OcTree`）空间划分优化
  - 碰撞参数（锁轴、触发器、碰撞半径、碰撞逻辑类型、碰撞体类型）

### 物理组件

- 物理引擎 `PhysicalEngine`
  - 重力、速度、加速度、摩擦系数、弹性系数、质量、旋转阻尼
  - 静态与动态物理体控制

### 资源管理逻辑

- 模型文件原生支持 OBJ
- 模型文件拓展支持DAE格式（基于 Assimp库），FBX因版权问题，暂不支持
- 纹理支持 JPG、PNG、BMP等格式（基于 stb_image 库）

### 游戏逻辑

- 游戏对象基类：`GameObject`
- 脚本模型管理：`ScriptModel`
- 综合脚本逻辑：`IntegratedScripts`（玩家控制、射击等）
- 帧率控制模块：`TControlFrameRate`
- 生命周期管理：`LifecycleManager<T>`（游戏对象、场景对象管理类，内置start update destoty等通用生命周期方法）

### 基础逻辑

- 协程管理：`CoroutineMethod`（异步方法）
- 摄像机控制器：`Controller`（移动、旋转、缩放）
- 场景辅助坐标标识：`BuildWidgetShader`
- 灯光渲染标识：`modelIdentification`

### 综合结构

- 全局枚举与结构定义：模型类型、初始化步长结构（`EnumTotal`）

### 模型管理

- 基础模型：`Cube`（天空盒）
- 通用管理模型：`MeshDataManager`
- 自定义模型：`CustomModel`（对象渲染基类）
- 设计模型：`DesignModel`

### 渲染逻辑

- 光照模型（PBR 光照模型，Cook-Torrance BRDF）
- 光源系统：点光源、平行光、聚光灯控制（`LightSpawner`）
- 光照渲染：`LightRender`
- 静态光照探针与环境映射
- GPU 实例化：`CustomModelInstance`
- 实时阴影与透视阴影映射：`LightRender`
- Shader 管理：`ShaderManager`
- 字体渲染：`TextRender`，优化像素对齐
- 后处理： PostProcessingT
- 基本处理：伽马矫正、背面剔除、混合测试

### Cpp 测试模块

- 微型测试脚本：`MicroCpp`、`Monter`、`Test`、`InlT.inl`
- 废弃代码备份：`ObsoleteCode`

### 其他扩展功能

- 视口深度：`LightRender`

---

## 优化技术说明

- GPU 批量渲染与动态实例化
- 深度阴影贴图优化与透视映射
- 碰撞检测八叉树优化，实现 NlogN 性能
- Shader 管理与状态机优化，提升渲染性能
- 动态对象池与缓存池技术，降低内存碎片与开销
- gl绘制中的批量绑定和传参管理

---

## 构建与运行

### 环境要求
- OPENGL4.5
- 支持 OpenGL 3.3 或更高的显卡
- 操作系统支持 Windows

### 编译运行

```bash
git clone https://github.com/yourusername/R-ENGINE-3DGAME.git
cd R-ENGINE-3DGAME
```
- 打开 Visual Studio 解决方案文件（`.sln`）
-（NeGet） 加载依赖库文件
- 构建并运行项目 Demo 程序

---

## 使用说明

- 通过鼠标和键盘控制摄像机，实现移动、旋转和缩放
- 运行 Demo 查看 PBR 渲染、阴影、动画效果与碰撞响应
- Shader 文件可实时修改，快速预览效果
- 体验完整游戏逻辑，包括玩家移动、射击、物理互动、动画交互等
- 基础操作键盘W、A、S、D 移动，F 反射，鼠标 左/右 键旋转

---

## 贡献指南
- 参与贡献请留言
- 保证代码风格统一，清晰且易维护
- 在提交前，请确保代码通过测试与编译
- 对新功能或 Bug 修复，请提供必要的文档更新

---

## 后期改进

- 连续碰撞检测（CCD）与软体模拟功能
- 高级动画混合技术与优化
- 全局光照（GI）与实时光线追踪支持
- 场景对象静态批处理与动态批处理
- 深入的性能监控与分析工具
- 完整工具链与编辑器（场景编辑、材质编辑）
---

## 许可证

- 本项目采用 MIT 开源许可证，详细信息参阅项目根目录的 LICENSE 文件。
- 声明：本引擎旨在提供学习和二次开发的基础框架，欢迎与社区开发者交流探讨，共同推动R引擎技术进步。

---

## Demo示例

- 场景展示
 
![完整场景](https://github.com/user-attachments/assets/ddc9f4c0-dd0b-49bc-a730-3701b308b137)

### 基础对象生成

```html
    //光照基础面 默认第一，ID 为1
    auto* basePlane = new  CustomModelShader("aloneCommonLight", ModelDic["basePlane"], false, true, false,false);
    basePlane->SetVariant(ModelClass::StaticPlane);
    basePlane->Initialize(glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, .0f, 0.0f)), glm::vec3(1000.0f, 0.1f, 1000.0f));
    manager->RegisterObject(basePlane);
    basePlane->AttachTexture(TextureDic["grass"],false ,0 ,glm::vec2(50, 50));
    basePlane->UniformParametersIns(glm::vec3(0.3f), glm::vec3(0.1f), 0.0f, 0.99f, 1, 1.33f, 0.1f, 0);
    basePlane->AttachPhysicalEngine(true);//声明为静态类型，目前注册为1
    basePlane->AttachCollider(CollisionType::Box,SpecialType::BasePlane,1);//注册特殊碰撞体--地板

    //生成游戏玩家
    auto* gamePlayer = new  GamePlayer("aloneCommonLight", ModelDic["testMonkey"], false, true, true,true);
    gamePlayer->SetVariant(ModelClass::Player);
    gamePlayer->Initialize(glm::vec3(0.0f, 5.0f, 0.0f), glm::quat(glm::vec3(0.0f,0, 0.0f)), glm::vec3(3));
    manager->RegisterSpecialObjects("player",gamePlayer);//注册入特殊对象储存器，方便在在玩家类内部使用，且在更新方法中引用
    gamePlayer->AttachTexture(TextureDic["wheel"]);
    gamePlayer->UniformParametersIns(glm::vec3(0.3f),glm::vec3(0.1f),0.9f,0.5f,1,1.33f,0.6f,0);
    gamePlayer->AttachPhysicalEngine();
    gamePlayer->GetComponent<PhysicalEngine>()->SetElasticity(0.0F);//设置弹性系数为0
    gamePlayer->GetComponent<PhysicalEngine>()->SetFriction(0.9f);//设置摩擦系数为0.5，减少滑动
    gamePlayer->GetComponent<PhysicalEngine>()->SetRotationDamping(1);//设置旋转阻尼为1，避免碰撞旋转
    gamePlayer->GetComponent<PhysicalEngine>()->SetRotationAdjust(1);//同时设置碰撞调整系数为1，杜绝物理系统的碰撞旋转
    gamePlayer->GetComponent<PhysicalEngine>()->SetMass(5);//设置较大质量，增强碰撞抗性
    gamePlayer->AttachCollider(CollisionType::Box,SpecialType::SPlayer,1.0F);//注册为玩家，设置较小碰撞半径避免与地面直接摩擦        
    gamePlayer->GetComponent<CollisionBody>()->SetGameProperties(1000, 20, 10);//设置玩家攻击力

    //光照石头怪
    for (int i = 0; i < 1; i++)
    {//这里添加了动画要声明蒙皮网格，在动态绘制区进行绘制
        auto* stoneMonster = new GameStoneMonser("commonLight", ModelDic["stoneMonster"], true, true, true);
        stoneMonster->SetVariant(ModelClass::StoneMonser);
        stoneMonster->Initialize(glm::vec3(50, 10, -100), glm::quat(glm::vec3(0, 0, 0.0f)), glm::vec3(0.1F));
        manager->RegisterObject(stoneMonster);
        stoneMonster->AttachTexture(TextureDic["stoneMonster"],true);//采用材质批量绘制，在外部显示调用全局绘制方法
        stoneMonster->UniformParametersIns(glm::vec3(0.3f), glm::vec3(0.1f), 0.0f, 0.99f, 1, 1.33f, 0.6f, 0);
        stoneMonster->AttachAnimationController(AnimationDic["stoneMonster"]["run"]);     
        stoneMonster->SelfIns();//用于特别情况或者异步函数或者普通的参数设置解耦
    }
    void Game::GameStoneMonser::SelfIns()
{
      //声明一个新动画
   auto* ani1 = new Animation((AnimationDic["stoneMonster"]["die"]));
   GetComponent<AnimationController>()->AddAnimation(ani1);
   //--
   AttachPhysicalEngine();
   AttachCollider();//增加碰撞半径
   GetComponent<PhysicalEngine>()->SetAcceleration(glm::vec3(0, -9.8f, 0));//设置重力为0
   GetComponent<PhysicalEngine>()->SetFriction(0.5f);
   GetComponent<CollisionBody>()->SetRadius(37);//修正原生模型大小造成的问题,可以通过开启重力与地板检测查看半径
   GetComponent<PhysicalEngine>()->SetTrigger(false);
   GetComponent<CollisionBody>()->SetGameProperties(200, 10, 1);

}
```
  - CustomModelShader: 派生自CustomModel 派生自 GameObject 基类 派生自 MonoBehaviour 接口，这四重结构，定义了对象的位置、平移、缩放、渲染、阴影渲染、状态管理等多种功能
  - 构造函数：传入编译好的GLuint shader，传入加载好的模型，传入动态网络、是否接收光照、是否渲染阴影、是否作为视口深度图的捕捉对象、是否进行渲染参数批量传入等参数 
  - Variant:变体，作为生命周期管理器的基本标识，用于全局的对象分类管理，生成的对象需要注册进入管理器之后，才能享受生命周期的自动管理功能
  - AttachTexture：附加纹理，类似于成熟商业引擎中的组件，为对象附件纹理，运行时在内部自动调用绘制纹理的Draw Call
  - UniformParametersIns：全局参数初始化，对于构造的shader进行全局参数的初始化，在CustomModel内部用相应变量储存
  - AttachPhysicalEngine：附加物理组件，拥有物理引擎模拟计算的基本功能
  - AttachCollider：附加碰撞组件，拥有碰撞功能，该功能基于物理组件的参数设定，就碰撞的对象进行物理计算，内置较齐全的碰撞结构体
  - GetComponent<PhysicalEngine>()：模板设计，所有组件均可通过模板方法获取相应指针，并对组件参数进行更改
  - AttachAnimationController：添加动画控制器，基于状态机进行管理
  - GetComponent<AnimationController>()：对象未来所需要使用的动画，声明并添加到对象的动画控制器进行管理
  - GetComponent<PhysicalEngine>()：碰撞参数设定，碰撞参数可以设置碰撞半径、是否为触发器（触发器只进行碰撞检测，不进行物理计算），设置对象游戏参数等，这里有生命值、速度、攻击力、死亡状态等基本参数，可基于需求自行添加
  - 鉴于FBX文件的版权问题，目前该框架的DesignModel类可以读取带骨骼的多mesh动画模型，但示例项目中使用的obj模型，动画模块采用的顶点动画设计
---

### 生命周期管理器

```html
      void RegisterObject(T* object);
      void RemoveObject(T* object);
      void CacheObject(T* object);
      void RemoveCacheObject(T* object);
      void SetActive(T* object, bool active);
      void DestroyObject(T* object);
      void RegisterSpecialObjects(const char* name, T* object);

      std::unordered_map<ModelClass, std::unordered_map<int, T*>>& GetNativeObjects();
      std::unordered_map<ModelClass, std::unordered_map<int, T*>>& GetCacheObjects();
      std::unordered_map<const char*, T*>& GetSpecialObjects();

      void StartAll();
      void UpdateAll(glm::mat4 view, glm::mat4 projection);
      void DestroyAll();

     std::unordered_map<ModelClass, std::unordered_map<int, T*>> nativeObjects;
     std::unordered_map<ModelClass, std::unordered_map<int, T*>> cacheObjects;
```
- 管理器内带有对象注册、移除、缓存、激活、失活、销毁等方法，并在初始化阶段自动调用StartAll 方法，进行阶段自动调用UpdateAll 方法，在结束阶段自动调用DestoryAll 方法
- 管理器带有原生对象容器、缓存对象容器、特殊对象容器，形成pool结构，这种结构在对象处理，以及后续的批量参数传入、材质绑定中起到重要作用
  
---

### 灯光生成

- 基础光照

![灯光标识](https://github.com/user-attachments/assets/dbe5df73-3899-4c09-b0c7-eb1d0fae6eab)

```html
void LightInitialization()
{

    //预定义最大光源数量，这里可以做性能限定，目前光照的实现均为实时光照
    //初始化用于渲染阴影的平行光深度图，可以在类中直接构造，编译阴影着色器
    lightRender->CreateShadowMapForParallelLight();
    //初始化用于渲染视口深度图的缓冲区，用于渲染视口深度图,用于后续渲染的深度运用
    lightRender->CreateDepthMapForTest();
    //初始化用于后处理离屏帧缓冲区，用于屏幕图像的后处理
    lightRender->CreatePostProcessingMap();
    //点光源生成使用灯光控制器完成,测试定义4个灯光，物体形态的变化
   // auto pointLight2 = lightSpawner->SpawPointLight(glm::vec3(60, 5, 20), glm::vec3(1, 1, 1), 10);
    auto pointLight = lightSpawner->SpawnPointLight(glm::vec3(30, 5, 0), glm::vec3(0, 1, 1), 10);
    auto pointLight3 = lightSpawner->SpawnPointLight(glm::vec3(-15, 5, 0), glm::vec3(0, 1, 0), 10);
    auto pointLight4 = lightSpawner->SpawnPointLight(glm::vec3(-30, 5, -30), glm::vec3(0, 0, 1), 10);
    //增加场景灯光
    auto pointLight5 = lightSpawner->SpawnPointLight(glm::vec3(-70, 5, -10), glm::vec3(1, 0, 1), 10);
   // auto pointLight6 = lightSpawner->SpawPointLight(glm::vec3(70, 5, 30), glm::vec3(1, 0, 0), 10);
    //平行光使用灯光生成器生成，默认一个
    auto parallelLight = lightSpawner->SpawnParallelLight(glm::vec3(1,-1,1), glm::vec3(1, 1, 1), 1);//使用默认值 强度10
    //手电筒光使用灯光生成器生成，默认支持4个
    auto splashLight = lightSpawner->SpawnFlashLight(glm::vec3(30, 30, 0), glm::vec3(0, -1, 0),glm::vec3(0.8F,0.5F,0.3F),30,0.6f);//使用默认值 强度10
}
```
- 支持实多个时电光源、多个手电筒光源以及唯一平行光，光照计算时，对象自动获取举例最近的4个点光源和4个手电筒光源进行光照计算
- 可以自由配置各种光源的颜色、强度、方向、位置、聚光角等参数
- 开启/关闭灯光标识
```html
  lightSpawner = LightSpawner::GetInstance();
  //开启灯光标识
  lightSpawner->modelIdentification = true;
```

- 灯光标识便于开发者标识灯光位置，具备光照计算的预期

---

### 实例化

```html
  //动态萤火虫
  auto* fireflyInstance = new  FireflyInstance("fireflyInstanceShader", ModelDic["baseSphere"], false, false, false, 25, glm::vec3(3,1.0F,3), glm::vec3(0, 0.3f, 0), 
      ModelClass::InstanceSphere);
  fireflyInstance->SetVariant(ModelClass::InstanceSphere);
  fireflyInstance->SelfIns();//派生类特殊初始化
  manager->RegisterObject(fireflyInstance);
  fireflyInstance->Initialize(glm::vec3(-40.0f, 5.0f, 30.0f), glm::quat(glm::vec3(0.0f, .0f, 0.0f)), glm::vec3(0.10F));
  fireflyInstance->AttachTexture(TextureDic["butterfly"]);

  //树实例化,使用视口深度图，该渲染的结果可以基于视口深度图，利用深度信息进行相关绘制
  auto* treeInstance = new  CustomModelInstance("noneLightDepthCalInstancer", ModelDic["tree"], false, false, false, 20000, glm::vec3(10),glm::vec3(0,1,0),
      ModelClass::InstanceCircle);
  treeInstance->EnableDepthcal();//允许使用视口深度图进行计算
  treeInstance->SetVariant(ModelClass::InstanceCircle);
  treeInstance->Initialize(glm::vec3(0.0f, -0.5f, 0.0f), glm::quat(glm::vec3(0.0f, .0f, 0.0f)), glm::vec3(5));
  manager->RegisterSpecialObjects("treeInstance",treeInstance);//这里的树作为特殊容器对象注入，以便之后进行深度测试调整alpha值
  treeInstance->AttachTexture(TextureDic["tree"]);

  //碎石1实例化
  auto* stone1Instance = new  CustomModelInstance("noneLightInstancer", ModelDic["stone1"], false, false, false, 500, glm::vec3(2), glm::vec3(0, 0.4F, 0),
      ModelClass::InstanceRound);
  stone1Instance->SetVariant(ModelClass::InstanceRound);
  stone1Instance->Initialize(glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, .0f, 0.0f)), glm::vec3(2));
  manager->RegisterObject(stone1Instance);
  stone1Instance->AttachTexture(TextureDic["stoneInstance"]);
```

- 基础实例化：new  CustomModelInstance 采用基础的实例化shader ，传入初始化的基本参数，shader类型、模型类型、是否动态网格、是否接收光照、是否渲染阴影，位置步长、旋转步长、实例化矩阵类型
- 后处理实例化：以场景中的树为例，采用noneLightDepthCalInstancer这个专用的shader，用于利用第二渲染通道的视口深度图的信息进行相关的计算，如批量全局无序透视效果
- 动态实例化： new  FireflyInstance，同样采用专门的fireflyInstanceShader，配合实例化中矩阵位置的实时更新，实现动态的萤火虫效果、顶点动画效果，开发者可根据自己需求定义

---

### 基础渲染

![带气泡弹](https://github.com/user-attachments/assets/3764de1f-3ca5-4866-a776-3d209dc7e1b1)

```html
    //基础shader
    ShaderCompile(noneLightVertexShaderSource, noneLightFragmentShaderSource, "noneLight");
    //功能全面的通用光照shader
    ShaderCompile(colorlightsArrayVertexShaderSource, colorlightsArraySourceFragmentShaderSource, "commonLight");
    //功能全面的通用光照独立shader
    ShaderCompile(colorlightsArrayVertexShaderSource, colorlightsArraySourceFragmentShaderSource, "aloneCommonLight");
    //无光照的基础实例化shader
    ShaderCompile(instanceNoLightingVertexShaderSource, instanceNoLightingFragmentShaderSource, "noneLightInstancer");
    //无光照的基础实例化shader,可进行深度图进行后处理
    ShaderCompile(instanceNoLightingVertexShaderSource, instanceNoLightingForViewPortDepthMapFragmentShaderSource, "noneLightDepthCalInstancer");
    //功能全面的无光照的通用shader(可生成阴影)
    ShaderCompile(CustomNoLightShadowShaderVertexSource, CustomNoLightShadowShaderFragmentSource, "commonNoneLight");

    //独立shader,构建玩家飞行道具,数量大的对象使用专用sahder
    ShaderCompile(waveVertexShaderSource, colorlightsArraySourceFragmentShaderSource, "waveShader");
    //独立shader,玩家模板测试
    ShaderCompile(stencilTestVertexShaderSource, stencilTestFragmentShaderSource, "stencilTestShader");
    //独立实例化shader,用于生成萤光效果
    ShaderCompile(instanceNoLightingVertexShaderSource, instanceNoLightingFireFlyFragmentShaderSource, "fireflyInstanceShader");
    //深度图着色器模块
    ShaderCompile(depthShaderVertexShaderSource, depthShaderFragmentShaderSource, "depthCal");
    ShaderCompile(depthVisualShaderVertexShaderSource, depthVisualShaderFragmentShaderSource, "depthVisual");
    ShaderCompile(depthTestShaderVertexShaderSource, depthTestShaderFragmentShaderSource, "depthTest");
    //投影深度图计算,投影深度图可视化
    ShaderCompile(depthViewPortShaderVertexShaderSource, depthViewPortShaderFragmentShaderSource, "depthViewPortCal");
    ShaderCompile(depthVisualShaderVertexShaderSource, depthLinerVisualShaderFragmentShaderSource, "depthLinerVisual");
    //后处理shader 通用、光晕
    ShaderCompile(postProcessingVertexShaderSource, postProcessingCommonFragmentShaderSource, "postProcessingCommon");
    ShaderCompile(postProcessingVertexShaderSource, postProcessingBloomFragmentShaderSource, "postProcessingBloom");
```

- shader编译：框架提供了用于开发者直接使用的基础shader、通用光照shader、通用无光照shader、基础实例化shader
- 基础shader和基础实例化shader提供一张基础贴图
- 通用shader提供PRB级参数支持，基础贴图、法线贴图、高度贴图、光照2D贴图、透明度贴图、糙度贴图、立方体光照贴图

```html
 glUseProgram(shaderProgram);
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
```

- 纹理绑定：场景种单一的对象，使用初始化传入的是否批量标识和shader索引，控制shader的渲染方式
- 如果确定为批量参数传入，则内部的update方法无法渲染，需要在外部进行批量参数传入
- 场景中的空气泡飞行道具，可以明显看到prb中对于外界环境的静态映射，次Demo中采用静态立方体贴图，开发者可基于基础逻辑设计光照探针

```html
case ModelClass::TsetButterfly:

          RenderingTextureGlobal(shaderManager->GetShader("commonLight"), TextureDic["butterfly"]);
          PassUniformParametersGlobal(shaderManager->GetShader("commonLight"), glm::vec3(0.3F), glm::vec3(0.1f), 0, 1, 1, 1.33f, 0.6f);
          for (auto& item : modelClassPair.second) {
              CustomModel* object = item.second; // 获取对象
              object->PlayAnimation(0, 0.1f); // 播放简易顶点动画
              if (object->timeAccumulator >= 120) {
                  toDestory.push_back(object); // 测试蝴蝶超过120秒消失
              }
              object->Update(view, projection);//更新状态
          }
          break;

void CustomModel::Update(glm::mat4 view, glm::mat4 projection)
{
    //先进行简单物理计算，无碰撞体   
      UpdatePhysics();  
    //有碰撞体，再进行碰撞计算,并内置独立物理计算
      UpdateCollisionAndPhysics(CollisionProps);
     //再进行变体方法计算
     UpdateVariant(view,projection);
    //再进行坐标更新
    //这种写法就，直接封装了更新变化和通知CPU渲染的两个方法，如果具体定义方法，只需要在其他脚本里面单独定义某个方法，更改transform即可，由管理池泛型调用
    UpdateTransform();
    //确认动态绘制或者静态绘制
    InnerRendringTexture(view, projection);

}
void Game::CustomModel::InnerRendringTexture(const glm::mat4& view, const glm::mat4& projection)
{
    if (!_drawTextureBatch)
    {
        glUseProgram(shaderProgram);
        BindTexture();
        BindTextureAdditional();
        UniformParametersInput();//传参应该在这里引用
    }
  
    UniformParametersChange();//变化传参，针对部分参数进行变化，在一帧内改变对应的值,这应该在批量绘制的外部引用    
    IsSkinnedMesh == true ? DrawDynamical(view, projection) : Draw(view, projection);
 
}
```

- 以上示例涉及到CustomModel渲染执行类的内部自动更新和外部渲染参数批量传入的两种状态
- UpdateVariant：变体方法，需要在customModel的基础派生，并且重写变体方法，外部的生命周期控制器可以全局调用执行每个对象内部的变体方法，以实现各自独立的状态，同样的逻辑还有UniformParametersChange和BindTextureAdditional等
  
```html
 GLuint ShaderCompile(const char* ver, const char* fra, const std::string& name);
 GLuint GetShader(const std::string& name);
 void UseShader(const std::string& name);
 //集成shader编译
 void IntegratedShaderCompile();
 // Set methods for various uniform types
 void SetBool(const std::string& shaderName, const std::string& uniformName, bool value) ;
 void SetInt(const std::string& shaderName, const std::string& uniformName, int value) ;
 void SetFloat(const std::string& shaderName, const std::string& uniformName, float value) ;
 void SetVec2(const std::string& shaderName, const std::string& uniformName, const glm::vec2& value) ;
 void SetVec2(const std::string& shaderName, const std::string& uniformName, float x, float y) ;
 void SetVec3(const std::string& shaderName, const std::string& uniformName, const glm::vec3& value) ;
 void SetVec3(const std::string& shaderName, const std::string& uniformName, float x, float y, float z) ;
 void SetVec4(const std::string& shaderName, const std::string& uniformName, const glm::vec4& value) ;
 void SetVec4(const std::string& shaderName, const std::string& uniformName, float x, float y, float z, float w) ;
 void SetMat2(const std::string& shaderName, const std::string& uniformName, const glm::mat2& mat) ;
 void SetMat3(const std::string& shaderName, const std::string& uniformName, const glm::mat3& mat) ;
 void SetMat4(const std::string& shaderName, const std::string& uniformName, const glm::mat4& mat) ;
 void SetTexture(const std::string& shaderName, const std::string& uniformName,GLuint textureID, int order);
 void SetCubeTexture(const std::string& shaderName, const std::string& uniformName, GLuint textureID, int order);
```
- ShaderManager：shader管理器主要执行shader的编译、使用、获取、参数传入等通用逻辑
---

### 平行光正交阴影渲染

![平行光正交阴影](https://github.com/user-attachments/assets/334066d0-bfaf-4380-953b-cf8ddb6b0e31)

```html
  void UseDepthShaderProgram(ShaderClass shader);
  //解绑深度缓冲区
  void UnbindFramebuffer();
  //绑定深度缓冲区
  void BindFramebuffer();
  //渲染阴影图
  void RenderShadowTexture(CustomModel* obj,glm::mat4 crossView);
  //渲染阴影图，重载,这里需要使用NDC（标准设备坐标空间，来进行正交投影）
  void RenderShadowTexture(GLuint shader);
  /// <summary>
  /// 创建深度缓冲区后处理比较贴图
  /// </summary>
  /// <returns></returns>
  GLuint CreateDepthMapForTest();

  //---渲染视口深度图
  void BindDepthTestBuffer();
  void UnbindDepthTestBuffer();
  void RenderDepthTestTexture(GLuint shader);  
  glm::mat4 GetDepthMapCrossView(const glm::mat4& camerView, CustomModel* player, glm::vec3 offset);
  /// <summary>
  /// 创建后处理贴图
  /// </summary>
  /// <returns></returns>
  GLuint CreatePostProcessingMap();

  void BindPostProcessingBuffer();
  void UbindPostProcessingBuffer();
  void RenderPostProcessingTexture(GLuint shader);
```

- LightRender：光照渲染器，除了封装主要的CPU端光照计算外，还集成了平行光正交阴影映射、视口深度映射、后处理贴图渲染的功能
- 基础框架只设计了基于平行光的正交深度图生成，并且作为主渲染逻辑的第一通道

---

### 视口深度图渲染

![视口透视深度图](https://github.com/user-attachments/assets/d49c1cf4-cb99-4dfc-b248-08468eb5d26a)

- 由构造函数传入对象是否进行视口深度计算的标识，用于在后续的离屏渲染通道中或者后处理通道中进行相关计算
  
```html
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
```

- 基础渲染管线分为4个通道；透视视口深度计算位于第二通道
- 离屏渲染：这里集成了场景中最主要的渲染逻辑和游戏进行时逻辑，包含天空盒渲染、独立对象通用渲染、多对象批量传参、实例化渲染、物理计算、碰撞计算、独立更新逻辑、集成更新逻辑integratedScript

---

### 主体逻辑

```html
void GameUpdateMainLogicT(const glm::mat4& view, const glm::mat4& projection, GLFWwindow* window, CustomModel* player)
{
  
    //执行光源参数的全局输入
    LightShaderGlobalParamerersCalculate();
    //每帧清空可移除对象
    toActiveFalse.clear();
    toActiveTrue.clear();
    toDestory.clear();
    //2.使用综合脚本进行控制，场景类独立性综合性的方法,这个方法也可以通过变体种子int参数来执行不同的脚本
    //遍历执行池
    for (auto& modelClassPair : manager->GetNativeObjects()) {
        ModelClass modelClass = modelClassPair.first; // 获取当前分类

        switch (modelClass) {
        case ModelClass::ParallelLight:
            // 直接进入内层遍历
            for (auto& item : modelClassPair.second) {
                CustomModel* object = item.second; // 获取对象
                scripts->TParallelLightRotation(object); // 平行光旋转
            }
            break;

        case ModelClass::StoneMonser:
            //批量对象开启纹理或参数的批量导入        
            RenderingTextureGlobal(shaderManager->GetShader("commonLight"), TextureDic["stoneMonster"]);
            PassUniformParametersGlobal(shaderManager->GetShader("commonLight"), glm::vec3(0.3F), glm::vec3(0.1f), 0, 1, 1, 1.33f, 0.6f);
            for (auto& item : modelClassPair.second) {
                CustomModel* object = item.second; // 获取对象              
                if (object->GetComponent<CollisionBody>()->GetCollisionProperties().gameProperties.death == true) {
                    toActiveFalse.push_back(object); // 怪物生命监测
                }
                object->Update(view, projection);//更新状态
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
  ```

 - 主体逻辑通过引用生命周期管理器，访问初始化对象时注入原始对象池的对象，根据其ModelClass类型进行对象的分类计算
 - IntegratedScripts：执行综合脚本逻辑，如玩家在GL中的控制PlayerControl，蝴蝶批量飞舞ActorButtfly等，开发者可以利用这个综合类构建自定义的方法，并在主体方法中引用
 - 缓存对象管理：主体逻辑中，基于生命周期控制器，对pool结构进行增、删、改、查，对于复用率高的对象可以进行失活/激活处理，对于复用率低的对象进行销毁处理，灵活的管理优化内存资源
 - 当前的pool结构是较为基础的版本，开发者可以根据实际需求进行拓展

---

### 模板测试

![模板测试](https://github.com/user-attachments/assets/b1295bd0-c8b7-4396-9de5-00ed87925c23)

- 模板测试会增加渲染的负载，但很多时候是必要的，Demo中放在主体渲染的最后进行

```html
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
```

- 模板测试：Opengl中通过相应的API直接引用，这里需要一个专用的着色器用于渲染模板测试的结果
- GetSpecialObjects：特殊对象，用于在不同的环境中获取对象参数，由生命周期控制器统一管理，其所在的渲染阶段通常有差别，需使用生命周期控制器单独调用
- 模板测试可以实现中如透视镜、遮挡绘制、对象描边等多种功能，开发者可基于不同的情况自行研发，但通常需要进行特殊对象注入，并在开发中注意渲染流程

---

### 后处理

- 锐化

![锐化](https://github.com/user-attachments/assets/128f4df2-3a42-427a-9825-eba4cd5fa065)

- 灰度
  
![灰度](https://github.com/user-attachments/assets/8aac572e-a354-443d-b087-2a54d95029b5)

- 后处理位于渲染的第四通道，Demo中自带一通用的后处理shader，开发者可以根据需求自行拓展

```html
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
    //shaderManager->SetInt("postProcessingCommon", "effectType", 4);
    //lightRender->RenderPostProcessingTexture(shaderManager->GetShader("postProcessingCommon"));
    
}
```

- 后处理阶段可以添加与后处理交互的游戏逻辑
- demo中仅使用的全屏后处理效果，开发者可基于第三通常的离屏渲染贴图，自行构建体积后处理或者多级后处理等

---

### 渲染设置

![伽马矫正](https://github.com/user-attachments/assets/206ba681-4f26-4a8f-9b4e-2b8e9a3248b5)

- 伽马矫正：可以在后处理shader中进行手动伽马矫正，也可在高版本的OPENGL中可以直接开启，建议采用前者，本Demo中未使用
- 
```html
    // 启用深度测试等
    glEnable(GL_DEPTH_TEST);//深度测试
    glEnable(GL_ALPHA_TEST);//alpha测试
    glEnable(GL_STENCIL_TEST);//模板测试
    glDepthFunc(GL_LESS); // 设置深度函数，使用GL_LESS
    glEnable(GL_BLEND);//启用混合
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//透明混合
    glEnable(GL_MULTISAMPLE);  // 启用多重采样抗锯齿
    glEnable(GL_CULL_FACE);       // 启用背面剔除
    glCullFace(GL_BACK);          // 只剔除背面，保留正面
```

- 初始化阶段可以根据需求开启多种图形属性，如背面剔除、多重采样抗锯齿、透明混合等，实际渲染阶段基于需求开启/关闭各种测试参数，详情参照OPENGL API
- 开发者可参照Demo中模板测试的案例，构建合适的独立渲染方法，但需注意面向过程的状态机特性

---
 
### 资源加载

- 使用SourceInitialize方法进行全局加载，OPENGL窗口初始化之前

```html
  std::vector<Vertex> verticesStruct;
  std::vector<unsigned int> indices;
  LoadOBJ("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Obj\\butterfly1.obj", verticesStruct, indices);
  ModelData modelData;
  modelData.verticesStruct = verticesStruct;
  modelData.indices = indices;
  ModelDic["butterfly"] = modelData;
  
  // 动画控制模块-创建关键帧数据
  std::vector<std::vector<Vertex>> keyFrames;
  std::vector<float> keyFrameTimes = { 0.0f, 3.0f ,6.0f };  // 每个关键帧的时间点
  // 添加关键帧数据1,2,3
  keyFrames.push_back(LoadVerticesFromFile("butterfly1.obj"));
  keyFrames.push_back(LoadVerticesFromFile("butterfly2.obj"));
  keyFrames.push_back(LoadVerticesFromFile("butterfly1.obj"));


  AnimationData animaitonData;
  animaitonData.keyFrames = keyFrames;
  animaitonData.keyFrameTimes = keyFrameTimes;
  animaitonData.loop = true;
  animaitonData.speed = 1.8f;
  AnimationDic["butterfly"]["fly"] = animaitonData;

    //轮子
    GLuint wheelTexture = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\wheel.png");
    //初始化参数
    std::vector<GLuint> picd;
    //加载默认纹理
    picd.push_back(defaultTexture);//默认灰色基础图
    picd.push_back(defaultW);//默认白色法线图

   FT_Library ft;
   if (FT_Init_FreeType(&ft)) {
       std::cerr << "Could not initialize FreeType Library" << std::endl;
       return;
   }

   FT_Face face;
   if (FT_New_Face(ft, fontPath, 0, &face)) {
       std::cerr << "Could not load font" << std::endl;
       return;
   }

   FT_Set_Pixel_Sizes(face, 0, 48);  // 设置字体大小
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);//
   // 加载所有字符
   for (GLubyte c = 0; c < 128; c++) {
       if (FT_Load_Char(face, c, FT_LOAD_RENDER | FT_LOAD_DEFAULT)) {
           std::cerr << "Could not load character " << c << std::endl;
           continue;
       }
```

- 模型加载：可以加载原生的obj模型，也可以加载Assimp库支持的其他模型，Demo使用obj完成
- 动画加载：因FBX文件版权问题，动画目前仅封装了顶点动画的方法
- 纹理加载：基于stb_image库，支持大多数纹理格式
- 字体加载：基于ft2build.h， 支持TTF、TIF 格式，Demo资源文件力附带上百种字体文件
- 所有资源加载的内容均储存在对应的全局数据结构中如AnimationDic、ModelDic、TextureDic，开发着可以根据需求构建

 ---

 
