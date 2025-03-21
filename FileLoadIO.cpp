#include "FileLoadIO.h"
#include <cstdio>
#include <vector>
#include <glm/glm.hpp>
#include <cstring>
#include "TextRender.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/ImporterDesc.h>
/// <summary>
/// ��Դ��������С�ͳ���������ȫ���ػ�����Դ
/// </summary>
namespace Game {
    std::unordered_map<std::string, std::unordered_map<std::string, AnimationData>> AnimationDic;
    std::unordered_map<std::string, ModelData> ModelDic;
    std::unordered_map<std::string, std::unordered_map<PictureTpye, GLuint>> TextureDic;
    std::unordered_map<std::string, Model> ModelDesignDic;
}
using namespace Game;
bool Game:: LoadOBJ(const char* path, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals)
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

bool Game::LoadOBJ(const char* path, std::vector<Vertex>& out_vertices, std::vector<unsigned int>& out_indices,bool removeDuplicates)
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

bool Game::LoadOBJ(const char* path, std::vector<Vertex>& out_vertices, std::vector<unsigned int>& out_indices)
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


GLuint Game::LoadPicTexture(const char* picImagePath)
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

std::vector<Vertex> Game::LoadVerticesFromFile(const std::string& name)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    // �ϲ�·�����ļ���
    std::string path = "Assets\\Obj\\" + name;

    // ���� LoadOBJ �����������ļ�
    if (!LoadOBJ(path.c_str(), vertices, indices)) {
        std::cerr << "���� OBJ �ļ�ʧ��: " << path << std::endl;
        return std::vector<Vertex>(); // ���ؿյĶ�������
    }

    return vertices; // ���ض�������
}

void Game::MakeAnimation()
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

    //-----����ʯͷ�ֶ��㶯��,�������������ؼ�֡
    keyFrames.clear();

    keyFrames.push_back(LoadVerticesFromFile("stoneMonster.obj"));
    keyFrames.push_back(LoadVerticesFromFile("stoneMonster2.obj"));
    keyFrames.push_back(LoadVerticesFromFile("stoneMonster.obj"));
    //keyFrames.push_back(LoadVerticesFromFile("butterfly2.obj"));

    animaitonData.keyFrames = keyFrames;
    animaitonData.keyFrameTimes = keyFrameTimes;
    animaitonData.loop = true;
    animaitonData.speed = 0.2f;

    AnimationDic["stoneMonster"]["run"] = animaitonData;
    
    keyFrames.clear();
    keyFrames.push_back(LoadVerticesFromFile("stoneMonster.obj"));
    keyFrames.push_back(LoadVerticesFromFile("stoneMonsterDeath3.obj"));
    keyFrames.push_back(LoadVerticesFromFile("stoneMonsterDeath3.obj"));
    animaitonData.keyFrames = keyFrames;
    animaitonData.keyFrameTimes = keyFrameTimes;
    animaitonData.loop = false;//����������ѭ��
    animaitonData.speed = 0.2f;
    AnimationDic["stoneMonster"]["die"] = animaitonData;


    keyFrames.clear();
    keyFrames.push_back(LoadVerticesFromFile("stoneMonster.obj"));
    keyFrames.push_back(LoadVerticesFromFile("stoneMonsterAttack1.obj"));
    keyFrames.push_back(LoadVerticesFromFile("stoneMonsterAttack2.obj"));
    keyFrames.push_back(LoadVerticesFromFile("stoneMonster.obj"));
    animaitonData.keyFrames = keyFrames;
    animaitonData.keyFrameTimes = keyFrameTimes;
    animaitonData.loop = true;//��������
    animaitonData.speed = 0.1f;
    AnimationDic["stoneMonster"]["attack"] = animaitonData;

    //������Ҷ���
    keyFrames.clear();
    keyFrames.push_back(LoadVerticesFromFile("player.obj"));
    keyFrames.push_back(LoadVerticesFromFile("player2.obj"));
    keyFrames.push_back(LoadVerticesFromFile("player.obj"));
    animaitonData.keyFrames = keyFrames;
    animaitonData.keyFrameTimes = keyFrameTimes;
    animaitonData.loop = true;
    animaitonData.speed = 1.8f;
    AnimationDic["playerEye"]["run"] = animaitonData;
#pragma endregion




}

void Game::MakeModel()
{
    std::vector<Vertex> verticesStruct;
    std::vector<unsigned int> indices;

    LoadOBJ("Assets\\Obj\\butterfly1.obj", verticesStruct, indices);

    ModelData modelData;
    
    modelData.verticesStruct = verticesStruct;
    modelData.indices = indices;

    ModelDic["butterfly"] = modelData;

#pragma region ������״����
    //������
    LoadOBJ("Assets\\Obj\\baseCube.obj", verticesStruct, indices);
 
    modelData.verticesStruct = verticesStruct;
    modelData.indices = indices;

    ModelDic["baseCube"] = modelData;
    
    //��γ��
    LoadOBJ("Assets\\Obj\\baseSphere.obj", verticesStruct, indices);

    modelData.verticesStruct = verticesStruct;
    modelData.indices = indices;

    ModelDic["baseSphere"] = modelData;


    //Բ׶
    LoadOBJ("Assets\\Obj\\baseCone.obj", verticesStruct, indices);

    modelData.verticesStruct = verticesStruct;
    modelData.indices = indices;

    ModelDic["baseCone"] = modelData;

    //Բ��
    LoadOBJ("Assets\\Obj\\baseCylinder.obj", verticesStruct, indices);

    modelData.verticesStruct = verticesStruct;
    modelData.indices = indices;

    ModelDic["baseCylinder"] = modelData;


    //ƽ��
    LoadOBJ("Assets\\Obj\\basePlane.obj", verticesStruct, indices);

    modelData.verticesStruct = verticesStruct;
    modelData.indices = indices;

    ModelDic["basePlane"] = modelData;
    
    //���Ժ���
    LoadOBJ("Assets\\Obj\\testMonkey.obj", verticesStruct, indices);

    modelData.verticesStruct = verticesStruct;
    modelData.indices = indices;

    ModelDic["testMonkey"] = modelData;
#pragma endregion
#pragma region ��ʯͷ��Ϸ

    //��
    LoadOBJ("Assets\\Obj\\tree.obj", verticesStruct, indices);

    modelData.verticesStruct = verticesStruct;
    modelData.indices = indices;

    ModelDic["tree"] = modelData;

    //����
    LoadOBJ("Assets\\Obj\\chest.obj", verticesStruct, indices);

    modelData.verticesStruct = verticesStruct;
    modelData.indices = indices;

    ModelDic["chest"] = modelData;


    //ʯͷ��
    LoadOBJ("Assets\\Obj\\stoneMonster.obj", verticesStruct, indices);

    modelData.verticesStruct = verticesStruct;
    modelData.indices = indices;

    ModelDic["stoneMonster"] = modelData;

    //��ʯ1
    LoadOBJ("Assets\\Obj\\stone1.obj", verticesStruct, indices);

    modelData.verticesStruct = verticesStruct;
    modelData.indices = indices;

    ModelDic["stone1"] = modelData;

    //��ʯ2
    LoadOBJ("Assets\\Obj\\stone2.obj", verticesStruct, indices);

    modelData.verticesStruct = verticesStruct;
    modelData.indices = indices;

    ModelDic["stone2"] = modelData;


    ////����
    //LoadOBJ("Assets\\Fbx\\backpack.obj", verticesStruct, indices);

    //modelData.verticesStruct = verticesStruct;
    //modelData.indices = indices;

    //ModelDic["backpack"] = modelData;
    //��
    LoadOBJ("Assets\\Obj\\grass.obj", verticesStruct, indices);

    modelData.verticesStruct = verticesStruct;
    modelData.indices = indices;

    ModelDic["grass1"] = modelData;
    //��2
    LoadOBJ("Assets\\Obj\\grass2.obj", verticesStruct, indices);

    modelData.verticesStruct = verticesStruct;
    modelData.indices = indices;

    ModelDic["grass2"] = modelData;
    //�ڶ�
    LoadOBJ("Assets\\Obj\\blackHole.obj", verticesStruct, indices);

    modelData.verticesStruct = verticesStruct;
    modelData.indices = indices;

    ModelDic["blackHole"] = modelData;

    //���
    LoadOBJ("Assets\\Obj\\player.obj", verticesStruct, indices);

    modelData.verticesStruct = verticesStruct;
    modelData.indices = indices;

    ModelDic["playerEye"] = modelData;
#pragma endregion





}

void Game::MakeModelFbx()
{
  // LoadFBX("chest", "Assets\\Fbx\\Chest.obj");
  //���ر���ģ��
    //LoadFBX("chest", "Assets\\Fbx\\Chest.obj");

  // LoadFBX("backpack", "Assets\\Fbx\\backpack.obj");
  // ͨ�����ƻ�ȡģ��
  auto& model = Game::ModelDesignDic["chest"];

  // ��ӡģ����Ϣ
  std::cout << "----- ģ����Ϣ -----" << std::endl;
  std::cout << "��������: " << model.meshes.size() << std::endl;
  for (size_t i = 0; i < model.meshes.size(); i++) {
      const auto& mesh = model.meshes[i];
      std::cout << "------------------------------" << std::endl;
      std::cout << "���� " << i << " ��������: " << mesh.vertices.size() << std::endl;
      std::cout << "���� " << i << " ��������: " << mesh.indices.size() << std::endl;

      // ����������ж��㣬��ӡ��һ���������ϸ��Ϣ
      if (!mesh.vertices.empty()) {
          const auto& vertex = mesh.vertices[0];
          std::cout << " ʾ������ 0 ��ϸ��Ϣ:" << std::endl;
          std::cout << "   λ��: (" << vertex.Position.x << ", " << vertex.Position.y << ", " << vertex.Position.z << ")" << std::endl;
          std::cout << "   ����: (" << vertex.Normal.x << ", " << vertex.Normal.y << ", " << vertex.Normal.z << ")" << std::endl;
          std::cout << "   ��������: (" << vertex.TexCoords.x << ", " << vertex.TexCoords.y << ")" << std::endl;
          std::cout << "   ����: (" << vertex.Tangent.x << ", " << vertex.Tangent.y << ", " << vertex.Tangent.z << ")" << std::endl;
          std::cout << "   ������: (" << vertex.Bitangent.x << ", " << vertex.Bitangent.y << ", " << vertex.Bitangent.z << ")" << std::endl;
          std::cout << "   ��������: ";
          for (int j = 0; j < 4; j++) {
              std::cout << "{" << vertex.BoneIDs[j] << ", " << vertex.Weights[j] << "} ";
          }
          std::cout << std::endl;
      }
  }
  std::cout << "------------------------------" << std::endl;
}

/// <summary>
/// �ڲ��������� ���ڼ�����պ�����
/// </summary>
/// <param name="faces"></param>
/// <returns></returns>
GLuint LoadCubemap(std::vector<std::string>& faces)
{
    GLuint _cubeMapID;
    
    glGenTextures(1, &_cubeMapID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeMapID);

    int width, height, nrChannels;
    for (GLuint i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            // stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return _cubeMapID;

}


void Game::MakeTxture()

{
    //���ؾ�̬��պ���������ͼ����
    stbi_set_flip_vertically_on_load(false);
    std::vector<std::string> faces = {
         "E:/C++/FirstOne/C++Steam52/Assets/Texture/skybox2/rightimage.png",  // ����
         "E:/C++/FirstOne/C++Steam52/Assets/Texture/skybox2/leftimage.png",   // ����
         "E:/C++/FirstOne/C++Steam52/Assets/Texture/skybox2/upimage.png",    // ����
         "E:/C++/FirstOne/C++Steam52/Assets/Texture/skybox2/downimage.png", // ����
         "E:/C++/FirstOne/C++Steam52/Assets/Texture/skybox2/backimage.png",    // ����
         "E:/C++/FirstOne/C++Steam52/Assets/Texture/skybox2/frontimage.png",  // ǰ��
          //opengl ��ǰ˳����ת
    };

    GLuint cubeMap = LoadCubemap(faces);//������պ�����
    //�������귭ת��ԭ����ͼ���ʽ����������opengl��һ�£�����ǳ���Ҫ
    stbi_set_flip_vertically_on_load(true);

    GLuint defaultTexture = LoadPicTexture("Assets\\Texture\\default.bmp");
    GLuint defaultW = LoadPicTexture("Assets\\Texture\\defaultW.jpg");
    GLuint picTexture = LoadPicTexture("Assets\\Texture\\butterfly.png");
    GLuint lightTexture = LoadPicTexture("Assets\\Texture\\light.bmp");
    GLuint skyboxTexture = LoadPicTexture("Assets\\Texture\\front.jpg");
    GLuint nosie1Texture = LoadPicTexture("Assets\\Texture\\noise1.png");
    GLuint nosie2Texture = LoadPicTexture("Assets\\Texture\\noise3.png");
    //--����
    GLuint grassTexture = LoadPicTexture("Assets\\Texture\\grass.jpg");
    GLuint stoneTexture = LoadPicTexture("Assets\\Texture\\stone.jpg");
    //��ʯͷ��Ϸ��
    GLuint treeTexture = LoadPicTexture("Assets\\Texture\\tree\\outPng\\Pine_tree_texture.png");
    //GLuint treeTexture = LoadPicTexture("Assets\\Texture\\tree\\outPng\\treePaint.png");
    GLuint treeTextureNromal = LoadPicTexture("Assets\\Texture\\tree\\outPng\\Pine_tree_normal.png");
    GLuint treeTextureSpecular = LoadPicTexture("Assets\\Texture\\tree\\outPng\\Pine_tree_specular.png");
    //��ʯͷ��Ϸ����
    GLuint chestTexture = LoadPicTexture("Assets\\Texture\\tree\\outPng\\Chest_texture.png");
    GLuint chestTextureNormal = LoadPicTexture("Assets\\Texture\\tree\\outPng\\Chest_normal.png");
    GLuint chseTextureSpecular = LoadPicTexture("Assets\\Texture\\tree\\outPng\\Chest_specular.png");
    //ʯͷ��
    GLuint stoneMonsterTexture = LoadPicTexture("Assets\\Texture\\stoneMonster.png");
    //��ʯ����
    GLuint stone = LoadPicTexture("Assets\\Texture\\tree\\outPng\\Rocks_3_4_texture.png");
    GLuint stoneNormal = LoadPicTexture("Assets\\Texture\\tree\\outPng\\Rocks_3_4_normal.png");
    GLuint stoneSpecular = LoadPicTexture("Assets\\Texture\\tree\\outPng\\Rocks_3_4_specular.png");
    //���屳��
    GLuint backpage = LoadPicTexture("Assets\\Texture\\backpack\\diffuse.jpg");
    GLuint backpageNormal = LoadPicTexture("Assets\\Texture\\backpack\\normal.png");
    GLuint backpageSpecular = LoadPicTexture("Assets\\Texture\\backpack\\specular.jpg");
    GLuint backpageRoughnees = LoadPicTexture("Assets\\Texture\\backpack\\roughness.jpg");
    GLuint backpageAO = LoadPicTexture("Assets\\Texture\\backpack\\ao.jpg");
    //���ֲ�
    GLuint grass1Texture = LoadPicTexture("Assets\\Texture\\grass1.png");
    GLuint grass2Texture = LoadPicTexture("Assets\\Texture\\grass2.png");
    //���ݵ�
    GLuint water = LoadPicTexture("Assets\\Texture\\water\\waterDiffuse.png");
    GLuint waterNormal = LoadPicTexture("Assets\\Texture\\water\\waterNormal.png");
    GLuint waterSpecular = LoadPicTexture("Assets\\Texture\\water\\waterNormal1.png");
    GLuint waterHeight = LoadPicTexture("Assets\\Texture\\water\\waterHeight.png");
    GLuint waterOpacity = LoadPicTexture("Assets\\Texture\\water\\waterOpacity.png");
    //����
    GLuint wheelTexture = LoadPicTexture("Assets\\Texture\\wheel.png");
    //��ʼ������
    std::vector<GLuint> picd;
    //����Ĭ������
    picd.push_back(defaultTexture);//Ĭ�ϻ�ɫ����ͼ
    picd.push_back(defaultW);//Ĭ�ϰ�ɫ����ͼ
    picd.push_back(defaultW);//Ĭ�ϰ�ɫ2D����ͼ
    picd.push_back(defaultW);//Ĭ�ϰ�ɫ����ͼ
    picd.push_back(defaultW);//Ĭ�ϰ�ɫ�߶�ͼ
    picd.push_back(defaultW);//Ĭ�ϰ�ɫ�ڶ�ͼ
    picd.push_back(defaultW);//Ĭ�ϰ�ɫ͸������ͼ
    picd.push_back(cubeMap);//Ĭ����պл�������ͼ
    picd.push_back(defaultW);//Ĭ�ϰ�ɫ����ͼ
    picd.push_back(nosie1Texture);//��������1
    picd.push_back(nosie2Texture);//��������2
    TextureDic["default"][PictureTpye::BaseP] = picd[0];
    TextureDic["default"][PictureTpye::NormalP] = picd[1];
    TextureDic["default"][PictureTpye::SpecularP] = picd[2];
    TextureDic["default"][PictureTpye::HeightP] = picd[3];
    TextureDic["default"][PictureTpye::RoughnessP] = picd[4];
    TextureDic["default"][PictureTpye::AOP]= picd[5];
    TextureDic["default"][PictureTpye::OpacityP] = picd[6];
    TextureDic["default"][PictureTpye::SpecularCubeP] = picd[7];
    TextureDic["default"][PictureTpye::OtherP] = picd[8];
    TextureDic["default"][PictureTpye::Noise1P] = picd[9];
    TextureDic["default"][PictureTpye::Noise2P] = picd[10];
    //��ʼ������       
    std::vector<GLuint> pic;
    //���غ�������
    pic.clear();
    pic.push_back(picTexture);
    TextureDic["butterfly"][PictureTpye::BaseP] = pic[0];
    TextureDic["butterfly"][PictureTpye::NormalP] = picd[1];
    TextureDic["butterfly"][PictureTpye::SpecularP] = picd[2];
    TextureDic["butterfly"][PictureTpye::HeightP] = picd[3];
    TextureDic["butterfly"][PictureTpye::RoughnessP] = picd[4];
    TextureDic["butterfly"][PictureTpye::AOP] = picd[5];
    TextureDic["butterfly"][PictureTpye::OpacityP] = picd[6];
    TextureDic["butterfly"][PictureTpye::SpecularCubeP] = picd[7];
    TextureDic["butterfly"][PictureTpye::OtherP] = picd[8];
    //���ع�Դ����Ⱦ����
    pic.clear();
    pic.push_back(lightTexture);
    TextureDic["light"][PictureTpye::BaseP] = pic[0];
    //������պ�
    pic.clear();
    pic.push_back(skyboxTexture);
    TextureDic["skybox"][PictureTpye::BaseP] = pic[0];
    //���زݵ�
    pic.clear();
    pic.push_back(grassTexture);
    TextureDic["grass"][PictureTpye::BaseP] = pic[0];
    TextureDic["grass"][PictureTpye::NormalP] = picd[1];
    TextureDic["grass"][PictureTpye::SpecularP] = picd[2];
    TextureDic["grass"][PictureTpye::HeightP] = picd[3];
    TextureDic["grass"][PictureTpye::RoughnessP] = picd[4];
    TextureDic["grass"][PictureTpye::AOP] = picd[5];
    TextureDic["grass"][PictureTpye::OpacityP] = picd[6];
    TextureDic["grass"][PictureTpye::SpecularCubeP] = picd[7];
    TextureDic["grass"][PictureTpye::OtherP] = picd[8];
    //����ʯͷ
    pic.clear();
    pic.push_back(stoneTexture);
    TextureDic["stone"][PictureTpye::BaseP] = pic[0];
    TextureDic["stone"][PictureTpye::NormalP] = picd[1];
    TextureDic["stone"][PictureTpye::SpecularP] = picd[2];
    TextureDic["stone"][PictureTpye::HeightP] = picd[3];
    TextureDic["stone"][PictureTpye::RoughnessP] = picd[4];
    TextureDic["stone"][PictureTpye::AOP] = picd[5];
    TextureDic["stone"][PictureTpye::OpacityP] = picd[6];
    TextureDic["stone"][PictureTpye::SpecularCubeP] = picd[7];
    TextureDic["stone"][PictureTpye::OtherP] = picd[8];

#pragma region ��ʯͷ��Ϸ����
    //��
    pic.clear();
    pic.push_back(treeTexture);
    pic.push_back(treeTextureNromal);
    pic.push_back(treeTextureSpecular);
    TextureDic["tree"][PictureTpye::BaseP] = pic[0];
    TextureDic["tree"][PictureTpye::NormalP] = pic[1];
    TextureDic["tree"][PictureTpye::SpecularP] = pic[2];
    TextureDic["tree"][PictureTpye::HeightP] = picd[3];
    TextureDic["tree"][PictureTpye::RoughnessP] = picd[4];
    TextureDic["tree"][PictureTpye::AOP] = picd[5];
    TextureDic["tree"][PictureTpye::OpacityP] = picd[6];
    TextureDic["tree"][PictureTpye::SpecularCubeP] = picd[7];
    TextureDic["tree"][PictureTpye::OtherP] = picd[8];

    //����
    pic.clear();
    pic.push_back(chestTexture);
    pic.push_back(chestTextureNormal);
    pic.push_back(chseTextureSpecular);// 
    TextureDic["chest"][PictureTpye::BaseP] = pic[0];
    TextureDic["chest"][PictureTpye::HeightP] = pic[1];
    TextureDic["chest"][PictureTpye::SpecularP] = pic[2];
    TextureDic["chest"][PictureTpye::HeightP] = picd[3];
    TextureDic["chest"][PictureTpye::RoughnessP] = picd[4];
    TextureDic["chest"][PictureTpye::AOP] = picd[5];
    TextureDic["chest"][PictureTpye::OpacityP] = picd[6];
    TextureDic["chest"][PictureTpye::SpecularCubeP] = picd[7];
    TextureDic["chest"][PictureTpye::OtherP] = picd[8];

    //ʯͷ��
    pic.clear();
    pic.push_back(stoneMonsterTexture);
    TextureDic["stoneMonster"][PictureTpye::BaseP] = pic[0];
    TextureDic["stoneMonster"][PictureTpye::NormalP] = picd[1];
    TextureDic["stoneMonster"][PictureTpye::SpecularP] = picd[2];
    TextureDic["stoneMonster"][PictureTpye::HeightP] = picd[3];
    TextureDic["stoneMonster"][PictureTpye::RoughnessP] = picd[4];
    TextureDic["stoneMonster"][PictureTpye::AOP] = picd[5];
    TextureDic["stoneMonster"][PictureTpye::OpacityP] = picd[6];
    TextureDic["stoneMonster"][PictureTpye::SpecularCubeP] = picd[7];
    TextureDic["stoneMonster"][PictureTpye::OtherP] = picd[8];

    //��ʯʵ����
    pic.clear();
    pic.push_back(stone);
    pic.push_back(stoneNormal);
    pic.push_back(stoneSpecular);// 
    TextureDic["stoneInstance"][PictureTpye::BaseP] = pic[0];
    TextureDic["stoneInstance"][PictureTpye::HeightP] = pic[1];
    TextureDic["stoneInstance"][PictureTpye::SpecularP] = pic[2];
    TextureDic["stoneInstance"][PictureTpye::HeightP] = picd[3];
    TextureDic["stoneInstance"][PictureTpye::RoughnessP] = picd[4];
    TextureDic["stoneInstance"][PictureTpye::AOP] = picd[5];
    TextureDic["stoneInstance"][PictureTpye::OpacityP] = picd[6];
    TextureDic["stoneInstance"][PictureTpye::SpecularCubeP] = picd[7];
    TextureDic["stoneInstance"][PictureTpye::OtherP] = picd[8];
    //���屳��
    pic.clear();
    pic.push_back(backpage);
    pic.push_back(backpageNormal);
    pic.push_back(backpageSpecular);
    pic.push_back(backpageRoughnees);
    pic.push_back(backpageAO);
    TextureDic["backpack"][PictureTpye::BaseP] = pic[0];
    TextureDic["backpack"][PictureTpye::NormalP] = pic[1];
    TextureDic["backpack"][PictureTpye::SpecularP] = pic[2];
    TextureDic["backpack"][PictureTpye::HeightP] = picd[3];
    TextureDic["backpack"][PictureTpye::RoughnessP] = pic[3];
    TextureDic["backpack"][PictureTpye::AOP] = pic[4];
    TextureDic["backpack"][PictureTpye::OpacityP] = picd[6];
    TextureDic["backpack"][PictureTpye::SpecularCubeP] = picd[7];
    TextureDic["backpack"][PictureTpye::OtherP] = picd[8];
    //�ڶ�����ʱ������ͼ������
    TextureDic["blackHole"][PictureTpye::BaseP] = picd[9];//����ͼ1
    TextureDic["blackHole"][PictureTpye::NormalP] = pic[1];
    TextureDic["blackHole"][PictureTpye::SpecularP] = pic[2];
    TextureDic["blackHole"][PictureTpye::HeightP] = picd[3];
    TextureDic["blackHole"][PictureTpye::RoughnessP] = pic[3];
    TextureDic["blackHole"][PictureTpye::AOP] = pic[4];
    TextureDic["blackHole"][PictureTpye::OpacityP] = picd[6];
    TextureDic["blackHole"][PictureTpye::SpecularCubeP] = picd[7];
    TextureDic["blackHole"][PictureTpye::OtherP] = picd[8];
    //ʵ������1
    pic.clear();
    pic.push_back(grass1Texture);
    TextureDic["grass1"][PictureTpye::BaseP] = pic[0];
    //ʵ������2
    pic.clear();
    pic.push_back(grass2Texture);
    TextureDic["grass2"][PictureTpye::BaseP] = pic[0];
    //���ݵ����е���
    pic.clear();
    //pic.push_back(water);
    pic.push_back(wheelTexture);//����ʹ��wheel�ƺ����ſ�
    pic.push_back(waterNormal);
    pic.push_back(waterSpecular);
    pic.push_back(waterHeight);
    pic.push_back(waterOpacity);
    TextureDic["water"][PictureTpye::BaseP] = pic[0];
    TextureDic["water"][PictureTpye::NormalP] = pic[1];
    TextureDic["water"][PictureTpye::SpecularP] = pic[2];
    TextureDic["water"][PictureTpye::HeightP] = picd[3];
    TextureDic["water"][PictureTpye::RoughnessP] = picd[4];
    TextureDic["water"][PictureTpye::AOP] = picd[5];
    TextureDic["water"][PictureTpye::OpacityP] = pic[4];
    TextureDic["water"][PictureTpye::SpecularCubeP] = picd[7];
    TextureDic["water"][PictureTpye::OtherP] = picd[8];
    //����
    pic.clear();
    pic.push_back(wheelTexture);
    pic.push_back(waterNormal);
    pic.push_back(waterSpecular);
    pic.push_back(waterHeight);
    pic.push_back(waterOpacity);
    TextureDic["wheel"][PictureTpye::BaseP] = pic[0];
    TextureDic["wheel"][PictureTpye::NormalP] = pic[1];
    TextureDic["wheel"][PictureTpye::SpecularP] = pic[2];
    TextureDic["wheel"][PictureTpye::HeightP] = picd[3];
    TextureDic["wheel"][PictureTpye::RoughnessP] = picd[4];
    TextureDic["wheel"][PictureTpye::AOP] = picd[5];
    TextureDic["wheel"][PictureTpye::OpacityP] = pic[4];
    TextureDic["wheel"][PictureTpye::SpecularCubeP] = picd[7];
    TextureDic["wheel"][PictureTpye::OtherP] = picd[8];

#pragma endregion



}

void Game::MakeFronts()
{
    TextRender* textRender = TextRender::GetInstance();
    textRender->MakeFronts();
    
}
// ����������Ϊ����������ӹ������ݣ���� 4 ��Ӱ�죩
void AddBoneData(MeshVertex& vertex, int boneID, float weight) {
    for (int i = 0; i < 4; i++) {
        if (vertex.Weights[i] == 0.0f) {
            vertex.BoneIDs[i] = boneID;
            vertex.Weights[i] = weight;
            break;
        }
    }
}

// ������������ȡ���㡢���������ߡ�Bitangent �Լ���������
Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene) {
    Mesh result;
    // Ԥ�ȷ��䶥��ռ�
    result.vertices.resize(mesh->mNumVertices);

    // ����������
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        MeshVertex vertex;
        // ����λ��
        vertex.Position = glm::vec3(
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z
        );

        // ����
        if (mesh->HasNormals()) {
            vertex.Normal = glm::vec3(
                mesh->mNormals[i].x,
                mesh->mNormals[i].y,
                mesh->mNormals[i].z
            );
        }
        else {
            vertex.Normal = glm::vec3(0.0f, 0.0f, 0.0f);
        }

        // �������ֻ꣨��ȡ��һ���������꣩
        if (mesh->mTextureCoords[0]) {
            vertex.TexCoords = glm::vec2(
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y
            );
        }
        else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        // �����븱����
        if (mesh->HasTangentsAndBitangents()) {
            vertex.Tangent = glm::vec3(
                mesh->mTangents[i].x,
                mesh->mTangents[i].y,
                mesh->mTangents[i].z
            );
            vertex.Bitangent = glm::vec3(
                mesh->mBitangents[i].x,
                mesh->mBitangents[i].y,
                mesh->mBitangents[i].z
            );
        }
        else {
            vertex.Tangent = glm::vec3(0.0f, 0.0f, 0.0f);
            vertex.Bitangent = glm::vec3(0.0f, 0.0f, 0.0f);
        }

        result.vertices[i] = vertex;
    }

    // ��������������ÿ���涼�������Σ�
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            result.indices.push_back(face.mIndices[j]);
        }
    }

    // ����������ݣ�������ڣ�
    if (mesh->HasBones()) {
        for (unsigned int i = 0; i < mesh->mNumBones; i++) {
            aiBone* bone = mesh->mBones[i];
            int boneID = i; // ��ʾ����ֱ��ʹ�õ�ǰ������ mBones �����е�������Ϊ���� ID
            for (unsigned int j = 0; j < bone->mNumWeights; j++) {
                unsigned int vertexID = bone->mWeights[j].mVertexId;
                float weight = bone->mWeights[j].mWeight;
                AddBoneData(result.vertices[vertexID], boneID, weight);
            }
        }
    }

    return result;
}

// �ݹ鴦��ڵ㣬��������ͼ����ȡ��������
void ProcessNode(aiNode* node, const aiScene* scene, Model& model) {
    // ����ǰ�ڵ��е���������
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        model.meshes.push_back(ProcessMesh(mesh, scene));
    }

    // �ݹ鴦�������ӽڵ�
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene, model);
    }
}

// ���� FBX �ļ����洢��ȫ��ģ���ֵ� ModelDesignDic ��
bool Game:: LoadFBX(const std::string& name, const std::string& path) {
    // ���ģ���Ѽ��أ��������ظ�����
    if (ModelDesignDic.find(name) != ModelDesignDic.end()) {
        std::cout << "ģ�� " << name << " �Ѿ����ع��������ظ����ء�" << std::endl;
        return true;
    }
    // ���� Assimp �������������ļ�
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return false;
    }

    // ���������Ӹ��ڵ㿪ʼ�ݹ���ȡ��������
    Model model;
    ProcessNode(scene->mRootNode, scene, model);

    // �����ص�ģ�ͱ��浽ȫ���ֵ���
    ModelDesignDic[name] = model;

    std::cout << "ģ�� " << name << " ���سɹ���" << std::endl;
    return true;
}




