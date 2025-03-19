#include "CoroutineMethod.h"

// 全局变量（根据实际项目定义）
extern const char* noneLightVertexShaderSource;
extern const char* noneLightFragmentShaderSource;

using namespace Game;

    CoroutineMethod* CoroutineMethod::instance = nullptr;

    CoroutineMethod* CoroutineMethod::GetInstance() {
        if (!instance)
            instance = new CoroutineMethod();
        return instance;
    }
      
    CoroutineMethod::CoroutineMethod() { }

    CoroutineMethod::~CoroutineMethod() { }
    //添加任务的方法
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

  

    

    

