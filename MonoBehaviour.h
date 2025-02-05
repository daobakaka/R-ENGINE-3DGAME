#ifndef MONOBEHAVIOUR_H
#define MONOBEHAVIOUR_H
#include "EnumTotal.h"

namespace Game {
    //这种接口类实际上就不需要单独的.CPP文件去实现了，直接在继承类的CPP文件中去实现
    class MonoBehaviour {
    public:
        virtual void Start() = 0;    // 游戏开始时调用
        virtual void Update(glm::mat4 view, glm::mat4 projection) = 0;   // 每帧更新时调用，因为视图和投影矩阵没有做成静态，所以这里需要传入，避免每个实例都去获取
        //感觉实际的引擎内部可能是做成静态的，或者框架自动传入单例参数        
        virtual void Destroy() = 0;  // 销毁时调用
        virtual void SetVariant(ModelClass par) = 0;
        virtual ModelClass GetVariant() = 0;
        virtual void UpdateVariant(glm::mat4 view, glm::mat4 projection) = 0;
        virtual ~MonoBehaviour() = default;  // 虚析构函数
    };
}
#endif // MONOBEHAVIOUR_H
#pragma once
