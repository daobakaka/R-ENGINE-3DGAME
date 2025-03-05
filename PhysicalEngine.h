#ifndef PHYSICAL_ENGINE_H
#define PHYSICAL_ENGINE_H

#include "GameObject.h"
#include <glm/glm.hpp>

namespace Game {


    class CollisionBody;
    class PhysicalEngine : public IComponent<PhysicalEngine> {
    public:
        PhysicalEngine(glm::vec3& positon,glm::quat &rotation,glm::vec3 &scale, bool& isActive,int id, float mass, float friction, glm::vec3 velocity=glm::vec3(0), glm::vec3 acceleration=glm::vec3(0,-9.8f,0),
            float elasticity=0.5f,bool lockXZAxi=true, float rotationDampping=0.15f, bool trigger=false, int layer=1,float rotationAdjust=0.382f, bool staticObj = false
            );  // ���캯��
        ~PhysicalEngine();

        // �������������״̬������λ�á��ٶȡ����ٶȣ�
        void UpdatePhysics(float deltaTime = 0.0167f);
        // ������������Ļ�������
        void SetMass(float mass);
        void SetFriction(float friction);
        void SetVelocity( glm::vec3 velocity);
        void SetAcceleration( glm::vec3 acceleration);
        void SetElasticity(float e);
        bool SetFixedAxisX(bool lock = true);
        void SetRotationDamping(float damp = 0.15f);
       
        void SetActive(bool active = true);
        void SetTrigger(bool trigger = false);
        void SetLayer(int layer = 1);
        void SetRotationAdjust(float r = 0.382f);

        // ��ȡ��ǰ���������״̬const�ؿ�Χ������get������ʶ
        glm::vec3 &GetPosition() ;
        glm::vec3 &GetVelocity() ;
        glm::vec3 &GetAcceleration() ;
        glm::quat& GetRotation();
        glm::vec3& GetScale();
        float& GetMass() ;
        float &GetFriction() ;
        bool &GetStatic();
        float& GetElasticity();
        bool &GetLockState() ;
        float &GetDamping() ;

        bool  &GetActive();
        bool &GetTrigger() ;
        int &GetLayer() ;
        float &GetRotationAdjust() ;

            
            
    private:
        bool Interface() override;

    private:
        float _mass;                  // ����
        float _friction;              // Ħ��ϵ��
        glm::vec3 &_position;          // ����λ��,�������ͱ���ͨ�����캯����ʼ���������ڲ��Ϳ���ֱ���޸�
        glm::vec3 _velocity;          // �����ٶ�
        glm::vec3 _acceleration;      // ������ٶ�
        glm::quat &_rotation;        //������ת�߼�
        glm::vec3 &_scale;        //�����С
        glm::vec3 _gravity;        //�����������ٶ�
        bool _ifStatic;      //�Ƿ�Ϊ��̬���壬����������ɸ��ģ���̬��������������ײ���㣬���ǿɱ�������������
        float _elasticity;//���嵯��ϵ��
        int ID;
        bool _lockXZAxi;
        float _rotationDamping;
        bool &_isActive;//����������ⲿ����,��������Ҫ���������������ײ�������Ҫ������Ⱦ�߼�
        bool _trigger;
        int _layer;
        float _rotationAdjust;//Ĭ�ϻƽ�ָ�


    };

}  // namespace Game

#endif // !PHYSICAL_ENGINE_H
