// GameObject.h
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>  // ������ת
#include "MonoBehaviour.h"
#include "Controller.h"
#include"IComponent.h"
#include "CollisionBody.h"
#include "PhysicalEngine.h"
namespace Game {

    class GameObject :public MonoBehaviour
    {
    public:

        ////������ʵ���Լ���ͼ(�������Ƕ�)��ͶӰ����(�����Ұ����Ļ����������Զ����)
        //Controller* controller;
        //glm::mat4 view;
        //glm::mat4 projection;
        //--�任ģ��
        glm::vec3 position;    // ����λ��
        glm::quat rotation;    // ������ת����Ԫ����
        glm::vec3 scale;       // ��������
        // �任����
        glm::mat4 transform;

        int testNum;

        //Ĭ�Ϲ��캯��
        GameObject();
       virtual ~GameObject();
        // ��ʼ����������������Ļ����任
        void Initialize(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::quat rotation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)),
            glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));
        // ���±任����
        void UpdateTransform();
        //��������
        void SetVariant(ModelClass par) override;
        ModelClass GetVariant() override;
        // ��ȡģ�;���
        glm::mat4 GetModelMatrix() const;
        //meshģ��
        GLuint VAO, VBO, EBO;
        GLuint shaderProgram;
        GLuint uvsVBO;
        GLuint normalsVBO;
        GLint previousShader;
        GLint previousTexture;

        //��ӹ���ģ��Ĭ��ֵ
        bool ifLight;
        //��ʼ���жϲ���ģ��
        bool justDrawVerteies = false;
        bool IsSkinnedMesh = false;
        //shaderģ��
           // �����ɫ�������Ƿ�ɹ�
        void CheckShaderCompilation(GLuint shader);

        // �����ɫ�����������Ƿ�ɹ�
        void CheckShaderProgramLinking(GLuint program);

        // �������徲̬����������д
        virtual bool Draw(glm::mat4 view, glm::mat4 projection) = 0;
        //�������嶯̬������������д������ LifeCyleManager ���й���
        virtual bool DrawDynamical(glm::mat4 view, glm::mat4 projection) = 0;
        //��������texture
       // virtual bool AttachTexture(GLuint name,int order);

        //--mono ������������д
        virtual void Start() override;//��ʵ�� unity �е�awake ����enable���ǹ����λ�ã�����start �������
        // virtual void Update() override;//���·�����ÿ��ģ�͵� ����������������ھ����ģ�����н�����д
        virtual void Destroy() override;//��������� ������߼�����ɾ�����õ�ʵ���࣬���Կ���ֱ��Ĭ��
        virtual void UpdateVariant(glm::mat4 view, glm::mat4 projection) override;

        //--��Դ���ͼ����

        virtual void DrawDepthPic(glm::mat4 lightSpaceMatrix,GLuint shader);
        virtual void DrawDepthPicDynamical(glm::mat4 lightSpaceMatrix,GLuint shader);
        virtual void UpdateDepthPic(glm::mat4 lightSpaceMatrix,GLuint shader) override;

        //--��ȡ��ײ�ж�
        virtual bool GetIfCollision() override;
        
        //��ȡ����ID

    private:
        //��ʼ������
        ModelClass variant = ModelClass::OriginalE;

    protected:
        bool _ifShadow;
        GLuint  _depthShaderProgram;
      
    };
}
#endif
