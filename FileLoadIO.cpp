#include "FileLoadIO.h"
#include <cstdio>
#include <vector>
#include <glm/glm.hpp>
#include <cstring>
#include "TextRender.h"
using namespace Game;
/// <summary>
/// ��Դ��������С�ͳ���������ȫ���ػ�����Դ
/// </summary>
std::unordered_map<std::string, std::unordered_map<std::string, AnimationData>> AnimationDic;
std::unordered_map<std::string, ModelData> ModelDic;
std::unordered_map<std::string, std::vector<GLuint>> TextureDic;


bool LoadOBJ(const char* path, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals)
{
    // Temporary storage for file data
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;

    // Open the file using fopen_s
    FILE* file = nullptr;
    errno_t err = fopen_s(&file, path, "r");
    if (err != 0 || file == nullptr) {
        printf("Impossible to open the file: %s\n", path);
        return false;
    }

    // Read file line by line
    while (1) {
        char lineHeader[128];
        int res = fscanf_s(file, "%s", lineHeader, sizeof(lineHeader)); // Use fscanf_s
        if (res == EOF) {
            break; // End of file
        }

        // Parse vertices
        if (strcmp(lineHeader, "v") == 0) {
            glm::vec3 vertex;
            fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z); // Use fscanf_s
            temp_vertices.push_back(vertex);
        }
        // Parse texture coordinates
        else if (strcmp(lineHeader, "vt") == 0) {
            glm::vec2 uv;
            fscanf_s(file, "%f %f\n", &uv.x, &uv.y); // Use fscanf_s
            temp_uvs.push_back(uv);
        }
        // Parse normals
        else if (strcmp(lineHeader, "vn") == 0) {
            glm::vec3 normal;
            fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z); // Use fscanf_s
            temp_normals.push_back(normal);
        }
        // Parse faces
        else if (strcmp(lineHeader, "f") == 0) {
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
                &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                &vertexIndex[2], &uvIndex[2], &normalIndex[2]); // Use fscanf_s
            if (matches != 9) {
                printf("File can't be read by our simple parser. Try exporting with other options.\n");
                fclose(file);
                return false;
            }

            // Store the indices
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);
            uvIndices.push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }
    }

    // Now we have all the data in temporary arrays, so we can process them.
    // Convert the indices into final vertex data
    for (size_t i = 0; i < vertexIndices.size(); i++) {
        unsigned int vertexIndex = vertexIndices[i];
        unsigned int uvIndex = uvIndices[i];
        unsigned int normalIndex = normalIndices[i];

        // Correct index usage (OBJ format uses 1-based indexing)
        glm::vec3 vertex = temp_vertices[vertexIndex - 1];
        out_vertices.push_back(vertex);

        if (uvIndex > 0) { // Check if UV exists
            glm::vec2 uv = temp_uvs[uvIndex - 1];
            out_uvs.push_back(uv);
        }
        else {
            out_uvs.push_back(glm::vec2(0.0f, 0.0f)); // Default UV
        }

        if (normalIndex > 0) { // Check if normal exists
            glm::vec3 normal = temp_normals[normalIndex - 1];
            out_normals.push_back(normal);
        }
        else {
            out_normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f)); // Default normal
        }
    }

    fclose(file); // Always close the file after processing
    return true;
}

bool LoadOBJ(const char* path, std::vector<Vertex>& out_vertices, std::vector<unsigned int>& out_indices,bool removeDuplicates)
{
    // ��ʱ�洢���������洢ÿ������
    std::vector<glm::vec3> temp_positions;
    std::vector<glm::vec2> temp_texcoords;
    std::vector<glm::vec3> temp_normals;

    std::vector<unsigned int> positionIndices, texcoordIndices, normalIndices;

    // ���ļ�
    FILE* file = nullptr;
    errno_t err = fopen_s(&file, path, "r");
    if (err != 0 || file == nullptr) {
        std::cerr << "�޷����ļ�: " << path << std::endl;
        return false;
    }

    // ============��3.1. ���� OBJ �ļ���===========
    while (true) {
        char lineHeader[128];
        int res = fscanf_s(file, "%s", lineHeader, (unsigned)_countof(lineHeader));
        if (res == EOF) {
            // �ļ�����
            break;
        }

        // ����λ�� v
        if (strcmp(lineHeader, "v") == 0) {
            glm::vec3 pos;
            fscanf_s(file, "%f %f %f\n", &pos.x, &pos.y, &pos.z);
            temp_positions.push_back(pos);
        }
        // �������� vt
        else if (strcmp(lineHeader, "vt") == 0) {
            glm::vec2 uv;
            fscanf_s(file, "%f %f\n", &uv.x, &uv.y);
            temp_texcoords.push_back(uv);
        }
        // ���� vn
        else if (strcmp(lineHeader, "vn") == 0) {
            glm::vec3 norm;
            fscanf_s(file, "%f %f %f\n", &norm.x, &norm.y, &norm.z);
            temp_normals.push_back(norm);
        }
        // �� f
        else if (strcmp(lineHeader, "f") == 0) {
            unsigned int posIdx[3], uvIdx[3], nrmIdx[3];

            // ���� "f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3"
            int matches = fscanf_s(file,
                "%d/%d/%d %d/%d/%d %d/%d/%d\n",
                &posIdx[0], &uvIdx[0], &nrmIdx[0],
                &posIdx[1], &uvIdx[1], &nrmIdx[1],
                &posIdx[2], &uvIdx[2], &nrmIdx[2]);

            if (matches != 9) {
                std::cerr << "�ļ�����ʧ��(�������λ�ȱ������): " << path << std::endl;
                fclose(file);
                return false;
            }

            positionIndices.push_back(posIdx[0]);
            positionIndices.push_back(posIdx[1]);
            positionIndices.push_back(posIdx[2]);

            texcoordIndices.push_back(uvIdx[0]);
            texcoordIndices.push_back(uvIdx[1]);
            texcoordIndices.push_back(uvIdx[2]);

            normalIndices.push_back(nrmIdx[0]);
            normalIndices.push_back(nrmIdx[1]);
            normalIndices.push_back(nrmIdx[2]);
        }
        // ������ ����
        else {
            // ��ȡ���У��������
            char buffer[1024];
            fgets(buffer, sizeof(buffer), file);
        }
    }

    fclose(file);

    // ============��3.2. ��ϲ�ȥ��: (Position, TexCoord, Normal) ��===========
    // ������ unordered_map ���洢�����ض���
    std::unordered_map<Vertex, unsigned int, VertexHash> uniqueVertices;
    uniqueVertices.reserve(positionIndices.size()); // Ԥ���ռ䣨��ѡ��

    for (size_t i = 0; i < positionIndices.size(); i++)
    {
        // OBJ ������1��ʼ����Ҫ-1��Ϊ0-based
        unsigned int posIndex = positionIndices[i] - 1;
        unsigned int uvIndex = texcoordIndices[i] - 1;
        unsigned int nrmIndex = normalIndices[i] - 1;

        // ����һ�� Vertex
        Vertex vertex{};
        vertex.Position = (posIndex < temp_positions.size())
            ? temp_positions[posIndex]
            : glm::vec3(0.0f, 0.0f, 0.0f);

        if (uvIndex < temp_texcoords.size() && uvIndex != (unsigned int)(-1)) {
            vertex.TexCoords = temp_texcoords[uvIndex];
        }
        else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f); // Ĭ��UV
        }

        if (nrmIndex < temp_normals.size() && nrmIndex != (unsigned int)(-1)) {
            vertex.Normal = temp_normals[nrmIndex];
        }
        else {
            vertex.Normal = glm::vec3(0.0f, 0.0f, 1.0f); // Ĭ��Normal
        }

        // ����: ���δ���ֹ�������㣬�Ͳ��룻������֮ǰ������
        if (uniqueVertices.find(vertex) == uniqueVertices.end()) {
            unsigned int newIndex = static_cast<unsigned int>(out_vertices.size());
            out_vertices.push_back(vertex);
            uniqueVertices[vertex] = newIndex;
            out_indices.push_back(newIndex);
        }
        else {
            out_indices.push_back(uniqueVertices[vertex]);
        }
    }

    return true;
}

bool LoadOBJ(const char* path, std::vector<Vertex>& out_vertices, std::vector<unsigned int>& out_indices)
{
    //��Դ����ȷ��ÿ�μ���ģ�Ͷ���
    out_vertices.clear();
    out_indices.clear();
    // ��ʱ�洢���������洢ÿ������
    std::vector<glm::vec3> temp_positions;
    std::vector<glm::vec2> temp_texcoords;
    std::vector<glm::vec3> temp_normals;

    std::vector<unsigned int> positionIndices, texcoordIndices, normalIndices;

    // ���ļ�
    FILE* file = nullptr;
    errno_t err = fopen_s(&file, path, "r");
    if (err != 0 || file == nullptr) {
        std::cerr << "�޷����ļ�: " << path << std::endl;
        return false;
    }

    // ���� OBJ �ļ�
    while (true) {
        char lineHeader[128];
        int res = fscanf_s(file, "%s", lineHeader, (unsigned)_countof(lineHeader));
        if (res == EOF) {
            // �ļ�����
            break;
        }

        // ����λ�� v
        if (strcmp(lineHeader, "v") == 0) {
            glm::vec3 pos;
            fscanf_s(file, "%f %f %f\n", &pos.x, &pos.y, &pos.z);
            temp_positions.push_back(pos);
        }
        // �������� vt
        else if (strcmp(lineHeader, "vt") == 0) {
            glm::vec2 uv;
            fscanf_s(file, "%f %f\n", &uv.x, &uv.y);
            temp_texcoords.push_back(uv);
        }
        // ���� vn
        else if (strcmp(lineHeader, "vn") == 0) {
            glm::vec3 norm;
            fscanf_s(file, "%f %f %f\n", &norm.x, &norm.y, &norm.z);
            temp_normals.push_back(norm);
        }
        // �� f
        else if (strcmp(lineHeader, "f") == 0) {
            unsigned int posIdx[3], uvIdx[3], nrmIdx[3];

            // ���� "f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3"
            int matches = fscanf_s(file,
                "%d/%d/%d %d/%d/%d %d/%d/%d\n",
                &posIdx[0], &uvIdx[0], &nrmIdx[0],
                &posIdx[1], &uvIdx[1], &nrmIdx[1],
                &posIdx[2], &uvIdx[2], &nrmIdx[2]);

            if (matches != 9) {
                std::cerr << "�ļ�����ʧ��(�������λ�ȱ������): " << path << std::endl;
                fclose(file);
                return false;
            }

            // ��������ӵ������б�
            positionIndices.push_back(posIdx[0] - 1); // ���� OBJ ������ 1 ��ʼ�����ȥ 1
            positionIndices.push_back(posIdx[1] - 1);
            positionIndices.push_back(posIdx[2] - 1);

            texcoordIndices.push_back(uvIdx[0] - 1);
            texcoordIndices.push_back(uvIdx[1] - 1);
            texcoordIndices.push_back(uvIdx[2] - 1);

            normalIndices.push_back(nrmIdx[0] - 1);
            normalIndices.push_back(nrmIdx[1] - 1);
            normalIndices.push_back(nrmIdx[2] - 1);
        }
        // ������ ����
        else {
            char buffer[1024];
            fgets(buffer, sizeof(buffer), file);
        }
    }

    fclose(file);

    // ֱ�Ӱ�������䵽 out_vertices ��
    for (size_t i = 0; i < positionIndices.size(); i++) {
        unsigned int posIndex = positionIndices[i];
        unsigned int uvIndex = texcoordIndices[i];
        unsigned int nrmIndex = normalIndices[i];

        // ���춥�㲢��ӵ� out_vertices
        Vertex vertex{};
        vertex.Position = temp_positions[posIndex];
        if (uvIndex < temp_texcoords.size() && uvIndex != (unsigned int)(-1)) {
            vertex.TexCoords = temp_texcoords[uvIndex];
        }
        else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);  // Ĭ����������
        }
        vertex.Normal = temp_normals[nrmIndex];

        out_vertices.push_back(vertex);
        out_indices.push_back(out_indices.size()); // �����ǰ�˳�����
    }

    return true;
}


GLuint LoadPicTexture(const char* picImagePath)
{
    // 1. ʹ�� stb_image ����ͼƬ
    int picWidth, picHeight, picChannels;
    // ��������һ��������Ϊ 0����ʾ����ԭʼͨ����
    unsigned char* picData = stbi_load(picImagePath, &picWidth, &picHeight, &picChannels, 0);
    if (!picData)
    {
        std::cerr << "Failed to load image: " << picImagePath << std::endl;
        return 0; // ����0��ʾ����ʧ��
    }

    // 2. ȷ�� OpenGL �����ʽ
    GLenum picFormat;
    if (picChannels == 1)
        picFormat = GL_RED;
    else if (picChannels == 3)
        picFormat = GL_RGB;
    else if (picChannels == 4)
        picFormat = GL_RGBA;
    else
    {
        std::cerr << "Unsupported number of channels: " << picChannels << std::endl;
        stbi_image_free(picData);
        return 0;
    }

    // 3. ���ɲ����������
    GLuint picTextureID;
    glGenTextures(1, &picTextureID);
    glBindTexture(GL_TEXTURE_2D, picTextureID);

    // 4. �������������������������ʱ��ѡ����ֲ�ͬ�Ĳ�����ʽ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   // S�ỷ�Ʒ�ʽ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);   // T�ỷ�Ʒ�ʽ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // ��Сʱʹ������˫���Թ��˺�mipmap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // �Ŵ�ʱʹ�����Թ���

    // 5. �ϴ��������ݵ� GPU
    glTexImage2D(
        GL_TEXTURE_2D,
        0,                // ����0Ϊ��������
        picFormat,        // �ڲ���ʽ
        picWidth,
        picHeight,
        0,                // �߿򣬱���Ϊ0
        picFormat,        // ��ʽ
        GL_UNSIGNED_BYTE, // ��������
        picData           // ��������
    );

    // 6. ���� mipmap
    glGenerateMipmap(GL_TEXTURE_2D);

    // 7. �������
    glBindTexture(GL_TEXTURE_2D, 0);

    // 8. �ͷ�ͼƬ����
    stbi_image_free(picData);

    // 9. ��������ID
    return picTextureID;
}

std::vector<Vertex> LoadVerticesFromFile(const std::string& name)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    // �ϲ�·�����ļ���
    std::string path = "E:\\C++\\FirstOne\\C++Steam52\\Assets\\Obj\\" + name;

    // ���� LoadOBJ �����������ļ�
    if (!LoadOBJ(path.c_str(), vertices, indices)) {
        std::cerr << "���� OBJ �ļ�ʧ��: " << path << std::endl;
        return std::vector<Vertex>(); // ���ؿյĶ�������
    }

    return vertices; // ���ض�������
}

void MakeAnimation()
{

    //�����õı��ݴ��룬���ݿ���һֱ����
#pragma region  butterfly:fly

    // ��������ģ��-�����ؼ�֡����
    std::vector<std::vector<Vertex>> keyFrames;
    std::vector<float> keyFrameTimes = { 0.0f, 3.0f ,6.0f };  // ÿ���ؼ�֡��ʱ���

    // ��ӹؼ�֡����1,2,3
    keyFrames.push_back(LoadVerticesFromFile("butterfly1.obj"));
    keyFrames.push_back(LoadVerticesFromFile("butterfly2.obj"));
    keyFrames.push_back(LoadVerticesFromFile("butterfly1.obj"));
    //keyFrames.push_back(LoadVerticesFromFile("butterfly2.obj"));

    AnimationData animaitonData;
    animaitonData.keyFrames = keyFrames;
    animaitonData.keyFrameTimes = keyFrameTimes;
    animaitonData.loop = true;
    animaitonData.speed = 1.8f;

    AnimationDic["butterfly"]["fly"] = animaitonData;

#pragma endregion




}

void MakeModel()
{
    std::vector<Vertex> verticesStruct;
    std::vector<unsigned int> indices;

    LoadOBJ("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Obj\\butterfly1.obj", verticesStruct, indices);

    ModelData modelData;
    
    modelData.verticesStruct = verticesStruct;
    modelData.indices = indices;

    ModelDic["butterfly"] = modelData;

#pragma region ������״����
    //������
    LoadOBJ("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Obj\\baseCube.obj", verticesStruct, indices);
 
    modelData.verticesStruct = verticesStruct;
    modelData.indices = indices;

    ModelDic["baseCube"] = modelData;
    
    //��γ��
    LoadOBJ("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Obj\\baseSphere.obj", verticesStruct, indices);

    modelData.verticesStruct = verticesStruct;
    modelData.indices = indices;

    ModelDic["baseSphere"] = modelData;


    //Բ׶
    LoadOBJ("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Obj\\baseCone.obj", verticesStruct, indices);

    modelData.verticesStruct = verticesStruct;
    modelData.indices = indices;

    ModelDic["baseCone"] = modelData;

    //Բ��
    LoadOBJ("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Obj\\baseCylinder.obj", verticesStruct, indices);

    modelData.verticesStruct = verticesStruct;
    modelData.indices = indices;

    ModelDic["baseCylinder"] = modelData;


    //ƽ��
    LoadOBJ("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Obj\\basePlane.obj", verticesStruct, indices);

    modelData.verticesStruct = verticesStruct;
    modelData.indices = indices;

    ModelDic["basePlane"] = modelData;
    
    //���Ժ���
    LoadOBJ("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Obj\\testMonkey.obj", verticesStruct, indices);

    modelData.verticesStruct = verticesStruct;
    modelData.indices = indices;

    ModelDic["testMonkey"] = modelData;
#pragma endregion
#pragma region ��ʯͷ��Ϸ

    //��
    LoadOBJ("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Obj\\tree.obj", verticesStruct, indices);

    modelData.verticesStruct = verticesStruct;
    modelData.indices = indices;

    ModelDic["tree"] = modelData;

    //����
    LoadOBJ("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Obj\\chest.obj", verticesStruct, indices);

    modelData.verticesStruct = verticesStruct;
    modelData.indices = indices;

    ModelDic["chest"] = modelData;
#pragma endregion





}

void MakeTxture()
{
    GLuint defaultTexture = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\default.bmp");
    GLuint picTexture = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\1.png");
    GLuint lightTexture = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\light.bmp");
    GLuint skyboxTexture = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\front.jpg");
    //--����
    GLuint grassTexture = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\grass.jpg");
    GLuint stoneTexture = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\stone.jpg");
    //��ʯͷ��Ϸ��
    GLuint treeTexture = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\tree\\outPng\\Pine_tree_texture.png");
    GLuint treeTextureSpecular = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\tree\\outPng\\Pine_tree_specular.png");
    //��ʯͷ��Ϸ����
    GLuint chestTexture = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\tree\\outPng\\Chest_texture.png");
    GLuint chsetextureSpecular = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\tree\\outPng\\Chest_specular.png");

    //��ʼ������
    std::vector<GLuint> pic;
    //����Ĭ������
    pic.push_back(defaultTexture);
    TextureDic["default"] = pic;
    //���غ�������
    pic.clear();
    pic.push_back(picTexture);
    TextureDic["butterfly"] = pic;
    //���ع�Դ����Ⱦ����
    pic.clear();
    pic.push_back(lightTexture);
    TextureDic["light"] = pic;
    //������պ�
    pic.clear();
    pic.push_back(skyboxTexture);
    TextureDic["skybox"] = pic;
    //���زݵ�
    pic.clear();
    pic.push_back(grassTexture);
    TextureDic["grass"] = pic;
    //����ʯͷ��
    pic.clear();
    pic.push_back(stoneTexture);
    TextureDic["stone"] = pic;
#pragma region ��ʯͷ��Ϸ����
    //��
    pic.clear();
    pic.push_back(treeTexture);
    pic.push_back(treeTextureSpecular);// �������������ͼƬ
    TextureDic["tree"] = pic;

    //����
    pic.clear();
    pic.push_back(chestTexture);
    pic.push_back(chsetextureSpecular);// �������������ͼƬ
    TextureDic["chest"] = pic;
#pragma endregion



}

void MakeFronts()
{
    TextRender* textRender = TextRender::GetInstance();
    textRender->MakeFronts();
    
}



