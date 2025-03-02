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

        // �������嵽�˲���
        void Insert(CollisionProperties* obj);
        // �Ӱ˲����Ƴ�����
        void Remove(CollisionProperties* obj);
        // ��������λ�ã���������ƶ���
        void Update(CollisionProperties* obj);
        // ��ѯ������ײ�������б�
        void Query(const CollisionProperties* obj, std::vector<CollisionProperties*>& result);

        std::vector<CollisionProperties*> _objects;  // ��ǰ�ڵ�洢������

    private:
        void Subdivide();  // �ӽڵ�ָ�
        int GetChildIndex(const CollisionProperties* obj) const;

        glm::vec3 _center;     // �ڵ�����
        glm::vec3 _halfSize;   // �ڵ�볤
        int _depth;            // ��ǰ���
        int _maxDepth;         // ������
        int _maxObjects;       // �ڵ����������

        OctreeNode* _children[8];                    // �ӽڵ�ָ������
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
