#ifndef COLLISION_BODY_H
#define COLLISION_BODY_H

#include "GameObject.h"
#include <vector>
#include <glm/glm.hpp>
#include "EnumTotal.h"

namespace Game {

    // ��ײ�����
    class CollisionBody : public IComponent<CollisionBody> {
    public:
        // ���캯������������
        CollisionBody();
        ~CollisionBody();

        // ������ײ���Ļ�������
        bool CheckCollision(CollisionBody& other);
        void ResolveCollision(CollisionBody& other);

        // ��������״̬����ײ����ܵ�λ�ƻ��ٶȱ仯��
        void Update(float deltaTime);

        // �����������������
        void SetPosition(const glm::vec3& pos) { collisionProperties.position = pos; }
        void SetVelocity(const glm::vec3& vel) { collisionProperties.velocity = vel; }
        void SetAcceleration(const glm::vec3& acc) { collisionProperties.acceleration = acc; }
        void SetMass(float m) { collisionProperties.mass = m; }

        // ��ȡ�������������
        CollisionProperties& GetCollisionProperties() { return collisionProperties; }

        // ��ײ�����ͣ����ͣ�AABB���߼�ֱ�ӷ�������
        bool CheckCollisionWithAABB(CollisionBody& other);
        void ResolveAABBCollision(CollisionBody& other);

    private:
        CollisionProperties collisionProperties;  // ��װ����������
    };

}

#endif // !COLLISION_BODY_H
