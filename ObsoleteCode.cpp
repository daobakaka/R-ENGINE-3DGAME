#if OBSOLETE_CODE_KAKA

//�����ʼ������
       //����ģ������
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
//��ѹͼƬ��ϢΪ���ø�ʽ
// ��������
GLuint picTexture = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assests\\Texture\\1.png");
if (picTexture == 0)
{
    // �������ʧ�ܵ����
    std::cerr << "Texture loading failed." << std::endl;
    // ������Ҫ�˳�������ȡ������ʩ
}
//���ֵ��뷽ʽ���ɣ��ڶ���Ч�ʸ��ߣ���Ҫԭ������û�п������ǻ�������ȫ���ı��Σ���ɻ����ο�      
// printVectors(vertices, uvs, normals);
CustomModel* LDmodel = new CustomModel(noneLightVertexShaderSource, noneLightcubeFragmentShaderSource, verticesStruct, indices, true);//������ǵ�ַ��Ҳ���Ǵ������ã����ַ�ʽ������ref
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


CustomModel* LDmodel = new CustomModel(noneLightVertexShaderSource, noneLightcubeFragmentShaderSource, ModelDic["butterfly"].verticesStruct, ModelDic["butterfly"].indices, true);//������ǵ�ַ��Ҳ���Ǵ������ã����ַ�ʽ������ref
///--��������
GLuint picTexture = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assests\\Texture\\1.png");


// ���������������Ƿ�ѭ�����ţ������ٶȣ����ö��������γɵĶ��������ļ�
Animation* anim = new Animation(AnimationDic["butterfly"]["fly"]);
// ��������������
AnimationController* animator = new AnimationController();
// ��������ӵ���������
animator->AddAnimation(anim);


//����󶨷���
  // ʹ����ɫ������
//glUseProgram(LDmodel->shaderProgram);
//glActiveTexture(GL_TEXTURE0);          // ��������Ԫ 0
//glBindTexture(GL_TEXTURE_2D, TextureDic["butterfly"][0]);  // �������������Ԫ 0,�������"butterfly"���ϵ��������
//// ����������Ԫ 0
//glUniform1i(picData, 0);
//--����ģ��
// LDmodel->UpdateVerticesForAnimation(10);//���α����Ч��
 //��������,����������ֱ�ӿ��ƶ������������ָ��
 // animator->PlayAnimation(0,0.03f,LDmodel);


//--OPENGL ������Ⱦ��ʽ

       // 1. ���浱ǰ�� OpenGL ״̬
        GLint previousShader;
        glGetIntegerv(GL_CURRENT_PROGRAM, &previousShader);  // ��ȡ��ǰ����

        GLint previousTexture;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &previousTexture);  // ��ȡ��ǰ�󶨵�����

        // 2. ��Ⱦ����
        cusText->RenderText("Hello World", 100.0f, 100.0f, 10.0f, glm::vec3(1.0f, 1.0f, 1.0f));

        // 3. �ָ�֮ǰ�� OpenGL ״̬
        glUseProgram(previousShader);  // �ָ�֮ǰ����ɫ��
        glBindTexture(GL_TEXTURE_2D, previousTexture);  // �ָ�֮ǰ�������



        //  Cube* newCube1 = new Cube(cubeVertexShaderSource, cubeFragmentShaderSource);//��������ʵ�ֲ����滻
// newCube1->Initialize(glm::vec3(2.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, 45.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f));
//// newCube1->SetVariant(ModelClass::CubeE);
// manager->RegisterObject(newCube1);
// scripts->TChangeRandom(-0.01f, 0.01f);//�ı乹���������

 Cube* newCube2 = new Cube(cubeVertexShaderSource, cubeFragmentShaderSource);//��������ʵ�ֲ����滻
 newCube2->Initialize(glm::vec3(.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(0.1f, 0.10f, .10f));
 // newCube1->SetVariant(ModelClass::CubeE);
 manager->RegisterObject(newCube2);
 // std::list<Cube*> cubes;  // ʹ�� std::list �洢 Cube ����ָ��//�ۺϽű���������
 for (int i = 0; i < 0; i++)
 {
     Cube* newCubeClone = new Cube(cubeVertexShaderSource, cubeFragmentShaderSource);
     float random = scripts->TGetRandomFloat(-1.0f, 1.0f);
     newCubeClone->Initialize(glm::vec3(random, random, random), glm::quat(glm::vec3(random * 5, random * 10, random * 10)), glm::vec3(0.8f, 0.8f, 0.8f));
     manager->RegisterObject(newCubeClone);//����������ע����
 }

 for (int i = 0; i < 0; i++)
 {
     Cube* newCubeClone = new Cube(cubeVertexShaderSource, cubeFragmentShaderSource);
     float random = scripts->TGetRandomFloat(-1.0f, 1.0f);
     newCubeClone->Initialize(glm::vec3(random, random, random), glm::quat(glm::vec3(random * 5, random * 10, random * 10)), glm::vec3(1.5f, 1.5f, 1.5f));
     newCubeClone->SetVariant(ModelClass::CubeE);
     manager->RegisterObject(newCubeClone);
 }
//---���ｫGanmeObject ����������������ƶ���ת��ΪCustomModel �࣬����һ���Ϳ���ִ���µĶ����˶���ģʽ������֮ǰ��GameObject ����
//---  �����������������ԭʼ����
        for (int i = 0; i < 30; i++)
        {
            CustomModel* LDmodel = new CustomModel(noneLightVertexShaderSource, noneLightcubeFragmentShaderSource, ModelDic["butterfly"], true);//������ǵ�ַ��Ҳ���Ǵ������ã����ַ�ʽ������ref
            manager->RegisterObject(LDmodel);
            LDmodel->Initialize(glm::vec3(2.0f, 0.0f, i * 0.5f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(0.1f, 0.1f, 0.1f));
            //--ģ�͸��Ӳ������   
            LDmodel->AttachTexture();
            //--ģ�͸�������������
            LDmodel->AttachAnimationController(AnimationDic["butterfly"]["fly"]);//���ﴫ��һ��Ĭ�ϲ���������֪������Ķ������ƺ��׶���
            LDmodel->AttachPhysicalEngine();
            LDmodel->AttachCollider();
            auto* aa = LDmodel->GetComponent<AnimationController>();
            DEBUGLOG(aa);
            //��ʼ������          
            LDmodel->SetVariant(ModelClass::ActorButterfly);
        }

        //--��ʼ�ĵƹⶨ�巽ʽ
        CustomPointLight pointLight;
        pointLight.position = glm::vec3(0.0f, 0.0f, 0.0f);
        pointLight.color = glm::vec3(1.0f, .0f, .0f);
        pointLight.intensity = 5.0f;

        GLuint lightshader = customCone->shaderProgram;
        GLuint lightPosLoc = glGetUniformLocation(lightshader, "lightPos");//��ȡshader�е���صĲ�����������������������ã���Ϊ����һ��shader ���Ըı�һ�����ڸı�ȫ��
        GLuint lightColorLoc = glGetUniformLocation(lightshader, "lightColor");
        GLuint lightIntensityLoc = glGetUniformLocation(lightshader, "lightIntensity");

// -- ���Դ��Ⱦ���ƴ���

         else if (item->GetVariant() == ModelClass::TsetButterfly)
         {

             //����Ҳ����ִ�б��巽������������֤
             item->UpdateVariant(view, projection);
             item->PlayAnimation(0, 0.1f);



             //--
             // ʹ�ø������ shader ����
             glUseProgram(item->shaderProgram);

             // ������������ݣ������� controller->front ��ʾ�����ǰ���λ�����ݣ��ɸ���ʵ������滻��
             GLuint viewPosLoc = glGetUniformLocation(item->shaderProgram, "viewPos");
             glm::vec3 viewP = glm::vec3(controller->front);
             glUniform3f(viewPosLoc, viewP.x, viewP.y, viewP.z);


             // ������Դ����
             auto pointLights = lightController->GetPointLights();
             for (int i = 0; i < MAX_LIGHTS; i++)
             {
                 // ƴ�� uniform ��������
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
                     // û�и�������Դ������ΪĬ��ֵ
                     glUniform3f(posLoc, 0.0f, 0.0f, 0.0f);
                     glUniform3f(colorLoc, 0.0f, 0.0f, 0.0f);
                     glUniform1f(intensityLoc, 0.0f);
                 }
             }

             // ����ƽ�й�����
             // ����ƽ�й�����ɫ���ж�Ӧ uniform ����Ϊ��
             //   parallelLightDirection, parallelLightColor, parallelLightIntensity
             Game::CustomParallelLight parallelLight = lightController->GetParallelLight();
             GLuint parallelDirLoc = glGetUniformLocation(item->shaderProgram, "parallelLightDirection");
             GLuint parallelColorLoc = glGetUniformLocation(item->shaderProgram, "parallelLightColor");
             GLuint parallelIntensityLoc = glGetUniformLocation(item->shaderProgram, "parallelLightIntensity");
             glUniform3f(parallelDirLoc, parallelLight.direction.x, parallelLight.direction.y, parallelLight.direction.z);
             glUniform3f(parallelColorLoc, parallelLight.color.x, parallelLight.color.y, parallelLight.color.z);
             glUniform1f(parallelIntensityLoc, parallelLight.intensity);

             // �����ֵ�Ͳ������
             // ���趨����һ���� MAX_FLASHLIGHTS (���� #define MAX_FLASHLIGHTS 2) ������ɫ���ж��������� uniform ���飺
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

             //--����ԴӰ�� �򵥴���
             glUseProgram(item->shaderProgram);

             // ��ȡÿ������Ĺ�Դ����λ��
             GLuint lightPosLoc = glGetUniformLocation(item->shaderProgram, "lightPos");
             GLuint lightColorLoc = glGetUniformLocation(item->shaderProgram, "lightColor");
             GLuint lightIntensityLoc = glGetUniformLocation(item->shaderProgram, "lightIntensity");
             GLuint viewPos = glGetUniformLocation(item->shaderProgram, "viewPos");
             glm::vec3 viewP = glm::vec3(controller->front);

             // ���ù�Դ����
             glUniform3f(lightPosLoc, pointLight.position.x, pointLight.position.y, pointLight.position.z);
             glUniform3f(lightColorLoc, pointLight.color.x, pointLight.color.y, pointLight.color.z);
             glUniform3f(viewPos, viewP.x, viewP.y, viewP.z);
             glUniform1f(lightIntensityLoc, pointLight.intensity);


             //--�������ɵ�������ģ��
             // ����С������ widget �ķ���ʵ��
            //void  Controller::RenderWidget(const char* widgetVertexShaderSource, const char* widgetFragmentShaderSource,
            //     GLuint widgetVAO, int widgetSize, int widgetPosX, int widgetPosY)
            // {
            //     // ע�⣺����ÿ�ε��ö����½�һ�� Shader ���������������뿼��Ԥ�ȹ��첢����ö���
            //     Shader widgetShader(widgetVertexShaderSource, widgetFragmentShaderSource);

            //     // ������Ȳ��ԣ�ȷ�� widget ������ǰ��
            //     glDisable(GL_DEPTH_TEST);

            //     // ���浱ǰ�ӿ�����
            //     GLint viewport[4];
            //     glGetIntegerv(GL_VIEWPORT, viewport);

            //     // ���� widget ���ӿ��������� widgetPosX �� widgetPosY Ϊ widget ���½�λ�ã�
            //     glViewport(widgetPosX, widgetPosY, widgetSize, widgetSize);

            //     // ʹ������ͶӰ�����귶Χ�̶��� [-1, 1]
            //     glm::mat4 widgetProjection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -10.0f, 10.0f);

            //     // ��ȡ��ǰ�������ͼ�����е���ת����
            //     glm::mat4 viewMat = GetViewMatrix();
            //     glm::mat3 camRot = glm::mat3(viewMat);
            //     // ������������ԣ�ת�ü�Ϊ����󣬴Ӷ���ԭ���������ת
            //     glm::mat3 widgetRot = glm::transpose(camRot);
            //     glm::mat4 widgetModel = glm::mat4(widgetRot);

            //     // ���� widget �� MVP ����
            //     glm::mat4 widgetMVP = widgetProjection * widgetModel;

            //     // ���� widget ��ɫ���������� MVP uniform
            //     widgetShader.use();
            //     widgetShader.setMat4("MVP", widgetMVP);

            //     // ���� widget �����ᣨ���� widgetVAO ���� 6 �����㣬����λ�ú���ɫ���ԣ�
            //     glBindVertexArray(widgetVAO);
            //     glDrawArrays(GL_LINES, 0, 6);
            //     glBindVertexArray(0);

            //     // �ָ�ԭʼ�ӿ����ò�������Ȳ���
            //     glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
            //     glEnable(GL_DEPTH_TEST);
            // }


             ////�������ö�����������̳�CustomModel����ֱ�ӿ�չ���巽����ģ�¶����ű���������֤
auto* axisWidget = new CoordinateSystemCus(
    axisVertexShaderSource,        // ������ widget ������ɫ��Դ��
    axisFragmentShaderSource,      // ������ widget ƬԪ��ɫ��Դ��
    meshData->axisWidgetVertices,        // ������� meshData �б��������� axisWidgetVertices ����
    meshData->axisWidgetIndices,         // ������� meshData �б����� axisWidgetIndices ���ݣ���ѡ��
    meshData->axisWidgetVertexCount,     // 36
    meshData->axisWidgetIndexCount,      // 6
    false,
    false// ������ widget ͨ������Ҫ����
);
axisWidget->SetVariant(ModelClass::AxisWidget);
axisWidget->Initialize(glm::vec3(0.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(1.0f));
manager->RegisterObject(axisWidget);

//--�����ǹ�����������







    //GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //glShaderSource(vertexShader, 1, &axisVertexShaderSource, nullptr);
    //glCompileShader(vertexShader);
    //// �˴�Ӧ�������Զ���� CheckShaderCompilation(vertexShader);
    //// ��鶥����ɫ�������Ƿ�ɹ�

    //// ����ƬԪ��ɫ��
    //GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //glShaderSource(fragmentShader, 1, &axisFragmentShaderSource, nullptr);
    //glCompileShader(fragmentShader);
    //// �˴�Ӧ���� CheckShaderCompilation(fragmentShader);
    //// ���ƬԪ��ɫ�������Ƿ�ɹ�

    //// ���������ӳ���
    //widgetShaderProgram = glCreateProgram();
    //glAttachShader(widgetShaderProgram, vertexShader);
    //glAttachShader(widgetShaderProgram, fragmentShader);
    //glLinkProgram(widgetShaderProgram);
    //// �˴�Ӧ���� CheckShaderProgramLinking(widgetShaderProgram);
    //// �����ɫ�����������Ƿ�ɹ�

    //// ɾ����ʱ shader ����
    //glDeleteShader(vertexShader);
    //glDeleteShader(fragmentShader);

bool CustomModel::DrawLineFixedWidgetObsolete(glm::mat4 view, glm::mat4 projection)
{
    //�����ӿ���Ⱦ
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    // �̶�����������������½����򣬳ߴ� 150x150 ����
    int widgetPosX = 0;
    int widgetPosY = 0;
    int widgetSize = 300;

    // �л��ӿڵ� widget ����
    glViewport(widgetPosX, widgetPosY, widgetSize, widgetSize);

    // ���� widget ʹ�õ�����ͶӰ�������귶Χ [-1, 1]
    glm::mat4 widgetProjection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -10.0f, 10.0f);

    // ��ȡ���������ͼ����ͨ�� Controller ��ȡ����������ȷ���ã�
    glm::mat4 mainView = view;

    // ������ͼ����ȡ��ת����
    glm::mat3 camRot = glm::mat3(mainView);
    glm::mat3 widgetRot = glm::transpose(camRot); // ת�ûָ��������ת
    glm::mat4 widgetModel = glm::mat4(widgetRot);

    // �̶� widget �� view Ϊ��λ����ʹ widget ���������ƽ��Ӱ��
    glm::mat4 widgetView = glm::mat4(1.0f);

    //

// ���ǰ����ɫ������
    glUseProgram(shaderProgram);

    // ��ȡ shader �� "model"��"view" �� "projection" uniform ������λ��
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");

    // ����任���� shader������������Ϊ GL_FALSE ��ʾ������Ҫת�ã�
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // �󶨵�ǰģ�͵� VAO
    glBindVertexArray(VAO);

    // ֱ��ʹ�� GL_LINES ģʽ�����߶�
    glDrawElements(GL_LINES, index, GL_UNSIGNED_INT, 0);

    // ��� VAO
    glBindVertexArray(0);

    // �ָ�ԭ�����ӿ�����
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

    return true;
}

//CUSTOMMOdel �� ATTACHTEXTURE ����
////--- ����״̬����д
 //glGetIntegerv(GL_CURRENT_PROGRAM, &previousShader);  // ��ȡ��ǰ����
 //glGetIntegerv(GL_TEXTURE_BINDING_2D, &previousTexture);  // ��ȡ��ǰ�󶨵�����
 //--
// glUseProgram(shaderProgram);
glUseProgram(shaderProgram);
GLuint picData = glGetUniformLocation(shaderProgram, "texture1");//Ԥд��ͼ���shader��������
glActiveTexture(GL_TEXTURE0 + order);          // ��������Ԫ 0
glBindTexture(GL_TEXTURE_2D, textureName);  // �������������Ԫ 0,�������"butterfly"���ϵ��������
// ����������Ԫ 0
glUniform1i(picData, order);

//// 3. д��֮ǰ��shaderprogram
//glUseProgram(previousShader);  // �ָ�֮ǰ����ɫ��
//glBindTexture(GL_TEXTURE_2D, previousTexture);  // �ָ�֮ǰ�������

return true;



//-- �������������Ϸ����ļ�������ķ��������Զ���Ϊ�����ģ���࣬�ɲ���GetComponenet<T>�ȷ��ͷ�ʽ�������������ÿ��ģ���漰��ͬ���������Ҫ��ɢ��ÿ�����������
       // 

       //���ֻ���������������Ҫ�������ɻ����Ĳ���ģ��
CustomModel* customCone = new CustomModel(colorlightsArrayVertexShaderSource, colorlightsArraySourceFragmentShaderSource, meshData->cylinderVertices, meshData->cylinderIndices, meshData->cylinderVertexCount, meshData->cylinderIndexCount);
customCone->SetVariant(ModelClass::CubeTestE);
customCone->Initialize(glm::vec3(2.0f, 0.0f, 1.0f), glm::quat(glm::vec3(0.0f, 45.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f));
manager->RegisterObject(customCone);

CustomModel* customSphere = new CustomModel(colorlightsArrayVertexShaderSource, colorlightsArraySourceFragmentShaderSource, meshData->colorCubeVertices, meshData->colorCubeIndices, meshData->colorCubeVertexCount, meshData->colorCubeIndexCount, true);
customSphere->SetVariant(ModelClass::CubeTestE);
customSphere->Initialize(glm::vec3(-2.0f, 0.0f, -1.0f), glm::quat(glm::vec3(0.0f, 45.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f));
manager->RegisterObject(customSphere);
//----

Game::Cube::CubeObstcle(GLuint textureName)
{
    _ifCubeMap = true;//������պб�ʶ
    _cubeMapID = textureName;
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &skyboxVertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    CheckShaderCompilation(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &skyboxFragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    CheckShaderCompilation(fragmentShader);

    //������Ǵ�������shader  һ������ һ��ƬԪ��Ȼ��Դ��������shader ���б��� glcompileShaer,����һ���Զ����鷽�������shader�����Ƿ�ɹ���nullptr 
    //C�����ַ�����\0��β,�м������1�������ַ����������Դ���� ����ǰ1������ж����Ϊconst GLchar* sources[] = { source1, source2 }; glShaderSource(shader, 2, sources, nullptr);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    CheckShaderProgramLinking(shaderProgram);  // �����ɫ�������Ƿ����ӳɹ�����Ҳ��һ���Զ���ķ������������������shader ���ӵ���Ŀ�У��������ӵ���Ŀ������

    glUseProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    //��һ�����֮�󣬾Ϳ���ʹ�������Ŀ�ˣ�ͬʱҲ����ɾ�������ƬԪ��ɫ����

    glGenVertexArrays(1, &_skyboxVAO);
    glGenBuffers(1, &_skyboxVBO);
    glBindVertexArray(_skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, _skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_skyboxVertices), _skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);



    glGenTextures(1, &_cubeMapID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeMapID);

    GLint width, height;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

    for (GLuint i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

}



//��ǰ���õı���������ײ�߼�
if (false)
{
    for (auto& pair : cop)
    {
        if (_collisionProperties.ID != pair.first)
        {
            //������Ը�����ײ����������ײ����

            CheckCollisionWithAABB(pair.second);//�������ã��������Ҫʹ��*����ֵ��������ָ��Ͳ���Ҫ
        }
    }
}



// ���������������Ŀ��Ĵ���
float minDistSq = std::numeric_limits<float>::max();
glm::vec3 closestObjPos;
bool found = false;

// ��������������ò�������ƽ�����ȣ�dot(diff, diff)���ж���С����
for (auto& item : _manager->GetVariantObjects())
{
    // �ж�Ŀ�����ͣ��˴�����ѡ TestPhysics ���ͣ����ڿ���չΪ����ȣ�
    if (item.second->GetVariant() == ModelClass::TestPhysics)
    {
        glm::vec3 diff = item.second->position - position;
        float distSq = glm::dot(diff, diff);  // ֱ�ӱȽ�ƽ�����룬���� sqrt
        if (distSq < minDistSq)
        {
            minDistSq = distSq;
            closestObjPos = item.second->position;
            found = true;
        }
    }
}

if (found)
{
    // ������ҵ�Ŀ��ķ��򣬺��� Y �ᣨֻ����ˮƽ�棩
    glm::vec3 dir = closestObjPos - position;
    dir.y = 0.0f;
    // ��鷽�����������Ƿ��㹻����ֹ�� 0
    if (glm::dot(dir, dir) > 0.0001f)
    {
        dir = glm::normalize(dir);
        // ���� atan2 ���� yaw �Ƕȣ�ע�⣺0���ȶ�ӦĬ��ǰ�� (0,0,-1)��
        float yaw = glm::atan(dir.x, -dir.z);
        // ���� yaw �Ƕ������� Y ����ת����Ԫ����ʹ���ֱ������Ŀ��
        rotation = glm::angleAxis(yaw, glm::vec3(0, 1, 0));
    }

}


//���Ա��伯�ɼ���
for (int i = 0; i < 1; i++)
{
    auto* chest = new DesignModel("noneLight", ModelDesignDic["chest"], false, true, true);
    chest->SetVariant(ModelClass::CubeE);
    chest->Initialize(glm::vec3(20, 0, 10), glm::quat(glm::vec3(0.0f, 45.0f, 0.0f)), glm::vec3(10));
    manager->RegisterObject(chest);
    chest->AttachTexture(TextureDic["chest"], 0);

}

/
#endif