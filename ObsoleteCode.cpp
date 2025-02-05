#if OBSOLETE_CODE_KAKA

//对象初始化区域
       //加载模型区域
       // Load OBJ file
std::vector<glm::vec3> vertices;
std::vector<glm::vec2> uvs;
std::vector<glm::vec3> normals;
std::vector <unsigned int> indeies;
bool res = LoadOBJ("E:\\C++\\FirstOne\\C++Steam52\\Assests\\Obj\\butterfly2.obj", vertices, uvs, normals);
if (!res) {
    std::cerr << "Failed to load OBJ file!" << std::endl;
    return -1;
}
std::vector<Vertex> verticesStruct;
std::vector<unsigned int> indices;
if (!LoadOBJ("E:\\C++\\FirstOne\\C++Steam52\\Assests\\Obj\\butterfly1.obj", verticesStruct, indices)) {
    DEBUGLOG("Faild to load obj verteices only");
}
//解压图片信息为可用格式
// 加载纹理
GLuint picTexture = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assests\\Texture\\1.png");
if (picTexture == 0)
{
    // 处理加载失败的情况
    std::cerr << "Texture loading failed." << std::endl;
    // 可能需要退出程序或采取其他措施
}
//两种导入方式均可，第二种效率更高，主要原因在于没有开启三角化，所以全是四边形，造成画面镂空      
// printVectors(vertices, uvs, normals);
CustomModel* LDmodel = new CustomModel(noneLightVertexShaderSource, noneLightcubeFragmentShaderSource, verticesStruct, indices, true);//定义的是地址，也就是传入引用，这种方式类似于ref
manager->RegisterObject(LDmodel);
LDmodel->Initialize(glm::vec3(2.0f, 0.0f, 1.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f));
GLuint picData = glGetUniformLocation(LDmodel->shaderProgram, "texture1");

//--
     // Load OBJ file
std::vector<Vertex> verticesStruct;
std::vector<unsigned int> indices;
if (!LoadOBJ("E:\\C++\\FirstOne\\C++Steam52\\Assests\\Obj\\butterfly1.obj", verticesStruct, indices)) {
    DEBUGLOG("Faild to load obj verteices only");
}


CustomModel* LDmodel = new CustomModel(noneLightVertexShaderSource, noneLightcubeFragmentShaderSource, ModelDic["butterfly"].verticesStruct, ModelDic["butterfly"].indices, true);//定义的是地址，也就是传入引用，这种方式类似于ref
///--加载纹理
GLuint picTexture = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assests\\Texture\\1.png");


// 创建动画，设置是否循环播放，播放速度，引用动画制作形成的动画控制文件
Animation* anim = new Animation(AnimationDic["butterfly"]["fly"]);
// 创建动画控制器
AnimationController* animator = new AnimationController();
// 将动画添加到控制器中
animator->AddAnimation(anim);


//纹理绑定方法
  // 使用着色器程序
//glUseProgram(LDmodel->shaderProgram);
//glActiveTexture(GL_TEXTURE0);          // 激活纹理单元 0
//glBindTexture(GL_TEXTURE_2D, TextureDic["butterfly"][0]);  // 绑定纹理对象到纹理单元 0,这里添加"butterfly"集合的纹理对象
//// 绑定纹理到纹理单元 0
//glUniform1i(picData, 0);
//--动画模块
// LDmodel->UpdateVerticesForAnimation(10);//添加伪动画效果
 //简化了流程,动画控制器直接控制动画，传入对象指针
 // animator->PlayAnimation(0,0.03f,LDmodel);


//--OPENGL 文字渲染方式

       // 1. 保存当前的 OpenGL 状态
        GLint previousShader;
        glGetIntegerv(GL_CURRENT_PROGRAM, &previousShader);  // 获取当前程序

        GLint previousTexture;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &previousTexture);  // 获取当前绑定的纹理

        // 2. 渲染文字
        cusText->RenderText("Hello World", 100.0f, 100.0f, 10.0f, glm::vec3(1.0f, 1.0f, 1.0f));

        // 3. 恢复之前的 OpenGL 状态
        glUseProgram(previousShader);  // 恢复之前的着色器
        glBindTexture(GL_TEXTURE_2D, previousTexture);  // 恢复之前的纹理绑定



        //  Cube* newCube1 = new Cube(cubeVertexShaderSource, cubeFragmentShaderSource);//这里轻松实现材质替换
// newCube1->Initialize(glm::vec3(2.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, 45.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f));
//// newCube1->SetVariant(ModelClass::CubeE);
// manager->RegisterObject(newCube1);
// scripts->TChangeRandom(-0.01f, 0.01f);//改变构造随机种子

 Cube* newCube2 = new Cube(cubeVertexShaderSource, cubeFragmentShaderSource);//这里轻松实现材质替换
 newCube2->Initialize(glm::vec3(.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(0.1f, 0.10f, .10f));
 // newCube1->SetVariant(ModelClass::CubeE);
 manager->RegisterObject(newCube2);
 // std::list<Cube*> cubes;  // 使用 std::list 存储 Cube 对象指针//综合脚本方法测试
 for (int i = 0; i < 0; i++)
 {
     Cube* newCubeClone = new Cube(cubeVertexShaderSource, cubeFragmentShaderSource);
     float random = scripts->TGetRandomFloat(-1.0f, 1.0f);
     newCubeClone->Initialize(glm::vec3(random, random, random), glm::quat(glm::vec3(random * 5, random * 10, random * 10)), glm::vec3(0.8f, 0.8f, 0.8f));
     manager->RegisterObject(newCubeClone);//控制器管理注册器
 }

 for (int i = 0; i < 0; i++)
 {
     Cube* newCubeClone = new Cube(cubeVertexShaderSource, cubeFragmentShaderSource);
     float random = scripts->TGetRandomFloat(-1.0f, 1.0f);
     newCubeClone->Initialize(glm::vec3(random, random, random), glm::quat(glm::vec3(random * 5, random * 10, random * 10)), glm::vec3(1.5f, 1.5f, 1.5f));
     newCubeClone->SetVariant(ModelClass::CubeE);
     manager->RegisterObject(newCubeClone);
 }
//---这里将GanmeObject 对象类的限制往下移动，转变为CustomModel 类，这样一来就可以执行新的动画运动等模式，舍弃之前的GameObject 控制
//---  批量创建蝴蝶对象的原始方法
        for (int i = 0; i < 30; i++)
        {
            CustomModel* LDmodel = new CustomModel(noneLightVertexShaderSource, noneLightcubeFragmentShaderSource, ModelDic["butterfly"], true);//定义的是地址，也就是传入引用，这种方式类似于ref
            manager->RegisterObject(LDmodel);
            LDmodel->Initialize(glm::vec3(2.0f, 0.0f, i * 0.5f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(0.1f, 0.1f, 0.1f));
            //--模型附加材质相关   
            LDmodel->AttachTexture();
            //--模型附件动画控制器
            LDmodel->AttachAnimationController(AnimationDic["butterfly"]["fly"]);//这里传入一个默认参数，可以知道具体的动画名称和首动画
            LDmodel->AttachPhysicalEngine();
            LDmodel->AttachCollider();
            auto* aa = LDmodel->GetComponent<AnimationController>();
            DEBUGLOG(aa);
            //初始化参数          
            LDmodel->SetVariant(ModelClass::ActorButterfly);
        }






#endif // !OBSOLETE_CODE_KAKA
