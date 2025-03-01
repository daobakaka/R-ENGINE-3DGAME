#include "PhysicalEngine.h"

namespace Game {

    // ���캯������ʼ����������Ļ�������
    PhysicalEngine::PhysicalEngine(glm::vec3 &positon,glm::quat &rotation,bool staticObj)
        : _mass(1.0f), _friction(0.1f), _velocity(0.0f), _acceleration(0.0f), _gravity(0, -9.8f, 0) ,_position(positon),_rotation(rotation){
        _ifStatic = staticObj;
    }

    // ��������
    void PhysicalEngine::SetMass(float m) {
       _mass = m;
    }

    // ����Ħ��ϵ��
    void PhysicalEngine::SetFriction(float f) {
        _friction = f;
    }

    // �����ٶ�
    void PhysicalEngine::SetVelocity( glm::vec3 v) {
        _velocity = v;
    }

    // ���ü��ٶ�
    void PhysicalEngine::SetAcceleration( glm::vec3 a) {
        _acceleration = a;
    }
    /// <summary>
    /// ���δ������
    /// </summary>
    /// <param name="position"></param>
    /// <param name="mass"></param>
    /// <param name="friction"></param>
    /// <param name="velocity"></param>
    /// <param name="acceleration"></param>
    void PhysicalEngine::SetParameters( float mass, float friction,  glm::vec3 velocity,  glm::vec3 acceleration, float elasticity )
    {
    
        _mass = mass;
        _friction = friction;
        _velocity = velocity;
        //���ó�ʼ���ٶȣ�������������������ٶ�
        _acceleration = acceleration;
        _elasticity = elasticity;

    }

    void PhysicalEngine::UpdatePhysics(float deltaTime)
    {
        if (!_ifStatic)
        {

            if (glm::dot(_velocity, _velocity) < 0.01f)
            {
                _velocity = glm::vec3(0.0f);
            }

            // �����ٶȣ�v = v0 + a * t
            _velocity += _acceleration * deltaTime;

            // ����λ�ã�s = s0 + v * t
            _position += _velocity * deltaTime;
        }


    }



    // ��ȡ����λ��
    glm::vec3 &PhysicalEngine::GetPosition()  {
        return _position;
    }

    // ��ȡ�����ٶ�
    glm::vec3 &PhysicalEngine::GetVelocity()  {
        return _velocity;
    }

    // ��ȡ������ٶ�
    glm::vec3 &PhysicalEngine::GetAcceleration() {
        
        return _acceleration;
    }

    glm::quat& PhysicalEngine::GetRotation()
    {
        return _rotation;
    }

    float PhysicalEngine::GetMass() const
    {
        return _mass;
    }

    float PhysicalEngine::GetFriction() const
    {
        return _friction;
    }

    bool PhysicalEngine::GetStatic() const
    {
        return _ifStatic;
    }

    float PhysicalEngine::GetElasticity() const
    {
        return _elasticity;
    }

    bool PhysicalEngine::Interface() {
        // �ڴ˴�ʵ�ֽӿڷ��������Ը������󷵻�һЩ״̬
        return true;
    }

}  // namespace Game
