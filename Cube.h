#ifndef CUBE_H
#define CUBE_H
#include "GameObject.h"


namespace Game {
    class Cube : public GameObject
    {
    public:
#pragma region   �����ʼ��ԭ��{}ֻ��������ʱ���г�ʼ��


        GLfloat vertices[24] = {
            // λ��
            -0.5f, -0.5f, -0.5f, // ǰ��
             0.5f, -0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,

            -0.5f, -0.5f,  0.5f, // ����
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f
        };
        GLuint indices[36] = {
            0, 1, 2, 2, 3, 0,  // ǰ��
            4, 5, 6, 6, 7, 4,  // ����
            0, 1, 5, 5, 4, 0,  // �ײ�
            2, 3, 7, 7, 6, 2,  // ����
            0, 3, 7, 7, 4, 0,  // ���
            1, 2, 6, 6, 5, 1   // �Ҳ�
        };
#pragma endregion

        Cube(const char* vertexShaderSourceIn, const  char* fragmentShaderSourceIn);
        //--�̳�GameObject ������
        Cube();


        bool Draw(glm::mat4 view, glm::mat4 projection) override;//C++ �ж��鷽������д��override �ؼ��ּ��ڷ������棬��һ��������consnt��
        bool DrawDynamical(glm::mat4 view, glm::mat4 projection) override;
       virtual ~Cube();
        //--�̳�MonoBehaviour������
        void Update(glm::mat4 view, glm::mat4 projection) override;
        void UpdateVariant(glm::mat4 view, glm::mat4 projection) override;
        virtual bool AttachTexture(GLuint textureName, int order) ;
        


    protected:
        GLuint _cubeMapID;
        GLuint _skyboxVAO;
        GLuint _skyboxVBO;
        GLuint _skyboxEBO;
        bool _ifCubeMap = false;
        GLfloat _skyboxVertices[108] = {
            // ��������
            // ǰ��
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            // ����
            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            // ����
            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,

            // ����
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

             // ����
             -1.0f,  1.0f, -1.0f,
             -1.0f,  1.0f,  1.0f,
              1.0f,  1.0f,  1.0f,
              1.0f,  1.0f,  1.0f,
              1.0f,  1.0f, -1.0f,
             -1.0f,  1.0f, -1.0f,

             // ����
             -1.0f, -1.0f, -1.0f,
             -1.0f, -1.0f,  1.0f,
              1.0f, -1.0f, -1.0f,
              1.0f, -1.0f, -1.0f,
             -1.0f, -1.0f,  1.0f,
              1.0f, -1.0f,  1.0f
        };


    private:
        GLuint LoadCubemap(std::vector<std::string>& faces);

    };
}
#endif // CUBE_H
