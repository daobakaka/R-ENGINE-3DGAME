#include "PhysicalEngine.h"
#include "CollisionBody.h"

namespace Game {

    // 构造函数，初始化物理引擎的基本参数
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

    // 设置质量
    void PhysicalEngine::SetMass(float m) {
       _mass = m;
    }

    // 设置摩擦系数
    void PhysicalEngine::SetFriction(float f) {
        _friction = f;
    }

    // 设置速度
    void PhysicalEngine::SetVelocity( glm::vec3 v) {
        _velocity = v;
    }

    // 设置加速度
    void PhysicalEngine::SetAcceleration( glm::vec3 a) {
        _acceleration = a;
    }
    void PhysicalEngine::SetElasticity(float e)
    {
        _elasticity = e;
    }
    /// <summary>
    /// 依次传入参数
    /// </summary>
    /// <param name="position"></param>
    /// <param name="mass"></param>
    /// <param name="friction"></param>
    /// <param name="velocity"></param>
    /// <param name="acceleration"></param>


    void PhysicalEngine::UpdatePhysics(float deltaTime)
    {//非静态物体，且处于激活状态
        if (!_ifStatic&&_isActive)
        {

            if (glm::dot(_velocity, _velocity) < 0.01f)
            {
                _velocity = glm::vec3(0.0f);
            }

            // 更新速度：v = v0 + a * t
            _velocity += _acceleration * deltaTime;

            // 更新位置：s = s0 + v * t
            _position += _velocity * deltaTime;
        }


    }



    // 获取物体位置
    glm::vec3 &PhysicalEngine::GetPosition()  {
        return _position;
    }

    // 获取物体速度
    glm::vec3 &PhysicalEngine::GetVelocity()  {
        return _velocity;
    }

    // 获取物体加速度
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
        // 在此处实现接口方法，可以根据需求返回一些状态
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
