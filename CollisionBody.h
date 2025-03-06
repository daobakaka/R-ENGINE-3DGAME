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
        CollisionBody(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, glm::quat& rot, glm::vec3& ratio, int id, float& mass, float& friction, float& elasticity, int& layer, bool& trigger,
            bool& active, float& rotationDamping, bool& lockXZAxi, float& rotaionAdjust, bool ifStatic = false, CollisionType collider = CollisionType::Box,
            float radius = 1.0f, SpecialType type = SpecialType::OriginalT,ModelClass logicType=ModelClass::OriginalE);
        ~CollisionBody();

        // ��������״̬�������ײ���߼�����ײ����ܵ�λ�ƻ��ٶȱ仯��
        void UpdateCollisionState(std::unordered_map<int, CollisionProperties*>& cop,float deltaTime=0.0167F);   
 

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
        //����������ײ�� ��ײִ�а취
        void UpdateSpecialCollider(std::unordered_map<int, CollisionProperties*>& cop, float deltaTime = 0.0167F);

        //ȷ����Ч��ײ��ⷶΧ
        bool CheckValidCollision(glm::vec3 range = glm::vec3(500));

        //�����ײ
        bool CheckCollisionWithAABB(CollisionProperties* other);

        //������ײ�����������ֵ��
        void CalculateGameParameters(CollisionProperties* other);
    public:
        //��ײ�뾶ֻ��ͨ����ײ�����ã�����������ͨ������ṹ����
        void SetRadius(float r = 1.0f);
        //��ײ��Ҳ����ͨ����ײ������
        void SetLayer(int layer = 1);
        //�Ƿ�Ϊ������Ҳ����ͨ����ײ������
        void SetTrigger(bool trigger=false);
        //������Ϸ����
        void SetGameProperties(float health=100,float damage=5,float speed=1);
    
        // ��ȡ��ײ�����������
        CollisionProperties& GetCollisionProperties() { return _collisionProperties; }
        

    private:
        CollisionProperties _collisionProperties;  // ��װ����������,��ײ����ȫ����װ����ײ�ṹ����

        Octree* _octree;
        std::vector<CollisionProperties*> _potentialCollisions;


    };

}

#endif // !COLLISION_BODY_H
