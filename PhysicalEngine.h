#ifndef PHYSICAL_ENGINE_H
#define PHYSICAL_ENGINE_H

#include "GameObject.h"
#include <glm/glm.hpp>

namespace Game {


    class CollisionBody;
    class PhysicalEngine : public IComponent<PhysicalEngine> {
    public:
        PhysicalEngine(glm::vec3& positon,glm::quat &rotation,glm::vec3 &scale, bool& isActive,int id, float mass, float friction, glm::vec3 velocity=glm::vec3(0), glm::vec3 acceleration=glm::vec3(0,-9.8f,0),
            float elasticity=0.5f,bool lockXZAxi=true, float rotationDampping=0.15f, bool trigger=false, int layer=1,float rotationAdjust=0.382f, bool staticObj = false
            );  // 构造函数
        ~PhysicalEngine();

        // 更新物体的物理状态（例如位置、速度、加速度）
        void UpdatePhysics(float deltaTime = 0.0167f);
        // 设置物理组件的基本参数
        void SetMass(float mass);
        void SetFriction(float friction);
        void SetVelocity( glm::vec3 velocity);
        void SetAcceleration( glm::vec3 acceleration);
        void SetElasticity(float e);
        bool SetFixedAxisX(bool lock = true);
        void SetRotationDamping(float damp = 0.15f);
       
        void SetActive(bool active = true);
        void SetTrigger(bool trigger = false);
        void SetLayer(int layer = 1);
        void SetRotationAdjust(float r = 0.382f);

        // 获取当前物体的物理状态const拓宽范围常用于get方法标识
        glm::vec3 &GetPosition() ;
        glm::vec3 &GetVelocity() ;
        glm::vec3 &GetAcceleration() ;
        glm::quat& GetRotation();
        glm::vec3& GetScale();
        float& GetMass() ;
        float &GetFriction() ;
        bool &GetStatic();
        float& GetElasticity();
        bool &GetLockState() ;
        float &GetDamping() ;

        bool  &GetActive();
        bool &GetTrigger() ;
        int &GetLayer() ;
        float &GetRotationAdjust() ;

            
            
    private:
        bool Interface() override;

    private:
        float _mass;                  // 质量
        float _friction;              // 摩擦系数
        glm::vec3 &_position;          // 物体位置,引用类型必须通过构造函数初始化，这样内部就可以直接修改
        glm::vec3 _velocity;          // 物体速度
        glm::vec3 _acceleration;      // 物体加速度
        glm::quat &_rotation;        //物体旋转逻辑
        glm::vec3 &_scale;        //物体大小
        glm::vec3 _gravity;        //物体重力加速度
        bool _ifStatic;      //是否为静态物体，这个参数不可更改，静态物体自身不参与碰撞计算，但是可被其他物体引用
        float _elasticity;//物体弹性系数
        int ID;
        bool _lockXZAxi;
        float _rotationDamping;
        bool &_isActive;//这个引用由外部控制,不仅仅是要控制物理组件和碰撞组件，还要控制渲染逻辑
        bool _trigger;
        int _layer;
        float _rotationAdjust;//默认黄金分割


    };

}  // namespace Game

#endif // !PHYSICAL_ENGINE_H
