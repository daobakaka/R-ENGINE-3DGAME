#include "CollisionBody.h"
#include <iostream>

namespace Game {

    // ���캯��
    CollisionBody::CollisionBody() {
        // ��ʼ����������
        collisionProperties.position = glm::vec3(0.0f);
        collisionProperties.velocity = glm::vec3(0.0f);
        collisionProperties.acceleration = glm::vec3(0.0f);
        collisionProperties.mass = 1.0f;
    }

    // ��������
    CollisionBody::~CollisionBody() {}

    // ����������ٶȺ�λ��
    void CollisionBody::Update(float deltaTime) {
        // ����λ��
        collisionProperties.position += collisionProperties.velocity * deltaTime;
    }

    // ��ײ��ⷽ��
    bool CollisionBody::CheckCollision(CollisionBody& other) {
        // ��� AABB ���͵���ײ��
        return CheckCollisionWithAABB(other);
    }

    // ������ײ���� AABB ��ײ��
    void CollisionBody::ResolveCollision(CollisionBody& other) {
        ResolveAABBCollision(other);
    }

    // �� AABB ��ײ���
    bool CollisionBody::CheckCollisionWithAABB(CollisionBody& other) {
        // ��� AABB ��ײ
        glm::vec3 otherMin = other.GetCollisionProperties().position - glm::vec3(1.0f);
        glm::vec3 otherMax = other.GetCollisionProperties().position + glm::vec3(1.0f);

        return (collisionProperties.position.x > otherMin.x && collisionProperties.position.x < otherMax.x) &&
            (collisionProperties.position.y > otherMin.y && collisionProperties.position.y < otherMax.y) &&
            (collisionProperties.position.z > otherMin.z && collisionProperties.position.z < otherMax.z);
    }

    // AABB ��ײ��Ĵ���
    void CollisionBody::ResolveAABBCollision(CollisionBody& other) {
        // �򵥴�����������ײʱ�������ײ��Ϣ
        std::cout << "�� AABB ��ײ��" << std::endl;
    }

}
