#include "CustomModel.h"
#include "Light.h"
using namespace Game;
extern const char* depthShaderVertexShaderSource;
extern const char* depthShaderFragmentShaderSource;
//声明全局ID 变量的初始值

Game::CustomModel::CustomModel()
{
}
CustomModel::CustomModel(const char* vertexShaderSourceIn, const char* fragmentShaderSourceIn, GLfloat *vertices, GLuint *indices, size_t vertexCount, size_t indexCount,bool ifLightIn)
{

    _index = indexCount; 
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



    // 设置顶点属性指针，实际上就是告诉 OpenGL，缓冲区中存储的顶点属性数据包含 3 个浮点数 的位置信息，存储在每 3 个浮点数（有颜色则为6）的起始位置，且位置信息从偏移量 0 开始。
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);//这是启用顶点着色器中 layout(location = 0) 的属性数据。
    //----

    glBindVertexArray(0);  //1 解绑当前绑定的 VAO，防止后续对 VAO 的不必要修改 2 解绑后，VAO 的配置被保存在 GPU 中，后续只需重新绑定即可使用
}

CustomModel::CustomModel(const char* vertexShaderSourceIn, const char* fragmentShaderSourceIn, GLfloat* vertices, GLuint* indices, size_t vertexCount, size_t indexCount,int ifVariant ,bool ifLightIn)
{

    _index = indexCount;
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
    _index = vertices.size();  // 获取索引的数量，即顶点索引的数量
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

    _vertexCount = vertices.size();
    _index = finalIndices.size();
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
    _vertexCount = vertices.size();
    _index = finalIndices.size();
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

CustomModel::CustomModel(const char* vertexShaderSourceIn, const char* fragmentShaderSourceIn, const ModelData& modelData, bool isSkinnedMesh, bool ifLightIn, bool ifShadow)
{

    _vertexCount = modelData.verticesStruct.size();
    _index = modelData.indices.size();
    verticesTras = modelData.verticesStruct;
    IsSkinnedMesh = isSkinnedMesh;
    ifLight = ifLightIn;
    _ifShadow = ifShadow;

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSourceIn, nullptr);
    glCompileShader(vertexShader);
    CheckShaderCompilation(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceIn, nullptr);
    glCompileShader(fragmentShader);
    CheckShaderCompilation(fragmentShader);

    // 创建着色器程序
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    CheckShaderProgramLinking(shaderProgram);

    // 删除临时Shader
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 生成 VAO, VBO, EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // 绑定VAO
    glBindVertexArray(VAO);

    // 根据是否为动态网格（如骨骼动画网格）决定是否使用动态或静态缓冲区
    GLuint bufferUsage = isSkinnedMesh ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;

    // VBO: 传入所有顶点
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
        modelData.verticesStruct.size() * sizeof(Vertex),
        modelData.verticesStruct.data(),
        bufferUsage);  // 根据是否为动态网格选择缓冲区类型

    // EBO: 传入索引
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        modelData.indices.size() * sizeof(unsigned int),
        modelData.indices.data(),
        bufferUsage);  // 同样根据是否为动态网格选择缓冲区类型

    // 设置顶点属性指针
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, Position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, TexCoords));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, Normal));

    // 解绑VAO
    glBindVertexArray(0);
}


bool CustomModel::DrawDynamical(glm::mat4 view, glm::mat4 projection)
{
   
    glUseProgram(shaderProgram);
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    //这里是对 三个定义的 参数进行抓取，在shaderProgram 里面，这个参数是写在对应的shader里面进行定义的
    glUniformMatrix4fv(modelLoc, 1, 0, glm::value_ptr(transform));
    glUniformMatrix4fv(viewLoc, 1, 0, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, 0, glm::value_ptr(projection));
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
        verticesTras.size() * sizeof(Vertex),
        verticesTras.data(),
        GL_DYNAMIC_DRAW);
    //绑定顶点数组对象（VAO），激活与 VAO 关联的顶点缓冲对象（VBO）和索引缓冲对象（EBO）。
    //保存顶点属性的配置，例如位置、颜色、法线、纹理坐标等，与 VBO 和 EBO 关联，用于存储顶点数据和绘制索引。
    justDrawVerteies == true ? glDrawArrays(GL_TRIANGLES, 0, _index) : glDrawElements(GL_TRIANGLES, _index, GL_UNSIGNED_INT, 0); 
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
bool CustomModel::AttachTexture(const std::unordered_map<PictureTpye, GLuint>& textureData,bool isBatch ,int order, glm::vec2 textureScale) {
    // 检查 order 是否有效
    if (order < 0) {
        return false; // 无效的 order
    }
    _textures = textureData;
    _textureOrder = order;
    _textureScale = textureScale;
    _drawTextureBatch = isBatch;
    return  true;
}
void CustomModel::BindTexture()
{   
        // 传入纹理缩放因子
        GLuint textureScaleLoc = glGetUniformLocation(shaderProgram, "textureScale");
        glUniform2f(textureScaleLoc, _textureScale.x, _textureScale.y);

        GLuint baseTextureLoc = glGetUniformLocation(shaderProgram, "baseTexture");
        glActiveTexture(GL_TEXTURE0+_textureOrder); // 激活纹理单元 0
        glBindTexture(GL_TEXTURE_2D, _textures[BaseP]); // 绑定基础纹理
        glUniform1i(baseTextureLoc, 0); // 传递纹理单元             
 }

void Game::CustomModel::BindTextureAdditional()
{   
    //commonlight 的参数输入留在子类进行，通用类就只有一张纹理图片

}
void Game::CustomModel::UniformParametersInput()
{


}
void Game::CustomModel::UniformParametersChange()
{
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
void Game::CustomModel::UniformParametersIns(glm::vec3 baseColor, glm::vec3 emission, float metallic, float roughness, float opacity, float IOR, float ao, float dissolveThreshold)
{
  
    _baseColor = baseColor;
    _emissionColor = emission;
    _metallic = metallic;
    _roughness = roughness;
    _opacity = opacity;
    _IOR = IOR;
    _ao = ao;
    _dissolveThreshold = dissolveThreshold;    

}

void Game::CustomModel::RenderingLight(LightSpawner* lightSpawner)
{
 
    // 2. 点光源数据
    const auto& pointLights = lightSpawner->GetPointLights();
    std::vector<std::pair<float, int>> pointLightDistances;  // 存储每个点光源到物体的距离平方和索引,新声明的结构
    glm::vec3 objectPosition = position;  // 使用物体的当前位置作为参考

    // 遍历点光源计算距离平方，并且筛选出距离小于等于 1000 的光源
    for (int i = 0; i < pointLights.size(); i++) {
        const auto& light = pointLights[i];
        float distanceSquared = glm::dot(objectPosition - light.position, objectPosition - light.position);
        if (distanceSquared <= 1000.0f) {  // 只选择距离平方小于等于 100 的光源
            pointLightDistances.push_back({ distanceSquared, i });
            //std::cout << i << "距离" << distanceSquared << std::endl;
        }
    }

    // 按照距离平方升序排序
    std::sort(pointLightDistances.begin(), pointLightDistances.end(), [](const std::pair<float, int>& a, const std::pair<float, int>& b) {
        return a.first < b.first;  // 按照距离平方升序排序
        });
    //不能动态传入数组数量，shader里面不支持，数组索引需写死，但是可以通过常量来约束 
    GLuint numPointLightsLoc = glGetUniformLocation(shaderProgram, "numPointLights");
    glUniform1i(numPointLightsLoc, std::min(static_cast<int>(pointLightDistances.size()), 4));

    // 只选择最近的 4 个点光源
    for (int i = 0; i < std::min(4, static_cast<int>(pointLightDistances.size())); i++) {
        int index = pointLightDistances[i].second;  // 获取最近的点光源的索引，第一个值是距离，第二个值是距离
        auto light = pointLights[index];
        //  std::cout << "点光源强度" << light.intensity << std::endl;
          // 计算距离平方并传递到 shader
        float distanceSquared = pointLightDistances[i].first;
        //  std::cout <<i<< "距离" << distanceSquared << std::endl;
        std::string posName = "lightPos[" + std::to_string(i) + "]";
        std::string colorName = "lightColor[" + std::to_string(i) + "]";
        std::string intensityName = "lightIntensity[" + std::to_string(i) + "]";
        std::string distanceSquaredName = "lightDistanceSquared[" + std::to_string(i) + "]"; // 新增变量

        GLuint posLoc = glGetUniformLocation(shaderProgram, posName.c_str());
        GLuint colorLoc = glGetUniformLocation(shaderProgram, colorName.c_str());
        GLuint intensityLoc = glGetUniformLocation(shaderProgram, intensityName.c_str());
        GLuint distanceSquaredLoc = glGetUniformLocation(shaderProgram, distanceSquaredName.c_str()); // 获取距离平方位置

        glUniform3fv(posLoc, 1, glm::value_ptr(light.position));
        glUniform3fv(colorLoc, 1, glm::value_ptr(light.color));
        glUniform1f(intensityLoc, light.intensity);
        glUniform1f(distanceSquaredLoc, distanceSquared);  // 传递距离平方
    }


    // 4. 手电筒（聚光灯）数据
    auto flashLights = lightSpawner->GetFlashLights();
    std::vector<std::pair<float, int>> flashLightDistances;  // 存储每个聚光灯到物体的距离平方和索引

    // 遍历聚光灯计算距离平方，并且筛选出距离小于等于 100 的聚光灯
    for (int j = 0; j < flashLights.size(); j++) {
        const auto& flash = flashLights[j];
        float distanceSquared = glm::dot(objectPosition - flash.position, objectPosition - flash.position);
        if (distanceSquared <= 100.0f) {  // 只选择距离小于等于 100 的聚光灯
            flashLightDistances.push_back({ distanceSquared, j });
        }
    }

    // 按照距离平方升序排序
    std::sort(flashLightDistances.begin(), flashLightDistances.end(), [](const std::pair<float, int>& a, const std::pair<float, int>& b) {
        return a.first < b.first;  // 按照距离平方升序排序
        });

    GLuint numFlashLightsLoc = glGetUniformLocation(shaderProgram, "numFlashLights");
    glUniform1i(numFlashLightsLoc, std::min(static_cast<int>(flashLightDistances.size()), 4));

    // 只选择最近的 4 个聚光灯
    for (int j = 0; j < std::min(4, static_cast<int>(flashLightDistances.size())); j++) {
        int index = flashLightDistances[j].second;  // 获取最近的聚光灯的索引
        auto flash = flashLights[index];

        // 计算距离平方并传递到 shader
        float distanceSquared = flashLightDistances[j].first;

        std::string flashPosName = "flashLightPos[" + std::to_string(j) + "]";
        std::string flashDirName = "flashLightDirection[" + std::to_string(j) + "]";
        std::string flashColorName = "flashLightColor[" + std::to_string(j) + "]";
        std::string flashIntensityName = "flashLightIntensity[" + std::to_string(j) + "]";
        std::string flashCutoffName = "flashLightCutoff[" + std::to_string(j) + "]";
        std::string flashDistanceSquaredName = "flashLightDistanceSquared[" + std::to_string(j) + "]"; // 新增变量

        GLuint flashPosLoc = glGetUniformLocation(shaderProgram, flashPosName.c_str());
        GLuint flashDirLoc = glGetUniformLocation(shaderProgram, flashDirName.c_str());
        GLuint flashColorLoc = glGetUniformLocation(shaderProgram, flashColorName.c_str());
        GLuint flashIntensityLoc = glGetUniformLocation(shaderProgram, flashIntensityName.c_str());
        GLuint flashCutoffLoc = glGetUniformLocation(shaderProgram, flashCutoffName.c_str());
        GLuint flashDistanceSquaredLoc = glGetUniformLocation(shaderProgram, flashDistanceSquaredName.c_str()); // 获取距离平方位置

        glUniform3fv(flashPosLoc, 1, glm::value_ptr(flash.position));
        glUniform3fv(flashDirLoc, 1, glm::value_ptr(flash.direction));
        glUniform3fv(flashColorLoc, 1, glm::value_ptr(flash.color));
        glUniform1f(flashIntensityLoc, flash.intensity);
        glUniform1f(flashCutoffLoc, flash.cutoff);
        glUniform1f(flashDistanceSquaredLoc, distanceSquared);  // 传递距离平方
    }

}
void CustomModel::AttachAnimationController(AnimationData animationData)
{
    if (_animator == nullptr)
        _animator = new AnimationController();

    Animation* anim = new Animation(animationData);
    _animator->AddAnimation(anim);
    
}
void CustomModel::AttachAnimationController()
{
}
void CustomModel::PlayAnimation(int index, float frame)
{

    _animator->PlayAnimation(index, frame, this);

}
void CustomModel::StopPlayAnimation()
{

    _animator->StopAnimation();
}
void CustomModel::DrawDepthPic(glm::mat4 lightSpaceMatrix,GLuint shader)
{
    
    
    //传入模型世界坐标，视图和透视矩阵，在lightRender中生成
    GLuint modelLoc = glGetUniformLocation(shader, "model");
    glUniformMatrix4fv(modelLoc, 1, 0, glm::value_ptr(transform));


    glBindVertexArray(VAO);
    justDrawVerteies == true ? glDrawArrays(GL_TRIANGLES, 0, _index) : glDrawElements(GL_TRIANGLES, _index, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);


}
void CustomModel::DrawDepthPicDynamical(glm::mat4 lightSpaceMatrix, GLuint shader)
{

    //传入模型世界坐标，视图和透视矩阵，在lightRender中生成
    GLuint modelLoc = glGetUniformLocation(shader, "model");
    glUniformMatrix4fv(modelLoc, 1, 0, glm::value_ptr(transform));

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER,
        verticesTras.size() * sizeof(Vertex),
        verticesTras.data(),
        GL_DYNAMIC_DRAW);
    justDrawVerteies == true ? glDrawArrays(GL_TRIANGLES, 0, _index) : glDrawElements(GL_TRIANGLES, _index, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //集成纹理渲染方法

}
void Game::CustomModel::UpdateDepthPic(glm::mat4 lightSpaceMatrix, GLuint shader)
{
    //因为通用的update方法已经传入了 UpdateTransform(),所以这里就没有必要再次传入了
    if (_ifShadow)
    {        
        IsSkinnedMesh == true ? DrawDepthPicDynamical(lightSpaceMatrix,shader) : DrawDepthPic(lightSpaceMatrix,shader);
       // std::cout << ID << "render shadow:" << _ifShadow << std::endl;
    }
}
void Game::CustomModel::DrawDepthViewPortPic(glm::mat4 view, glm::mat4 projection, GLuint shader)
{



}
void Game::CustomModel::DrawDepthViewPortPicDynamical(glm::mat4 view, glm::mat4 projection, GLuint shader)
{


}
void Game::CustomModel::UpdateDepthViewPortPic(glm::mat4 view, glm::mat4 projection, GLuint shader)
{
   if(_enableDepth)
     //这里的视图参数由外部直接传入，这里只需要计算模型本身位置即可，并且取消阴影判断，实例化类中重写空方法
    IsSkinnedMesh == true ? DrawDepthPicDynamical(view, shader) : DrawDepthPic(view, shader);

}
void Game::CustomModel::UpdatePhysics()
{
    //开启物理模式，却未开启碰撞，才进行简单加速度物理计算
    if (_ifPhysics&&!_ifCollision)
    {
        _physicsBody->UpdatePhysics();

    }


}
void Game::CustomModel::UpdateCollisionAndPhysics(std::unordered_map<int, CollisionProperties*>& cop)
{

    if (_ifCollision)
    {
  
        _collider->UpdateCollisionState(cop);
       
    }
}

bool Game::CustomModel::AttachPhysicalEngine(bool staticObj,float mass, float friction, glm::vec3 velocity, glm::vec3 acceleration, float elasticity, bool lockXZAxi,
    float rotationDampping,  bool trigger, int layer, float rotationAdjust)
{
    
    if (_physicsBody == nullptr)
        //这里直接将位置的引用传入，方便在物理引擎的内部对position的值进行直接修改,激活状态作为引用传入，因为这里涉及到渲染逻辑，其他才是物理逻辑
        _physicsBody = new PhysicalEngine(position, rotation,scale, _ifActive,ID,mass,friction,velocity,acceleration,
            elasticity,lockXZAxi,rotationDampping,trigger,layer,rotationAdjust,staticObj);
    
    return _ifPhysics=true;
}
/// <summary>
/// 设置基本碰撞体的范围
/// </summary>
/// <param name="type"></param>
/// <param name="radians"></param>
/// <param name="ratio"></param>
/// <returns></returns>
bool CustomModel::AttachCollider(CollisionType collider , SpecialType type,float radius)
{
   
    if (_physicsBody==nullptr)
    {
        return false;
    }
    
    if (_collider == nullptr)
        _collider = new CollisionBody(position,_physicsBody->GetVelocity(),
                                      _physicsBody->GetAcceleration(),rotation,scale,ID,_physicsBody->GetMass(),_physicsBody->GetFriction(),
                                    _physicsBody->GetElasticity(),_physicsBody->GetLayer(),_physicsBody->GetTrigger(),_physicsBody->GetActive()
                                    ,_physicsBody->GetDamping(),_physicsBody->GetLockState(),_physicsBody->GetRotationAdjust(), 
            _physicsBody->GetStatic(),collider,radius,type,this->GetVariant()
            );
   
    
  
    return _ifCollision=true;
}
bool Game::CustomModel::GetIfCollision()
{
    return _ifCollision;
}
int Game::CustomModel::GetID()
{
    return ID;
}
int Game::CustomModel::SetID(int id)
{
  return ID = id;
}
bool Game::CustomModel::GetActiveState()const
{
    return _ifActive;
}

bool Game::CustomModel::GetBatchDrawState()
{
    return _drawTextureBatch;
}

void Game::CustomModel::DestroySelf()
{
   
        std::cout << "销毁:"<<ID << std::endl;
    
        delete this;

}
bool Game::CustomModel::SetActive(bool active)
{
   
    std::cout<<ID << "激活:" << active << std::endl;
    
    return _ifActive=active;
}
bool CustomModel::Draw (glm::mat4 view, glm::mat4 projection)
{

    glUseProgram(shaderProgram);
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    //这里是对 三个定义的 参数进行抓取，在shaderProgram 里面，这个参数是写在对应的shader里面进行定义的
    glUniformMatrix4fv(modelLoc, 1, 0, glm::value_ptr(transform));
    glUniformMatrix4fv(viewLoc, 1, 0, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, 0, glm::value_ptr(projection));
    glBindVertexArray(VAO);
    //绑定顶点数组对象（VAO），激活与 VAO 关联的顶点缓冲对象（VBO）和索引缓冲对象（EBO）。
    //保存顶点属性的配置，例如位置、颜色、法线、纹理坐标等，与 VBO 和 EBO 关联，用于存储顶点数据和绘制索引。
    justDrawVerteies == true ? glDrawArrays(GL_TRIANGLES, 0, _index) : glDrawElements(GL_TRIANGLES, _index, GL_UNSIGNED_INT, 0);
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
    glDrawElements(GL_LINES, _index, GL_UNSIGNED_INT, 0);

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
    glDrawElements(GL_LINES, _index, GL_UNSIGNED_INT, 0);

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

    if (_animator != nullptr)
        delete _animator;
    if (_physicsBody != nullptr)
        delete _physicsBody;
    if (_collider != nullptr)
        delete _collider;


}
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
//变体方法，可以增加变体的运行行为
void  CustomModel::UpdateVariant(glm::mat4 view, glm::mat4 projection)
{
    //父类放置空方法

}
// 根据动画帧更新顶点数据
void CustomModel::UpdateVerticesForAnimation(size_t animationFrame) {
    // 根据当前动画帧来更新 verticesTras 中的顶点数据
    // 示例：更新位置或其他属性
    for (size_t i = 0; i < verticesTras.size(); ++i) {
        // 根据 animationFrame 更新顶点位置
        // 比如通过插值来更新顶点的 Position、TexCoords 等
        Vertex& vertex = (verticesTras)[i];
        // 这里假设有某种规律来改变顶点属性
        vertex.Position.x += 0.001f * animationFrame;
        vertex.Position.y += 0.001f * animationFrame;
        vertex.Position.z += 0.001f * animationFrame;
    }
}

void CustomModel::UpdateVerticesForAnimation(const std::vector<Vertex>& vertex)
{
    verticesTras = vertex;
}

void Game::CustomModel::Start()
{

    std::cout << ID << ":" << _ifShadow << std::endl;
}

void Game::CustomModel::SpecialMethod()
{

}

void Game::CustomModel::SelfIns()
{
}


