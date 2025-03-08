#include "CollisionBody.h"
#include <iostream>
#include "PhysicalEngine.h"
#include "Octree.h"

namespace Game {

    //�ⲿ������ʶ
    std::unordered_map<int, CollisionProperties*> CollisionProps;


    CollisionBody::CollisionBody(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, glm::quat& rot, glm::vec3& ratio, int id, float& mass, float& friction, float& elasticity,
        int& layer, bool& trigger, bool& active, float& rotationDamping, bool& lockXZAxi, float &rotationAdjust,
         bool ifStatic, CollisionType collider, float radius,SpecialType type, ModelClass logicType)
        :_collisionProperties(pos, vel, acc,rot,ratio,active,rotationDamping,lockXZAxi,mass,friction,elasticity,layer,trigger, rotationAdjust)//��ʼ���ṹ������
    {       
        //��Щ�����ڳ�ʼ��ʱ��ȷ�ϣ������ʼ���Ĳ���������֮���ܸı䣬������
  
        _collisionProperties.ID = id;//����ʹ����ײ�ṹ�崢����صĲ�������ID����ײ�����Ҳֱ����ṹ����п��ٽ���
        _collisionProperties.staticObj = ifStatic;
        _collisionProperties.radius = radius;
        _collisionProperties.collider = collider;
        _collisionProperties.sType = type;

        //��ʼ���������ٶȣ���������ײ�ṹ���У����ڼ���������
        _collisionProperties.logicType = logicType;
        _collisionProperties.gravity = _collisionProperties.acceleration;
        _collisionProperties.timer = 0;//��ʼ����ײ��ʱ�����
        _collisionProperties.gravityEnabled = true;//Ĭ�Ͼ������������������Բ�����ʼ���ٶ���Ӱ������������Ŀ����ر�ֻ�����������Ĵ������
        _collisionProperties.angularVelocity = glm::vec3(0);//Ĭ����ת�ٶ�
        if (_collisionProperties.elasticity>1)
        {
           _collisionProperties. elasticity = 1;
        }
        if (ifStatic)
            _collisionProperties.mass = 100000000;//���þ�̬���������Ϊ��        

        //������ײ����ע��ȫ�ַ���ģ��,�����ʼ���˲���ʵ����ע�����˲���
        RegisterCollisionBody();

    }

    // ��������
    CollisionBody::~CollisionBody()
    {
        //�Ƴ��˲����еĶ�Ӧ�ṹ
        _octree->Remove(&_collisionProperties);

        if (_octree != nullptr)
        {
            delete _octree;
        }
        //�Ƴ������еĶ�ӦID����
        CollisionProps.erase(_collisionProperties.ID);
    }

    bool CollisionBody::Interface()
    {
        return false;
    }

    // ����������ٶȺ�λ��
    void CollisionBody::UpdateCollisionState(std::unordered_map<int, CollisionProperties*>& cop,float deltaTime) {
        
        //������ײ��ⷶΧ�ſ�����ײ�߼��������Ƴ���ײ���ڰ˲����е�λ�ã���ȡ��������ײ�����߼����ϲ�������㣩
        if (CheckValidCollision())
        { 
            _collisionProperties.timer += deltaTime;
            //���¶���״̬        
            UpdateCollisionParameters();
            //�����ײ�ṹ���ڲ�����
            _collisionProperties.collidingBodies.clear();
            //�˲����߼����
            if (true)
            {
                _potentialCollisions.clear();
                //����ѭ���������⹹���壬������壬�ذ壬�������ǣ����ڳ����Լ��
                //������Է�װ��ר�ŵķ������������ʶ���Ա㵥��������ײ�߼�
                UpdateSpecialCollider(cop);
                _octree->Query(&_collisionProperties, _potentialCollisions); // �˲�����ѯ

                // ��ӡ��ǰ����� ID
               // std::cout << "Current object ID: " << _collisionProperties.ID << "\n";

                for (auto other : _potentialCollisions) {
                    if (_collisionProperties.ID != other->ID) {

                        CheckCollisionWithAABB(other);
                        // std::cout << "  Object ID: " << other->ID << "\n";
                    }
                }
            }


            //������ײ״̬
            _collisionProperties.isCollision = !_collisionProperties.collidingBodies.empty();
            //��ײ��ĺ���������㣬�ŵ���ײ����
            UpdatePhysicsCollision();
        }
        else
        {
            _octree->Remove(&_collisionProperties);
        }
      
      




        // �������ӡ��ײ��Ĳ���
     // std::cout << "Collision Body ID: " << _collisionProperties.ID << "\n";
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

        // ��ӡ���ٶ�
        //std::cout << "Angular Velocity: (" << _collisionProperties.angularVelocity.x << ", "
        //    << _collisionProperties.angularVelocity.y << ", "
        //    << _collisionProperties.angularVelocity.z << ")\n";

        //// ��ӡ��Ԫ����ת
        //std::cout << "Rotation (Quaternion): (" << _collisionProperties.rotation.w << ", "
        //    << _collisionProperties.rotation.x << ", "
        //    << _collisionProperties.rotation.y << ", "
        //    << _collisionProperties.rotation.z << ")\n";


    }



    void CollisionBody::SetRadius(float r)
    {
        _collisionProperties.radius = r;
    }

    void CollisionBody::SetLayer(int layer)
    {
        _collisionProperties.layer = layer;
    }

    void CollisionBody::SetTrigger(bool trigger)
    {
        _collisionProperties.trigger = trigger;
    }
    void CollisionBody::SetGameProperties(float health, float damage,float speed)
    {
       _collisionProperties.gameProperties.health = health;
       _collisionProperties.gameProperties.damage = damage;
       _collisionProperties.gameProperties.speed = speed;

    }
    /// <summary>
    ///ֻ��������һ���������ߴ���active״̬��ʱ��Ż����
    /// </summary>
    /// <param name="range"></param>
    /// <returns></returns>
    bool CollisionBody::CheckValidCollision(glm::vec3 range)
    {
        
        if (glm::abs(_collisionProperties.position.x)<=range.x&&
            glm::abs(_collisionProperties.position.y)<=range.y&&
            glm::abs(_collisionProperties.position.z)<=range.z&&_collisionProperties.isActive==true)
        {
            return true;
        }

       
        return false;
    }

    void CollisionBody::RegisterCollisionBody()
    {
        //���ﾲ̬��������ʱ����ר�ŵ��߼��������,����ҵȣ� �������Ա��ֳ����Լ��
        if (_collisionProperties.sType != SpecialType::OriginalT)
        {
            CollisionProps[_collisionProperties.ID] = &_collisionProperties;
        }
      
            //��ȡ�˲����ṹ����
            _collisionProperties.currentNode = nullptr;//Ϊ��ײ����ڰ˲�����ָ�븳�յĳ�ʼֵ
            _octree = Octree::GetInstance();
            _octree->Insert(&_collisionProperties);
        
    }

    void CollisionBody::UpdateCollisionParameters()
    {
        switch (_collisionProperties.collider)
        {
        case CollisionType:: Box:
            _collisionProperties._collisionMin.x = _collisionProperties.position.x - _collisionProperties.radius * _collisionProperties.ratio.x;
            _collisionProperties._collisionMin.y = _collisionProperties.position.y - _collisionProperties.radius * _collisionProperties.ratio.y;
            _collisionProperties._collisionMin.z = _collisionProperties.position.z - _collisionProperties.radius * _collisionProperties.ratio.z;
            _collisionProperties._collisionMax.x = _collisionProperties.position.x + _collisionProperties.radius * _collisionProperties.ratio.x;
            _collisionProperties._collisionMax.y = _collisionProperties.position.y + _collisionProperties.radius * _collisionProperties.ratio.y;
            _collisionProperties._collisionMax.z = _collisionProperties.position.z + _collisionProperties.radius * _collisionProperties.ratio.z;
      
         //��������ײ��
            //if (_collisionProperties.sType == SpecailType::OriginalT)
                _octree->Update(&_collisionProperties); // ���°˲����е�λ��
            
            break;
        case CollisionType::Sphere:
            _collisionProperties._collisionMin.x = _collisionProperties.position.x - _collisionProperties.radius * _collisionProperties.ratio.x;
            _collisionProperties._collisionMin.y = _collisionProperties.position.y - _collisionProperties.radius * _collisionProperties.ratio.y;
            _collisionProperties._collisionMin.z = _collisionProperties.position.z - _collisionProperties.radius * _collisionProperties.ratio.z;
            _collisionProperties._collisionMax.x = _collisionProperties.position.x + _collisionProperties.radius * _collisionProperties.ratio.x;
            _collisionProperties._collisionMax.y = _collisionProperties.position.y + _collisionProperties.radius * _collisionProperties.ratio.y;
            _collisionProperties._collisionMax.z = _collisionProperties.position.z + _collisionProperties.radius * _collisionProperties.ratio.z;
         //��������ײ��   
            //if (_collisionProperties.sType == SpecailType::OriginalT)
                _octree->Update(&_collisionProperties); // ���°˲����е�λ��
            
            break;
        default:
            break;
        }


    }

    void CollisionBody::ResolveCollision(CollisionProperties* other) {
        
      //�ж���ײ�岻Ϊ����������������ײ�ṹ��Ϊ��̬,�������ײ����
        if (!_collisionProperties.staticObj) {
          // std::cout << "�ⲿ������ײ" <<std::endl;
            //������ײ������
            auto collisionNormal = CalculateCollisionNormal(other);
            //�������������е���Ħ��ϵ��ֵ�����ں��ڼ���
            _collisionProperties.otherFriction = other->friction;
            // ���������������ٶȣ�������ײ���������ٶȣ�
            float relativeVelocity = glm::dot(_collisionProperties.velocity - other->velocity, collisionNormal);


            // �������ٶȴ���0��������ײ
            if (relativeVelocity >1.05- _collisionProperties.elasticity) {
               
              // std::cout << _collisionProperties.ID<<"�ڲ�������ײ" << std::endl;
                //�����Ǿ�̬��trigger��ײ����
                CalculateGameParameters(other);
                //���Ϊ���������򲻲����������,����Ϊ������Ҳ��Ӧ�ò������Եȼ���
                if (!other->trigger&&!_collisionProperties.trigger)
                {    // ����ϵ�����ָ�ϵ����
                    float e = _collisionProperties.elasticity;  //��������ײģ���ж�ȡ����ϵ��

                    // �������
                    float impulse = -(1 + e) * relativeVelocity /
                        (1 / _collisionProperties.mass + 1 / other->mass);

                    // ���µ�ǰ������ٶ�
                    glm::vec3 newVelocity = _collisionProperties.velocity + impulse * collisionNormal / _collisionProperties.mass;

                    // ���ø��º���ٶ�
                    _collisionProperties.velocity = newVelocity;



                    // ������ײ�㵽���ĵ�����
                    glm::vec3 r = _collisionProperties.position - other->position;

                    // ����Ť��
                    glm::vec3 torque = glm::cross(r, impulse * collisionNormal);

                    // ��ʱ������ٶ�,��һ������ϵ��rotationPar --0.25f
                    _collisionProperties.angularVelocity = _collisionProperties.rotationAdjust * r * torque * (1 - _collisionProperties.rotationDamping) / _collisionProperties.mass; // ����ת������Ϊ����
                }
            }

        }
    }

    glm::vec3 CollisionBody::CalculateCollisionNormal(CollisionProperties* other) {
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

        // ����ÿ�����Ȩ�أ�����ԽС��Ȩ��Խ��
        float weightLeft = 1.0f / (distToLeft + 1e-5f);   // �������
        float weightRight = 1.0f / (distToRight + 1e-5f);
        float weightBottom = 1.0f / (distToBottom + 1e-5f);
        float weightTop = 1.0f / (distToTop + 1e-5f);
        float weightBack = 1.0f / (distToBack + 1e-5f);
        float weightFront = 1.0f / (distToFront + 1e-5f);

        // �����Ȩƽ���ķ�����
        glm::vec3 collisionNormal =
            (weightLeft * glm::vec3(-1.0f, 0.0f, 0.0f) +  // ���淨����
                (weightRight * glm::vec3(1.0f, 0.0f, 0.0f)) + // ���淨����
                (weightBottom * glm::vec3(0.0f, -1.0f, 0.0f)) + // ���淨����
                (weightTop * glm::vec3(0.0f, 1.0f, 0.0f)) +   // ���淨����
                (weightBack * glm::vec3(0.0f, 0.0f, -1.0f)) + // ���淨����
                (weightFront * glm::vec3(0.0f, 0.0f, 1.0f)));  // ǰ�淨����

        // ��һ��������
        collisionNormal = glm::normalize(collisionNormal);

        return collisionNormal;
    }

    void CollisionBody::UpdateGravityState(float normalY)
    {

    }

    void CollisionBody::UpdatePhysicsCollision(float deltaTime)
    {
        //�Ǿ�̬����Ž����������
        if (!_collisionProperties.staticObj)
        {

            // �����ٶȣ�v = v0 + a * t
            _collisionProperties.velocity += _collisionProperties.acceleration * deltaTime;
            //�ٶȽ���
            if (glm::dot(_collisionProperties.velocity, _collisionProperties.velocity) < 0.01f)
            {
                _collisionProperties.velocity = glm::vec3(0.0f);
            }
            //�������Ǵ��������Ž���������ײ����
            if (!_collisionProperties.trigger)
            {


                //����Ħ��ϵ���������˶���Ӱ��
                if (_collisionProperties.isCollision)
                {
                    float totalFriction = (_collisionProperties.friction + _collisionProperties.otherFriction) / 1.5f;

                    // ����Ħ�����ٶ�
                    glm::vec3 frictionAcceleration = totalFriction * _collisionProperties.gravity.y * glm::normalize(glm::vec3(_collisionProperties.velocity.x, 0, _collisionProperties.velocity.z));
                    //��ˮƽ�ٶȴ���ĳ��ֵʱ
                    if (glm::dot(glm::vec2(_collisionProperties.velocity.x, _collisionProperties.velocity.z), glm::vec2(_collisionProperties.velocity.x, _collisionProperties.velocity.z)) > 0.01f)
                    {  // ����������ٶ�
                        _collisionProperties.velocity += frictionAcceleration * deltaTime;
                    }
                    else
                    {
                        _collisionProperties.velocity.x = 0;
                        _collisionProperties.velocity.z = 0;
                    }
                }
                // ������ת�Ƕȣ���Ԫ����
                float  angularSpeedSquared = 0;
                if (_collisionProperties.lockXZAxi) {
                    _collisionProperties.angularVelocity.x = 0.0f; // ���� X ��
                    _collisionProperties.angularVelocity.z = 0.0f; // ���� Z ��
                    angularSpeedSquared = glm::dot(_collisionProperties.angularVelocity, _collisionProperties.angularVelocity); // ���¼�����ٶȵ�ƽ������
                }
                else
                {
                    // ������ת�Ƕȣ���Ԫ����
                    angularSpeedSquared = glm::dot(_collisionProperties.angularVelocity, _collisionProperties.angularVelocity);
                }

                // ������ٶ���Ȼ�㹻���������ת
                if (angularSpeedSquared > 0.01f) {


                    // Ӧ����ת���᣺ÿ֡������ϵ��˥�����ٶ�
                    _collisionProperties.angularVelocity *= (1.0f - _collisionProperties.rotationDamping * (1 - _collisionProperties.rotationAdjust) * (1 + _collisionProperties.rotationAdjust));
                    // ������ת�Ƕ�
                    float angle = glm::sqrt(angularSpeedSquared) * deltaTime;

                    // ������ת��
                    glm::vec3 axis = glm::normalize(_collisionProperties.angularVelocity); // ȷ����ת���һ��

                    // ������ת��������Ԫ����
                    glm::quat deltaRotation = glm::angleAxis(angle, axis);

                    // ������ת
                    _collisionProperties.rotation = deltaRotation * _collisionProperties.rotation;

                    // ��һ����Ԫ������ֹ����
                    _collisionProperties.rotation = glm::normalize(_collisionProperties.rotation);
                }
                else {
                    // ���ٶȹ�С��ֹͣ��ת
                    _collisionProperties.angularVelocity = glm::vec3(0.0f);
                }
                //��ֹ�ذ崩ģ.���һ���µذ��жϣ���ֹ����ذ���ײ
                if (_collisionProperties.isCollision&&_collisionProperties.isOnGround)
                {
                    if (_collisionProperties.velocity.y < 0 && _collisionProperties.position.y <= _collisionProperties.ratio.y)
                    {
                        _collisionProperties.position.y = _collisionProperties.ratio.y;
                        _collisionProperties.velocity.y = 0;
                    }
                }

            }

            // ����λ�ã�s = s0 + v * t
            _collisionProperties.position += _collisionProperties.velocity * deltaTime;
            
   
        }
    }

    void CollisionBody::UpdateSpecialCollider(std::unordered_map<int, CollisionProperties*>& cop, float deltaTime)
    {

        for (auto& pair : cop)
        {
            switch (pair.second->sType)
            {
            case SpecialType::BasePlane:
                if (_collisionProperties.position.y > -10)
                {
                    _potentialCollisions.push_back(pair.second);

                }
                break;

           
                //���ǿ��ײ
            case SpecialType::SPlayer:               
                    _potentialCollisions.push_back(pair.second);
                 
               break;



            default:
                break;


            }




        }

    }



    

    // �� AABB ��ײ���
    bool CollisionBody::CheckCollisionWithAABB(CollisionProperties* other) {
        // ��ȡ��ǰ����ͶԷ������ AABB �߽�
        glm::vec3 thisMin = _collisionProperties._collisionMin;
        glm::vec3 thisMax = _collisionProperties._collisionMax;
        glm::vec3 otherMin = other->_collisionMin;
        glm::vec3 otherMax = other->_collisionMax;

        // ������� AABB �Ƿ��ཻ
        bool isColliding = (thisMin.x <= otherMax.x && thisMax.x >= otherMin.x) &&
            (thisMin.y <= otherMax.y && thisMax.y >= otherMin.y) &&
            (thisMin.z <= otherMax.z && thisMax.z >= otherMin.z);

        if (isColliding) {
            // �����ǰ��ײ�岻����ײ�����У�����Ӳ�������ײ
            if (_collisionProperties.collidingBodies.find(other->ID) == _collisionProperties.collidingBodies.end()) {
               // std::cout << _collisionProperties.ID << " ��ײ�� " << other->ID << std::endl;
                _collisionProperties.collidingBodies.insert(other->ID); // ��ӵ���ײ����
                if (other->sType == SpecialType::BasePlane)
                {
                    _collisionProperties.isOnGround = true;
                }
                ResolveCollision(other); // ������ײ
            }
            return true; // ��ײ����
        }
        else {
            // �����ǰ��ײ������ײ�����У����Ƴ��������ײ����
            if (_collisionProperties.collidingBodies.find(other->ID) != _collisionProperties.collidingBodies.end()) {
                std::cout << _collisionProperties.ID << " ��ײ���� " << other->ID << std::endl;
                if (other->sType == SpecialType::BasePlane)
                {
                    _collisionProperties.isOnGround = false;
                }

                _collisionProperties.collidingBodies.erase(other->ID); // ����ײ�����Ƴ�
            }
            return false; // ��ײδ����
        }
    }

    void CollisionBody::CalculateGameParameters(CollisionProperties* other)
    {
       
        if (other->logicType==ModelClass::TestPhysics&&_collisionProperties.logicType!=ModelClass::TestPhysics)
        {
            _collisionProperties.gameProperties.health -= other->gameProperties.damage;
           std::cout << _collisionProperties.ID << "����ֵΪ��" << _collisionProperties.gameProperties.health << std::endl;
        }


    

    
    }


}
