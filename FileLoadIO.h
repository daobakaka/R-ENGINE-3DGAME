#ifndef FILE_LOAD_IO_H
#define FILE_LOAD_IO_H
#include"GameObject.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "stb_image.h"
#include <unordered_map>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace Game {

#pragma region 结构体区域
    struct Vertex {
        glm::vec3 Position;   // 顶点位置
        glm::vec2 TexCoords;  // 纹理坐标
        glm::vec3 Normal;     // 法线

        // 用于 unordered_map 查重，需要 == 运算符
        bool operator==(const Vertex& other) const {
            return Position == other.Position
                && TexCoords == other.TexCoords
                && Normal == other.Normal;
        }//运算符重载，定义类的自定比比较，功能类似于运算符宏
    };

    struct AnimationData {
        std::vector<float> keyFrameTimes;
        std::vector<std::vector<Vertex>> keyFrames;
        bool loop;
        float speed;
    };

    struct ModelData
    {

        std::vector<Vertex> verticesStruct;
        std::vector<unsigned int> indices;

    };

    // FBX结构体
    struct MeshVertex {
        glm::vec3 Position;    // 顶点位置
        glm::vec3 Normal;      // 法线
        glm::vec2 TexCoords;   // 纹理坐标
        glm::vec3 Tangent;     // 切线
        glm::vec3 Bitangent;   // 副切线
        int BoneIDs[4];        // 骨骼索引（最多支持 4 个骨骼影响）
        float Weights[4];      // 每个骨骼对应的权重

        MeshVertex() {
            // 初始化骨骼数据
            for (int i = 0; i < 4; i++) {
                BoneIDs[i] = 0;
                Weights[i] = 0.0f;
            }
        }
    };
    // 网格结构体--用于加载FBX文件
    struct Mesh {
        std::vector<MeshVertex> vertices;       // 顶点数据
        std::vector<unsigned int> indices;  // 索引数据
    };

    // 模型结构体--用于加载FBX文件
    struct Model {
        std::vector<Mesh> meshes;           // 模型的所有网格
    };

    struct VertexHash {
        size_t operator()(const Vertex& vertex) const {
            // 简单哈希组合示例（Xors、shifts等）
            // 也可以采用更成熟的哈希组合函数
            auto h1 = std::hash<float>()(vertex.Position.x)
                ^ (std::hash<float>()(vertex.Position.y) << 1)
                ^ (std::hash<float>()(vertex.Position.z) << 2);
            auto h2 = std::hash<float>()(vertex.TexCoords.x)
                ^ (std::hash<float>()(vertex.TexCoords.y) << 1);
            auto h3 = std::hash<float>()(vertex.Normal.x)
                ^ (std::hash<float>()(vertex.Normal.y) << 1)
                ^ (std::hash<float>()(vertex.Normal.z) << 2);
            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };




#pragma endregion

#pragma region 文件储存区域

    //动画索引列表,在单独的源文件中进行定义，避免多个源文件引用头文件引起的重复引用问题，在类里面就不存在，独立于类之外需要注意这个问题
    extern std::unordered_map<std::string, std::unordered_map<std::string, AnimationData>> AnimationDic;
    //静态或者蒙皮模型列表，使用不同的构造函数构造不同属性的模型
    extern std::unordered_map<std::string, ModelData> ModelDic;
    //物体表面贴图，1基础 2法线 3 高度 4 光照 5 其他 6.。
    extern std::unordered_map<std::string, std::unordered_map<PictureTpye,GLuint>> TextureDic;
    //设计模型加载，后期常用这个结构
    extern std::unordered_map<std::string, Model> ModelDesignDic;

#pragma endregion

#pragma region 传统加载方法



    bool LoadOBJ(
        const char* path,
        std::vector<glm::vec3>& out_vertices,
        std::vector<glm::vec2>& out_uvs,
        std::vector<glm::vec3>& out_normals
    );//非索引顶点逻辑
    bool LoadOBJ(const char* path,
        std::vector<Vertex>& out_vertices,
        std::vector<unsigned int>& out_indices, bool removeDuplicates);// 静态物体去重逻辑


    bool LoadOBJ(const char* path,
        std::vector<Vertex>& out_vertices,      //动态数组，可扩展大小的数组
        std::vector<unsigned int>& out_indices);// meshSkinned 动态顶点 索引非去重逻辑
    GLuint LoadPicTexture(const char* picImagePath);

    std::vector<Vertex> LoadVerticesFromFile(const std::string& name);//定义一个快捷获取关键帧数组办法

    //# 一键化方法区域
    void MakeAnimation();

    void MakeModel();
    //多模型的加载方法
    void MakeModelFbx();

    void MakeTxture();

    void MakeFronts();




#pragma endregion


#pragma region 基于Assimp库的加载方法

    // 加载 FBX 文件并存储到 ModelDesignDic 中
    bool LoadFBX(const std::string& name, const std::string& path);




#pragma endregion



}
#endif
