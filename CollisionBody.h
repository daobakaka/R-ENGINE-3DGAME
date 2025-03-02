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
    extern std::unordered_map<int, CollisionProperties*> CollisionProps;//������ײ���ڲ���������ڲ����м��
    // ��ײ�����,���캯��������ײ������������������ã���ͨ����ײ��ļ��㴫ֱ�Ӹı���Ӧ���������
    class CollisionBody : public IComponent<CollisionBody> {
    public:
        // ���캯������������
        CollisionBody(glm::vec3 &pos,glm::vec3 &vel,glm::vec3&acc, glm::quat& rot, float mass ,float friction, float elasticity,bool ifStatic);
        ~CollisionBody();

        // ��������״̬�������ײ���߼�����ײ����ܵ�λ�ƻ��ٶȱ仯��
        void UpdateCollisionState(std::unordered_map<int, CollisionProperties*>& cop,float deltaTime=0.0167F);
        //������ײ��ĳ�ʼ������,������ײ��
        void SetCollisionParameters(CollisionType type, float radius, glm::vec3 ratiom, int layer = 1, bool trigger = false);

    private:
        //ʵ��IComponenet�ӿ�,�ӿ��Զ�public
        bool Interface() override;
        //ע����ײ��
        void RegisterCollisionBody();
        //������ײ�����
        void UpdateCollisionParameters();
        //����������ײ״̬
        void ResolveCollision(CollisionProperties* other);
        //������ײ������
        glm::vec3 CalculateCollisionNormal(CollisionProperties* other);
        // ��������Ķ��������״̬
        void UpdateGravityState(float normalY);
        //������ײ�巽��������ײ������������������
        void UpdatePhysicsCollision(float deltaTime = 0.0167f);
        //����Ĭ����ת������Ĭ����������X��Z;

                // ����������������ԣ���һϵ�з���ͨ�������߼��Ż�
        void SetPosition(glm::vec3 pos) { _collisionProperties.position = pos; }
        void SetVelocity(glm::vec3 vel) { _collisionProperties.velocity = vel; }
        void SetAcceleration(glm::vec3 acc) { _collisionProperties.acceleration = acc; }
        //��ʱû�д������ã��������������
        void SetMass(float m) { _collisionProperties.mass = m; }
    public:
        bool SetFixedAxisX(bool lock=true);
        //������ײ��ת����
        void SetRotationDamping(float damp);


        // ��ȡ�Է���ײ�����������
        CollisionProperties& GetCollisionProperties() { return _collisionProperties; }


        //�����ײ
        bool CheckCollisionWithAABB(CollisionProperties* other);

    private:
        CollisionProperties _collisionProperties;  // ��װ����������,��ײ����ȫ����װ����ײ�ṹ����
        static int NEXTINT;
        float _rotationPar;
        bool _lockXAxi;
        Octree* _octree;
        std::vector<CollisionProperties*> _potentialCollisions;
   

    };

}

#endif // !COLLISION_BODY_H
