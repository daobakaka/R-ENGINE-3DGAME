#include "CustomModel.h"
using namespace Game;
CustomModel::CustomModel(const char* vertexShaderSourceIn, const char* fragmentShaderSourceIn, GLfloat *vertices, GLuint *indices, size_t vertexCount, size_t indexCount,bool ifLightIn)
{
    
    index = indexCount; 
    ifLight = ifLightIn;
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSourceIn, nullptr);
    glCompileShader(vertexShader);
    CheckShaderCompilation(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceIn, nullptr);
    glCompileShader(fragmentShader);
    CheckShaderCompilation(fragmentShader);

    //这里就是传入两个shader  一个顶点 一个片元，然后对代码的两个shader 进行编译 glcompileShaer,后面一个自定义检查方法，检查shader编译是否成功，nullptr 
    //C风格的字符串以\0结尾,中间的数字1，代表字符串数组的资源数量 ，当前1个如果有多个则为const GLchar* sources[] = { source1, source2 }; glShaderSource(shader, 2, sources, nullptr);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    CheckShaderProgramLinking(shaderProgram);  // 检查着色器程序是否链接成功，这也是一个自定义的方法，这里代表将创建的shader 附加到项目中，并且链接到项目程序中

    glUseProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    //这一步完成之后，就可以使用这个项目了，同时也可以删除顶点和片元着色器，
    // 
    // 
    // 创建和绑定VAO, VBO, EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    //绑定顶点缓冲数组
    //这里先生成图形所需要的 顶点数组 和缓冲buffer，包括顶点缓冲对象VBO，顶点数组对象VAO，元素缓冲对象 EBO

    // VBO: Vertex Buffer Object (顶点缓冲对象)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);//GL_ARRAY_BUFFER 0x8892 数组缓冲对象是定义到固定的内存地址，绑定缓冲
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(GLfloat), vertices, GL_STATIC_DRAW);//GL_STATIC_DRAW 0x88E4   静态绘制区域

    

    // EBO: Element Buffer Object (元素缓冲对象)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);// GL_ELEMENT_ARRAY_BUFFER 0x8893  绑定元素缓冲对象，或者叫索引缓冲对象，储存数组的索引信息
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLuint), indices, GL_STATIC_DRAW);//GL_STATIC_DRAW 0x88E4   静态绘制区域

    //这里绘制需要的数据 都必须进入到 静态绘制区域才行，这样才能绘制，索引数组和顶点数组都在 cube.h的头文件中定义


    // 设置顶点属性指针，实际上就是告诉 OpenGL，缓冲区中存储的顶点属性数据包含 3 个浮点数 的位置信息，存储在每 3 个浮点数（有颜色则为6）的起始位置，且位置信息从偏移量 0 开始。
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);//这是启用顶点着色器中 layout(location = 0) 的属性数据。
    //----

    glBindVertexArray(0);  //1 解绑当前绑定的 VAO，防止后续对 VAO 的不必要修改 2 解绑后，VAO 的配置被保存在 GPU 中，后续只需重新绑定即可使用
}

CustomModel::CustomModel(const char* vertexShaderSourceIn, const char* fragmentShaderSourceIn, GLfloat* vertices, GLuint* indices, size_t vertexCount, size_t indexCount,int ifVariant ,bool ifLightIn)
{
    index = indexCount;
    ifLight = ifLightIn;

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSourceIn, nullptr);
    glCompileShader(vertexShader);
    CheckShaderCompilation(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceIn, nullptr);
    glCompileShader(fragmentShader);
    CheckShaderCompilation(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    CheckShaderProgramLinking(shaderProgram);



    glUseProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Bind VBO and upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    // Bind EBO and upload index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLuint), indices, GL_STATIC_DRAW);

    // 8. 顶点属性：位置 (location=0)
     //    注意 stride = 6 * sizeof(GLfloat)
     //          offset = 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(0));
    glEnableVertexAttribArray(0);

    // 9. 顶点属性：法线 (location=1)
    //    offset = 3 * sizeof(GLfloat)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

CustomModel::CustomModel(const char* vertexShaderSourceIn, const char* fragmentShaderSourceIn,std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& normals,bool ifLightIn)
{
    
    justDrawVerteies = true;
    index = vertices.size();  // 获取索引的数量，即顶点索引的数量
    ifLight = ifLightIn;
    // Create vertex and fragment shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSourceIn, nullptr);
    glCompileShader(vertexShader);
    CheckShaderCompilation(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceIn, nullptr);
    glCompileShader(fragmentShader);
    CheckShaderCompilation(fragmentShader);

    // Create shader program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    CheckShaderProgramLinking(shaderProgram);

    glUseProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
//编译着色器，完成之后就可以清楚Gluint 缓存了
    // Generate buffers and arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
 /*   glGenBuffers(1, &EBO);*/

    glBindVertexArray(VAO);

    // Create and bind VBO for vertices (position data)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    //因为没有进行顶点复用，所以，这里的索引数组就不再被需要了
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, index * sizeof(GLuint), &vertices[0], GL_STATIC_DRAW);

    // Set up position attribute (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Create and bind VBO for texture coordinates (uvs)
    glGenBuffers(1, &uvsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, uvsVBO);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);//静态网格的缓冲区数据渲染未 Static，若需要动态蒙皮渲染，则更新为 dynamical

    // Set up texture coordinate attribute (location = 1)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid*)0);//(GLvoid*)0 当前VOB里面的偏移量
    glEnableVertexAttribArray(1);

    // Create and bind VBO for normals
    glGenBuffers(1, &normalsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

    // Set up normal attribute (location = 2)
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);  // Unbind VAO
}

CustomModel::CustomModel(const char* vertexShaderSourceIn, const char* fragmentShaderSourceIn, std::vector<Vertex>& vertices, std::vector<unsigned int>& finalIndices,bool ifLightIn)
{
    vertexCount = vertices.size();
    index = finalIndices.size();
    verticesTras = vertices;
    ifLight = ifLightIn;
   


    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSourceIn, nullptr);
    glCompileShader(vertexShader);
    CheckShaderCompilation(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceIn, nullptr);
    glCompileShader(fragmentShader);
    CheckShaderCompilation(fragmentShader);

    // 2) 创建着色器程序
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    CheckShaderProgramLinking(shaderProgram);

    // 删除临时Shader
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 3) 生成 VAO, VBO, EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);


    // 4) 绑定VAO
    glBindVertexArray(VAO);

    // 5) VBO: 传入所有顶点
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
        vertices.size() * sizeof(Vertex),
        vertices.data(),
        GL_STATIC_DRAW);

    // 6) EBO: 传入索引
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        finalIndices.size() * sizeof(unsigned int),
        finalIndices.data(),
        GL_STATIC_DRAW);

    // 7) 设置顶点属性指针
    // layout(location=0): position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, Position));

    // layout(location=1): texcoords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, TexCoords));

    // layout(location=2): normal
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, Normal));

    // 解绑VAO
    glBindVertexArray(0);

}

CustomModel::CustomModel(const char* vertexShaderSourceIn, const char* fragmentShaderSourceIn, std::vector<Vertex>& vertices, std::vector<unsigned int>& finalIndices,bool isSkinnedMesh, bool ifLightIn)
{
    //verticesTras = new std::vector<Vertex>(vertices);  // 这种才是指针的有效初始化，析构中才能使用delete
    vertexCount = vertices.size();
    index = finalIndices.size();
    verticesTras = vertices;
    IsSkinnedMesh = isSkinnedMesh;
    ifLight = ifLightIn;

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSourceIn, nullptr);
    glCompileShader(vertexShader);
    CheckShaderCompilation(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceIn, nullptr);
    glCompileShader(fragmentShader);
    CheckShaderCompilation(fragmentShader);

    // 2) 创建着色器程序
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    CheckShaderProgramLinking(shaderProgram);

    // 删除临时Shader
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 3) 生成 VAO, VBO, EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);


    // 4) 绑定VAO
    glBindVertexArray(VAO);

    // 5) VBO: 传入所有顶点
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
        vertices.size() * sizeof(Vertex),
        vertices.data(),
        GL_DYNAMIC_DRAW);

    // 6) EBO: 传入索引
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        finalIndices.size() * sizeof(unsigned int),
        finalIndices.data(),
        GL_DYNAMIC_DRAW);

    // 7) 设置顶点属性指针
    // layout(location=0): position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, Position));

    // layout(location=1): texcoords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, TexCoords));

    // layout(location=2): normal
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, Normal));

    // 解绑VAO
    glBindVertexArray(0);

}

CustomModel::CustomModel(const char* vertexShaderSourceIn, const char* fragmentShaderSourceIn, const ModelData &modelData,bool isSkinnedMesh, bool ifLightIn)
{
    //verticesTras = new std::vector<Vertex>(vertices);  // 这种才是指针的有效初始化，析构中才能使用delete
    vertexCount = modelData.verticesStruct.size();
    index = modelData.indices.size();
    verticesTras = modelData.verticesStruct;
    IsSkinnedMesh =isSkinnedMesh;
    ifLight = ifLightIn;

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSourceIn, nullptr);
    glCompileShader(vertexShader);
    CheckShaderCompilation(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceIn, nullptr);
    glCompileShader(fragmentShader);
    CheckShaderCompilation(fragmentShader);

    // 2) 创建着色器程序
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    CheckShaderProgramLinking(shaderProgram);

    // 删除临时Shader
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 3) 生成 VAO, VBO, EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);


    // 4) 绑定VAO
    glBindVertexArray(VAO);

    // 5) VBO: 传入所有顶点
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
        modelData.verticesStruct.size() * sizeof(Vertex),
        modelData.verticesStruct.data(),
        GL_DYNAMIC_DRAW);

    // 6) EBO: 传入索引
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        modelData.indices.size() * sizeof(unsigned int),
        modelData.indices.data(),
        GL_DYNAMIC_DRAW);

    // 7) 设置顶点属性指针
    // layout(location=0): position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, Position));

    // layout(location=1): texcoords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, TexCoords));

    // layout(location=2): normal
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, Normal));

    // 解绑VAO
    glBindVertexArray(0);

}

bool CustomModel::DrawDynamical(glm::mat4 view, glm::mat4 projection)
{
   
    //集成纹理渲染方法,要在draw之前调用，才能生效
    RenderingTexture();
    
   // glUseProgram(shaderProgram);

    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    //这里是对 三个定义的 参数进行抓取，在shaderProgram 里面，这个参数是写在对应的shader里面进行定义的
    glUniformMatrix4fv(modelLoc, 1, 0, glm::value_ptr(transform));
    glUniformMatrix4fv(viewLoc, 1, 0, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, 0, glm::value_ptr(projection));
    //这里的glm是一个常用的数学库，专门用与图形的矩阵操作，这里就充分诠释了 CPU之负责传输数据，GPU 来负责传输图形，
    // shader语言里面 使用 gl_Position = projection * view * model * vec4(aPos, 1.0f);进行渲染计算 
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
  //  glBufferSubData(GL_ARRAY_BUFFER, 0,vertex * sizeof(glm::vec3), (void*)&((*verticesTras)[0].Position));//指向容器的数据
    glBufferData(GL_ARRAY_BUFFER,
        verticesTras.size() * sizeof(Vertex),
        verticesTras.data(),
        GL_DYNAMIC_DRAW);
    //绑定顶点数组对象（VAO），激活与 VAO 关联的顶点缓冲对象（VBO）和索引缓冲对象（EBO）。
    //保存顶点属性的配置，例如位置、颜色、法线、纹理坐标等，与 VBO 和 EBO 关联，用于存储顶点数据和绘制索引。
    justDrawVerteies == true ? glDrawArrays(GL_TRIANGLES, 0, index) : glDrawElements(GL_TRIANGLES, index, GL_UNSIGNED_INT, 0); 
    //索引数组存储在 EBO（Element Buffer Object）中，EBO 已绑定到 VAO，三角形，36个索引，无符号整形数组，由0开始
    glBindVertexArray(0);
    //集成纹理渲染方法
  //  RenderingTexture();


    return true;
}
/// <summary>
/// attach一次进行有效传值，将方法集成到draw或者drawDynamicl里
/// </summary>
/// <param name="textureName"></param>
/// <param name="order"></param>
/// <returns></returns>
bool CustomModel::AttachTexture(GLuint textureName, int order)//opengl 采用的是状态机模式
{  

    texture = textureName;
    textureOrder = order;
   // RenderingTexture();
    return true;
}
void CustomModel::RenderingTexture()
{

    //这里传入的order 应该是从前面每一个纹理单元都进行更改，也就是大的order 可以覆盖小的order
    glUseProgram(shaderProgram);
    GLuint picData = glGetUniformLocation(shaderProgram, "texture1");//预写入图像的shader定义内容
    glActiveTexture(GL_TEXTURE0 + textureOrder);          // 激活纹理单元 0+order
    glBindTexture(GL_TEXTURE_2D,texture );  // 绑定纹理对象到纹理单元 0+order,这里添加DicTexture集合的纹理对象
    // 绑定纹理到纹理单元 0+order，这个顺序的所有纹理单元都遍历绑定一次
    glUniform1i(picData, textureOrder);
}
void CustomModel::AttachAnimationController(AnimationData animationData)
{
    if (animator == nullptr)
        animator = new AnimationController();

    Animation* anim = new Animation(animationData);
    animator->AddAnimation(anim);
    
}
void CustomModel::AttachAnimationController()
{
}
void CustomModel::PlayAnimation(int index, float frame)
{

    animator->PlayAnimation(index, frame, this);

}
void CustomModel::StopPlayAnimation()
{

    animator->StopAnimation();
}
bool CustomModel::AttachPhysicalEngine()
{
    return false;
}
bool CustomModel::AttachCollider()
{
    return false;
}
bool CustomModel::Draw (glm::mat4 view, glm::mat4 projection)
{
    //集成纹理渲染方法,要在draw之前调用，才能生效
    RenderingTexture();
    
   // glUseProgram(shaderProgram);
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    //这里是对 三个定义的 参数进行抓取，在shaderProgram 里面，这个参数是写在对应的shader里面进行定义的
    glUniformMatrix4fv(modelLoc, 1, 0, glm::value_ptr(transform));
    glUniformMatrix4fv(viewLoc, 1, 0, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, 0, glm::value_ptr(projection));
    //这里的glm是一个常用的数学库，专门用与图形的矩阵操作，这里就充分诠释了 CPU之负责传输数据，GPU 来负责传输图形，
    // shader语言里面 使用 gl_Position = projection * view * model * vec4(aPos, 1.0f);进行渲染计算 
    glBindVertexArray(VAO);
    //绑定顶点数组对象（VAO），激活与 VAO 关联的顶点缓冲对象（VBO）和索引缓冲对象（EBO）。
    //保存顶点属性的配置，例如位置、颜色、法线、纹理坐标等，与 VBO 和 EBO 关联，用于存储顶点数据和绘制索引。
    justDrawVerteies == true ? glDrawArrays(GL_TRIANGLES, 0, index) : glDrawElements(GL_TRIANGLES, index, GL_UNSIGNED_INT, 0);
    //索引数组存储在 EBO（Element Buffer Object）中，EBO 已绑定到 VAO，三角形，36个索引，无符号整形数组，由0开始
    glBindVertexArray(0);
    
    //集成纹理渲染方法
  // RenderingTexture();
    return true;
}
/// <summary>
/// 绘制线段
/// </summary>
/// <param name="view"></param>
/// <param name="projection"></param>
/// <returns></returns>
bool CustomModel::DrawLine(glm::mat4 view, glm::mat4 projection)
{
    
    // 激活当前的着色器程序
    glUseProgram(shaderProgram);

    // 获取 shader 中 "model"、"view" 和 "projection" uniform 变量的位置
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");

    // 传入变换矩阵到 shader（第三个参数为 GL_FALSE 表示矩阵不需要转置）
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // 绑定当前模型的 VAO
    glBindVertexArray(VAO);

    // 直接使用 GL_LINES 模式绘制线段
    glDrawElements(GL_LINES, index, GL_UNSIGNED_INT, 0);

    // 解绑 VAO
    glBindVertexArray(0);


    return true;
}
/// <summary>
/// 分GL窗口，单独绘制线段
/// </summary>
/// <param name="view"></param>
/// <param name="projection"></param>
/// <returns></returns>
bool CustomModel::DrawLineFixedWidget(glm::mat4 view, glm::mat4 projection)
{
    //增加视口渲染
  //  glDisable(GL_DEPTH_TEST);
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    // 固定区域参数：例如左下角区域，尺寸 150x150 像素
    int widgetPosX = 300;
    int widgetPosY = 300;
    int widgetSize = 500;

    // 切换视口到 widget 区域
    glViewport(widgetPosX, widgetPosY, 300, 200);

    //auto fixedView = Controller::GetInstance()->GetFixedViewMatrix();
  // auto NProjection= glm::mat4(1.0f);  // 初始化为单位矩阵
  // transform = glm::translate(NProjection, glm::vec3(0,0,-5));  // 平移，这里对物体平移的实现事实上是调用了一个GL中 translate的API
   
  //  glDisable(GL_DEPTH_TEST);
// 激活当前的着色器程序
    glUseProgram(shaderProgram);

    // 获取 shader 中 "model"、"view" 和 "projection" uniform 变量的位置
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");

    // 传入变换矩阵到 shader（第三个参数为 GL_FALSE 表示矩阵不需要转置）
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // 绑定当前模型的 VAO
    glBindVertexArray(VAO);

    // 直接使用 GL_LINES 模式绘制线段
    glDrawElements(GL_LINES, index, GL_UNSIGNED_INT, 0);

    // 解绑 VAO
    glBindVertexArray(0);

    
    // 恢复原来的视口设置
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
   // glEnable(GL_DEPTH_TEST);
    return true;
}

CustomModel::~CustomModel()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    //主对象析构之后，指针貌似已经被释放了
   // delete verticesTras;//释放内存即释放栈上的引用，引用丢失，内存无意义，告诉编译器指向地址可以重新分配
   // verticesTras = nullptr;//避免内存污染，解决潜在问题


    if (animator == nullptr)
        delete animator;
}
void CustomModel::Update(glm::mat4 view, glm::mat4 projection)
{

    //这种写法就，直接封装了更新变化和通知CPU渲染的两个方法，如果具体定义方法，只需要在其他脚本里面单独定义某个方法，更改transform即可，由管理池泛型调用
    UpdateTransform();

   IsSkinnedMesh==true? DrawDynamical(view, projection): Draw(view, projection);
}
//变体方法，可以增加变体的运行行为
void  CustomModel::UpdateVariant(glm::mat4 view, glm::mat4 projection)
{
   // scale = glm::vec3(0.3f, 0.3f, 0.3f);
    position += glm::vec3(0, 0.002f,   0.0f);

}
// 根据动画帧更新顶点数据
void CustomModel::UpdateVerticesForAnimation(size_t animationFrame) {
    // 根据当前动画帧来更新 verticesTras 中的顶点数据
    // 示例：更新位置或其他属性
    for (size_t i = 0; i < verticesTras.size(); ++i) {
        // 根据 animationFrame 更新顶点位置
        // 比如通过插值来更新顶点的 Position、TexCoords 等
        Vertex& vertex = (verticesTras)[i];
        // 这里假设你有某种规律来改变顶点属性
        vertex.Position.x += 0.001f * animationFrame;
        vertex.Position.y += 0.001f * animationFrame;
        vertex.Position.z += 0.001f * animationFrame;
    }
}

void CustomModel::UpdateVerticesForAnimation(const std::vector<Vertex>& vertex)
{
    verticesTras = vertex;
}

//// GetComponent 方法
//template <typename T>
//T* CustomModel::GetComponent()
//{
//    if (std::is_same<T, AnimationController>::value)
//    {
//        return animator;
//    }
//    else if (std::is_same<T, PhysicalEngine>::value)
//    {
//        return PhysicsBody;
//    }
//    else if (std::is_same<T, CollisionBody>::value)
//    {
//        return collider;
//    }
//
//    // 如果找不到匹配的类型，抛出异常或返回 nullptr
//    throw std::runtime_error("Requested component type not found!");
//    // 或者使用返回nullptr的方式：
//    // return nullptr;
//}
//
//// 显式实例化模板
//template AnimationController* CustomModel::GetComponent<AnimationController>();
//template PhysicalEngine* CustomModel::GetComponent<PhysicalEngine>();
//template CollisionBody* CustomModel::GetComponent<CollisionBody>();
