#ifndef _COROUTINEMETHOD_H_
#define _COROUTINEMETHOD_H_

#include <functional>
#include <vector>
#include <thread>
#include <chrono>
#include <algorithm>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <type_traits>
#include <memory>            // std::make_shared
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "EnumTotal.h"
#include "LifecycleManager.h"
#include "CustomModel.h"

namespace Game {

    class CoroutineMethod {
    public:
        static CoroutineMethod* GetInstance();

        void StartSpawnButterfliesAsync(LifecycleManager<CustomModel>* manager);

        // 添加定时任务
        void AddTimerTask(float intervalSeconds, int repeatCount, std::function<void()> callback);

        // 更新所有定时任务
        void CoroutineUpdate();

        // 示例：每隔1秒创建蝴蝶，通过定时任务
        void StartSpawnButterfliesByTimer(LifecycleManager<CustomModel>* manager,GLuint textureName, int order);

    private:
        CoroutineMethod();
        ~CoroutineMethod();
        CoroutineMethod(const CoroutineMethod&) = delete;
        CoroutineMethod& operator=(const CoroutineMethod&) = delete;

        static CoroutineMethod* instance;

        struct TimerTask {
            float interval;
            int repeatCount;
            float nextTriggerTime;
            std::function<void()> callback;
        };

        std::vector<TimerTask> tasks;

    public:
        // -----------------------------
        // 泛型方法：带动画的版本
        // -----------------------------
        template<typename T>
        void StartSpawnByTimerAnimation(
            LifecycleManager<CustomModel>* manager,
            const char* vertexShader,
            const char* fragmentShader,
            bool ifLight,
            const ModelData& modelData,
            const AnimationData& animationData,
            GLuint textureName = TextureDic["default"][0],
            int order = 0,
            ModelClass modelEnum = ModelClass::OriginalE,
            float interval = 1.0f,
            int count = 1,
            const StepVector3& stepVector3 = {},
            glm::vec3 position = glm::vec3(0, 0, 0),
            glm::vec3 rotation = glm::vec3(0, 0, 0),
            glm::vec3 scale = glm::vec3(1, 1, 1),
            bool ifPhysical = true,
            bool ifCollider = true
        )
        {// 编译断言，类似于泛型约束
            static_assert(std::is_base_of_v<CustomModel, T>, "T must inherit from CustomModel!");

            // 每个任务都有自己独立的 counterPtr
            auto counterPtr = std::make_shared<int>(0);//这里可以为每个对象添加共享智能指针，如果直接 int countPtr=0,这样会使得在回调函数运行
                                                        //时，重新复制一个副本，导致每次归零，智能指针由编译器后台自动管理和释放，非常方便
                                                        //其可以在回调的时候，通过lamda的捕获，记住初始化的引用，不会因函数重新回调而结束            

            AddTimerTask(interval, count,
                [manager, vertexShader, fragmentShader, modelData, animationData,
                modelEnum,  ifPhysical, ifCollider, stepVector3,
                position, rotation, scale, counterPtr,ifLight,textureName,order]()
                {
                    // 自增计数
                    (*counterPtr)++;

                    T* model = new T(
                        vertexShader,
                        fragmentShader,
                        modelData,
                        true,
                        ifLight
                    );
                    manager->RegisterObject(model);

                    // 位置：position + stepVector3.position * counter
                    // 旋转：将 (rotation + stepVector3.rotation * counter) 视为欧拉角再转 quaternion
                    // 缩放：在 scale 基础上乘以 (stepVector3.scale ^ counter)
                    model->Initialize(
                        position + stepVector3.position * float(*counterPtr),
                        glm::quat(rotation + stepVector3.rotation * float(*counterPtr)),
                        scale * glm::vec3(
                            std::pow(stepVector3.scale.x, float(*counterPtr)),
                            std::pow(stepVector3.scale.y, float(*counterPtr)),
                            std::pow(stepVector3.scale.z, float(*counterPtr))
                        )
                    );

                   
                    model->AttachTexture(textureName,order);
                    model->AttachAnimationController(animationData);

                    if (ifPhysical) {
                        model->AttachPhysicalEngine();
                    }
                    if (ifCollider) {
                        model->AttachCollider();
                    }
                    model->SetVariant(modelEnum);
                }
            );
        }

        // -----------------------------
        // 泛型方法：无动画的版本
        // -----------------------------
        template<typename T>
        void StartSpawnByTimerNoneAnimation(
            LifecycleManager<CustomModel>* manager,
            const char* vertexShader,
            const char* fragmentShader,
            bool ifLight,
            const ModelData& modelData,
            GLuint textureName = TextureDic["default"][0],
            int order=0,
            ModelClass modelEnum = ModelClass::OriginalE,
            float interval = 1.0f,
            int count = 1,
            const StepVector3& stepVector3 = {},
            glm::vec3 position = glm::vec3(0, 0, 0),
            glm::vec3 rotation = glm::vec3(0, 0, 0),
            glm::vec3 scale = glm::vec3(1, 1, 1),
            bool ifPhysical = true,
            bool ifCollider = true
        )
        {
            static_assert(std::is_base_of_v<CustomModel, T>, "T must inherit from CustomModel!");

            auto counterPtr = std::make_shared<int>(0);

            AddTimerTask(interval, count,
                [manager, vertexShader, fragmentShader, modelData, modelEnum,
                 ifPhysical, ifCollider, stepVector3,
                position, rotation, scale, counterPtr, ifLight, textureName,order]()
                {
                    // 自增计数
                    (*counterPtr)++;

                    T* model = new T(
                        vertexShader,
                        fragmentShader,
                        modelData,
                        false,
                        ifLight
                    );
                    manager->RegisterObject(model);

                    model->Initialize(
                        position + stepVector3.position * float(*counterPtr),
                        glm::quat(rotation + stepVector3.rotation * float(*counterPtr)),
                        scale * glm::vec3(
                            std::pow(stepVector3.scale.x, float(*counterPtr)),
                            std::pow(stepVector3.scale.y, float(*counterPtr)),
                            std::pow(stepVector3.scale.z, float(*counterPtr))
                        )
                    );

     
                    model->AttachTexture(textureName,order);
                
                    if (ifPhysical) {
                        model->AttachPhysicalEngine();
                    }
                    if (ifCollider) {
                        model->AttachCollider();
                    }
                    model->SetVariant(modelEnum);
                }
            );
        }
    };

} // namespace Game

#endif // _COROUTINEMETHOD_H_
