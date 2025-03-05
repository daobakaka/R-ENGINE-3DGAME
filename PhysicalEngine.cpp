#include "PhysicalEngine.h"
#include "CollisionBody.h"

namespace Game {

    // ���캯������ʼ����������Ļ�������
    PhysicalEngine::PhysicalEngine(glm::vec3& positon, glm::quat& rotation, glm::vec3& scale, bool& isActive ,int id, float mass, float friction,
        glm::vec3 velocity, glm::vec3 acceleration, float elasticity, bool lockXZAxi, float rotationDampping,
        bool trigger, int layer, float rotationAdjust, bool staticObj)
        : _mass(mass), _friction(friction), _velocity(velocity), _acceleration(acceleration), _gravity(0, -9.8f, 0), _position(positon), _rotation(rotation),_scale(scale)
        ,_elasticity(elasticity),_lockXZAxi(lockXZAxi),_rotationDamping(rotationDampping),_isActive(isActive),_trigger(trigger),_layer(layer),
        _rotationAdjust(rotationAdjust)
    {
            _ifStatic = staticObj;
            ID = id;
    }

    PhysicalEngine::~PhysicalEngine()
    {
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
    void PhysicalEngine::SetElasticity(float e)
    {
        _elasticity = e;
    }
    /// <summary>
    /// ���δ������
    /// </summary>
    /// <param name="position"></param>
    /// <param name="mass"></param>
    /// <param name="friction"></param>
    /// <param name="velocity"></param>
    /// <param name="acceleration"></param>


    void PhysicalEngine::UpdatePhysics(float deltaTime)
    {//�Ǿ�̬���壬�Ҵ��ڼ���״̬
        if (!_ifStatic&&_isActive)
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

    glm::vec3& PhysicalEngine::GetScale()
    {
        return _scale;
    }

    float &PhysicalEngine::GetMass() 
    {
        return _mass;
    }

    float &PhysicalEngine::GetFriction() 
    {
        return _friction;
    }

    bool &PhysicalEngine::GetStatic() 
    {
        return _ifStatic;
    }

    float &PhysicalEngine::GetElasticity() 
    {
        return _elasticity;
    }

    bool &PhysicalEngine::GetLockState() 
    {
        return _lockXZAxi;
    }

    float &PhysicalEngine::GetDamping() 
    {
        return _rotationDamping;
    }

    bool &PhysicalEngine::GetActive() 
    {
        return _isActive;
    }

    bool &PhysicalEngine::GetTrigger() 
    {
        return _trigger;
    }

    int &PhysicalEngine::GetLayer() 
    {
        return _layer;
    }

    float &PhysicalEngine::GetRotationAdjust() 
    {
        return _rotationAdjust;
    }

    bool PhysicalEngine::Interface() {
        // �ڴ˴�ʵ�ֽӿڷ��������Ը������󷵻�һЩ״̬
        return true;
    }

    void PhysicalEngine::SetRotationDamping(float damp)
    {

        _rotationDamping = damp;
    }

    void PhysicalEngine::SetActive(bool active)
    {
        _isActive = active;
    }

    void PhysicalEngine::SetTrigger(bool trigger)
    {
        _trigger = trigger;
    }

    void PhysicalEngine::SetLayer(int layer)
    {
        _layer = layer;
    }

    void PhysicalEngine::SetRotationAdjust(float r)
    {

        _rotationAdjust = r;
    }

    bool PhysicalEngine::SetFixedAxisX(bool lock)
    {
        return _lockXZAxi=lock;
    }

}  // namespace Game
