#ifndef COLLISION_BODY_H
#define COLLISION_BODY_H

#include "GameObject.h"
#include <vector>
#include <glm/glm.hpp>
#include "EnumTotal.h"

namespace Game {

    // 碰撞体基类
    class CollisionBody : public IComponent<CollisionBody> {
    public:
        // 构造函数和析构函数
        CollisionBody();
        ~CollisionBody();

        // 用于碰撞检测的基本方法
        bool CheckCollision(CollisionBody& other);
        void ResolveCollision(CollisionBody& other);

        // 更新物体状态（碰撞后可能的位移或速度变化）
        void Update(float deltaTime);

        // 设置物体的物理属性
        void SetPosition(const glm::vec3& pos) { collisionProperties.position = pos; }
        void SetVelocity(const glm::vec3& vel) { collisionProperties.velocity = vel; }
        void SetAcceleration(const glm::vec3& acc) { collisionProperties.acceleration = acc; }
        void SetMass(float m) { collisionProperties.mass = m; }

        // 获取物体的物理属性
        CollisionProperties& GetCollisionProperties() { return collisionProperties; }

        // 碰撞体类型：盒型（AABB）逻辑直接放在这里
        bool CheckCollisionWithAABB(CollisionBody& other);
        void ResolveAABBCollision(CollisionBody& other);

    private:
        CollisionProperties collisionProperties;  // 封装的物理属性
    };

}

#endif // !COLLISION_BODY_H
