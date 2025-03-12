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
        void CacheObject(T* object);
        void RemoveCacheObject(T* object);
        //�����߼�
        void SetActive(T* object, bool active);
        //�����߼�
        void DestroyObject(T* object);
        //ע��������󣬷��������ֻ�ȡ
        void RegisterSpecialObjects(T* object,const char * name);

        std::unordered_map<int, T*>& GetNativeObjects();
        std::unordered_map<int, T*>& GetVariantObjects();
        //����Ķ����ȡ�����ķ������д������ͬ�ֱ���Ķ��󣬽��и�ԭ������Ҫ��setActiveFalse�Ľṹ��ʹ��
        std::unordered_map<int, T*>& GetCacheObjects();
        //�������������������һ��������ı�����Ҫ������� alpha ģ����Ե�����ʵ��
        std::unordered_map<const char*, T*>& GetspecialObjects();

        void StartAll();
        void UpdateAll(glm::mat4 view, glm::mat4 projection);
        void DestroyAll();
        void UpdateAllVariant(glm::mat4 view, glm::mat4 projection);
        void UpdateDepthPic(glm::mat4 projection, GLuint shader);
        void UpdateDepthViewPortPic(glm::mat4 view, glm::mat4 projection,GLuint shader);

    private:
        LifecycleManager() : GUID(0) {} // ��ʼ��GUID

        // ��GUIDȫ�ֹ���;
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
        //�ӻ���������Ƴ�
        cacheObjects.erase(newID);

    }

    template<typename T>
     void LifecycleManager<T>::SetActive(T* object, bool active)
    {
         //����Ϊtrue,����״̬Ϊfalse�������
         if (active&&!object->GetActiveState())
         {
             //����ԭ������ID��û�б仯�����ԷŻ�ԭ����λ�ã��൱��һ���µ�ע�᷽��
             int newID = object->GetID();
             nativeObjects[newID] = object;
             if (object->GetVariant() != 0) {
                 variantObjects[newID] = object;
             }
             //���»�ȡ����֮�󣬽���������Ƴ������б�
             RemoveCacheObject(object);
             //�����ڲ������
             object->SetActive(active);
         }
         if (!active)
         {
             RemoveObject(object);
             //�Ƴ�����֮��Ҫ�����󻺴��ڻ����б��У������Ժ���
             CacheObject(object);
             //�����ڲ������
             object->SetActive(active);
         }
 
    }

    template<typename T>
    void LifecycleManager<T>::DestroyObject(T* object)
    {

        RemoveObject(object);
        //���ö����ڲ����ٷ���
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
        //��������������Ӱ��Ⱦ����
        for (auto& kv : specialObjects) {
            T* object = kv.second;
            object->UpdateDepthPic(projection, shader);
        }
    }
    /// <summary>
    /// ������ӿ����ͼ����
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
