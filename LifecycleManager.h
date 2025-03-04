#ifndef LIFECYCLE_MANAGER_H
#define LIFECYCLE_MANAGER_H

#include <unordered_map>   // һ��Ҫ����
#include <vector>
#include <algorithm>       // std::find_if, etc
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Game {

    template <typename T>
    class LifecycleManager
    {
    public:
        static LifecycleManager<T>* GetInstance();

        void RegisterObject(T* object);
        void RemoveObject(T* object);
        //�����߼�
        void SetActive(T* object, bool active);
        //�����߼�
        void DestroyObject(T* object);

        std::unordered_map<int, T*>& GetNativeObjects();
        std::unordered_map<int, T*>& GetVariantObjects();

        void StartAll();
        void UpdateAll(glm::mat4 view, glm::mat4 projection);
        void DestroyAll();
        void UpdateAllVariant(glm::mat4 view, glm::mat4 projection);
        void UpdateDepthPic(glm::mat4 projection, GLuint shader);
        void PhysicalUpdateAll();

    private:
        LifecycleManager() : GUID(0) {} // ��ʼ��GUID

        // �������GUIDȫ�ֹ���;
        int GUID;

        static LifecycleManager<T>* instance;

        std::unordered_map<int, T*> nativeObjects;
        std::unordered_map<int, T*> variantObjects;
    };

    template <typename T>
    LifecycleManager<T>* LifecycleManager<T>::GetInstance() {
        if (!instance) {
            instance = new LifecycleManager<T>();
        }
        return instance;
    }
    //����Ҫ��ע��֮�󣬲��������߼�����������Ⱦ ��Ӱ ��Ϸ�߼���
    template <typename T>
    void LifecycleManager<T>::RegisterObject(T* object) {
        int newID = GUID++;
        object->SetID(newID);

        nativeObjects[newID] = object;
        if (object->GetVariant() != 0) {
            variantObjects[newID] = object;
        }
    }

    template <typename T>
    void LifecycleManager<T>::RemoveObject(T* object) {
        // fix method name: GetVariant()
        if (object->GetVariant() == 0) {
            nativeObjects.erase(object->GetID());
        }
        else {
            nativeObjects.erase(object->GetID());
            variantObjects.erase(object->GetID());
        }
        // fix method name: DestroySelf()
        // to free memory: delete object;
    }

    template<typename T>
     void LifecycleManager<T>::SetActive(T* object, bool active)
    {
         if (active)
         {
             //����ԭ������ID��û�б仯�����ԷŻ�ԭ����λ�ã��൱��һ���µ�ע�᷽��
             int newID = object->GetID();
             nativeObjects[newID] = object;
             if (object->GetVariant() != 0) {
                 variantObjects[newID] = object;
             }

         }
         else
         {
             RemoveObject(object);

         }
         //�����ڲ������
         object->SetActive(active);
    }

    template<typename T>
    void LifecycleManager<T>::DestroyObject(T* object)
    {

        RemoveObject(object);
        //���ö����ڲ����ٷ���
        object->DestroySelf();
    }

    template <typename T>
    std::unordered_map<int, T*>& LifecycleManager<T>::GetNativeObjects() {
        return nativeObjects;
    }

    template <typename T>
    std::unordered_map<int, T*>& LifecycleManager<T>::GetVariantObjects() {
        return variantObjects;
    }

    template <typename T>
    void LifecycleManager<T>::StartAll() {
        for (auto& kv : nativeObjects) {
            T* obj = kv.second;
            obj->Start();
        }
    }

    template <typename T>
    void LifecycleManager<T>::UpdateAll(glm::mat4 view, glm::mat4 projection) {
        for (auto& kv : nativeObjects) {
            T* obj = kv.second;
            obj->Update(view, projection);
        }
    }

    template <typename T>
    void LifecycleManager<T>::DestroyAll() {
        for (auto& kv : nativeObjects) {
            T* obj = kv.second;
            obj->Destroy();
            //  to free memory: delete obj;
        }
        nativeObjects.clear();
        variantObjects.clear();
    }

    template <typename T>
    void LifecycleManager<T>::UpdateAllVariant(glm::mat4 view, glm::mat4 projection) {
        for (auto& kv : variantObjects) {
            T* object = kv.second;
            object->UpdateVariant(view, projection);
        }
    }

    template <typename T>
    void LifecycleManager<T>::UpdateDepthPic(glm::mat4 projection, GLuint shader) {
        for (auto& kv : variantObjects) {
            T* object = kv.second;
            object->UpdateDepthPic(projection, shader);
        }
    }

    template <typename T>
    void LifecycleManager<T>::PhysicalUpdateAll() {
        // ...
    }

} // namespace Game

#endif
