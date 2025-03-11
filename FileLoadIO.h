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

#pragma region �ṹ������
    struct Vertex {
        glm::vec3 Position;   // ����λ��
        glm::vec2 TexCoords;  // ��������
        glm::vec3 Normal;     // ����

        // ���� unordered_map ���أ���Ҫ == �����
        bool operator==(const Vertex& other) const {
            return Position == other.Position
                && TexCoords == other.TexCoords
                && Normal == other.Normal;
        }//��������أ���������Զ��ȱȽϣ������������������
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

    // FBX�ṹ��
    struct MeshVertex {
        glm::vec3 Position;    // ����λ��
        glm::vec3 Normal;      // ����
        glm::vec2 TexCoords;   // ��������
        glm::vec3 Tangent;     // ����
        glm::vec3 Bitangent;   // ������
        int BoneIDs[4];        // �������������֧�� 4 ������Ӱ�죩
        float Weights[4];      // ÿ��������Ӧ��Ȩ��

        MeshVertex() {
            // ��ʼ����������
            for (int i = 0; i < 4; i++) {
                BoneIDs[i] = 0;
                Weights[i] = 0.0f;
            }
        }
    };
    // ����ṹ��--���ڼ���FBX�ļ�
    struct Mesh {
        std::vector<MeshVertex> vertices;       // ��������
        std::vector<unsigned int> indices;  // ��������
    };

    // ģ�ͽṹ��--���ڼ���FBX�ļ�
    struct Model {
        std::vector<Mesh> meshes;           // ģ�͵���������
    };

    struct VertexHash {
        size_t operator()(const Vertex& vertex) const {
            // �򵥹�ϣ���ʾ����Xors��shifts�ȣ�
            // Ҳ���Բ��ø�����Ĺ�ϣ��Ϻ���
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

#pragma region �ļ���������

    //���������б�,�ڵ�����Դ�ļ��н��ж��壬������Դ�ļ�����ͷ�ļ�������ظ��������⣬��������Ͳ����ڣ���������֮����Ҫע���������
    extern std::unordered_map<std::string, std::unordered_map<std::string, AnimationData>> AnimationDic;
    //��̬������Ƥģ���б�ʹ�ò�ͬ�Ĺ��캯�����첻ͬ���Ե�ģ��
    extern std::unordered_map<std::string, ModelData> ModelDic;
    //���������ͼ��1���� 2���� 3 �߶� 4 ���� 5 ���� 6.��
    extern std::unordered_map<std::string, std::unordered_map<PictureTpye,GLuint>> TextureDic;
    //���ģ�ͼ��أ����ڳ�������ṹ
    extern std::unordered_map<std::string, Model> ModelDesignDic;

#pragma endregion

#pragma region ��ͳ���ط���



    bool LoadOBJ(
        const char* path,
        std::vector<glm::vec3>& out_vertices,
        std::vector<glm::vec2>& out_uvs,
        std::vector<glm::vec3>& out_normals
    );//�����������߼�
    bool LoadOBJ(const char* path,
        std::vector<Vertex>& out_vertices,
        std::vector<unsigned int>& out_indices, bool removeDuplicates);// ��̬����ȥ���߼�


    bool LoadOBJ(const char* path,
        std::vector<Vertex>& out_vertices,      //��̬���飬����չ��С������
        std::vector<unsigned int>& out_indices);// meshSkinned ��̬���� ������ȥ���߼�
    GLuint LoadPicTexture(const char* picImagePath);

    std::vector<Vertex> LoadVerticesFromFile(const std::string& name);//����һ����ݻ�ȡ�ؼ�֡����취

    //# һ������������
    void MakeAnimation();

    void MakeModel();
    //��ģ�͵ļ��ط���
    void MakeModelFbx();

    void MakeTxture();

    void MakeFronts();




#pragma endregion


#pragma region ����Assimp��ļ��ط���

    // ���� FBX �ļ����洢�� ModelDesignDic ��
    bool LoadFBX(const std::string& name, const std::string& path);




#pragma endregion



}
#endif
