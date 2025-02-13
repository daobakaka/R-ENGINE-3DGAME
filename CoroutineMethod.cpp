#include "CoroutineMethod.h"

// 全局变量（根据实际项目定义）
extern const char* noneLightVertexShaderSource;
extern const char* noneLightcubeFragmentShaderSource;

using namespace Game;

    CoroutineMethod* CoroutineMethod::instance = nullptr;

    CoroutineMethod* CoroutineMethod::GetInstance() {
        if (!instance)
            instance = new CoroutineMethod();
        return instance;
    }
      
    CoroutineMethod::CoroutineMethod() { }

    CoroutineMethod::~CoroutineMethod() { }
    //原生的方法，但是目前渲染会出问题---
    void CoroutineMethod::StartSpawnButterfliesAsync(LifecycleManager<CustomModel>* manager) {
        std::thread([manager]() {
            for (int i = 0; i < 30; i++) {
                CustomModel* model = new CustomModel(
                    noneLightVertexShaderSource,
                    noneLightcubeFragmentShaderSource,
                    ModelDic["butterfly"],
                    true
                );
                manager->RegisterObject(model);
                model->Initialize(
                    glm::vec3(2.0f, 0.0f, i * 0.5f),
                    glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)),
                    glm::vec3(0.1f, 0.1f, 0.1f)
                );
                model->AttachTexture(TextureDic["default"][0],0);
                model->AttachAnimationController(AnimationDic["butterfly"]["fly"]);
                model->AttachPhysicalEngine();
                model->AttachCollider();
                model->SetVariant(ModelClass::ActorButterfly);
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            }).detach();
    }
    //一个新增的添加任务的方法
    void CoroutineMethod::AddTimerTask(float intervalSeconds, int repeatCount, std::function<void()> callback) {
        float currentTime = static_cast<float>(glfwGetTime());
        TimerTask task;
        task.interval = intervalSeconds;
        task.repeatCount = repeatCount;
        task.nextTriggerTime = currentTime + intervalSeconds;
        task.callback = callback;
        tasks.push_back(task);
    }
    //主循环中更新的方法
    void CoroutineMethod::CoroutineUpdate() {
        float currentTime = static_cast<float>(glfwGetTime());
        for (auto& task : tasks) {
            if (currentTime >= task.nextTriggerTime) {
                task.callback();
                if (task.repeatCount > 0)
                    task.repeatCount--;
                task.nextTriggerTime += task.interval;
            }
        }
        tasks.erase(
            std::remove_if(tasks.begin(), tasks.end(), [](const TimerTask& t) {
                return t.repeatCount == 0;
                }),
            tasks.end()
        );
    }

    //事实上这个方法可以通过配置，转换成通用的初始化方法
    void CoroutineMethod::StartSpawnButterfliesByTimer(LifecycleManager<CustomModel>* manager, GLuint textureName, int order) {
        // 重复 5 次、每次间隔 1 秒
        AddTimerTask(1.0f, 20, [manager, textureName,order]() {
             int counter = 0;
            std::cout << "[TimerTask] 创建蝴蝶对象 #" << counter << std::endl;
            counter++;

            CustomModel* model = new CustomModel(
                noneLightVertexShaderSource,
                noneLightcubeFragmentShaderSource,
                ModelDic["butterfly"],
                true
            );
            manager->RegisterObject(model);
            model->Initialize(
                glm::vec3(2.0f, 0.0f, counter * 0.5f),
                glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)),
                glm::vec3(0.1f, 0.1f, 0.1f)
            );
            model->AttachTexture(textureName,order);
            model->AttachAnimationController((AnimationDic["butterfly"]["fly"]));
            model->AttachPhysicalEngine();
            model->AttachCollider();
            model->SetVariant(ModelClass::ActorButterfly);
            });
    }
  

    

    

