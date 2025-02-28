#include "CustomModel.h"
using namespace Game;
extern const char* depthShaderVertexShaderSource;
extern const char* depthShaderFragmentShaderSource;

Game::CustomModel::CustomModel()
{
}
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

    //���������Ҫ������ ��������뵽 ��̬����������У��������ܻ��ƣ���������Ͷ������鶼�� cube.h��ͷ�ļ��ж���


    // ���ö�������ָ�룬ʵ���Ͼ��Ǹ��� OpenGL���������д洢�Ķ����������ݰ��� 3 �������� ��λ����Ϣ���洢��ÿ 3 ��������������ɫ��Ϊ6������ʼλ�ã���λ����Ϣ��ƫ���� 0 ��ʼ��
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);//�������ö�����ɫ���� layout(location = 0) ���������ݡ�
    //----

    glBindVertexArray(0);  //1 ���ǰ�󶨵� VAO����ֹ������ VAO �Ĳ���Ҫ�޸� 2 ����VAO �����ñ������� GPU �У�����ֻ�����°󶨼���ʹ��
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
    index = vertices.size();  // ��ȡ����������������������������
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

    //��Ϊû�н��ж��㸴�ã����ԣ��������������Ͳ��ٱ���Ҫ��
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, index * sizeof(GLuint), &vertices[0], GL_STATIC_DRAW);

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
    vertexCount = modelData.verticesStruct.size();
    index = modelData.indices.size();
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
   
    //����������Ⱦ����,Ҫ��draw֮ǰ���ã�������Ч
    RenderingTexture();
    
   // glUseProgram(shaderProgram);

    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    //�����Ƕ� ��������� ��������ץȡ����shaderProgram ���棬���������д�ڶ�Ӧ��shader������ж����
    glUniformMatrix4fv(modelLoc, 1, 0, glm::value_ptr(transform));
    glUniformMatrix4fv(viewLoc, 1, 0, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, 0, glm::value_ptr(projection));
    //�����glm��һ�����õ���ѧ�⣬ר������ͼ�εľ������������ͳ��ڹ���� CPU֮���������ݣ�GPU ��������ͼ�Σ�
    // shader�������� ʹ�� gl_Position = projection * view * model * vec4(aPos, 1.0f);������Ⱦ���� 
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
  //  glBufferSubData(GL_ARRAY_BUFFER, 0,vertex * sizeof(glm::vec3), (void*)&((*verticesTras)[0].Position));//ָ������������
    glBufferData(GL_ARRAY_BUFFER,
        verticesTras.size() * sizeof(Vertex),
        verticesTras.data(),
        GL_DYNAMIC_DRAW);
    //�󶨶����������VAO���������� VAO �����Ķ��㻺�����VBO���������������EBO����
    //���涥�����Ե����ã�����λ�á���ɫ�����ߡ���������ȣ��� VBO �� EBO ���������ڴ洢�������ݺͻ���������
    justDrawVerteies == true ? glDrawArrays(GL_TRIANGLES, 0, index) : glDrawElements(GL_TRIANGLES, index, GL_UNSIGNED_INT, 0); 
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
bool CustomModel::AttachTexture(GLuint textureName, int order, glm::vec2 textureScale)//opengl ���õ���״̬��ģʽ
{  

    texture = textureName;
    textureOrder = order;
    _textureScale = textureScale;
   // RenderingTexture();
    return _drawTexture = true;
}
void CustomModel::RenderingTexture()
{
   
    glUseProgram(shaderProgram);
    if (_drawTexture)
    {

        // ����������������
        GLuint textureScaleLoc = glGetUniformLocation(shaderProgram, "textureScale");
        glUniform2f(textureScaleLoc, _textureScale.x, _textureScale.y); // ����������������

        GLuint picData = glGetUniformLocation(shaderProgram, "baseTexture");//Ԥд��ͼ���shader��������
        glActiveTexture(GL_TEXTURE0 + textureOrder);          // ��������Ԫ 0+order
        glBindTexture(GL_TEXTURE_2D, texture);  // �������������Ԫ 0+order,�������DicTexture���ϵ��������
        // ����������Ԫ 0+order�����˳�����������Ԫ��������һ��
        glUniform1i(picData, textureOrder);
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
    justDrawVerteies == true ? glDrawArrays(GL_TRIANGLES, 0, index) : glDrawElements(GL_TRIANGLES, index, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // ��ӡ lightSpaceMatrix
    //std::cout << "LightSpaceMatrix: \n";
    //for (int i = 0; i < 4; i++) {
    //    std::cout << lightSpaceMatrix[i][0] << " "
    //        << lightSpaceMatrix[i][1] << " "
    //        << lightSpaceMatrix[i][2] << " "
    //        << lightSpaceMatrix[i][3] << std::endl;
    //}
    //// ��ӡ transform��ģ�;���
    //std::cout << "Transform (Model Matrix): \n";
    //for (int i = 0; i < 4; i++) {
    //    std::cout << transform[i][0] << " "
    //        << transform[i][1] << " "
    //        << transform[i][2] << " "
    //        << transform[i][3] << std::endl;
    //}


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
    justDrawVerteies == true ? glDrawArrays(GL_TRIANGLES, 0, index) : glDrawElements(GL_TRIANGLES, index, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //����������Ⱦ����


}
void Game::CustomModel::UpdateDepthPic(glm::mat4 lightSpaceMatrix, GLuint shader)
{
    //��Ϊͨ�õ�update�����Ѿ������� UpdateTransform(),���������û�б�Ҫ�ٴδ�����
    if (_ifShadow)
    {        
        IsSkinnedMesh == true ? DrawDepthPicDynamical(lightSpaceMatrix,shader) : DrawDepthPic(lightSpaceMatrix,shader);

    }
}
void Game::CustomModel::UpdatePhysics()
{
    if (_ifPhysics)
    {

        _physicsBody->UpdatePhysics();


    }


}
void Game::CustomModel::CheckCollision()
{


    if (_ifCollision)
    {
  


        _collider->UpdateCollisionState();

        
    }

}
bool CustomModel::AttachPhysicalEngine(bool ifStatic ,float mass, float friction , glm::vec3 velocity , glm::vec3 acceleration )
{
    if (_physicsBody == nullptr)
        //����ֱ�ӽ�λ�õ����ô��룬����������������ڲ���position��ֵ����ֱ���޸�
        _physicsBody = new PhysicalEngine(position,ifStatic);
    
    _physicsBody->SetParameters( mass, friction, velocity, acceleration);


    return _ifPhysics = true;
}
/// <summary>
/// ���û�����ײ��ķ�Χ
/// </summary>
/// <param name="type"></param>
/// <param name="radians"></param>
/// <param name="ratio"></param>
/// <returns></returns>
bool CustomModel::AttachCollider(CollisionType type , float radius,int layer,bool trigger)
{
   
    if (_physicsBody==nullptr)
    {
        return false;
    }
    
    if (_collider == nullptr)
        _collider = new CollisionBody(position, _physicsBody->GetVelocity(),
                                      _physicsBody->GetAcceleration(),_physicsBody->GetMass(),_physicsBody->GetFriction(),_physicsBody->GetStatic());
   
    
    _collider->SetCollisionParameters(type, radius, scale,layer,trigger);
    
  
    return _ifCollision=true;
}
bool Game::CustomModel::GetIfCollision()
{
    return _ifCollision;
}
bool CustomModel::Draw (glm::mat4 view, glm::mat4 projection)
{
    //����������Ⱦ����,Ҫ��draw֮ǰ���ã�������Ч
    RenderingTexture();
    
   // glUseProgram(shaderProgram);
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    //�����Ƕ� ��������� ��������ץȡ����shaderProgram ���棬���������д�ڶ�Ӧ��shader������ж����
    glUniformMatrix4fv(modelLoc, 1, 0, glm::value_ptr(transform));
    glUniformMatrix4fv(viewLoc, 1, 0, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, 0, glm::value_ptr(projection));
    //�����glm��һ�����õ���ѧ�⣬ר������ͼ�εľ������������ͳ��ڹ���� CPU֮���������ݣ�GPU ��������ͼ�Σ�
    // shader�������� ʹ�� gl_Position = projection * view * model * vec4(aPos, 1.0f);������Ⱦ���� 
    glBindVertexArray(VAO);
    //�󶨶����������VAO���������� VAO �����Ķ��㻺�����VBO���������������EBO����
    //���涥�����Ե����ã�����λ�á���ɫ�����ߡ���������ȣ��� VBO �� EBO ���������ڴ洢�������ݺͻ���������
    justDrawVerteies == true ? glDrawArrays(GL_TRIANGLES, 0, index) : glDrawElements(GL_TRIANGLES, index, GL_UNSIGNED_INT, 0);
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
    glDrawElements(GL_LINES, index, GL_UNSIGNED_INT, 0);

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

    //auto fixedView = Controller::GetInstance()->GetFixedViewMatrix();
  // auto NProjection= glm::mat4(1.0f);  // ��ʼ��Ϊ��λ����
  // transform = glm::translate(NProjection, glm::vec3(0,0,-5));  // ƽ�ƣ����������ƽ�Ƶ�ʵ����ʵ���ǵ�����һ��GL�� translate��API
   
  //  glDisable(GL_DEPTH_TEST);
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
   // glEnable(GL_DEPTH_TEST);
    return true;
}

CustomModel::~CustomModel()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    //����������֮��ָ��ò���Ѿ����ͷ���
   // delete verticesTras;//�ͷ��ڴ漴�ͷ�ջ�ϵ����ã����ö�ʧ���ڴ������壬���߱�����ָ���ַ�������·���
   // verticesTras = nullptr;//�����ڴ���Ⱦ�����Ǳ������


    if (_animator == nullptr)
        delete _animator;
}
void CustomModel::Update(glm::mat4 view, glm::mat4 projection)
{
    //�Ƚ����������
    UpdatePhysics();
    //�ٽ�����ײ����
    CheckCollision();
    //�ٽ��б��巽������
    UpdateVariant(view,projection);
    //�ٽ����������
    //����д���ͣ�ֱ�ӷ�װ�˸��±仯��֪ͨCPU��Ⱦ������������������嶨�巽����ֻ��Ҫ�������ű����浥������ĳ������������transform���ɣ��ɹ���ط��͵���
    UpdateTransform();
    //ȷ�϶�̬���ƻ��߾�̬����
   IsSkinnedMesh==true? DrawDynamical(view, projection): Draw(view, projection);
}
//���巽�����������ӱ����������Ϊ
void  CustomModel::UpdateVariant(glm::mat4 view, glm::mat4 projection)
{

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

//// GetComponent ����
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
//    // ����Ҳ���ƥ������ͣ��׳��쳣�򷵻� nullptr
//    throw std::runtime_error("Requested component type not found!");
//    // ����ʹ�÷���nullptr�ķ�ʽ��
//    // return nullptr;
//}
//
//// ��ʽʵ����ģ��
//template AnimationController* CustomModel::GetComponent<AnimationController>();
//template PhysicalEngine* CustomModel::GetComponent<PhysicalEngine>();
//template CollisionBody* CustomModel::GetComponent<CollisionBody>();
