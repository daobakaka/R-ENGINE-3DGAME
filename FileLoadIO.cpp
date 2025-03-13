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
/// 资源缓存区域，小型场景可以完全加载缓存资源
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
    // 临时存储：按索引存储每种属性
    std::vector<glm::vec3> temp_positions;
    std::vector<glm::vec2> temp_texcoords;
    std::vector<glm::vec3> temp_normals;

    std::vector<unsigned int> positionIndices, texcoordIndices, normalIndices;

    // 打开文件
    FILE* file = nullptr;
    errno_t err = fopen_s(&file, path, "r");
    if (err != 0 || file == nullptr) {
        std::cerr << "无法打开文件: " << path << std::endl;
        return false;
    }

    // ============【3.1. 解析 OBJ 文件】===========
    while (true) {
        char lineHeader[128];
        int res = fscanf_s(file, "%s", lineHeader, (unsigned)_countof(lineHeader));
        if (res == EOF) {
            // 文件结束
            break;
        }

        // 顶点位置 v
        if (strcmp(lineHeader, "v") == 0) {
            glm::vec3 pos;
            fscanf_s(file, "%f %f %f\n", &pos.x, &pos.y, &pos.z);
            temp_positions.push_back(pos);
        }
        // 纹理坐标 vt
        else if (strcmp(lineHeader, "vt") == 0) {
            glm::vec2 uv;
            fscanf_s(file, "%f %f\n", &uv.x, &uv.y);
            temp_texcoords.push_back(uv);
        }
        // 法线 vn
        else if (strcmp(lineHeader, "vn") == 0) {
            glm::vec3 norm;
            fscanf_s(file, "%f %f %f\n", &norm.x, &norm.y, &norm.z);
            temp_normals.push_back(norm);
        }
        // 面 f
        else if (strcmp(lineHeader, "f") == 0) {
            unsigned int posIdx[3], uvIdx[3], nrmIdx[3];

            // 解析 "f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3"
            int matches = fscanf_s(file,
                "%d/%d/%d %d/%d/%d %d/%d/%d\n",
                &posIdx[0], &uvIdx[0], &nrmIdx[0],
                &posIdx[1], &uvIdx[1], &nrmIdx[1],
                &posIdx[2], &uvIdx[2], &nrmIdx[2]);

            if (matches != 9) {
                std::cerr << "文件解析失败(非三角形或缺少属性): " << path << std::endl;
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
        // 其他行 忽略
        else {
            // 读取整行，避免干扰
            char buffer[1024];
            fgets(buffer, sizeof(buffer), file);
        }
    }

    fclose(file);

    // ============【3.2. 组合并去重: (Position, TexCoord, Normal) 】===========
    // 这里用 unordered_map 来存储并查重顶点
    std::unordered_map<Vertex, unsigned int, VertexHash> uniqueVertices;
    uniqueVertices.reserve(positionIndices.size()); // 预留空间（可选）

    for (size_t i = 0; i < positionIndices.size(); i++)
    {
        // OBJ 索引从1开始，需要-1变为0-based
        unsigned int posIndex = positionIndices[i] - 1;
        unsigned int uvIndex = texcoordIndices[i] - 1;
        unsigned int nrmIndex = normalIndices[i] - 1;

        // 构造一个 Vertex
        Vertex vertex{};
        vertex.Position = (posIndex < temp_positions.size())
            ? temp_positions[posIndex]
            : glm::vec3(0.0f, 0.0f, 0.0f);

        if (uvIndex < temp_texcoords.size() && uvIndex != (unsigned int)(-1)) {
            vertex.TexCoords = temp_texcoords[uvIndex];
        }
        else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f); // 默认UV
        }

        if (nrmIndex < temp_normals.size() && nrmIndex != (unsigned int)(-1)) {
            vertex.Normal = temp_normals[nrmIndex];
        }
        else {
            vertex.Normal = glm::vec3(0.0f, 0.0f, 1.0f); // 默认Normal
        }

        // 查重: 如果未出现过这个顶点，就插入；否则复用之前的索引
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
    //资源清理，确保每次加载模型独立
    out_vertices.clear();
    out_indices.clear();
    // 临时存储：按索引存储每种属性
    std::vector<glm::vec3> temp_positions;
    std::vector<glm::vec2> temp_texcoords;
    std::vector<glm::vec3> temp_normals;

    std::vector<unsigned int> positionIndices, texcoordIndices, normalIndices;

    // 打开文件
    FILE* file = nullptr;
    errno_t err = fopen_s(&file, path, "r");
    if (err != 0 || file == nullptr) {
        std::cerr << "无法打开文件: " << path << std::endl;
        return false;
    }

    // 解析 OBJ 文件
    while (true) {
        char lineHeader[128];
        int res = fscanf_s(file, "%s", lineHeader, (unsigned)_countof(lineHeader));
        if (res == EOF) {
            // 文件结束
            break;
        }

        // 顶点位置 v
        if (strcmp(lineHeader, "v") == 0) {
            glm::vec3 pos;
            fscanf_s(file, "%f %f %f\n", &pos.x, &pos.y, &pos.z);
            temp_positions.push_back(pos);
        }
        // 纹理坐标 vt
        else if (strcmp(lineHeader, "vt") == 0) {
            glm::vec2 uv;
            fscanf_s(file, "%f %f\n", &uv.x, &uv.y);
            temp_texcoords.push_back(uv);
        }
        // 法线 vn
        else if (strcmp(lineHeader, "vn") == 0) {
            glm::vec3 norm;
            fscanf_s(file, "%f %f %f\n", &norm.x, &norm.y, &norm.z);
            temp_normals.push_back(norm);
        }
        // 面 f
        else if (strcmp(lineHeader, "f") == 0) {
            unsigned int posIdx[3], uvIdx[3], nrmIdx[3];

            // 解析 "f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3"
            int matches = fscanf_s(file,
                "%d/%d/%d %d/%d/%d %d/%d/%d\n",
                &posIdx[0], &uvIdx[0], &nrmIdx[0],
                &posIdx[1], &uvIdx[1], &nrmIdx[1],
                &posIdx[2], &uvIdx[2], &nrmIdx[2]);

            if (matches != 9) {
                std::cerr << "文件解析失败(非三角形或缺少属性): " << path << std::endl;
                fclose(file);
                return false;
            }

            // 将索引添加到索引列表
            positionIndices.push_back(posIdx[0] - 1); // 由于 OBJ 索引从 1 开始，需减去 1
            positionIndices.push_back(posIdx[1] - 1);
            positionIndices.push_back(posIdx[2] - 1);

            texcoordIndices.push_back(uvIdx[0] - 1);
            texcoordIndices.push_back(uvIdx[1] - 1);
            texcoordIndices.push_back(uvIdx[2] - 1);

            normalIndices.push_back(nrmIdx[0] - 1);
            normalIndices.push_back(nrmIdx[1] - 1);
            normalIndices.push_back(nrmIdx[2] - 1);
        }
        // 其他行 忽略
        else {
            char buffer[1024];
            fgets(buffer, sizeof(buffer), file);
        }
    }

    fclose(file);

    // 直接按索引填充到 out_vertices 中
    for (size_t i = 0; i < positionIndices.size(); i++) {
        unsigned int posIndex = positionIndices[i];
        unsigned int uvIndex = texcoordIndices[i];
        unsigned int nrmIndex = normalIndices[i];

        // 构造顶点并添加到 out_vertices
        Vertex vertex{};
        vertex.Position = temp_positions[posIndex];
        if (uvIndex < temp_texcoords.size() && uvIndex != (unsigned int)(-1)) {
            vertex.TexCoords = temp_texcoords[uvIndex];
        }
        else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);  // 默认纹理坐标
        }
        vertex.Normal = temp_normals[nrmIndex];

        out_vertices.push_back(vertex);
        out_indices.push_back(out_indices.size()); // 索引是按顺序递增
    }

    return true;
}


GLuint Game::LoadPicTexture(const char* picImagePath)
{
    // 1. 使用 stb_image 加载图片
    int picWidth, picHeight, picChannels;
    // 这里的最后一个参数设为 0，表示保持原始通道数
    unsigned char* picData = stbi_load(picImagePath, &picWidth, &picHeight, &picChannels, 0);
    if (!picData)
    {
        std::cerr << "Failed to load image: " << picImagePath << std::endl;
        return 0; // 返回0表示加载失败
    }

    // 2. 确定 OpenGL 纹理格式
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

    // 3. 生成并绑定纹理对象
    GLuint picTextureID;
    glGenTextures(1, &picTextureID);
    glBindTexture(GL_TEXTURE_2D, picTextureID);

    // 4. 设置纹理参数，这里加载纹理的时候，选择各种不同的采样方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   // S轴环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);   // T轴环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // 缩小时使用线性双线性过滤和mipmap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // 放大时使用线性过滤

    // 5. 上传纹理数据到 GPU
    glTexImage2D(
        GL_TEXTURE_2D,
        0,                // 级别，0为基础级别
        picFormat,        // 内部格式
        picWidth,
        picHeight,
        0,                // 边框，必须为0
        picFormat,        // 格式
        GL_UNSIGNED_BYTE, // 数据类型
        picData           // 像素数据
    );

    // 6. 生成 mipmap
    glGenerateMipmap(GL_TEXTURE_2D);

    // 7. 解绑纹理
    glBindTexture(GL_TEXTURE_2D, 0);

    // 8. 释放图片数据
    stbi_image_free(picData);

    // 9. 返回纹理ID
    return picTextureID;
}

std::vector<Vertex> Game::LoadVerticesFromFile(const std::string& name)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    // 合并路径与文件名
    std::string path = "E:\\C++\\FirstOne\\C++Steam52\\Assets\\Obj\\" + name;

    // 调用 LoadOBJ 函数来加载文件
    if (!LoadOBJ(path.c_str(), vertices, indices)) {
        std::cerr << "加载 OBJ 文件失败: " << path << std::endl;
        return std::vector<Vertex>(); // 返回空的顶点数组
    }

    return vertices; // 返回顶点数据
}

void Game::MakeAnimation()
{

    //非引用的备份传入，内容可以一直更改
#pragma region  butterfly:fly

    // 动画控制模块-创建关键帧数据
    std::vector<std::vector<Vertex>> keyFrames;
    std::vector<float> keyFrameTimes = { 0.0f, 3.0f ,6.0f };  // 每个关键帧的时间点

    // 添加关键帧数据1,2,3
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

    //加载石头怪顶点动画
    keyFrames.clear();

    // 添加关键帧数据1,2,3
    keyFrames.push_back(LoadVerticesFromFile("stoneMonster.obj"));
    keyFrames.push_back(LoadVerticesFromFile("stoneMonster2.obj"));
    keyFrames.push_back(LoadVerticesFromFile("stoneMonster.obj"));
    //keyFrames.push_back(LoadVerticesFromFile("butterfly2.obj"));

    animaitonData.keyFrames = keyFrames;
    animaitonData.keyFrameTimes = keyFrameTimes;
    animaitonData.loop = true;
    animaitonData.speed = 0.2f;

    AnimationDic["stoneMonster"]["fly"] = animaitonData;





#pragma endregion




}

void Game::MakeModel()
{
    std::vector<Vertex> verticesStruct;
    std::vector<unsigned int> indices;

    LoadOBJ("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Obj\\butterfly1.obj", verticesStruct, indices);

    ModelData modelData;
    
    modelData.verticesStruct = verticesStruct;
    modelData.indices = indices;

    ModelDic["butterfly"] = modelData;

#pragma region 基础形状区域
    //立方体
    LoadOBJ("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Obj\\baseCube.obj", verticesStruct, indices);
 
    modelData.verticesStruct = verticesStruct;
    modelData.indices = indices;

    ModelDic["baseCube"] = modelData;
    
    //经纬球
    LoadOBJ("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Obj\\baseSphere.obj", verticesStruct, indices);

    modelData.verticesStruct = verticesStruct;
    modelData.indices = indices;

    ModelDic["baseSphere"] = modelData;


    //圆锥
    LoadOBJ("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Obj\\baseCone.obj", verticesStruct, indices);

    modelData.verticesStruct = verticesStruct;
    modelData.indices = indices;

    ModelDic["baseCone"] = modelData;

    //圆柱
    LoadOBJ("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Obj\\baseCylinder.obj", verticesStruct, indices);

    modelData.verticesStruct = verticesStruct;
    modelData.indices = indices;

    ModelDic["baseCylinder"] = modelData;


    //平面
    LoadOBJ("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Obj\\basePlane.obj", verticesStruct, indices);

    modelData.verticesStruct = verticesStruct;
    modelData.indices = indices;

    ModelDic["basePlane"] = modelData;
    
    //测试猴子
    LoadOBJ("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Obj\\testMonkey.obj", verticesStruct, indices);

    modelData.verticesStruct = verticesStruct;
    modelData.indices = indices;

    ModelDic["testMonkey"] = modelData;
#pragma endregion
#pragma region 打石头游戏

    //树
    LoadOBJ("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Obj\\tree.obj", verticesStruct, indices);

    modelData.verticesStruct = verticesStruct;
    modelData.indices = indices;

    ModelDic["tree"] = modelData;

    //宝箱
    LoadOBJ("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Obj\\chest.obj", verticesStruct, indices);

    modelData.verticesStruct = verticesStruct;
    modelData.indices = indices;

    ModelDic["chest"] = modelData;


    //石头怪
    LoadOBJ("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Obj\\stoneMonster.obj", verticesStruct, indices);

    modelData.verticesStruct = verticesStruct;
    modelData.indices = indices;

    ModelDic["stoneMonster"] = modelData;

    //碎石1
    LoadOBJ("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Obj\\stone1.obj", verticesStruct, indices);

    modelData.verticesStruct = verticesStruct;
    modelData.indices = indices;

    ModelDic["stone1"] = modelData;

    //碎石2
    LoadOBJ("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Obj\\stone2.obj", verticesStruct, indices);

    modelData.verticesStruct = verticesStruct;
    modelData.indices = indices;

    ModelDic["stone2"] = modelData;


    ////背包
    //LoadOBJ("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Fbx\\backpack.obj", verticesStruct, indices);

    //modelData.verticesStruct = verticesStruct;
    //modelData.indices = indices;

    //ModelDic["backpack"] = modelData;
    //草
    LoadOBJ("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Obj\\grass.obj", verticesStruct, indices);

    modelData.verticesStruct = verticesStruct;
    modelData.indices = indices;

    ModelDic["grass1"] = modelData;
    //草2
    LoadOBJ("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Obj\\grass2.obj", verticesStruct, indices);

    modelData.verticesStruct = verticesStruct;
    modelData.indices = indices;

    ModelDic["grass2"] = modelData;
    //黑洞
    LoadOBJ("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Obj\\blackHole.obj", verticesStruct, indices);

    modelData.verticesStruct = verticesStruct;
    modelData.indices = indices;

    ModelDic["blackHole"] = modelData;


#pragma endregion





}

void Game::MakeModelFbx()
{
  // LoadFBX("chest", "E:\\C++\\FirstOne\\C++Steam52\\Assets\\Fbx\\Chest.obj");
  //加载宝箱模型
    //LoadFBX("chest", "E:\\C++\\FirstOne\\C++Steam52\\Assets\\Fbx\\Chest.obj");

   // LoadFBX("backpack", "E:\\C++\\FirstOne\\C++Steam52\\Assets\\Fbx\\backpack.obj");
  // 通过名称获取模型
  auto& model = Game::ModelDesignDic["chest"];

  // 打印模型信息
  std::cout << "----- 模型信息 -----" << std::endl;
  std::cout << "网格数量: " << model.meshes.size() << std::endl;
  for (size_t i = 0; i < model.meshes.size(); i++) {
      const auto& mesh = model.meshes[i];
      std::cout << "------------------------------" << std::endl;
      std::cout << "网格 " << i << " 顶点数量: " << mesh.vertices.size() << std::endl;
      std::cout << "网格 " << i << " 索引数量: " << mesh.indices.size() << std::endl;

      // 如果网格中有顶点，打印第一个顶点的详细信息
      if (!mesh.vertices.empty()) {
          const auto& vertex = mesh.vertices[0];
          std::cout << " 示例顶点 0 详细信息:" << std::endl;
          std::cout << "   位置: (" << vertex.Position.x << ", " << vertex.Position.y << ", " << vertex.Position.z << ")" << std::endl;
          std::cout << "   法线: (" << vertex.Normal.x << ", " << vertex.Normal.y << ", " << vertex.Normal.z << ")" << std::endl;
          std::cout << "   纹理坐标: (" << vertex.TexCoords.x << ", " << vertex.TexCoords.y << ")" << std::endl;
          std::cout << "   切线: (" << vertex.Tangent.x << ", " << vertex.Tangent.y << ", " << vertex.Tangent.z << ")" << std::endl;
          std::cout << "   副切线: (" << vertex.Bitangent.x << ", " << vertex.Bitangent.y << ", " << vertex.Bitangent.z << ")" << std::endl;
          std::cout << "   骨骼数据: ";
          for (int j = 0; j < 4; j++) {
              std::cout << "{" << vertex.BoneIDs[j] << ", " << vertex.Weights[j] << "} ";
          }
          std::cout << std::endl;
      }
  }
  std::cout << "------------------------------" << std::endl;
}


void Game::MakeTxture()

{
    //纹理坐标翻转，原生的图像格式纹理坐标与opengl不一致，这里非常重要
    stbi_set_flip_vertically_on_load(true);

    GLuint defaultTexture = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\default.bmp");
    GLuint defaultW = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\defaultW.jpg");
    GLuint picTexture = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\butterfly.png");
    GLuint lightTexture = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\light.bmp");
    GLuint skyboxTexture = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\front.jpg");
    GLuint nosie1Texture = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\noise1.png");
    GLuint nosie2Texture = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\noise3.png");
    //--地形
    GLuint grassTexture = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\grass.jpg");
    GLuint stoneTexture = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\stone.jpg");
    //打石头游戏树
    GLuint treeTexture = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\tree\\outPng\\Pine_tree_texture.png");
    //GLuint treeTexture = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\tree\\outPng\\treePaint.png");
    GLuint treeTextureNromal = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\tree\\outPng\\Pine_tree_normal.png");
    GLuint treeTextureSpecular = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\tree\\outPng\\Pine_tree_specular.png");
    //打石头游戏宝箱
    GLuint chestTexture = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\tree\\outPng\\Chest_texture.png");
    GLuint chestTextureNormal = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\tree\\outPng\\Chest_normal.png");
    GLuint chseTextureSpecular = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\tree\\outPng\\Chest_specular.png");
    //石头怪
    GLuint stoneMonsterTexture = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\stoneMonster.png");
    //碎石纹理
    GLuint stone = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\tree\\outPng\\Rocks_3_4_texture.png");
    GLuint stoneNormal = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\tree\\outPng\\Rocks_3_4_normal.png");
    GLuint stoneSpecular = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\tree\\outPng\\Rocks_3_4_specular.png");
    //高清背包
    GLuint backpage = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\backpack\\diffuse.jpg");
    GLuint backpageNormal = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\backpack\\normal.png");
    GLuint backpageSpecular = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\backpack\\specular.jpg");
    GLuint backpageRoughnees = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\backpack\\roughness.jpg");
    GLuint backpageAO = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\backpack\\ao.jpg");
    //两种草
    GLuint grass1Texture = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\grass1.png");
    GLuint grass2Texture = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\grass2.png");
    //初始化参数
    std::vector<GLuint> picd;
    //加载默认纹理
    picd.push_back(defaultTexture);//默认灰色基础图
    picd.push_back(defaultW);//默认白色法线图
    picd.push_back(defaultW);//默认白色高亮图
    picd.push_back(defaultW);//默认白色环境图
    picd.push_back(defaultW);//默认白色高度图
    picd.push_back(defaultW);//默认白色糙度图
    picd.push_back(defaultW);//默认白色其他图
    picd.push_back(nosie1Texture);//噪声纹理1
    picd.push_back(nosie2Texture);//噪声纹理2
    TextureDic["default"][PictureTpye::BaseP] = picd[0];
    TextureDic["default"][PictureTpye::NormalP] = picd[1];
    TextureDic["default"][PictureTpye::SpecularP] = picd[2];
    TextureDic["default"][PictureTpye::HightP] = picd[3];
    TextureDic["default"][PictureTpye::RoughnessP] = picd[4];
    TextureDic["default"][PictureTpye::AOP]= picd[5];
    TextureDic["default"][PictureTpye::OtherP] = picd[6];
    TextureDic["default"][PictureTpye::Noise1P] = picd[7];
    TextureDic["default"][PictureTpye::Noise2P] = picd[8];
    //初始化参数       
    std::vector<GLuint> pic;
    //加载蝴蝶纹理
    pic.clear();
    pic.push_back(picTexture);
    TextureDic["butterfly"][PictureTpye::BaseP] = pic[0];
    TextureDic["butterfly"][PictureTpye::NormalP] = picd[1];
    TextureDic["butterfly"][PictureTpye::SpecularP] = picd[2];
    TextureDic["butterfly"][PictureTpye::HightP] = picd[3];
    TextureDic["butterfly"][PictureTpye::RoughnessP] = picd[4];
    TextureDic["butterfly"][PictureTpye::AOP] = picd[5];
    TextureDic["butterfly"][PictureTpye::OtherP] = picd[6];
    //加载光源点渲染纹理
    pic.clear();
    pic.push_back(lightTexture);
    TextureDic["light"][PictureTpye::BaseP] = pic[0];
    //加载天空盒
    pic.clear();
    pic.push_back(skyboxTexture);
    TextureDic["skybox"][PictureTpye::BaseP] = pic[0];
    //加载草地
    pic.clear();
    pic.push_back(grassTexture);
    TextureDic["grass"][PictureTpye::BaseP] = pic[0];
    TextureDic["grass"][PictureTpye::NormalP] = picd[1];
    TextureDic["grass"][PictureTpye::SpecularP] = picd[2];
    TextureDic["grass"][PictureTpye::HightP] = picd[3];
    TextureDic["grass"][PictureTpye::RoughnessP] = picd[4];
    TextureDic["grass"][PictureTpye::AOP] = picd[5];
    TextureDic["grass"][PictureTpye::OtherP] = picd[6];
    //加载石头
    pic.clear();
    pic.push_back(stoneTexture);
    TextureDic["stone"][PictureTpye::BaseP] = pic[0];
    TextureDic["stone"][PictureTpye::NormalP] = picd[1];
    TextureDic["stone"][PictureTpye::SpecularP] = picd[2];
    TextureDic["stone"][PictureTpye::HightP] = picd[3];
    TextureDic["stone"][PictureTpye::RoughnessP] = picd[4];
    TextureDic["stone"][PictureTpye::AOP] = picd[5];
    TextureDic["stone"][PictureTpye::OtherP] = picd[6];

#pragma region 打石头游戏区域
    //树
    pic.clear();
    pic.push_back(treeTexture);
    pic.push_back(treeTextureNromal);
    pic.push_back(treeTextureSpecular);
    TextureDic["tree"][PictureTpye::BaseP] = pic[0];
    TextureDic["tree"][PictureTpye::NormalP] = pic[1];
    TextureDic["tree"][PictureTpye::SpecularP] = pic[2];
    TextureDic["tree"][PictureTpye::HightP] = picd[3];
    TextureDic["tree"][PictureTpye::RoughnessP] = picd[4];
    TextureDic["tree"][PictureTpye::AOP] = picd[5];
    TextureDic["tree"][PictureTpye::OtherP] = picd[6];

    //宝箱
    pic.clear();
    pic.push_back(chestTexture);
    pic.push_back(chestTextureNormal);
    pic.push_back(chseTextureSpecular);// 
    TextureDic["chest"][PictureTpye::BaseP] = pic[0];
    TextureDic["chest"][PictureTpye::HightP] = pic[1];
    TextureDic["chest"][PictureTpye::SpecularP] = pic[2];
    TextureDic["chest"][PictureTpye::HightP] = picd[3];
    TextureDic["chest"][PictureTpye::RoughnessP] = picd[4];
    TextureDic["chest"][PictureTpye::AOP] = picd[5];
    TextureDic["chest"][PictureTpye::OtherP] = picd[6];

    //石头怪
    pic.clear();
    pic.push_back(stoneMonsterTexture);
    TextureDic["stoneMonster"][PictureTpye::BaseP] = pic[0];
    TextureDic["stoneMonster"][PictureTpye::NormalP] = picd[1];
    TextureDic["stoneMonster"][PictureTpye::SpecularP] = picd[2];
    TextureDic["stoneMonster"][PictureTpye::HightP] = picd[3];
    TextureDic["stoneMonster"][PictureTpye::RoughnessP] = picd[4];
    TextureDic["stoneMonster"][PictureTpye::AOP] = picd[5];
    TextureDic["stoneMonster"][PictureTpye::OtherP] = picd[6];

    //碎石实例化
    pic.clear();
    pic.push_back(stone);
    pic.push_back(stoneNormal);
    pic.push_back(stoneSpecular);// 
    TextureDic["stoneInstance"][PictureTpye::BaseP] = pic[0];
    TextureDic["stoneInstance"][PictureTpye::HightP] = pic[1];
    TextureDic["stoneInstance"][PictureTpye::SpecularP] = pic[2];
    TextureDic["stoneInstance"][PictureTpye::HightP] = picd[3];
    TextureDic["stoneInstance"][PictureTpye::RoughnessP] = picd[4];
    TextureDic["stoneInstance"][PictureTpye::AOP] = picd[5];
    TextureDic["stoneInstance"][PictureTpye::OtherP] = picd[6];
    //高清背包
    pic.clear();
    pic.push_back(backpage);
    pic.push_back(backpageNormal);
    pic.push_back(backpageSpecular);
    pic.push_back(backpageRoughnees);
    pic.push_back(backpageAO);
    TextureDic["backpack"][PictureTpye::BaseP] = pic[0];
    TextureDic["backpack"][PictureTpye::NormalP] = pic[1];
    TextureDic["backpack"][PictureTpye::SpecularP] = pic[2];
    TextureDic["backpack"][PictureTpye::HightP] = picd[3];
    TextureDic["backpack"][PictureTpye::RoughnessP] = pic[3];
    TextureDic["backpack"][PictureTpye::AOP] = pic[4];
    TextureDic["backpack"][PictureTpye::OtherP] = picd[6];
    //黑洞，暂时用其他图像生成
    TextureDic["blackHole"][PictureTpye::BaseP] = picd[7];//黑噪图1
    TextureDic["blackHole"][PictureTpye::NormalP] = pic[1];
    TextureDic["blackHole"][PictureTpye::SpecularP] = pic[2];
    TextureDic["blackHole"][PictureTpye::HightP] = picd[3];
    TextureDic["blackHole"][PictureTpye::RoughnessP] = pic[3];
    TextureDic["blackHole"][PictureTpye::AOP] = pic[4];
    TextureDic["blackHole"][PictureTpye::OtherP] = picd[6];
    //实例化草1
    pic.clear();
    pic.push_back(grass1Texture);
    TextureDic["grass1"][PictureTpye::BaseP] = pic[0];
    //实例化草2
    pic.clear();
    pic.push_back(grass2Texture);
    TextureDic["grass2"][PictureTpye::BaseP] = pic[0];
#pragma endregion



}

void Game::MakeFronts()
{
    TextRender* textRender = TextRender::GetInstance();
    textRender->MakeFronts();
    
}
// 辅助函数：为单个顶点添加骨骼数据（最多 4 个影响）
void AddBoneData(MeshVertex& vertex, int boneID, float weight) {
    for (int i = 0; i < 4; i++) {
        if (vertex.Weights[i] == 0.0f) {
            vertex.BoneIDs[i] = boneID;
            vertex.Weights[i] = weight;
            break;
        }
    }
}

// 处理单个网格，提取顶点、索引、切线、Bitangent 以及骨骼数据
Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene) {
    Mesh result;
    // 预先分配顶点空间
    result.vertices.resize(mesh->mNumVertices);

    // 处理顶点数据
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        MeshVertex vertex;
        // 顶点位置
        vertex.Position = glm::vec3(
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z
        );

        // 法线
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

        // 纹理坐标（只读取第一组纹理坐标）
        if (mesh->mTextureCoords[0]) {
            vertex.TexCoords = glm::vec2(
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y
            );
        }
        else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        // 切线与副切线
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

    // 处理索引（假设每个面都是三角形）
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            result.indices.push_back(face.mIndices[j]);
        }
    }

    // 处理骨骼数据（如果存在）
    if (mesh->HasBones()) {
        for (unsigned int i = 0; i < mesh->mNumBones; i++) {
            aiBone* bone = mesh->mBones[i];
            int boneID = i; // 简单示例：直接使用当前骨骼在 mBones 数组中的索引作为骨骼 ID
            for (unsigned int j = 0; j < bone->mNumWeights; j++) {
                unsigned int vertexID = bone->mWeights[j].mVertexId;
                float weight = bone->mWeights[j].mWeight;
                AddBoneData(result.vertices[vertexID], boneID, weight);
            }
        }
    }

    return result;
}

// 递归处理节点，遍历场景图并提取所有网格
void ProcessNode(aiNode* node, const aiScene* scene, Model& model) {
    // 处理当前节点中的所有网格
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        model.meshes.push_back(ProcessMesh(mesh, scene));
    }

    // 递归处理所有子节点
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene, model);
    }
}

// 加载 FBX 文件并存储到全局模型字典 ModelDesignDic 中
bool Game:: LoadFBX(const std::string& name, const std::string& path) {
    // 如果模型已加载，则跳过重复加载
    if (ModelDesignDic.find(name) != ModelDesignDic.end()) {
        std::cout << "模型 " << name << " 已经加载过，跳过重复加载。" << std::endl;
        return true;
    }
    // 创建 Assimp 导入器并加载文件
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return false;
    }

    // 处理场景，从根节点开始递归提取网格数据
    Model model;
    ProcessNode(scene->mRootNode, scene, model);

    // 将加载的模型保存到全局字典中
    ModelDesignDic[name] = model;

    std::cout << "模型 " << name << " 加载成功！" << std::endl;
    return true;
}




