#include "FileLoadIO.h"
#include <cstdio>
#include <vector>
#include <glm/glm.hpp>
#include <cstring>
#include "TextRender.h"
using namespace Game;
/// <summary>
/// 资源缓存区域，小型场景可以完全加载缓存资源
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

bool LoadOBJ(const char* path, std::vector<Vertex>& out_vertices, std::vector<unsigned int>& out_indices)
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


GLuint LoadPicTexture(const char* picImagePath)
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

std::vector<Vertex> LoadVerticesFromFile(const std::string& name)
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

void MakeAnimation()
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
#pragma endregion





}

void MakeTxture()
{
    GLuint defaultTexture = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\default.bmp");
    GLuint picTexture = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\1.png");
    GLuint lightTexture = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\light.bmp");
    GLuint skyboxTexture = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\front.jpg");
    //--地形
    GLuint grassTexture = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\grass.jpg");
    GLuint stoneTexture = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\stone.jpg");
    //打石头游戏树
    GLuint treeTexture = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\tree\\outPng\\Pine_tree_texture.png");
    GLuint treeTextureSpecular = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\tree\\outPng\\Pine_tree_specular.png");
    //打石头游戏宝箱
    GLuint chestTexture = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\tree\\outPng\\Chest_texture.png");
    GLuint chsetextureSpecular = LoadPicTexture("E:\\C++\\FirstOne\\C++Steam52\\Assets\\Texture\\tree\\outPng\\Chest_specular.png");

    //初始化参数
    std::vector<GLuint> pic;
    //加载默认纹理
    pic.push_back(defaultTexture);
    TextureDic["default"] = pic;
    //加载蝴蝶纹理
    pic.clear();
    pic.push_back(picTexture);
    TextureDic["butterfly"] = pic;
    //加载光源点渲染纹理
    pic.clear();
    pic.push_back(lightTexture);
    TextureDic["light"] = pic;
    //加载天空盒
    pic.clear();
    pic.push_back(skyboxTexture);
    TextureDic["skybox"] = pic;
    //加载草地
    pic.clear();
    pic.push_back(grassTexture);
    TextureDic["grass"] = pic;
    //加载石头地
    pic.clear();
    pic.push_back(stoneTexture);
    TextureDic["stone"] = pic;
#pragma region 打石头游戏区域
    //树
    pic.clear();
    pic.push_back(treeTexture);
    pic.push_back(treeTextureSpecular);// 这里加载了两张图片
    TextureDic["tree"] = pic;

    //宝箱
    pic.clear();
    pic.push_back(chestTexture);
    pic.push_back(chsetextureSpecular);// 这里加载了两张图片
    TextureDic["chest"] = pic;
#pragma endregion



}

void MakeFronts()
{
    TextRender* textRender = TextRender::GetInstance();
    textRender->MakeFronts();
    
}



