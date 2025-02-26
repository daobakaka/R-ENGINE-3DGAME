#include "CollisionBody.h"
#include <iostream>

namespace Game {

    // 构造函数
    CollisionBody::CollisionBody() {
        // 初始化物理属性
        collisionProperties.position = glm::vec3(0.0f);
        collisionProperties.velocity = glm::vec3(0.0f);
        collisionProperties.acceleration = glm::vec3(0.0f);
        collisionProperties.mass = 1.0f;
    }

    // 析构函数
    CollisionBody::~CollisionBody() {}

    // 更新物体的速度和位置
    void CollisionBody::Update(float deltaTime) {
        // 更新位置
        collisionProperties.position += collisionProperties.velocity * deltaTime;
    }

    // 碰撞检测方法
    bool CollisionBody::CheckCollision(CollisionBody& other) {
        // 检测 AABB 类型的碰撞体
        return CheckCollisionWithAABB(other);
    }

    // 处理碰撞（与 AABB 碰撞）
    void CollisionBody::ResolveCollision(CollisionBody& other) {
        ResolveAABBCollision(other);
    }

    // 与 AABB 碰撞检测
    bool CollisionBody::CheckCollisionWithAABB(CollisionBody& other) {
        // 检查 AABB 碰撞
        glm::vec3 otherMin = other.GetCollisionProperties().position - glm::vec3(1.0f);
        glm::vec3 otherMax = other.GetCollisionProperties().position + glm::vec3(1.0f);

        return (collisionProperties.position.x > otherMin.x && collisionProperties.position.x < otherMax.x) &&
            (collisionProperties.position.y > otherMin.y && collisionProperties.position.y < otherMax.y) &&
            (collisionProperties.position.z > otherMin.z && collisionProperties.position.z < otherMax.z);
    }

    // AABB 碰撞体的处理
    void CollisionBody::ResolveAABBCollision(CollisionBody& other) {
        // 简单处理：当发生碰撞时，输出碰撞信息
        std::cout << "与 AABB 碰撞！" << std::endl;
    }

}
