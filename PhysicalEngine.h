#ifndef PHYSICAL_ENGINE_H
#define PHYSICAL_ENGINE_H

#include "GameObject.h"
#include <glm/glm.hpp>

namespace Game {

    class PhysicalEngine : public IComponent<PhysicalEngine> {
    public:
        PhysicalEngine(glm::vec3& positon,bool staticObj=false);  // ���캯��

        // ������������Ļ�������
        void SetMass(float mass);
        void SetFriction(float friction);
        void SetVelocity( glm::vec3 velocity);
        void SetAcceleration( glm::vec3 acceleration);

        void SetParameters( float mass, float friction,  glm::vec3 velocity,  glm::vec3 acceleration);

        // �������������״̬������λ�á��ٶȡ����ٶȣ�
        void UpdatePhysics(float deltaTime=0.0167f);
        

        // ��ȡ��ǰ���������״̬const�ؿ�Χ������get������ʶ
        glm::vec3 &GetPosition() ;
        glm::vec3 &GetVelocity() ;
        glm::vec3 &GetAcceleration() ;
        float GetMass() const;
        float GetFriction() const;
        bool GetStatic()const;

        bool Interface() override;

    private:
        float _mass;                  // ����
        float _friction;              // Ħ��ϵ��
        glm::vec3 &_position;          // ����λ��,�������ͱ���ͨ�����캯����ʼ���������ڲ��Ϳ���ֱ���޸�
        glm::vec3 _velocity;          // �����ٶ�
        glm::vec3 _acceleration;      // ������ٶ�
        glm::vec3 _gravity;        //�����������ٶ�
        bool _ifStatic;
    };

}  // namespace Game

#endif // !PHYSICAL_ENGINE_H
