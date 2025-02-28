#include "CollisionBody.h"
#include <iostream>
#include "PhysicalEngine.h"

namespace Game {


    std::unordered_map<int, CollisionProperties*> CollisionProps;
    int CollisionBody::NEXTINT = 0;
    // ���캯��
    CollisionBody::CollisionBody(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, float mass, float friction,bool ifStatic=false)
        :_collisionProperties(pos, vel, acc)//��ʼ���ṹ������
    {
        
        _collisionProperties.mass = mass;
        _collisionProperties.friction = friction;
        _collisionProperties.ID = NEXTINT++;//����ʹ����ײ�ṹ�崢����صĲ�������ID����ײ�����Ҳֱ����ṹ����п��ٽ���
        _ifStatic = ifStatic;
        if (_ifStatic)
            _collisionProperties.mass = 100000000;//���þ�̬���������Ϊ��        
    }
    // ��������
    CollisionBody::~CollisionBody() {}

    bool CollisionBody::Interface()
    {
        return false;
    }

    // ����������ٶȺ�λ��
    void CollisionBody::UpdateCollisionState(float deltaTime) {
       
        
        UpdateCollisionParameters();
        if (_collisionProperties.ID ==0)
        {
           //������Ը�����ײ����������ײ����
           
         CheckCollisionWithAABB(CollisionProps[1]);//�������ã��������Ҫʹ��*����ֵ��������ָ��Ͳ���Ҫ
        }
  

      
        
        //// �������ӡ��ײ��Ĳ���
        //std::cout << "Position: (" << _collisionProperties.position.x << ", "
        //    << _collisionProperties.position.y << ", "
        //    << _collisionProperties.position.z << ")\n";

        //std::cout << "Velocity: (" << _collisionProperties.velocity.x << ", "
        //    << _collisionProperties.velocity.y << ", "
        //    << _collisionProperties.velocity.z << ")\n";

        //std::cout << "Acceleration: (" << _collisionProperties.acceleration.x << ", "
        //    << _collisionProperties.acceleration.y << ", "
        //    << _collisionProperties.acceleration.z << ")\n";

        //std::cout << "Mass: " << _collisionProperties.mass << "\n";
        //std::cout << "Friction: " << _collisionProperties.friction << "\n";
        //std::cout << "Collision Min: (" << _collisionProperties._collisionMin.x << ", "
        //    << _collisionProperties._collisionMin.y << ", "
        //    << _collisionProperties._collisionMin.z << ")\n";
        //std::cout << "Collision Max: (" << _collisionProperties._collisionMax.x << ", "
        //    << _collisionProperties._collisionMax.y << ", "
        //    << _collisionProperties._collisionMax.z << ")\n";


    }


    void CollisionBody::SetCollisionParameters(CollisionType type,float radius, glm::vec3 ratio,int layer, bool trigger)
    {

        _collisionProperties.type = type;
        _collisionProperties.radius = radius;
        _collisionProperties.ratio = ratio;
        _collisionProperties.layer = layer;
        _collisionProperties.trigger = trigger;
       //ע��ȫ�ַ���ģ��
        RegisterCollisionBody();
    }

    void CollisionBody::RegisterCollisionBody()
    {

        CollisionProps[_collisionProperties.ID] = &_collisionProperties;

    }

    void CollisionBody::UpdateCollisionParameters()
    {
        switch (_collisionProperties.type)
        {
        case CollisionType:: Box:
            _collisionProperties._collisionMin.x = _collisionProperties.position.x - _collisionProperties.radius * _collisionProperties.ratio.x;
            _collisionProperties._collisionMin.y = _collisionProperties.position.y - _collisionProperties.radius * _collisionProperties.ratio.y;
            _collisionProperties._collisionMin.z = _collisionProperties.position.z - _collisionProperties.radius * _collisionProperties.ratio.z;
            _collisionProperties._collisionMax.x = _collisionProperties.position.x + _collisionProperties.radius * _collisionProperties.ratio.x;
            _collisionProperties._collisionMax.y = _collisionProperties.position.y + _collisionProperties.radius * _collisionProperties.ratio.y;
            _collisionProperties._collisionMax.z = _collisionProperties.position.z + _collisionProperties.radius * _collisionProperties.ratio.z;

            break;
        case CollisionType::Sphere:
            _collisionProperties._collisionMin.x = _collisionProperties.position.x - _collisionProperties.radius * _collisionProperties.ratio.x;
            _collisionProperties._collisionMin.y = _collisionProperties.position.y - _collisionProperties.radius * _collisionProperties.ratio.y;
            _collisionProperties._collisionMin.z = _collisionProperties.position.z - _collisionProperties.radius * _collisionProperties.ratio.z;
            _collisionProperties._collisionMax.x = _collisionProperties.position.x + _collisionProperties.radius * _collisionProperties.ratio.x;
            _collisionProperties._collisionMax.y = _collisionProperties.position.y + _collisionProperties.radius * _collisionProperties.ratio.y;
            _collisionProperties._collisionMax.z = _collisionProperties.position.z + _collisionProperties.radius * _collisionProperties.ratio.z;

            break;
        default:
            break;
        }


    }

    void CollisionBody::ResolveCollision(CollisionProperties* other) {
        
        if (!other->trigger) {
            

            // ��ȡ��ǰ����ͶԷ������ AABB �߽�
            glm::vec3 currentMin = _collisionProperties._collisionMin;
            glm::vec3 currentMax = _collisionProperties._collisionMax;
            glm::vec3 otherMin = other->_collisionMin;
            glm::vec3 otherMax = other->_collisionMax;

            // ������ײ�㣨������ײ���ǵ�ǰ��������ģ�
            glm::vec3 collisionPoint = _collisionProperties.position;

            // ������ײ�㵽�Է�����ÿ����ľ���
            float distToLeft = collisionPoint.x - otherMin.x;   // ���� (x = minX)
            float distToRight = otherMax.x - collisionPoint.x;  // ���� (x = maxX)
            float distToBottom = collisionPoint.y - otherMin.y; // ���� (y = minY)
            float distToTop = otherMax.y - collisionPoint.y;    // ���� (y = maxY)
            float distToBack = collisionPoint.z - otherMin.z;   // ���� (z = minZ)
            float distToFront = otherMax.z - collisionPoint.z;  // ǰ�� (z = maxZ)

            // �ҵ�������С����
            float minDist = std::min({ distToLeft, distToRight, distToBottom, distToTop, distToBack, distToFront });

            // ȷ����ײ������
            glm::vec3 collisionNormal;
            if (minDist == distToLeft) {
                collisionNormal = glm::vec3(-1.0f, 0.0f, 0.0f); // ���淨����
            }
            else if (minDist == distToRight) {
                collisionNormal = glm::vec3(1.0f, 0.0f, 0.0f);  // ���淨����
            }
            else if (minDist == distToBottom) {
                collisionNormal = glm::vec3(0.0f, -1.0f, 0.0f); // ���淨����
            }
            else if (minDist == distToTop) {
                collisionNormal = glm::vec3(0.0f, 1.0f, 0.0f);  // ���淨����
            }
            else if (minDist == distToBack) {
                collisionNormal = glm::vec3(0.0f, 0.0f, -1.0f); // ���淨����
            }
            else if (minDist == distToFront) {
                collisionNormal = glm::vec3(0.0f, 0.0f, 1.0f);  // ǰ�淨����
            }

            // ���������������ٶȣ�������ײ���������ٶȣ�
            float relativeVelocity = glm::dot(_collisionProperties.velocity - other->velocity, collisionNormal);


            // �������ٶ�С���㣬������ײ
            if (relativeVelocity < 0) {
               
                std::cout << "������ײ" << std::endl;

                // ����ϵ�����ָ�ϵ����
                float e = 0.8f;  // ���赯��ϵ��Ϊ0.8�����ֵ�����ײ

                // �������
                float impulse = -(1 + e) * relativeVelocity /
                    (1 / _collisionProperties.mass + 1 / other->mass);

                // ���µ�ǰ������ٶ�
                glm::vec3 newVelocity = _collisionProperties.velocity + impulse * collisionNormal / _collisionProperties.mass;

                // ���ø��º���ٶ�
                _collisionProperties.velocity = newVelocity;

            }
        }
    }

    // �� AABB ��ײ���
    bool CollisionBody::CheckCollisionWithAABB(CollisionProperties* other) {
        // ��� AABB ��ײ
        glm::vec3 otherMin = other->_collisionMin;
        glm::vec3 otherMax = other->_collisionMax;

        if (!_collisionProperties.isCollision)
        {
            if ((_collisionProperties.position.x > otherMin.x && _collisionProperties.position.x < otherMax.x) &&
                (_collisionProperties.position.y > otherMin.y && _collisionProperties.position.y < otherMax.y) &&
                (_collisionProperties.position.z > otherMin.z && _collisionProperties.position.z < otherMax.z))

            {

                std::cout << _collisionProperties.ID << " ��ײ��" << other->ID << std::endl;
                //��ײ֮�������ײ����
                ResolveCollision(other);

               _collisionProperties.isCollision = true;
                return true;  // ��ײ����
            }

           
        }
        else
        {

            if (!((_collisionProperties.position.x > otherMin.x && _collisionProperties.position.x < otherMax.x) &&
                (_collisionProperties.position.y > otherMin.y && _collisionProperties.position.y < otherMax.y) &&
                (_collisionProperties.position.z > otherMin.z && _collisionProperties.position.z < otherMax.z)))

            {

                std::cout << _collisionProperties.ID << " ��ײ����" << other->ID << std::endl;
                _collisionProperties.isCollision = false;
                return false;
             }

        }
        return true;

    }


}
