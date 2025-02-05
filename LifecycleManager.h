#ifndef LIFECYCLE_MANAGER_H
#define LIFECYCLE_MANAGER_H

#include <vector>//容器
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
        static LifecycleManager<T>* GetInstance();
        // 在主循环中调用每个对象的生命周期方法
        void StartAll();
        void UpdateAll(glm::mat4 view, glm::mat4 projection);
        void DestroyAll();
        //--
        void UpdateAllVariant(glm::mat4 view, glm::mat4 projection);
    private:
        std::vector<T*> nativeObjects;   // 存储注册的对象
        std::vector<T*> variantObjects;//变体对象，用于作用预制体
        LifecycleManager() {};
        static LifecycleManager<T>* instance;

    public:
        //动画控制

    };
    
}
#endif // LIFECYCLE_MANAGER_H


