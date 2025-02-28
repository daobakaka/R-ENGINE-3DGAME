#ifndef LIFECYCLE_MANAGER_H
#define LIFECYCLE_MANAGER_H

#include <vector> //容器
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>  // 用于旋转

namespace Game {

    //模板的声明，泛型的最初表达
    template <typename T>
    class LifecycleManager
    {
    public:
        // 注册 MonoBehaviour 对象到生命周期管理器
        void RegisterObject(T* object);
        std::vector<T*> GetNativeObjects();
        std::vector<T*> GetVariantObjects();
        std::vector<T*> GetColloderObjects();
        static LifecycleManager<T>* GetInstance();
        // 在主循环中调用每个对象的生命周期方法
        void StartAll();
        void UpdateAll(glm::mat4 view, glm::mat4 projection);
        void DestroyAll();
        //--变体更新方法
        void UpdateAllVariant(glm::mat4 view, glm::mat4 projection);

        //光源深度模型绘制
        void UpdateDepthPic(glm::mat4 projection, GLuint shader);

        //物体引擎更新
        void PhysicalUpdateAll();

    private:
        std::vector<T*> nativeObjects;   // 存储注册的对象
        std::vector<T*> variantObjects;  // 存储变体对象
        std::vector<T*> colliderObjects; //储存碰撞对象
        LifecycleManager() {};
        static LifecycleManager<T>* instance ;

    public:
        //动画控制
    };

    // 在头文件中实现模板方法,这里都是最基类的重写，避免报错
    //原始对象封装，无条件执行所有更新逻辑，变体对象封装，执行变体逻辑，待优化，碰撞对象封装，专门执行碰撞逻辑
    template <typename T>
    void LifecycleManager<T>::RegisterObject(T* object) {
        nativeObjects.push_back(object);
        if (object->GetVariant() != 0)
            variantObjects.push_back(object);
        if (object->GetIfCollision() == true)
            colliderObjects.push_back(object);
    }

    template <typename T>
    std::vector<T*> LifecycleManager<T>::GetNativeObjects() {
        return nativeObjects;
    }

    template <typename T>
    std::vector<T*> LifecycleManager<T>::GetVariantObjects() {
        return variantObjects;
    }

    template<typename T>
    inline std::vector<T*> LifecycleManager<T>::GetColloderObjects()
    {
        return colliderObjects;
    }

    template <typename T>
    LifecycleManager<T>* LifecycleManager<T>::GetInstance() {
        if (instance == nullptr) {
            instance = new LifecycleManager<T>();
        }
        return instance;
    }

    template <typename T>
    void LifecycleManager<T>::StartAll() {
        for (T* object : nativeObjects) {
            object->Start();  // 调用每个对象的 Start 方法
        }
    }

    template <typename T>
    void LifecycleManager<T>::UpdateAll(glm::mat4 view, glm::mat4 projection) {
        for (T* object : nativeObjects) {
            object->Update(view, projection);  // 调用每个对象的 Update 方法
        }
    }

    template <typename T>
    void LifecycleManager<T>::DestroyAll() {
        for (T* object : nativeObjects) {
            object->Destroy();  // 调用每个对象的 Destroy 方法
        }
    }

    template <typename T>
    void LifecycleManager<T>::UpdateAllVariant(glm::mat4 view, glm::mat4 projection) {
        for (T* object : variantObjects) {
            object->UpdateVariant(view, projection);
        }
    }

    template <typename T>
    void LifecycleManager<T>::UpdateDepthPic(glm::mat4 projection, GLuint shader) {
        for (T* object : variantObjects) {
            object->UpdateDepthPic(projection, shader);
        }
    }




} // namespace Game

#endif // LIFECYCLE_MANAGER_H
