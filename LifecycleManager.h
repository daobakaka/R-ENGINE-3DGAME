#ifndef LIFECYCLE_MANAGER_H
#define LIFECYCLE_MANAGER_H

#include <unordered_map>   // 一定要包含
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
        //激活逻辑
        void SetActive(T* object, bool active);
        //销毁逻辑
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
        LifecycleManager() : GUID(0) {} // 初始化GUID

        // 如果想让GUID全局共享;
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
    //对象要被注册之后，才能在主逻辑里面享受渲染 阴影 游戏逻辑等
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
             //这里原容器的ID并没有变化，可以放回原来的位置，相当于一个新的注册方法
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
         //调用内部激活方法
         object->SetActive(active);
    }

    template<typename T>
    void LifecycleManager<T>::DestroyObject(T* object)
    {

        RemoveObject(object);
        //调用对象内部销毁方法
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
