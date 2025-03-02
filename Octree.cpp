// Octree.cpp
#include "Octree.h"
#include <algorithm>
#include "CollisionBody.h"
#include "EnumTotal.h"

using namespace Game;

    Octree* Octree::instance = nullptr;  // ！在源文件中初始化静态成员变量,而且需要放在类外部进行，它是属于整个程序生命周期的级别，和类隶属同意级别
    // OctreeNode 实现
    OctreeNode::OctreeNode(const glm::vec3& center, const glm::vec3& halfSize, int depth, int maxDepth, int maxObjects)
        : _center(center), _halfSize(halfSize), _depth(depth), _maxDepth(maxDepth), _maxObjects(maxObjects) {
        memset(_children, 0, sizeof(_children));
    }

    OctreeNode::~OctreeNode() {
        for (int i = 0; i < 8; i++) {
            if (_children[i]) delete _children[i];
        }
    }
    //  AABB 相交检测辅助函数
    bool CheckAABBOverlap(const glm::vec3& min1, const glm::vec3& max1, const glm::vec3& min2, const glm::vec3& max2) {
        return (min1.x <= max2.x && max1.x >= min2.x) &&
            (min1.y <= max2.y && max1.y >= min2.y) &&
            (min1.z <= max2.z && max1.z >= min2.z);
    }

    void OctreeNode::Insert(CollisionProperties* obj) {
        // 如果存在子节点，尝试插入到子节点
        if (_children[0] != nullptr) {
            int index = GetChildIndex(obj);
            if (index != -1) {
                _children[index]->Insert(obj);
                return;
            }
        }

        // 插入到当前节点
        _objects.push_back(obj);

        // 如果超过最大物体数且未达到最大深度，分割节点
        if (_objects.size() > _maxObjects && _depth < _maxDepth) {
            if (!_children[0]) Subdivide();

            // 重新分配物体到子节点
            auto it = _objects.begin();
            while (it != _objects.end()) {
                int index = GetChildIndex(*it);
                if (index != -1) {
                    _children[index]->Insert(*it);
                    it = _objects.erase(it);
                }
                else {
                    ++it;
                }
            }
        }
    }

    void OctreeNode::Remove(CollisionProperties* obj) {
        auto it = std::find(_objects.begin(), _objects.end(), obj);
        if (it != _objects.end()) {
            _objects.erase(it);
        }

        if (_children[0]) {
            for (int i = 0; i < 8; i++) {
                _children[i]->Remove(obj);
            }
        }
    }

    void OctreeNode::Update(CollisionProperties* obj) {
        Remove(obj);
        Insert(obj);
    }

    void OctreeNode::Query(const CollisionProperties* obj, std::vector<CollisionProperties*>& result) {
        // 检查当前节点是否与物体 AABB 相交
        if (!CheckAABBOverlap(obj->_collisionMin, obj->_collisionMax, _center - _halfSize, _center + _halfSize)) {
            return;
        }

        // 添加当前节点的物体
        result.insert(result.end(), _objects.begin(), _objects.end());

        // 递归查询子节点
        if (_children[0]) {
            for (int i = 0; i < 8; i++) {
                _children[i]->Query(obj, result);
            }
        }
    }

    void OctreeNode::Subdivide() {
        glm::vec3 childHalfSize = _halfSize * 0.5f;
        for (int i = 0; i < 8; i++) {
            glm::vec3 childCenter = _center;
            childCenter.x += (i & 1) ? childHalfSize.x : -childHalfSize.x;
            childCenter.y += (i & 2) ? childHalfSize.y : -childHalfSize.y;
            childCenter.z += (i & 4) ? childHalfSize.z : -childHalfSize.z;
            //新增加节点对象
            _children[i] = new OctreeNode(childCenter, childHalfSize, _depth + 1, _maxDepth, _maxObjects);
        }
    }

    int OctreeNode::GetChildIndex(const CollisionProperties* obj) const {
        int index = 0;
        glm::vec3 objCenter = (obj->_collisionMin + obj->_collisionMax) * 0.5f;
        if (objCenter.x > _center.x) index |= 1;
        if (objCenter.y > _center.y) index |= 2;
        if (objCenter.z > _center.z) index |= 4;
        return index;
    }

    // Octree 实现
    Octree::Octree() {
        
        
        _root = new OctreeNode(glm::vec3(0), glm::vec3(1000)*0.5f ,0 ,5,8);
    }

    Octree::~Octree() {
        delete _root;
    }

    Octree* Octree::GetInstance()
    {
        if (instance==nullptr)
        {
            instance = new Octree();
        }
        
        return instance;
    }

    void Octree::Insert(CollisionProperties* obj) {
        _root->Insert(obj);
    }

    void Octree::Remove(CollisionProperties* obj) {
        _root->Remove(obj);
    }

    void Octree::Update(CollisionProperties* obj) {
        _root->Update(obj);
    }

    void Octree::Query(const CollisionProperties* obj, std::vector<CollisionProperties*>& result) {
        _root->Query(obj, result);
    }


  

