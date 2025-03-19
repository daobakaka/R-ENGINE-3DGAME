#include "CustomModel.h"
#include "Light.h"
using namespace Game;
extern const char* depthShaderVertexShaderSource;
extern const char* depthShaderFragmentShaderSource;
//����ȫ��ID �����ĳ�ʼֵ

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
    CheckShaderProgramLinking(shaderProgram);  // �����ɫ�������Ƿ����ӳɹ�����Ҳ��һ���Զ���ķ������������������shader ���ӵ���Ŀ�У��������ӵ���Ŀ������

    glUseProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    //��һ�����֮�󣬾Ϳ���ʹ�������Ŀ�ˣ�ͬʱҲ����ɾ�������ƬԪ��ɫ����
    // 
    // 
    // �����Ͱ�VAO, VBO, EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    //�󶨶��㻺������
    //����������ͼ������Ҫ�� �������� �ͻ���buffer���������㻺�����VBO�������������VAO��Ԫ�ػ������ EBO

    // VBO: Vertex Buffer Object (���㻺�����)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);//GL_ARRAY_BUFFER 0x8892 ���黺������Ƕ��嵽�̶����ڴ��ַ���󶨻���
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(GLfloat), vertices, GL_STATIC_DRAW);//GL_STATIC_DRAW 0x88E4   ��̬��������

    

    // EBO: Element Buffer Object (Ԫ�ػ������)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);// GL_ELEMENT_ARRAY_BUFFER 0x8893  ��Ԫ�ػ�����󣬻��߽�����������󣬴��������������Ϣ
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLuint), indices, GL_STATIC_DRAW);//GL_STATIC_DRAW 0x88E4   ��̬��������



    // ���ö�������ָ�룬ʵ���Ͼ��Ǹ��� OpenGL���������д洢�Ķ����������ݰ��� 3 �������� ��λ����Ϣ���洢��ÿ 3 ��������������ɫ��Ϊ6������ʼλ�ã���λ����Ϣ��ƫ���� 0 ��ʼ��
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);//�������ö�����ɫ���� layout(location = 0) ���������ݡ�
    //----

    glBindVertexArray(0);  //1 ���ǰ�󶨵� VAO����ֹ������ VAO �Ĳ���Ҫ�޸� 2 ����VAO �����ñ������� GPU �У�����ֻ�����°󶨼���ʹ��
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

    // 8. �������ԣ�λ�� (location=0)
     //    ע�� stride = 6 * sizeof(GLfloat)
     //          offset = 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(0));
    glEnableVertexAttribArray(0);

    // 9. �������ԣ����� (location=1)
    //    offset = 3 * sizeof(GLfloat)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

CustomModel::CustomModel(const char* vertexShaderSourceIn, const char* fragmentShaderSourceIn,std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& normals,bool ifLightIn)
{

    justDrawVerteies = true;
    _index = vertices.size();  // ��ȡ����������������������������
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
//������ɫ�������֮��Ϳ������Gluint ������
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
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);//��̬����Ļ�����������Ⱦδ Static������Ҫ��̬��Ƥ��Ⱦ�������Ϊ dynamical

    // Set up texture coordinate attribute (location = 1)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid*)0);//(GLvoid*)0 ��ǰVOB�����ƫ����
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

    // 2) ������ɫ������
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    CheckShaderProgramLinking(shaderProgram);

    // ɾ����ʱShader
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 3) ���� VAO, VBO, EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);


    // 4) ��VAO
    glBindVertexArray(VAO);

    // 5) VBO: �������ж���
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
        vertices.size() * sizeof(Vertex),
        vertices.data(),
        GL_STATIC_DRAW);

    // 6) EBO: ��������
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        finalIndices.size() * sizeof(unsigned int),
        finalIndices.data(),
        GL_STATIC_DRAW);

    // 7) ���ö�������ָ��
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

    // ���VAO
    glBindVertexArray(0);

}

CustomModel::CustomModel(const char* vertexShaderSourceIn, const char* fragmentShaderSourceIn, std::vector<Vertex>& vertices, std::vector<unsigned int>& finalIndices,bool isSkinnedMesh, bool ifLightIn)
{

    //verticesTras = new std::vector<Vertex>(vertices);  // ���ֲ���ָ�����Ч��ʼ���������в���ʹ��delete
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

    // 2) ������ɫ������
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    CheckShaderProgramLinking(shaderProgram);

    // ɾ����ʱShader
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 3) ���� VAO, VBO, EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);


    // 4) ��VAO
    glBindVertexArray(VAO);

    // 5) VBO: �������ж���
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
        vertices.size() * sizeof(Vertex),
        vertices.data(),
        GL_DYNAMIC_DRAW);

    // 6) EBO: ��������
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        finalIndices.size() * sizeof(unsigned int),
        finalIndices.data(),
        GL_DYNAMIC_DRAW);

    // 7) ���ö�������ָ��
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

    // ���VAO
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

    // ������ɫ������
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    CheckShaderProgramLinking(shaderProgram);

    // ɾ����ʱShader
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // ���� VAO, VBO, EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // ��VAO
    glBindVertexArray(VAO);

    // �����Ƿ�Ϊ��̬����������������񣩾����Ƿ�ʹ�ö�̬��̬������
    GLuint bufferUsage = isSkinnedMesh ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;

    // VBO: �������ж���
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
        modelData.verticesStruct.size() * sizeof(Vertex),
        modelData.verticesStruct.data(),
        bufferUsage);  // �����Ƿ�Ϊ��̬����ѡ�񻺳�������

    // EBO: ��������
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        modelData.indices.size() * sizeof(unsigned int),
        modelData.indices.data(),
        bufferUsage);  // ͬ�������Ƿ�Ϊ��̬����ѡ�񻺳�������

    // ���ö�������ָ��
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

    // ���VAO
    glBindVertexArray(0);
}


bool CustomModel::DrawDynamical(glm::mat4 view, glm::mat4 projection)
{
   
    glUseProgram(shaderProgram);
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    //�����Ƕ� ��������� ��������ץȡ����shaderProgram ���棬���������д�ڶ�Ӧ��shader������ж����
    glUniformMatrix4fv(modelLoc, 1, 0, glm::value_ptr(transform));
    glUniformMatrix4fv(viewLoc, 1, 0, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, 0, glm::value_ptr(projection));
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
        verticesTras.size() * sizeof(Vertex),
        verticesTras.data(),
        GL_DYNAMIC_DRAW);
    //�󶨶����������VAO���������� VAO �����Ķ��㻺�����VBO���������������EBO����
    //���涥�����Ե����ã�����λ�á���ɫ�����ߡ���������ȣ��� VBO �� EBO ���������ڴ洢�������ݺͻ���������
    justDrawVerteies == true ? glDrawArrays(GL_TRIANGLES, 0, _index) : glDrawElements(GL_TRIANGLES, _index, GL_UNSIGNED_INT, 0); 
    //��������洢�� EBO��Element Buffer Object���У�EBO �Ѱ󶨵� VAO�������Σ�36���������޷����������飬��0��ʼ
    glBindVertexArray(0);
    //����������Ⱦ����
  //  RenderingTexture();


    return true;
}
/// <summary>
/// attachһ�ν�����Ч��ֵ�����������ɵ�draw����drawDynamicl��
/// </summary>
/// <param name="textureName"></param>
/// <param name="order"></param>
/// <returns></returns>
bool CustomModel::AttachTexture(const std::unordered_map<PictureTpye, GLuint>& textureData,bool isBatch ,int order, glm::vec2 textureScale) {
    // ��� order �Ƿ���Ч
    if (order < 0) {
        return false; // ��Ч�� order
    }
    _textures = textureData;
    _textureOrder = order;
    _textureScale = textureScale;
    _drawTextureBatch = isBatch;
    return  true;
}
void CustomModel::BindTexture()
{   
        // ����������������
        GLuint textureScaleLoc = glGetUniformLocation(shaderProgram, "textureScale");
        glUniform2f(textureScaleLoc, _textureScale.x, _textureScale.y);

        GLuint baseTextureLoc = glGetUniformLocation(shaderProgram, "baseTexture");
        glActiveTexture(GL_TEXTURE0+_textureOrder); // ��������Ԫ 0
        glBindTexture(GL_TEXTURE_2D, _textures[BaseP]); // �󶨻�������
        glUniform1i(baseTextureLoc, 0); // ��������Ԫ             
 }

void Game::CustomModel::BindTextureAdditional()
{   
    //commonlight �Ĳ�����������������У�ͨ�����ֻ��һ������ͼƬ

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
        UniformParametersInput();//����Ӧ������������
    }
  
    UniformParametersChange();//�仯���Σ���Բ��ֲ������б仯����һ֡�ڸı��Ӧ��ֵ,��Ӧ�����������Ƶ��ⲿ����    
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
 
    // 2. ���Դ����
    const auto& pointLights = lightSpawner->GetPointLights();
    std::vector<std::pair<float, int>> pointLightDistances;  // �洢ÿ�����Դ������ľ���ƽ��������,�������Ľṹ
    glm::vec3 objectPosition = position;  // ʹ������ĵ�ǰλ����Ϊ�ο�

    // �������Դ�������ƽ��������ɸѡ������С�ڵ��� 1000 �Ĺ�Դ
    for (int i = 0; i < pointLights.size(); i++) {
        const auto& light = pointLights[i];
        float distanceSquared = glm::dot(objectPosition - light.position, objectPosition - light.position);
        if (distanceSquared <= 1000.0f) {  // ֻѡ�����ƽ��С�ڵ��� 100 �Ĺ�Դ
            pointLightDistances.push_back({ distanceSquared, i });
            //std::cout << i << "����" << distanceSquared << std::endl;
        }
    }

    // ���վ���ƽ����������
    std::sort(pointLightDistances.begin(), pointLightDistances.end(), [](const std::pair<float, int>& a, const std::pair<float, int>& b) {
        return a.first < b.first;  // ���վ���ƽ����������
        });
    //���ܶ�̬��������������shader���治֧�֣�����������д�������ǿ���ͨ��������Լ�� 
    GLuint numPointLightsLoc = glGetUniformLocation(shaderProgram, "numPointLights");
    glUniform1i(numPointLightsLoc, std::min(static_cast<int>(pointLightDistances.size()), 4));

    // ֻѡ������� 4 �����Դ
    for (int i = 0; i < std::min(4, static_cast<int>(pointLightDistances.size())); i++) {
        int index = pointLightDistances[i].second;  // ��ȡ����ĵ��Դ����������һ��ֵ�Ǿ��룬�ڶ���ֵ�Ǿ���
        auto light = pointLights[index];
        //  std::cout << "���Դǿ��" << light.intensity << std::endl;
          // �������ƽ�������ݵ� shader
        float distanceSquared = pointLightDistances[i].first;
        //  std::cout <<i<< "����" << distanceSquared << std::endl;
        std::string posName = "lightPos[" + std::to_string(i) + "]";
        std::string colorName = "lightColor[" + std::to_string(i) + "]";
        std::string intensityName = "lightIntensity[" + std::to_string(i) + "]";
        std::string distanceSquaredName = "lightDistanceSquared[" + std::to_string(i) + "]"; // ��������

        GLuint posLoc = glGetUniformLocation(shaderProgram, posName.c_str());
        GLuint colorLoc = glGetUniformLocation(shaderProgram, colorName.c_str());
        GLuint intensityLoc = glGetUniformLocation(shaderProgram, intensityName.c_str());
        GLuint distanceSquaredLoc = glGetUniformLocation(shaderProgram, distanceSquaredName.c_str()); // ��ȡ����ƽ��λ��

        glUniform3fv(posLoc, 1, glm::value_ptr(light.position));
        glUniform3fv(colorLoc, 1, glm::value_ptr(light.color));
        glUniform1f(intensityLoc, light.intensity);
        glUniform1f(distanceSquaredLoc, distanceSquared);  // ���ݾ���ƽ��
    }


    // 4. �ֵ�Ͳ���۹�ƣ�����
    auto flashLights = lightSpawner->GetFlashLights();
    std::vector<std::pair<float, int>> flashLightDistances;  // �洢ÿ���۹�Ƶ�����ľ���ƽ��������

    // �����۹�Ƽ������ƽ��������ɸѡ������С�ڵ��� 100 �ľ۹��
    for (int j = 0; j < flashLights.size(); j++) {
        const auto& flash = flashLights[j];
        float distanceSquared = glm::dot(objectPosition - flash.position, objectPosition - flash.position);
        if (distanceSquared <= 100.0f) {  // ֻѡ�����С�ڵ��� 100 �ľ۹��
            flashLightDistances.push_back({ distanceSquared, j });
        }
    }

    // ���վ���ƽ����������
    std::sort(flashLightDistances.begin(), flashLightDistances.end(), [](const std::pair<float, int>& a, const std::pair<float, int>& b) {
        return a.first < b.first;  // ���վ���ƽ����������
        });

    GLuint numFlashLightsLoc = glGetUniformLocation(shaderProgram, "numFlashLights");
    glUniform1i(numFlashLightsLoc, std::min(static_cast<int>(flashLightDistances.size()), 4));

    // ֻѡ������� 4 ���۹��
    for (int j = 0; j < std::min(4, static_cast<int>(flashLightDistances.size())); j++) {
        int index = flashLightDistances[j].second;  // ��ȡ����ľ۹�Ƶ�����
        auto flash = flashLights[index];

        // �������ƽ�������ݵ� shader
        float distanceSquared = flashLightDistances[j].first;

        std::string flashPosName = "flashLightPos[" + std::to_string(j) + "]";
        std::string flashDirName = "flashLightDirection[" + std::to_string(j) + "]";
        std::string flashColorName = "flashLightColor[" + std::to_string(j) + "]";
        std::string flashIntensityName = "flashLightIntensity[" + std::to_string(j) + "]";
        std::string flashCutoffName = "flashLightCutoff[" + std::to_string(j) + "]";
        std::string flashDistanceSquaredName = "flashLightDistanceSquared[" + std::to_string(j) + "]"; // ��������

        GLuint flashPosLoc = glGetUniformLocation(shaderProgram, flashPosName.c_str());
        GLuint flashDirLoc = glGetUniformLocation(shaderProgram, flashDirName.c_str());
        GLuint flashColorLoc = glGetUniformLocation(shaderProgram, flashColorName.c_str());
        GLuint flashIntensityLoc = glGetUniformLocation(shaderProgram, flashIntensityName.c_str());
        GLuint flashCutoffLoc = glGetUniformLocation(shaderProgram, flashCutoffName.c_str());
        GLuint flashDistanceSquaredLoc = glGetUniformLocation(shaderProgram, flashDistanceSquaredName.c_str()); // ��ȡ����ƽ��λ��

        glUniform3fv(flashPosLoc, 1, glm::value_ptr(flash.position));
        glUniform3fv(flashDirLoc, 1, glm::value_ptr(flash.direction));
        glUniform3fv(flashColorLoc, 1, glm::value_ptr(flash.color));
        glUniform1f(flashIntensityLoc, flash.intensity);
        glUniform1f(flashCutoffLoc, flash.cutoff);
        glUniform1f(flashDistanceSquaredLoc, distanceSquared);  // ���ݾ���ƽ��
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
    
    
    //����ģ���������꣬��ͼ��͸�Ӿ�����lightRender������
    GLuint modelLoc = glGetUniformLocation(shader, "model");
    glUniformMatrix4fv(modelLoc, 1, 0, glm::value_ptr(transform));


    glBindVertexArray(VAO);
    justDrawVerteies == true ? glDrawArrays(GL_TRIANGLES, 0, _index) : glDrawElements(GL_TRIANGLES, _index, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);


}
void CustomModel::DrawDepthPicDynamical(glm::mat4 lightSpaceMatrix, GLuint shader)
{

    //����ģ���������꣬��ͼ��͸�Ӿ�����lightRender������
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
    //����������Ⱦ����

}
void Game::CustomModel::UpdateDepthPic(glm::mat4 lightSpaceMatrix, GLuint shader)
{
    //��Ϊͨ�õ�update�����Ѿ������� UpdateTransform(),���������û�б�Ҫ�ٴδ�����
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
     //�������ͼ�������ⲿֱ�Ӵ��룬����ֻ��Ҫ����ģ�ͱ���λ�ü��ɣ�����ȡ����Ӱ�жϣ�ʵ����������д�շ���
    IsSkinnedMesh == true ? DrawDepthPicDynamical(view, shader) : DrawDepthPic(view, shader);

}
void Game::CustomModel::UpdatePhysics()
{
    //��������ģʽ��ȴδ������ײ���Ž��м򵥼��ٶ��������
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
        //����ֱ�ӽ�λ�õ����ô��룬����������������ڲ���position��ֵ����ֱ���޸�,����״̬��Ϊ���ô��룬��Ϊ�����漰����Ⱦ�߼����������������߼�
        _physicsBody = new PhysicalEngine(position, rotation,scale, _ifActive,ID,mass,friction,velocity,acceleration,
            elasticity,lockXZAxi,rotationDampping,trigger,layer,rotationAdjust,staticObj);
    
    return _ifPhysics=true;
}
/// <summary>
/// ���û�����ײ��ķ�Χ
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
   
        std::cout << "����:"<<ID << std::endl;
    
        delete this;

}
bool Game::CustomModel::SetActive(bool active)
{
   
    std::cout<<ID << "����:" << active << std::endl;
    
    return _ifActive=active;
}
bool CustomModel::Draw (glm::mat4 view, glm::mat4 projection)
{

    glUseProgram(shaderProgram);
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    //�����Ƕ� ��������� ��������ץȡ����shaderProgram ���棬���������д�ڶ�Ӧ��shader������ж����
    glUniformMatrix4fv(modelLoc, 1, 0, glm::value_ptr(transform));
    glUniformMatrix4fv(viewLoc, 1, 0, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, 0, glm::value_ptr(projection));
    glBindVertexArray(VAO);
    //�󶨶����������VAO���������� VAO �����Ķ��㻺�����VBO���������������EBO����
    //���涥�����Ե����ã�����λ�á���ɫ�����ߡ���������ȣ��� VBO �� EBO ���������ڴ洢�������ݺͻ���������
    justDrawVerteies == true ? glDrawArrays(GL_TRIANGLES, 0, _index) : glDrawElements(GL_TRIANGLES, _index, GL_UNSIGNED_INT, 0);
    //��������洢�� EBO��Element Buffer Object���У�EBO �Ѱ󶨵� VAO�������Σ�36���������޷����������飬��0��ʼ
    glBindVertexArray(0);
    
    //����������Ⱦ����
  // RenderingTexture();
    return true;
}
/// <summary>
/// �����߶�
/// </summary>
/// <param name="view"></param>
/// <param name="projection"></param>
/// <returns></returns>
bool CustomModel::DrawLine(glm::mat4 view, glm::mat4 projection)
{
    
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
    glDrawElements(GL_LINES, _index, GL_UNSIGNED_INT, 0);

    // ��� VAO
    glBindVertexArray(0);


    return true;
}
/// <summary>
/// ��GL���ڣ����������߶�
/// </summary>
/// <param name="view"></param>
/// <param name="projection"></param>
/// <returns></returns>
bool CustomModel::DrawLineFixedWidget(glm::mat4 view, glm::mat4 projection)
{
   
    //�����ӿ���Ⱦ
  //  glDisable(GL_DEPTH_TEST);
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    // �̶�����������������½����򣬳ߴ� 150x150 ����
    int widgetPosX = 300;
    int widgetPosY = 300;
    int widgetSize = 500;

    // �л��ӿڵ� widget ����
    glViewport(widgetPosX, widgetPosY, 300, 200);


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
    glDrawElements(GL_LINES, _index, GL_UNSIGNED_INT, 0);

    // ��� VAO
    glBindVertexArray(0);

    
    // �ָ�ԭ�����ӿ�����
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
    //�Ƚ��м�������㣬����ײ��   
      UpdatePhysics();  
    //����ײ�壬�ٽ�����ײ����,�����ö����������
      UpdateCollisionAndPhysics(CollisionProps);
     //�ٽ��б��巽������
     UpdateVariant(view,projection);
    //�ٽ����������
    //����д���ͣ�ֱ�ӷ�װ�˸��±仯��֪ͨCPU��Ⱦ������������������嶨�巽����ֻ��Ҫ�������ű����浥������ĳ������������transform���ɣ��ɹ���ط��͵���
    UpdateTransform();
    //ȷ�϶�̬���ƻ��߾�̬����
    InnerRendringTexture(view, projection);

}
//���巽�����������ӱ����������Ϊ
void  CustomModel::UpdateVariant(glm::mat4 view, glm::mat4 projection)
{
    //������ÿշ���

}
// ���ݶ���֡���¶�������
void CustomModel::UpdateVerticesForAnimation(size_t animationFrame) {
    // ���ݵ�ǰ����֡������ verticesTras �еĶ�������
    // ʾ��������λ�û���������
    for (size_t i = 0; i < verticesTras.size(); ++i) {
        // ���� animationFrame ���¶���λ��
        // ����ͨ����ֵ�����¶���� Position��TexCoords ��
        Vertex& vertex = (verticesTras)[i];
        // ���������ĳ�ֹ������ı䶥������
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


