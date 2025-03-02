#ifndef OCTREE_H
#define OCTREE_H


#include <vector>
#include <glm/glm.hpp>

namespace Game {

    class CollisionBody;
    struct CollisionProperties;
    class OctreeNode {
    public:
        OctreeNode(const glm::vec3& center, const glm::vec3& halfSize, int depth = 0, int maxDepth = 5, int maxObjects = 8);
        ~OctreeNode();

        // 插入物体到八叉树
        void Insert(CollisionProperties* obj);
        // 从八叉树移除物体
        void Remove(CollisionProperties* obj);
        // 更新物体位置（如果物体移动）
        void Update(CollisionProperties* obj);
        // 查询可能碰撞的物体列表
        void Query(const CollisionProperties* obj, std::vector<CollisionProperties*>& result);

        std::vector<CollisionProperties*> _objects;  // 当前节点存储的物体

    private:
        void Subdivide();  // 子节点分割
        int GetChildIndex(const CollisionProperties* obj) const;

        glm::vec3 _center;     // 节点中心
        glm::vec3 _halfSize;   // 节点半长
        int _depth;            // 当前深度
        int _maxDepth;         // 最大深度
        int _maxObjects;       // 节点最大物体数

        OctreeNode* _children[8];                    // 子节点指针数组
    };

    class Octree {
    public:
        
        ~Octree();
        static Octree* GetInstance();
        void Insert(CollisionProperties* obj);
        void Remove(CollisionProperties* obj);
        void Update(CollisionProperties* obj);
        void Query(const CollisionProperties* obj, std::vector<CollisionProperties*>& result);

     

    private:
        Octree();
        OctreeNode* _root;
        static Octree *instance;
    };
}
#endif
