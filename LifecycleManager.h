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
        void CacheObject(T* object);
        void RemoveCacheObject(T* object);
        //激活逻辑
        void SetActive(T* object, bool active);
        //销毁逻辑
        void DestroyObject(T* object);
        //注册特殊对象，方便由名字获取
        void RegisterSpecialObjects(T* object,const char * name);

        std::unordered_map<int, T*>& GetNativeObjects();
        std::unordered_map<int, T*>& GetVariantObjects();
        //这里的对象采取遍历的方法进行处理，针对同种变体的对象，进行复原，必须要对setActiveFalse的结构才使用
        std::unordered_map<int, T*>& GetCacheObjects();
        //特殊对象容器，比如玩家或者其他的比如需要进行深度 alpha 模板测试的特殊实例
        std::unordered_map<const char*, T*>& GetspecialObjects();

        void StartAll();
        void UpdateAll(glm::mat4 view, glm::mat4 projection);
        void DestroyAll();
        void UpdateAllVariant(glm::mat4 view, glm::mat4 projection);
        void UpdateDepthPic(glm::mat4 projection, GLuint shader);
        void UpdateDepthViewPortPic(glm::mat4 view, glm::mat4 projection,GLuint shader);

    private:
        LifecycleManager() : GUID(0) {} // 初始化GUID

        // 让GUID全局共享;
        int GUID;

        static LifecycleManager<T>* instance;

        std::unordered_map<int, T*> nativeObjects;
        std::unordered_map<int, T*> variantObjects;
        std::unordered_map<int, T*> cacheObjects;
        std::unordered_map<const char*, T*> specialObjects;
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
        if (object->GetVariant() != 0) {
            variantObjects.erase(object->GetID());
        }
            nativeObjects.erase(object->GetID());
          
    }



    template<typename T>
    void LifecycleManager<T>::CacheObject(T* object)
    {
        int newID = object->GetID();
        cacheObjects[newID] = object;
    }

    template<typename T>
    inline void LifecycleManager<T>::RemoveCacheObject(T* object)
    {
        int newID = object->GetID();
        //从缓存对象中移除
        cacheObjects.erase(newID);

    }

    template<typename T>
     void LifecycleManager<T>::SetActive(T* object, bool active)
    {
         //设置为true,激活状态为false的情况下
         if (active&&!object->GetActiveState())
         {
             //这里原容器的ID并没有变化，可以放回原来的位置，相当于一个新的注册方法
             int newID = object->GetID();
             nativeObjects[newID] = object;
             if (object->GetVariant() != 0) {
                 variantObjects[newID] = object;
             }
             //重新获取对象之后，将缓存对象移除缓存列表
             RemoveCacheObject(object);
             //调用内部激活方法
             object->SetActive(active);
         }
         if (!active)
         {
             RemoveObject(object);
             //移除对象之后，要将对象缓存在缓存列表中，便于以后复用
             CacheObject(object);
             //调用内部激活方法
             object->SetActive(active);
         }
 
    }

    template<typename T>
    void LifecycleManager<T>::DestroyObject(T* object)
    {

        RemoveObject(object);
        //调用对象内部销毁方法
        object->DestroySelf();
    }

    template<typename T>
     void LifecycleManager<T>::RegisterSpecialObjects(T* object,const char* name)
    {
         specialObjects[name] = object;

    }

    template <typename T>
    std::unordered_map<int, T*>& LifecycleManager<T>::GetNativeObjects() {
        return nativeObjects;
    }

    template <typename T>
    std::unordered_map<int, T*>& LifecycleManager<T>::GetVariantObjects() {
        return variantObjects;
    }

    template<typename T>
     std::unordered_map<int, T*>& LifecycleManager<T>::GetCacheObjects()
    {
        return cacheObjects;
    }

     template<typename T>
 std::unordered_map<const char*, T*>& LifecycleManager<T>::GetspecialObjects()
     {
     return specialObjects;
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
        //增加特殊对象的阴影渲染调用
        for (auto& kv : specialObjects) {
            T* object = kv.second;
            object->UpdateDepthPic(projection, shader);
        }
    }
    /// <summary>
    /// 变体的视口深度图绘制
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <param name="view"></param>
    /// <param name="projectionm"></param>
    /// <param name="shader"></param>
    template<typename T>
    inline void LifecycleManager<T>::UpdateDepthViewPortPic(glm::mat4 view, glm::mat4 projection, GLuint shader)
    {
        for (auto& kv : variantObjects) {
            T* object = kv.second;
            object->UpdateDepthViewPortPic(view,projection, shader);
        }
    }


} // namespace Game

#endif
