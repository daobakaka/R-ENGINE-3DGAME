#ifndef PHYSICAL_ENGINE_H
#define PHYSICAL_ENGINE_H

#include "GameObject.h"
#include <glm/glm.hpp>

namespace Game {


    class CollisionBody;
    class PhysicalEngine : public IComponent<PhysicalEngine> {
    public:
        PhysicalEngine(glm::vec3& positon,glm::quat &rotation,bool staticObj=false);  // ���캯��

        // �������������״̬������λ�á��ٶȡ����ٶȣ�
        void UpdatePhysics(float deltaTime = 0.0167f);
        // ������������Ļ�������
        void SetMass(float mass);
        void SetFriction(float friction);
        void SetVelocity( glm::vec3 velocity);
        void SetAcceleration( glm::vec3 acceleration);

        void SetParameters( float mass, float friction,  glm::vec3 velocity,  glm::vec3 acceleration, float elasticity);
        

        // ��ȡ��ǰ���������״̬const�ؿ�Χ������get������ʶ
        glm::vec3 &GetPosition() ;
        glm::vec3 &GetVelocity() ;
        glm::vec3 &GetAcceleration() ;
        glm::quat& GetRotation();
        float GetMass() const;
        float GetFriction() const;
        bool GetStatic()const;
        float GetElasticity()const;

            
            
    private:
        bool Interface() override;
        //������ת���ᣬĬ��0.15f
        void SetRotationDamping(float damp=0.15f);
        //����Ĭ����ת������Ĭ����������X��Z;
        bool SetFixedAxisX(bool lock = true);

        bool GetLockState() const;
        float GetDampping() const;

    private:
        float _mass;                  // ����
        float _friction;              // Ħ��ϵ��
        glm::vec3 &_position;          // ����λ��,�������ͱ���ͨ�����캯����ʼ���������ڲ��Ϳ���ֱ���޸�
        glm::vec3 _velocity;          // �����ٶ�
        glm::vec3 _acceleration;      // ������ٶ�
        glm::quat &_rotation;        //������ת�߼�
        glm::vec3 _gravity;        //�����������ٶ�
        bool _ifStatic;      //�Ƿ�Ϊ��̬����
        float _elasticity;//���嵯��ϵ��

        bool _lockxz;
        float _rotationDampping;

    };

}  // namespace Game

#endif // !PHYSICAL_ENGINE_H
