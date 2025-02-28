#include "CollisionBody.h"
#include <iostream>
#include "PhysicalEngine.h"

namespace Game {


    std::unordered_map<int, CollisionProperties*> CollisionProps;
    int CollisionBody::NEXTINT = 0;
    // 构造函数
    CollisionBody::CollisionBody(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, float mass, float friction,bool ifStatic=false)
        :_collisionProperties(pos, vel, acc)//初始化结构体引用
    {
        
        _collisionProperties.mass = mass;
        _collisionProperties.friction = friction;
        _collisionProperties.ID = NEXTINT++;//现在使用碰撞结构体储存相关的参数包括ID，碰撞检测中也直接与结构体进行快速交互
        _ifStatic = ifStatic;
        if (_ifStatic)
            _collisionProperties.mass = 100000000;//设置静态物体的质量为亿        
    }
    // 析构函数
    CollisionBody::~CollisionBody() {}

    bool CollisionBody::Interface()
    {
        return false;
    }

    // 更新物体的速度和位置
    void CollisionBody::UpdateCollisionState(float deltaTime) {
       
        
        UpdateCollisionParameters();
        if (_collisionProperties.ID ==0)
        {
           //这里可以根据碰撞层来进行碰撞分类
           
         CheckCollisionWithAABB(CollisionProps[1]);//传入引用，这里才需要使用*访问值，而传入指针就不需要
        }
  

      
        
        //// 在这里打印碰撞体的参数
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
       //注入全局访问模块
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
            

            // 获取当前物体和对方物体的 AABB 边界
            glm::vec3 currentMin = _collisionProperties._collisionMin;
            glm::vec3 currentMax = _collisionProperties._collisionMax;
            glm::vec3 otherMin = other->_collisionMin;
            glm::vec3 otherMax = other->_collisionMax;

            // 计算碰撞点（假设碰撞点是当前物体的中心）
            glm::vec3 collisionPoint = _collisionProperties.position;

            // 计算碰撞点到对方物体每个面的距离
            float distToLeft = collisionPoint.x - otherMin.x;   // 左面 (x = minX)
            float distToRight = otherMax.x - collisionPoint.x;  // 右面 (x = maxX)
            float distToBottom = collisionPoint.y - otherMin.y; // 下面 (y = minY)
            float distToTop = otherMax.y - collisionPoint.y;    // 上面 (y = maxY)
            float distToBack = collisionPoint.z - otherMin.z;   // 后面 (z = minZ)
            float distToFront = otherMax.z - collisionPoint.z;  // 前面 (z = maxZ)

            // 找到距离最小的面
            float minDist = std::min({ distToLeft, distToRight, distToBottom, distToTop, distToBack, distToFront });

            // 确定碰撞法向量
            glm::vec3 collisionNormal;
            if (minDist == distToLeft) {
                collisionNormal = glm::vec3(-1.0f, 0.0f, 0.0f); // 左面法向量
            }
            else if (minDist == distToRight) {
                collisionNormal = glm::vec3(1.0f, 0.0f, 0.0f);  // 右面法向量
            }
            else if (minDist == distToBottom) {
                collisionNormal = glm::vec3(0.0f, -1.0f, 0.0f); // 下面法向量
            }
            else if (minDist == distToTop) {
                collisionNormal = glm::vec3(0.0f, 1.0f, 0.0f);  // 上面法向量
            }
            else if (minDist == distToBack) {
                collisionNormal = glm::vec3(0.0f, 0.0f, -1.0f); // 后面法向量
            }
            else if (minDist == distToFront) {
                collisionNormal = glm::vec3(0.0f, 0.0f, 1.0f);  // 前面法向量
            }

            // 计算两物体的相对速度（沿着碰撞法向量的速度）
            float relativeVelocity = glm::dot(_collisionProperties.velocity - other->velocity, collisionNormal);


            // 如果相对速度小于零，则发生碰撞
            if (relativeVelocity < 0) {
               
                std::cout << "计算碰撞" << std::endl;

                // 弹性系数（恢复系数）
                float e = 0.8f;  // 假设弹性系数为0.8，部分弹性碰撞

                // 计算冲量
                float impulse = -(1 + e) * relativeVelocity /
                    (1 / _collisionProperties.mass + 1 / other->mass);

                // 更新当前物体的速度
                glm::vec3 newVelocity = _collisionProperties.velocity + impulse * collisionNormal / _collisionProperties.mass;

                // 设置更新后的速度
                _collisionProperties.velocity = newVelocity;

            }
        }
    }

    // 与 AABB 碰撞检测
    bool CollisionBody::CheckCollisionWithAABB(CollisionProperties* other) {
        // 检查 AABB 碰撞
        glm::vec3 otherMin = other->_collisionMin;
        glm::vec3 otherMax = other->_collisionMax;

        if (!_collisionProperties.isCollision)
        {
            if ((_collisionProperties.position.x > otherMin.x && _collisionProperties.position.x < otherMax.x) &&
                (_collisionProperties.position.y > otherMin.y && _collisionProperties.position.y < otherMax.y) &&
                (_collisionProperties.position.z > otherMin.z && _collisionProperties.position.z < otherMax.z))

            {

                std::cout << _collisionProperties.ID << " 碰撞了" << other->ID << std::endl;
                //碰撞之后进行碰撞分析
                ResolveCollision(other);

               _collisionProperties.isCollision = true;
                return true;  // 碰撞发生
            }

           
        }
        else
        {

            if (!((_collisionProperties.position.x > otherMin.x && _collisionProperties.position.x < otherMax.x) &&
                (_collisionProperties.position.y > otherMin.y && _collisionProperties.position.y < otherMax.y) &&
                (_collisionProperties.position.z > otherMin.z && _collisionProperties.position.z < otherMax.z)))

            {

                std::cout << _collisionProperties.ID << " 碰撞结束" << other->ID << std::endl;
                _collisionProperties.isCollision = false;
                return false;
             }

        }
        return true;

    }


}
