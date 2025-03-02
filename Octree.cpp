// Octree.cpp
#include "Octree.h"
#include <algorithm>
#include "CollisionBody.h"
#include "EnumTotal.h"

using namespace Game;

    Octree* Octree::instance = nullptr;  // ����Դ�ļ��г�ʼ����̬��Ա����,������Ҫ�������ⲿ���У������������������������ڵļ��𣬺�������ͬ�⼶��
    // OctreeNode ʵ��
    OctreeNode::OctreeNode(const glm::vec3& center, const glm::vec3& halfSize, int depth, int maxDepth, int maxObjects)
        : _center(center), _halfSize(halfSize), _depth(depth), _maxDepth(maxDepth), _maxObjects(maxObjects) {
        memset(_children, 0, sizeof(_children));
    }

    OctreeNode::~OctreeNode() {
        for (int i = 0; i < 8; i++) {
            if (_children[i]) delete _children[i];
        }
    }
    //  AABB �ཻ��⸨������
    bool CheckAABBOverlap(const glm::vec3& min1, const glm::vec3& max1, const glm::vec3& min2, const glm::vec3& max2) {
        return (min1.x <= max2.x && max1.x >= min2.x) &&
            (min1.y <= max2.y && max1.y >= min2.y) &&
            (min1.z <= max2.z && max1.z >= min2.z);
    }

    void OctreeNode::Insert(CollisionProperties* obj) {
        // ��������ӽڵ㣬���Բ��뵽�ӽڵ�
        if (_children[0] != nullptr) {
            int index = GetChildIndex(obj);
            if (index != -1) {
                _children[index]->Insert(obj);
                return;
            }
        }

        // ���뵽��ǰ�ڵ�
        _objects.push_back(obj);

        // ������������������δ�ﵽ�����ȣ��ָ�ڵ�
        if (_objects.size() > _maxObjects && _depth < _maxDepth) {
            if (!_children[0]) Subdivide();

            // ���·������嵽�ӽڵ�
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
        // ��鵱ǰ�ڵ��Ƿ������� AABB �ཻ
        if (!CheckAABBOverlap(obj->_collisionMin, obj->_collisionMax, _center - _halfSize, _center + _halfSize)) {
            return;
        }

        // ��ӵ�ǰ�ڵ������
        result.insert(result.end(), _objects.begin(), _objects.end());

        // �ݹ��ѯ�ӽڵ�
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
            //�����ӽڵ����
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

    // Octree ʵ��
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


  

