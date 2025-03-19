#ifndef MESH_DATA_MANAGER_H
#define MESH_DATA_MANAGER_H
#include <vector>
#include <GL/glew.h>  
#include <glm/glm.hpp> /
 namespace Game {
    class MeshDataManager {
    public:
        // ��ȡ����ʵ��
        static MeshDataManager* GetInstance();

#pragma region ģ������  
        //������
        GLfloat cubeVertices[24] = {
            // λ��
            -0.5f, -0.5f, -0.5f,  // ǰ��
             0.5f, -0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,

            -0.5f, -0.5f,  0.5f,  // ����
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f
        };

        GLuint cubeIndices[36] = {
            0, 1, 2, 2, 3, 0,  // ǰ��
            4, 5, 6, 6, 7, 4,  // ����
            0, 1, 5, 5, 4, 0,  // �ײ�
            2, 3, 7, 7, 6, 2,  // ����
            0, 3, 7, 7, 4, 0,  // ���
            1, 2, 6, 6, 5, 1   // �Ҳ�
        };

        size_t cubeVertexCount = sizeof(cubeVertices) / sizeof(cubeVertices[0]);
        size_t cubeIndexCount = sizeof(cubeIndices) / sizeof(cubeIndices[0]);

        //��ɫ�����壬��������Ⱦ����
        GLfloat colorCubeVertices[144] = {
            // ǰ��(+Z) 4������
            //   λ��               ����
            -1.0f, -1.0f, +1.0f,   0.0f, 0.0f, +1.0f,
            +1.0f, -1.0f, +1.0f,   0.0f, 0.0f, +1.0f,
            +1.0f, +1.0f, +1.0f,   0.0f, 0.0f, +1.0f,
            -1.0f, +1.0f, +1.0f,   0.0f, 0.0f, +1.0f,

            // ����(-Z)
            +1.0f, -1.0f, -1.0f,   0.0f, 0.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,   0.0f, 0.0f, -1.0f,
            -1.0f, +1.0f, -1.0f,   0.0f, 0.0f, -1.0f,
            +1.0f, +1.0f, -1.0f,   0.0f, 0.0f, -1.0f,

            // ����(+X)
            +1.0f, -1.0f, +1.0f,   +1.0f, 0.0f, 0.0f,
            +1.0f, -1.0f, -1.0f,   +1.0f, 0.0f, 0.0f,
            +1.0f, +1.0f, -1.0f,   +1.0f, 0.0f, 0.0f,
            +1.0f, +1.0f, +1.0f,   +1.0f, 0.0f, 0.0f,

            // ����(-X)
            -1.0f, -1.0f, -1.0f,   -1.0f, 0.0f, 0.0f,
            -1.0f, -1.0f, +1.0f,   -1.0f, 0.0f, 0.0f,
            -1.0f, +1.0f, +1.0f,   -1.0f, 0.0f, 0.0f,
            -1.0f, +1.0f, -1.0f,   -1.0f, 0.0f, 0.0f,

            // ����(+Y)
            -1.0f, +1.0f, +1.0f,   0.0f, +1.0f, 0.0f,
            +1.0f, +1.0f, +1.0f,   0.0f, +1.0f, 0.0f,
            +1.0f, +1.0f, -1.0f,   0.0f, +1.0f, 0.0f,
            -1.0f, +1.0f, -1.0f,   0.0f, +1.0f, 0.0f,

            // ����(-Y)
            -1.0f, -1.0f, -1.0f,   0.0f, -1.0f, 0.0f,
            +1.0f, -1.0f, -1.0f,   0.0f, -1.0f, 0.0f,
            +1.0f, -1.0f, +1.0f,   0.0f, -1.0f, 0.0f,
            -1.0f, -1.0f, +1.0f,   0.0f, -1.0f, 0.0f
        };


        GLuint colorCubeIndices[36] = {
            // ǰ�棨+Z �棩
         0, 1, 2,
         2, 3, 0,

         // ���棨-Z �棩
         4, 5, 6,
         6, 7, 4,

         // ���棨+X �棩
         8, 9, 10,
         10, 11, 8,

         // ���棨-X �棩
         12, 13, 14,
         14, 15, 12,

         // ���棨+Y �棩
         16, 17, 18,
         18, 19, 16,

         // ���棨-Y �棩
         20, 21, 22,
         22, 23, 20
        };


        size_t colorCubeVertexCount = sizeof(colorCubeVertices) / sizeof(colorCubeVertices[0]) / 8;  // ÿ���������8��ֵ��λ��+��������+���ߣ�
        size_t colorCubeIndexCount = sizeof(colorCubeIndices) / sizeof(colorCubeIndices[0]);  // ÿ��������һ��GLuint
        //Բ����
         // Բ���嶥������
        GLfloat cylinderVertices[60] = {
             0.0f, 0.0f, 0.0f, // Բ��
             // �ײ�Բ�����ɸ���
             0.5f, 0.0f, 0.0f,
             0.353f, 0.0f, 0.353f,
             0.0f, 0.0f, 0.5f,
             -0.353f, 0.0f, 0.353f,
             -0.5f, 0.0f, 0.0f,
             -0.353f, 0.0f, -0.353f,
             0.0f, 0.0f, -0.5f,
             0.353f, 0.0f, -0.353f,
             0.5f, 0.0f, 0.0f, // �ظ���һ���㣬ȷ��Բ���պ�

             // ����Բ��Բ��
             0.0f, 1.0f, 0.0f, // Բ��
             // ����Բ�����ɸ��㣨ͬ�ײ�Բ��Ӧ��
             0.5f, 1.0f, 0.0f,
             0.353f, 1.0f, 0.353f,
             0.0f, 1.0f, 0.5f,
             -0.353f, 1.0f, 0.353f,
             -0.5f, 1.0f, 0.0f,
             -0.353f, 1.0f, -0.353f,
             0.0f, 1.0f, -0.5f,
             0.353f, 1.0f, -0.353f,
             0.5f, 1.0f, 0.0f  // �ظ���һ���㣬ȷ��Բ���պ�
        };

        // Բ�������������
        GLuint cylinderIndices[108] = {
        0, 1, 2,
        2, 3, 0,
        3, 4, 0,
        4, 5, 0,
        5, 6, 0,
        6, 7, 0,
        7, 8, 0,
        8, 9, 0,
        9, 1, 0,  // �������һ����͵�һ����

        // ���������Σ�����Բ�ĺͶ���Բ�ı�
        10, 11, 12,
        12, 13, 10,
        13, 14, 10,
        14, 15, 10,
        15, 16, 10,
        16, 17, 10,
        17, 18, 10,
        18, 19, 10,
        19, 11, 10,  // �������һ����͵�һ����

        // ���������Σ����ӵײ��Ͷ�������Ӧ�㣬�γɾ���
        1, 2, 11, 11, 2, 12,
        2, 3, 12, 12, 3, 13,
        3, 4, 13, 13, 4, 14,
        4, 5, 14, 14, 5, 15,
        5, 6, 15, 15, 6, 16,
        6, 7, 16, 16, 7, 17,
        7, 8, 17, 17, 8, 18,
        8, 9, 18, 18, 9, 19,
        9, 1, 19, 19, 1, 11  // �������һ�β���
        };


        size_t cylinderVertexCount = sizeof(cylinderVertices) / sizeof(cylinderVertices[0]);
        size_t cylinderIndexCount = sizeof(cylinderIndices) / sizeof(cylinderIndices[0]);
        //Բ׶��
        GLfloat coneVertices[18] = {
            // Բ����������ԭ�㣬��������
            0.0f, 0.0f, 0.5f,  // ����
            0.5f, 0.0f, -0.5f,  // �ײ�Բ��һ����
            -0.5f, 0.0f, -0.5f, // �ײ�Բ����һ����

            0.0f, 0.0f, 0.5f,  // ����
            0.5f, 0.0f, -0.5f, // �ײ�Բ����һ����
            0.5f, 0.0f, 0.5f,  // �ײ�Բ��һ����
        };

        GLuint coneIndices[18] = {
          0, 1, 2,  // ����������
          0, 2, 3,  // ����������
          0, 3, 4,  // ����������
          0, 4, 1,  // ����������

          // ������Ա����Ϊһ���ı��Σ���������������
          1, 2, 3,  // ����������1
          1, 3, 4   // ����������2
        };


        size_t coneVertexCount = sizeof(coneVertices) / sizeof(coneVertices[0]);
        size_t coneIndexCount = sizeof(coneIndices) / sizeof(coneIndices[0]);
        //����
            // ��������嶥�������ʾ��Ϊ�򻯰棬ʵ��������Ҫ���㷨���ɡ�
        GLfloat sphereVertices[27] = {
            // ����
           0.0f,  1.0f,  0.0f,   // ��������
           0.5f,  0.866f, 0.0f,   // ��һ�������ϵĵ�
           0.0f,  0.866f, 0.5f,   // �ڶ��������ϵĵ�
           -0.5f, 0.866f, 0.0f,   // �����������ϵĵ�
           -0.5f, 0.866f, -0.5f,  // �����������ϵĵ�
           0.0f, 0.866f, -0.5f,   // �����������ϵĵ�
           0.5f,  0.0f,  0.5f,   // �����������ϵĵ�
           0.5f,  0.0f, -0.5f,   // �����������ϵĵ�
           0.0f,  -1.0f, 0.0f,   // �ײ�����
        };

        GLuint sphereIndices[39] = {
            0, 1, 2,
            0, 2, 3,
            0, 3, 4,
            0, 4, 5,
            0, 5, 6,
            0, 6, 7,
            0, 7, 8,
            // ����
            8, 7, 6,
            8, 6, 5,
            8, 5, 4,
            8, 4, 3,
            8, 3, 2,
            8, 2, 1
        };

        // ��������Ķ�������������
        size_t sphereVertexCount = sizeof(sphereVertices) / sizeof(sphereVertices[0]);
        size_t sphereIndexCount = sizeof(sphereIndices) / sizeof(sphereIndices[0]);

        //������
        GLfloat capsuleVertices[72] = {
            // ���Ҷ������ݣ��򻯰棨��Ҫ�����������ݣ�
            0.0f, 1.0f, 0.0f,  // ����
            0.5f, 0.0f, 0.0f,
            // ������...
        };

        GLuint capsuleIndices[72] = {
            // ���ҵ���������
            0, 1, 2,  // ʾ������
        };


        // ����ϵ��ʶģ�ͣ�
// ÿ�������������㹹�ɣ�ÿ��������� 3 ��λ�÷����� 3 ����ɫ����
// ���� 3 * 2 = 6 �����㣬ÿ������ 6 �����������ܹ� 6 * 6 = 36 ������
        GLfloat axisWidgetVertices[36] = {
            // X�� (��ɫ)����ԭ�㵽 (1,0,0)
            0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,  // ���� 0
            10000.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,  // ���� 1

            // Y�� (��ɫ)����ԭ�㵽 (0,1,0)
            0.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,  // ���� 2
            0.0f, 10000.0f, 0.0f,   0.0f, 1.0f, 0.0f,  // ���� 3

            // Z�� (��ɫ)����ԭ�㵽 (0,0,1)   
            0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,  // ���� 4
            0.0f, 0.0f, -10000.0f,   0.0f, 0.0f, 1.0f   // ���� 5
        };
        GLuint axisWidgetIndices[6] = {
            0, 1,  // X��
            2, 3,  // Y��
            4, 5   // Z��
        };
        size_t axisWidgetVertexCount = sizeof(axisWidgetVertices) / sizeof(axisWidgetVertices[0]);  // Ӧ��Ϊ 36
        size_t axisWidgetIndexCount = sizeof(axisWidgetIndices) / sizeof(axisWidgetIndices[0]);     // Ӧ��Ϊ 6

       //������ģ��
        GLfloat customizeRay[6] = { 0,0,0,5,0,0 };
        GLuint customizeRayIncices[2] = { 0,1 };
        size_t customizeRayCount = 6;
        size_t customizeRayIndexCount = 2;
      //--  

#pragma endregion


    private:
        // ˽�й��캯������ֹ�ⲿ����ʵ��
        MeshDataManager() {};

        static MeshDataManager* instance;

    };
}
#endif // MESH_DATA_MANAGER_H
