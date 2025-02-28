#ifndef COLLISION_BODY_H
#define COLLISION_BODY_H

#include "GameObject.h"
#include <vector>
#include <glm/glm.hpp>
#include "EnumTotal.h"

namespace Game {
        
    class PhysicalEngine;
    class CollisionBody;
    extern std::unordered_map<int, CollisionProperties*> CollisionProps;//������ײ���ڲ���������ڲ����м��

    // ��ײ�����,���캯��������ײ������������������ã���ͨ����ײ��ļ��㴫ֱ�Ӹı���Ӧ���������
    class CollisionBody : public IComponent<CollisionBody> {
    public:
        // ���캯������������
        CollisionBody(glm::vec3 &pos,glm::vec3 &vel,glm::vec3&acc,float mass ,float friction,bool ifStatic);
        ~CollisionBody();
        //ʵ��IComponenet�ӿ�
        bool Interface() override;

        // ��������״̬�������ײ���߼�����ײ����ܵ�λ�ƻ��ٶȱ仯��
        void UpdateCollisionState(float deltaTime=0.0167F);

        // ����������������ԣ���һϵ�з���ͨ�������߼��Ż�
        void SetPosition( glm::vec3 pos) { _collisionProperties.position = pos; }
        void SetVelocity( glm::vec3 vel) { _collisionProperties.velocity = vel; }
        void SetAcceleration( glm::vec3 acc) { _collisionProperties.acceleration = acc; }
        void SetMass(float m) { _collisionProperties.mass = m; }

        //������ײ��ĳ�ʼ������,������ײ��
        void SetCollisionParameters(CollisionType type, float radius, glm::vec3 ratiom,int layer=1,bool trigger=false);
        //ע����ײ��
        void RegisterCollisionBody();
        //������ײ�����
        void UpdateCollisionParameters();
        //����������ײ״̬
        void ResolveCollision(CollisionProperties* other);

        // ��ȡ�Է���ײ�����������
        CollisionProperties& GetCollisionProperties() { return _collisionProperties; }


        //�����ײ
        bool CheckCollisionWithAABB(CollisionProperties* other);
        int ID;

    private:
        CollisionProperties _collisionProperties;  // ��װ����������,��ײ����ȫ����װ����ײ�ṹ����
        bool _ifStatic;
        static int NEXTINT;
   

    };

}

#endif // !COLLISION_BODY_H
