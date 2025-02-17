#ifndef CUBE_H
#define CUBE_H
#include "GameObject.h"


namespace Game {
    class Cube : public GameObject
    {
    public:
#pragma region   数组初始化原则：{}只能在声明时进行初始化


        GLfloat vertices[24] = {
            // 位置
            -0.5f, -0.5f, -0.5f, // 前面
             0.5f, -0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,

            -0.5f, -0.5f,  0.5f, // 后面
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f
        };
        GLuint indices[36] = {
            0, 1, 2, 2, 3, 0,  // 前面
            4, 5, 6, 6, 7, 4,  // 后面
            0, 1, 5, 5, 4, 0,  // 底部
            2, 3, 7, 7, 6, 2,  // 顶部
            0, 3, 7, 7, 4, 0,  // 左侧
            1, 2, 6, 6, 5, 1   // 右侧
        };
#pragma endregion

        Cube(const char* vertexShaderSourceIn, const  char* fragmentShaderSourceIn);
        //--继承GameObject 的声明
        Cube(GLuint textureName);


        bool Draw(glm::mat4 view, glm::mat4 projection) override;//C++ 中对虚方法的重写，override 关键字加在方法后面，这一特征包括consnt等
        bool DrawDynamical(glm::mat4 view, glm::mat4 projection) override;
        ~Cube();
        //--继承MonoBehaviour的声明
        void Update(glm::mat4 view, glm::mat4 projection) override;
        void UpdateVariant(glm::mat4 view, glm::mat4 projection) override;
        virtual bool AttachTexture(GLuint textureName, int order) override;



    protected:
        GLuint _cubeMapID;
        GLuint _skyboxVAO;
        GLuint _skyboxVBO;
        bool _ifCubeMap = false;
        GLfloat _skyboxVertices[108] = {
            // 顶点坐标
            // 前面
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            // 后面
            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            // 左面
            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,

            // 右面
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

             // 上面
             -1.0f,  1.0f, -1.0f,
             -1.0f,  1.0f,  1.0f,
              1.0f,  1.0f,  1.0f,
              1.0f,  1.0f,  1.0f,
              1.0f,  1.0f, -1.0f,
             -1.0f,  1.0f, -1.0f,

             // 下面
             -1.0f, -1.0f, -1.0f,
             -1.0f, -1.0f,  1.0f,
              1.0f, -1.0f, -1.0f,
              1.0f, -1.0f, -1.0f,
             -1.0f, -1.0f,  1.0f,
              1.0f, -1.0f,  1.0f
        };




    };
}
#endif // CUBE_H
