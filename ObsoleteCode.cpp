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

        //--初始的灯光定义方式
        CustomPointLight pointLight;
        pointLight.position = glm::vec3(0.0f, 0.0f, 0.0f);
        pointLight.color = glm::vec3(1.0f, .0f, .0f);
        pointLight.intensity = 5.0f;

        GLuint lightshader = customCone->shaderProgram;
        GLuint lightPosLoc = glGetUniformLocation(lightshader, "lightPos");//获取shader中的相关的参数，这个参数可以自行设置，因为共用一个shader 所以改变一个等于改变全部
        GLuint lightColorLoc = glGetUniformLocation(lightshader, "lightColor");
        GLuint lightIntensityLoc = glGetUniformLocation(lightshader, "lightIntensity");

// -- 多光源渲染控制代码

         else if (item->GetVariant() == ModelClass::TsetButterfly)
         {

             //这样也可以执行变体方法，待后期验证
             item->UpdateVariant(view, projection);
             item->PlayAnimation(0, 0.1f);



             //--
             // 使用该物体的 shader 程序
             glUseProgram(item->shaderProgram);

             // 传递摄像机数据（这里用 controller->front 表示摄像机前向或位置数据，可根据实际情况替换）
             GLuint viewPosLoc = glGetUniformLocation(item->shaderProgram, "viewPos");
             glm::vec3 viewP = glm::vec3(controller->front);
             glUniform3f(viewPosLoc, viewP.x, viewP.y, viewP.z);


             // 传入点光源数据
             auto pointLights = lightController->GetPointLights();
             for (int i = 0; i < MAX_LIGHTS; i++)
             {
                 // 拼接 uniform 数组名称
                 std::string posName = "lightPos[" + std::to_string(i) + "]";
                 std::string colorName = "lightColor[" + std::to_string(i) + "]";
                 std::string intensityName = "lightIntensity[" + std::to_string(i) + "]";

                 GLuint posLoc = glGetUniformLocation(item->shaderProgram, posName.c_str());
                 GLuint colorLoc = glGetUniformLocation(item->shaderProgram, colorName.c_str());
                 GLuint intensityLoc = glGetUniformLocation(item->shaderProgram, intensityName.c_str());

                 if (i < pointLights.size())
                 {
                     glUniform3f(posLoc, pointLights[i].position.x, pointLights[i].position.y, pointLights[i].position.z);
                     glUniform3f(colorLoc, pointLights[i].color.x, pointLights[i].color.y, pointLights[i].color.z);
                     glUniform1f(intensityLoc, pointLights[i].intensity);
                 }
                 else
                 {
                     // 没有该索引光源，则设为默认值
                     glUniform3f(posLoc, 0.0f, 0.0f, 0.0f);
                     glUniform3f(colorLoc, 0.0f, 0.0f, 0.0f);
                     glUniform1f(intensityLoc, 0.0f);
                 }
             }

             // 传入平行光数据
             // 假设平行光在着色器中对应 uniform 名称为：
             //   parallelLightDirection, parallelLightColor, parallelLightIntensity
             Game::CustomParallelLight parallelLight = lightController->GetParallelLight();
             GLuint parallelDirLoc = glGetUniformLocation(item->shaderProgram, "parallelLightDirection");
             GLuint parallelColorLoc = glGetUniformLocation(item->shaderProgram, "parallelLightColor");
             GLuint parallelIntensityLoc = glGetUniformLocation(item->shaderProgram, "parallelLightIntensity");
             glUniform3f(parallelDirLoc, parallelLight.direction.x, parallelLight.direction.y, parallelLight.direction.z);
             glUniform3f(parallelColorLoc, parallelLight.color.x, parallelLight.color.y, parallelLight.color.z);
             glUniform1f(parallelIntensityLoc, parallelLight.intensity);

             // 传入手电筒光数据
             // 假设定义了一个宏 MAX_FLASHLIGHTS (例如 #define MAX_FLASHLIGHTS 2) 并在着色器中定义了如下 uniform 数组：
             //   flashLightPos[], flashLightDirection[], flashLightColor[], flashLightIntensity[], flashLightCutoff[]
             auto flashLights = lightController->GetFlashLights();
             for (int j = 0; j < MAX_LIGHTS; j++)
             {
                 std::string flashPosName = "flashLightPos[" + std::to_string(j) + "]";
                 std::string flashDirName = "flashLightDirection[" + std::to_string(j) + "]";
                 std::string flashColorName = "flashLightColor[" + std::to_string(j) + "]";
                 std::string flashIntensityName = "flashLightIntensity[" + std::to_string(j) + "]";
                 std::string flashCutoffName = "flashLightCutoff[" + std::to_string(j) + "]";

                 GLuint flashPosLoc = glGetUniformLocation(item->shaderProgram, flashPosName.c_str());
                 GLuint flashDirLoc = glGetUniformLocation(item->shaderProgram, flashDirName.c_str());
                 GLuint flashColorLoc = glGetUniformLocation(item->shaderProgram, flashColorName.c_str());
                 GLuint flashIntensityLoc = glGetUniformLocation(item->shaderProgram, flashIntensityName.c_str());
                 GLuint flashCutoffLoc = glGetUniformLocation(item->shaderProgram, flashCutoffName.c_str());

                 if (j < flashLights.size())
                 {
                     glUniform3f(flashPosLoc, flashLights[j].position.x, flashLights[j].position.y, flashLights[j].position.z);
                     glUniform3f(flashDirLoc, flashLights[j].direction.x, flashLights[j].direction.y, flashLights[j].direction.z);
                     glUniform3f(flashColorLoc, flashLights[j].color.x, flashLights[j].color.y, flashLights[j].color.z);
                     glUniform1f(flashIntensityLoc, flashLights[j].intensity);
                     glUniform1f(flashCutoffLoc, flashLights[j].cutoff);
                 }
                 else
                 {
                     glUniform3f(flashPosLoc, 0.0f, 0.0f, 0.0f);
                     glUniform3f(flashDirLoc, 0.0f, 0.0f, 0.0f);
                     glUniform3f(flashColorLoc, 0.0f, 0.0f, 0.0f);
                     glUniform1f(flashIntensityLoc, 0.0f);
                     glUniform1f(flashCutoffLoc, 0.0f);
                 }
             }

             }

             //--单光源影响 简单代码
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
             glUniform3f(viewPos, viewP.x, viewP.y, viewP.z);
             glUniform1f(lightIntensityLoc, pointLight.intensity);



#endif // !OBSOLETE_CODE_KAKA
