#ifndef LIFECYCLE_MANAGER_H
#define LIFECYCLE_MANAGER_H

#include <unordered_map>
#include <vector>
#include <algorithm>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include "EnumTotal.h"

namespace Game {

    template <typename T>
    class LifecycleManager
    {
    public:
        static LifecycleManager<T>* GetInstance();

        void RegisterObject(T* object);
        void RemoveObject(T* object);
        void CacheObject(T* object);
        void RemoveCacheObject(T* object);
        void SetActive(T* object, bool active);
        void DestroyObject(T* object);
        void RegisterSpecialObjects(const char* name, T* object);

        std::unordered_map<ModelClass, std::unordered_map<int, T*>>& GetNativeObjects();
        std::unordered_map<ModelClass, std::unordered_map<int, T*>>& GetCacheObjects();
        std::unordered_map<const char*, T*>& GetSpecialObjects();

        void StartAll();
        void UpdateAll(glm::mat4 view, glm::mat4 projection);
        void DestroyAll();
        void UpdateDepthPic(glm::mat4 projection, GLuint shader);
        void UpdateDepthViewPortPic(glm::mat4 view, glm::mat4 projection, GLuint shader);

    private:
        LifecycleManager() : GUID(0) {} // 初始化GUID

        // 让GUID全局共享;
        int GUID;

        static LifecycleManager<T>* instance;
        std::unordered_map<ModelClass, std::unordered_map<int, T*>> nativeObjects;
        std::unordered_map<ModelClass, std::unordered_map<int, T*>> cacheObjects;
        std::unordered_map<const char*, T*> specialObjects;
    };

    template <typename T>
    LifecycleManager<T>* LifecycleManager<T>::GetInstance() {
        if (!instance) {
            instance = new LifecycleManager<T>();
        }
        return instance;
    }

    // 注册对象
    template <typename T>
    void LifecycleManager<T>::RegisterObject(T* object) {
        int newID = GUID++;
        object->SetID(newID);
        ModelClass modelClass = object->GetVariant(); // 获取对象的类型
        nativeObjects[modelClass][newID] = object;
    }

    // 移除对象
    template <typename T>
    void LifecycleManager<T>::RemoveObject(T* object) {
        ModelClass modelClass = object->GetVariant(); // 获取对象的类型
        nativeObjects[modelClass].erase(object->GetID());
    }

    // 缓存对象
    template <typename T>
    void LifecycleManager<T>::CacheObject(T* object) {
        ModelClass modelClass = object->GetVariant(); // 获取对象的类型
        int newID = object->GetID();
        cacheObjects[modelClass][newID] = object;
    }

    // 移除缓存对象
    template <typename T>
    void LifecycleManager<T>::RemoveCacheObject(T* object) {
        ModelClass modelClass = object->GetVariant(); // 获取对象的类型
        cacheObjects[modelClass].erase(object->GetID());
    }

    // 设置对象的激活状态
    template <typename T>
    void LifecycleManager<T>::SetActive(T* object, bool active) {
        ModelClass modelClass = object->GetVariant(); // 获取对象的类型
        if (active && !object->GetActiveState()) {
            // 激活对象并移出缓存
            int newID = object->GetID();
            nativeObjects[modelClass][newID] = object;
            RemoveCacheObject(object);
            object->SetActive(active);
        }
        if (!active) {
            // 移除对象并缓存
            RemoveObject(object);
            CacheObject(object);
            object->SetActive(active);
        }
    }

    // 销毁对象
    template <typename T>
    void LifecycleManager<T>::DestroyObject(T* object) {
        ModelClass modelClass = object->GetVariant(); // 获取对象的类型
        RemoveObject(object);
        object->DestroySelf();
    }

    // 注册特殊对象
    template <typename T>
    void LifecycleManager<T>::RegisterSpecialObjects(const char* name, T* object) {
        specialObjects[name] = object;
    }

    // 获取原生对象容器
    template <typename T>
    std::unordered_map<ModelClass, std::unordered_map<int, T*>>& LifecycleManager<T>::GetNativeObjects() {
        return nativeObjects;
    }

    // 获取缓存对象容器
    template <typename T>
    std::unordered_map<ModelClass, std::unordered_map<int, T*>>& LifecycleManager<T>::GetCacheObjects() {
        return cacheObjects;
    }

    // 获取特殊对象容器
    template <typename T>
    std::unordered_map<const char*, T*>& LifecycleManager<T>::GetSpecialObjects() {
        return specialObjects;
    }

    // 启动所有对象
    template <typename T>
    void LifecycleManager<T>::StartAll() {
        for (auto& modelClassPair : nativeObjects) {
            for (auto& kv : modelClassPair.second) {
                T* obj = kv.second;
                obj->Start();
            }
        }
    }

    // 更新所有对象
    template <typename T>
    void LifecycleManager<T>::UpdateAll(glm::mat4 view, glm::mat4 projection) {
        for (auto& modelClassPair : nativeObjects) {
            for (auto& kv : modelClassPair.second) {
                T* obj = kv.second;
                //让批量绘制的对象在外部单独调用
                if (!obj->GetBatchDrawState())
                {
                    obj->Update(view, projection);
                }
            }
        }
    }

    // 销毁所有对象
    template <typename T>
    void LifecycleManager<T>::DestroyAll() {
        for (auto& modelClassPair : nativeObjects) {
            for (auto& kv : modelClassPair.second) {
                T* obj = kv.second;
                obj->Destroy();
            }
            modelClassPair.second.clear();
        }
        nativeObjects.clear();
        cacheObjects.clear();
        specialObjects.clear();
    }

    // 更新深度图
    template <typename T>
    void LifecycleManager<T>::UpdateDepthPic(glm::mat4 projection, GLuint shader) {
        for (auto& modelClassPair : nativeObjects) {
            for (auto& kv : modelClassPair.second) {
                T* object = kv.second;
                object->UpdateDepthPic(projection, shader);
            }
        }
        for (auto& kv : specialObjects) {
            T* object = kv.second;
            object->UpdateDepthPic(projection, shader);
        }
    }

    // 更新视口深度图
    template <typename T>
    void LifecycleManager<T>::UpdateDepthViewPortPic(glm::mat4 view, glm::mat4 projection, GLuint shader) {
        for (auto& modelClassPair : nativeObjects) {
            for (auto& kv : modelClassPair.second) {
                T* object = kv.second;
                object->UpdateDepthViewPortPic(view, projection, shader);
            }
        }
    }

} // namespace Game

#endif