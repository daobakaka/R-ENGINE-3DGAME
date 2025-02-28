#include "PhysicalEngine.h"

namespace Game {

    // 构造函数，初始化物理引擎的基本参数
    PhysicalEngine::PhysicalEngine(glm::vec3 &positon,bool staticObj)
        : _mass(1.0f), _friction(0.1f), _velocity(0.0f), _acceleration(0.0f), _gravity(0, -9.8f, 0) ,_position(positon){
        _ifStatic = staticObj;
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
    /// <summary>
    /// 依次传入参数
    /// </summary>
    /// <param name="position"></param>
    /// <param name="mass"></param>
    /// <param name="friction"></param>
    /// <param name="velocity"></param>
    /// <param name="acceleration"></param>
    void PhysicalEngine::SetParameters( float mass, float friction,  glm::vec3 velocity,  glm::vec3 acceleration)
    {
    
        _mass = mass;
        _friction = friction;
        _velocity = velocity;
        //设置初始加速度
        _acceleration = acceleration;
    }

    // 更新物理状态，计算位置、速度、加速度（考虑外力和摩擦力）
    void PhysicalEngine::UpdatePhysics(float deltaTime) {
          
        if (!_ifStatic)
        {
            // 计算摩擦加速度（假设摩擦加速度与速度平方成反比）,这里应该在产生碰撞的情况下计算静摩擦力
            //if (glm::dot(_velocity, _velocity) > 0.001f) {  // 如果速度的平方大于一个小阈值
            //    glm::vec3 frictionAcceleration = -_friction * glm::normalize(_velocity);  // 速度平方反比
            //    _acceleration += frictionAcceleration;
            //}
            //else {
            //    _velocity = glm::vec3(0.0f);  // 停止物体
            //}

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
        
        std::cout << "acc:" << _acceleration.y << std::endl;
        return _acceleration;
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

    bool PhysicalEngine::Interface() {
        // 在此处实现接口方法，可以根据需求返回一些状态
        return true;
    }

}  // namespace Game
