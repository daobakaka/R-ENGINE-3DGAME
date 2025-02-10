#ifndef MESH_DATA_MANAGER_H
#define MESH_DATA_MANAGER_H
#include <vector>
#include <GL/glew.h>  // OpenGL 库
#include <glm/glm.hpp> // GLM 数学库
 namespace Game {
    class MeshDataManager {
    public:
        // 获取单例实例
        static MeshDataManager* GetInstance();

#pragma region 模型区域  
        //立方体
        GLfloat cubeVertices[24] = {
            // 位置
            -0.5f, -0.5f, -0.5f,  // 前面
             0.5f, -0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,

            -0.5f, -0.5f,  0.5f,  // 后面
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f
        };

        GLuint cubeIndices[36] = {
            0, 1, 2, 2, 3, 0,  // 前面
            4, 5, 6, 6, 7, 4,  // 后面
            0, 1, 5, 5, 4, 0,  // 底部
            2, 3, 7, 7, 6, 2,  // 顶部
            0, 3, 7, 7, 4, 0,  // 左侧
            1, 2, 6, 6, 5, 1   // 右侧
        };

        size_t cubeVertexCount = sizeof(cubeVertices) / sizeof(cubeVertices[0]);
        size_t cubeIndexCount = sizeof(cubeIndices) / sizeof(cubeIndices[0]);

        //彩色立方体，用来做渲染测试
        GLfloat colorCubeVertices[144] = {
            // 前面(+Z) 4个顶点
            //   位置               法线
            -1.0f, -1.0f, +1.0f,   0.0f, 0.0f, +1.0f,
            +1.0f, -1.0f, +1.0f,   0.0f, 0.0f, +1.0f,
            +1.0f, +1.0f, +1.0f,   0.0f, 0.0f, +1.0f,
            -1.0f, +1.0f, +1.0f,   0.0f, 0.0f, +1.0f,

            // 后面(-Z)
            +1.0f, -1.0f, -1.0f,   0.0f, 0.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,   0.0f, 0.0f, -1.0f,
            -1.0f, +1.0f, -1.0f,   0.0f, 0.0f, -1.0f,
            +1.0f, +1.0f, -1.0f,   0.0f, 0.0f, -1.0f,

            // 右面(+X)
            +1.0f, -1.0f, +1.0f,   +1.0f, 0.0f, 0.0f,
            +1.0f, -1.0f, -1.0f,   +1.0f, 0.0f, 0.0f,
            +1.0f, +1.0f, -1.0f,   +1.0f, 0.0f, 0.0f,
            +1.0f, +1.0f, +1.0f,   +1.0f, 0.0f, 0.0f,

            // 左面(-X)
            -1.0f, -1.0f, -1.0f,   -1.0f, 0.0f, 0.0f,
            -1.0f, -1.0f, +1.0f,   -1.0f, 0.0f, 0.0f,
            -1.0f, +1.0f, +1.0f,   -1.0f, 0.0f, 0.0f,
            -1.0f, +1.0f, -1.0f,   -1.0f, 0.0f, 0.0f,

            // 上面(+Y)
            -1.0f, +1.0f, +1.0f,   0.0f, +1.0f, 0.0f,
            +1.0f, +1.0f, +1.0f,   0.0f, +1.0f, 0.0f,
            +1.0f, +1.0f, -1.0f,   0.0f, +1.0f, 0.0f,
            -1.0f, +1.0f, -1.0f,   0.0f, +1.0f, 0.0f,

            // 下面(-Y)
            -1.0f, -1.0f, -1.0f,   0.0f, -1.0f, 0.0f,
            +1.0f, -1.0f, -1.0f,   0.0f, -1.0f, 0.0f,
            +1.0f, -1.0f, +1.0f,   0.0f, -1.0f, 0.0f,
            -1.0f, -1.0f, +1.0f,   0.0f, -1.0f, 0.0f
        };


        GLuint colorCubeIndices[36] = {
            // 前面（+Z 面）
         0, 1, 2,
         2, 3, 0,

         // 后面（-Z 面）
         4, 5, 6,
         6, 7, 4,

         // 右面（+X 面）
         8, 9, 10,
         10, 11, 8,

         // 左面（-X 面）
         12, 13, 14,
         14, 15, 12,

         // 上面（+Y 面）
         16, 17, 18,
         18, 19, 16,

         // 下面（-Y 面）
         20, 21, 22,
         22, 23, 20
        };


        size_t colorCubeVertexCount = sizeof(colorCubeVertices) / sizeof(colorCubeVertices[0]) / 8;  // 每个顶点包含8个值（位置+纹理坐标+法线）
        size_t colorCubeIndexCount = sizeof(colorCubeIndices) / sizeof(colorCubeIndices[0]);  // 每个索引是一个GLuint
        //圆柱体
         // 圆柱体顶点数据
        GLfloat cylinderVertices[60] = {
             0.0f, 0.0f, 0.0f, // 圆心
             // 底部圆的若干个点
             0.5f, 0.0f, 0.0f,
             0.353f, 0.0f, 0.353f,
             0.0f, 0.0f, 0.5f,
             -0.353f, 0.0f, 0.353f,
             -0.5f, 0.0f, 0.0f,
             -0.353f, 0.0f, -0.353f,
             0.0f, 0.0f, -0.5f,
             0.353f, 0.0f, -0.353f,
             0.5f, 0.0f, 0.0f, // 重复第一个点，确保圆环闭合

             // 顶部圆的圆心
             0.0f, 1.0f, 0.0f, // 圆心
             // 顶部圆的若干个点（同底部圆对应）
             0.5f, 1.0f, 0.0f,
             0.353f, 1.0f, 0.353f,
             0.0f, 1.0f, 0.5f,
             -0.353f, 1.0f, 0.353f,
             -0.5f, 1.0f, 0.0f,
             -0.353f, 1.0f, -0.353f,
             0.0f, 1.0f, -0.5f,
             0.353f, 1.0f, -0.353f,
             0.5f, 1.0f, 0.0f  // 重复第一个点，确保圆环闭合
        };

        // 圆柱体的索引数据
        GLuint cylinderIndices[108] = {
        0, 1, 2,
        2, 3, 0,
        3, 4, 0,
        4, 5, 0,
        5, 6, 0,
        6, 7, 0,
        7, 8, 0,
        8, 9, 0,
        9, 1, 0,  // 连接最后一个点和第一个点

        // 顶面三角形：连接圆心和顶面圆的边
        10, 11, 12,
        12, 13, 10,
        13, 14, 10,
        14, 15, 10,
        15, 16, 10,
        16, 17, 10,
        17, 18, 10,
        18, 19, 10,
        19, 11, 10,  // 连接最后一个点和第一个点

        // 侧面三角形：连接底部和顶部的相应点，形成矩形
        1, 2, 11, 11, 2, 12,
        2, 3, 12, 12, 3, 13,
        3, 4, 13, 13, 4, 14,
        4, 5, 14, 14, 5, 15,
        5, 6, 15, 15, 6, 16,
        6, 7, 16, 16, 7, 17,
        7, 8, 17, 17, 8, 18,
        8, 9, 18, 18, 9, 19,
        9, 1, 19, 19, 1, 11  // 连接最后一段侧面
        };


        size_t cylinderVertexCount = sizeof(cylinderVertices) / sizeof(cylinderVertices[0]);
        size_t cylinderIndexCount = sizeof(cylinderIndices) / sizeof(cylinderIndices[0]);
        //圆锥体
        GLfloat coneVertices[18] = {
            // 圆顶，中心在原点，向上延伸
            0.0f, 0.0f, 0.5f,  // 顶部
            0.5f, 0.0f, -0.5f,  // 底部圆的一个点
            -0.5f, 0.0f, -0.5f, // 底部圆的另一个点

            0.0f, 0.0f, 0.5f,  // 顶部
            0.5f, 0.0f, -0.5f, // 底部圆的另一个点
            0.5f, 0.0f, 0.5f,  // 底部圆的一个点
        };

        GLuint coneIndices[18] = {
          0, 1, 2,  // 顶面三角形
          0, 2, 3,  // 顶面三角形
          0, 3, 4,  // 顶面三角形
          0, 4, 1,  // 顶面三角形

          // 底面可以被拆解为一个四边形，其中两个三角形
          1, 2, 3,  // 底面三角形1
          1, 3, 4   // 底面三角形2
        };


        size_t coneVertexCount = sizeof(coneVertices) / sizeof(coneVertices[0]);
        size_t coneIndexCount = sizeof(coneIndices) / sizeof(coneIndices[0]);
        //球体
            // 这里的球体顶点和索引示例为简化版，实际球体需要用算法生成。
        GLfloat sphereVertices[27] = {
            // 顶部
           0.0f,  1.0f,  0.0f,   // 顶部顶点
           0.5f,  0.866f, 0.0f,   // 第一条经度上的点
           0.0f,  0.866f, 0.5f,   // 第二条经度上的点
           -0.5f, 0.866f, 0.0f,   // 第三条经度上的点
           -0.5f, 0.866f, -0.5f,  // 第四条经度上的点
           0.0f, 0.866f, -0.5f,   // 第五条经度上的点
           0.5f,  0.0f,  0.5f,   // 第六条经度上的点
           0.5f,  0.0f, -0.5f,   // 第七条经度上的点
           0.0f,  -1.0f, 0.0f,   // 底部顶点
        };

        GLuint sphereIndices[39] = {
            0, 1, 2,
            0, 2, 3,
            0, 3, 4,
            0, 4, 5,
            0, 5, 6,
            0, 6, 7,
            0, 7, 8,
            // 底面
            8, 7, 6,
            8, 6, 5,
            8, 5, 4,
            8, 4, 3,
            8, 3, 2,
            8, 2, 1
        };

        // 计算球体的顶点数和索引数
        size_t sphereVertexCount = sizeof(sphereVertices) / sizeof(sphereVertices[0]);
        size_t sphereIndexCount = sizeof(sphereIndices) / sizeof(sphereIndices[0]);

        //胶囊体
        GLfloat capsuleVertices[72] = {
            // 胶囊顶点数据，简化版（需要具体生成数据）
            0.0f, 1.0f, 0.0f,  // 顶部
            0.5f, 0.0f, 0.0f,
            // 其他点...
        };

        GLuint capsuleIndices[72] = {
            // 胶囊的索引数据
            0, 1, 2,  // 示例索引
        };


        // 坐标系标识模型：
// 每个轴由两个顶点构成，每个顶点包含 3 个位置分量和 3 个颜色分量
// 共计 3 * 2 = 6 个顶点，每个顶点 6 个浮点数，总共 6 * 6 = 36 个数据
        GLfloat axisWidgetVertices[36] = {
            // X轴 (红色)：从原点到 (1,0,0)
            0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,  // 顶点 0
            1.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,  // 顶点 1

            // Y轴 (绿色)：从原点到 (0,1,0)
            0.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,  // 顶点 2
            0.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f,  // 顶点 3

            // Z轴 (蓝色)：从原点到 (0,0,1)   
            0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,  // 顶点 4
            0.0f, 0.0f, -1.0f,   0.0f, 0.0f, 1.0f   // 顶点 5
        };
        GLuint axisWidgetIndices[6] = {
            0, 1,  // X轴
            2, 3,  // Y轴
            4, 5   // Z轴
        };
        size_t axisWidgetVertexCount = sizeof(axisWidgetVertices) / sizeof(axisWidgetVertices[0]);  // 应该为 36
        size_t axisWidgetIndexCount = sizeof(axisWidgetIndices) / sizeof(axisWidgetIndices[0]);     // 应该为 6




#pragma endregion


    private:
        // 私有构造函数，防止外部创建实例
        MeshDataManager() {};

        static MeshDataManager* instance;

    };
}
#endif // MESH_DATA_MANAGER_H
