// GameObject.h
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>  // 用于旋转
#include "MonoBehaviour.h"
#include "Controller.h"
#include"IComponent.h"
#include "CollisionBody.h"
#include "PhysicalEngine.h"
namespace Game {

    class GameObject :public MonoBehaviour
    {
    public:

        ////控制器实例以及视图(相机拍摄角度)和投影矩阵(相机视野，屏幕比例，近、远裁面)
        //Controller* controller;
        //glm::mat4 view;
        //glm::mat4 projection;
        //--变换模块
        glm::vec3 position;    // 物体位置
        glm::quat rotation;    // 物体旋转（四元数）
        glm::vec3 scale;       // 物体缩放
        // 变换矩阵
        glm::mat4 transform;

        int testNum;

        //默认构造函数
        GameObject();
       virtual ~GameObject();
        // 初始化函数，设置物体的基本变换
        void Initialize(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::quat rotation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)),
            glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));
        // 更新变换矩阵
        void UpdateTransform();
        //变体属性
        void SetVariant(ModelClass par) override;
        ModelClass GetVariant() override;
        // 获取模型矩阵
        glm::mat4 GetModelMatrix() const;
        //mesh模块
        GLuint VAO, VBO, EBO;
        GLuint shaderProgram;
        GLuint uvsVBO;
        GLuint normalsVBO;
        GLint previousShader;
        GLint previousTexture;

        //添加光照模型默认值
        bool ifLight;
        //初始化判断参数模块
        bool justDrawVerteies = false;
        bool IsSkinnedMesh = false;
        //shader模块
           // 检查着色器编译是否成功
        void CheckShaderCompilation(GLuint shader);

        // 检查着色器程序链接是否成功
        void CheckShaderProgramLinking(GLuint program);

        // 绘制物体静态方法必须重写
        virtual bool Draw(glm::mat4 view, glm::mat4 projection) = 0;
        //绘制物体动态方法，必须重写，交由 LifeCyleManager 进行管理
        virtual bool DrawDynamical(glm::mat4 view, glm::mat4 projection) = 0;
        //加载物体texture
       // virtual bool AttachTexture(GLuint name,int order);

        //--mono 方法基础性重写
        virtual void Start() override;//事实上 unity 中的awake 或者enable就是构造的位置，所以start 在其后面
        // virtual void Update() override;//更新方法和每个模型的 顶点相关联，所以在具体的模型类中进行重写
        virtual void Destroy() override;//这里的销毁 本身的逻辑就是删除引用的实例类，所以可以直接默认
        virtual void UpdateVariant(glm::mat4 view, glm::mat4 projection) override;

        //--光源深度图绘制

        virtual void DrawDepthPic(glm::mat4 lightSpaceMatrix,GLuint shader);
        virtual void DrawDepthPicDynamical(glm::mat4 lightSpaceMatrix,GLuint shader);
        virtual void UpdateDepthPic(glm::mat4 lightSpaceMatrix,GLuint shader) override;

        //--获取碰撞判断
        virtual bool GetIfCollision() override;
        
        //获取自增ID

    private:
        //初始化变体
        ModelClass variant = ModelClass::OriginalE;

    protected:
        bool _ifShadow;
        GLuint  _depthShaderProgram;
      
    };
}
#endif
