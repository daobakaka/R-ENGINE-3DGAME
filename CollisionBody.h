#ifndef COLLISION_BODY_H
#define COLLISION_BODY_H

#include "GameObject.h"
#include <vector>
#include <glm/glm.hpp>
#include "EnumTotal.h"

namespace Game {
        
    class PhysicalEngine;
    class CollisionBody;
    class Octree;
    extern std::unordered_map<int, CollisionProperties*> CollisionProps;//就在碰撞体内部这个大类内部进行检查
    // 碰撞体基类,构造函数传入碰撞体的物理参数的相关引用，并通过碰撞体的计算传直接改变相应的物理参数
    class CollisionBody : public IComponent<CollisionBody> {
    public:
        // 构造函数和析构函数
        CollisionBody(glm::vec3 &pos,glm::vec3 &vel,glm::vec3&acc, glm::quat& rot, float mass ,float friction, float elasticity,bool ifStatic);
        ~CollisionBody();

        // 更新物体状态，检测碰撞等逻辑（碰撞后可能的位移或速度变化）
        void UpdateCollisionState(std::unordered_map<int, CollisionProperties*>& cop,float deltaTime=0.0167F);
        //设置碰撞体的初始化参数,设置碰撞层
        void SetCollisionParameters(CollisionType type, float radius, glm::vec3 ratiom, int layer = 1, bool trigger = false);

    private:
        //实现IComponenet接口,接口自动public
        bool Interface() override;
        //注册碰撞体
        void RegisterCollisionBody();
        //更新碰撞体参数
        void UpdateCollisionParameters();
        //分析物理碰撞状态
        void ResolveCollision(CollisionProperties* other);
        //计算碰撞法向量
        glm::vec3 CalculateCollisionNormal(CollisionProperties* other);
        // 更新物体的对象的重力状态
        void UpdateGravityState(float normalY);
        //更新碰撞体方法，无碰撞体才运行物理基本运算
        void UpdatePhysicsCollision(float deltaTime = 0.0167f);
        //设置默认旋转锁定，默认设置锁定X和Z;

                // 设置物体的物理属性，这一系列方法通过引用逻辑优化
        void SetPosition(glm::vec3 pos) { _collisionProperties.position = pos; }
        void SetVelocity(glm::vec3 vel) { _collisionProperties.velocity = vel; }
        void SetAcceleration(glm::vec3 acc) { _collisionProperties.acceleration = acc; }
        //暂时没有传递引用，这里是无意义的
        void SetMass(float m) { _collisionProperties.mass = m; }
    public:
        bool SetFixedAxisX(bool lock=true);
        //设置碰撞旋转阻尼
        void SetRotationDamping(float damp);


        // 获取对方碰撞体的物理属性
        CollisionProperties& GetCollisionProperties() { return _collisionProperties; }


        //检测碰撞
        bool CheckCollisionWithAABB(CollisionProperties* other);

    private:
        CollisionProperties _collisionProperties;  // 封装的物理属性,碰撞属性全部封装在碰撞结构体中
        static int NEXTINT;
        float _rotationPar;
        bool _lockXAxi;
        Octree* _octree;
        std::vector<CollisionProperties*> _potentialCollisions;
   

    };

}

#endif // !COLLISION_BODY_H
