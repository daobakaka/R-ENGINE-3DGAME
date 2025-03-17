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

        // ��Ӷ�ʱ����
        void AddTimerTask(float intervalSeconds, int repeatCount, std::function<void()> callback);

        // �������ж�ʱ����
        void CoroutineUpdate();

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
        // ���ͷ������������İ汾
        // -----------------------------
        template<typename T>
        void StartSpawnByTimerAnimation(
            LifecycleManager<CustomModel>* manager,
            const char *name,
            bool ifLight,
            bool ifShadow,
            const ModelData& modelData,
            const AnimationData& animationData,
            std::unordered_map<PictureTpye,GLuint> textures = TextureDic["default"],
            int order = 0,
            glm::vec2 section=glm::vec2(1,1),
            ModelClass modelEnum = ModelClass::OriginalE,
            float interval = 1.0f,
            int count = 1,
            const StepVector3& stepVector3 = {},
            glm::vec3 position = glm::vec3(0, 0, 0),
            glm::vec3 rotation = glm::vec3(0, 0, 0),
            glm::vec3 scale = glm::vec3(1, 1, 1),
            bool ifPhysical = false,
            bool ifCollider = false
        )
        {// ������ԣ������ڷ���Լ��
            static_assert(std::is_base_of_v<CustomModel, T>, "T must inherit from CustomModel!");

            // ÿ���������Լ������� counterPtr
            auto counterPtr = std::make_shared<int>(0);//�������Ϊÿ��������ӹ�������ָ�룬���ֱ�� int countPtr=0,������ʹ���ڻص���������
                                                        //ʱ�����¸���һ������������ÿ�ι��㣬����ָ���ɱ�������̨�Զ�������ͷţ��ǳ�����
                                                        //������ڻص���ʱ��ͨ��lamda�Ĳ��񣬼�ס��ʼ�������ã������������»ص�������            

            AddTimerTask(interval, count,
                [manager, name, modelData, animationData,
                modelEnum,  ifPhysical, ifCollider, stepVector3,
                position, rotation, scale, counterPtr,ifLight,textures,order, ifShadow,section]()
                {
                    // ��������
                    (*counterPtr)++;

                    T* model = new T(
                        name,
                        modelData,
                        true,
                        ifLight,
                        ifShadow
                    );
                    model->SetVariant(modelEnum);
                    manager->RegisterObject(model);

                    // λ�ã�position + stepVector3.position * counter
                    // ��ת���� (rotation + stepVector3.rotation * counter) ��Ϊŷ������ת quaternion
                    // ���ţ��� scale �����ϳ��� (stepVector3.scale ^ counter)
                    model->Initialize(
                        position + stepVector3.position * float(*counterPtr),
                        glm::quat(rotation + stepVector3.rotation * float(*counterPtr)),
                        scale * glm::vec3(
                            std::pow(stepVector3.scale.x, float(*counterPtr)),
                            std::pow(stepVector3.scale.y, float(*counterPtr)),
                            std::pow(stepVector3.scale.z, float(*counterPtr))
                        )
                    );

                   
                    model->AttachTexture(textures,order,section);
                    model->UniformParametersIns();
                    model->AttachAnimationController(animationData);

                    if (ifPhysical) {
                        model->AttachPhysicalEngine();
                    }
                    if (ifCollider) {
                        model->AttachCollider();
                    }
                    model->SelfIns();//��������ó�ʼ������
                }
            );
        }

        // -----------------------------
        // ���ͷ������޶����İ汾
        // -----------------------------
        template<typename T>
        void StartSpawnByTimerNoneAnimation(
            LifecycleManager<CustomModel>* manager,
            const char* name,
            bool ifLight,
            bool ifShadow,
            const ModelData& modelData,
            std::unordered_map<PictureTpye, GLuint> textures = TextureDic["default"],
            int order=0,
            glm::vec2 section = glm::vec2(1, 1),
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
                [manager, modelData, modelEnum,
                 ifPhysical, ifCollider, stepVector3,
                position, rotation, scale, counterPtr, ifLight, textures,order,name,ifShadow, section]()
                {
                    // ��������
                    (*counterPtr)++;

                    T* model = new T(
                        name,
                        modelData,
                        false,
                        ifLight,
                        ifShadow
                    );
                    model->SetVariant(modelEnum);
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

     
                    model->AttachTexture(textures,order,section);
                    model->UniformParametersIns();
                
                    if (ifPhysical) {
                        model->AttachPhysicalEngine();
                    }
                    if (ifCollider) {
                        model->AttachCollider();
                    }
                    model->SelfIns();
                }
            );
        }
    };

} // namespace Game

#endif // _COROUTINEMETHOD_H_
