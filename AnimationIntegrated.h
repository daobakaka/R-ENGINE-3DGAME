#ifndef ANIMATION_INTERGRATED_H
#define ANIMATION_INTERGRATED_H
#include"GameObject.h"
#include "FileLoadIO.h"
namespace Game {

    class CustomModel; // 前向声明 CustomModel 类

    class Animation
    {
    public:
        Animation(const std::vector<std::vector<Vertex>>& keyFrames, const std::vector<float>& keyFrameTimes, bool loop, float speed = 1.0f);
        Animation(AnimationData animationData);
        void Update(float deltaTime);
        std::vector<Vertex> GetInterpolatedVertices() const;//获取插值顶点对象，即动画中的关键帧
        void Play();
        void Reset();
        bool IsPlaying() const;//read only 声明， 方法内部不能对类的成员做任何修改，另有 只读数据的编译优化
        // const std::string& GetName() const ;//前后声明，1 方法内部不能对类的成员做任何修改，2 返回的引用类型是只读的，不能通过引用修改引用的内存,对于引用类型这里需要配套使用
                                             //前面不加const会造成方法声明返回对象不能被修改的逻辑冲突，因此前面必须加const, 单独放在前面可以，效果是一样的            

    private:
        bool loop;  // 是否循环播放
        float currentTime;
        bool isPlaying;
        size_t currentKeyFrameIndex;
        size_t nextKeyFrameIndex;
        float speed;  // 播放速度控制
        std::vector<std::vector<Vertex>> keyFrames;       // 存储关键帧顶点数据
        std::vector<float> keyFrameTimes;

    };



    class AnimationController :IComponent<AnimationController>
    {
    public:
        AnimationController();
        //void AddAnimation(const std::string& name, float duration);
        void AddAnimation(Animation* animation);
        void PlayAnimation(int index, float  deltaTime, CustomModel* model);
        void StopAnimation(int index=0);
        void Update(float deltaTime);
        std::vector<Vertex> GetCurrentAnimationVertices();

        bool Interface() override;//实现接口 ，定义为object 的Componenet组件

    private:
        std::vector<Animation> animations;
        int currentAnimationIndex;
        bool ifplay = false;
    };

}
#endif // ANIMATION_CONTROLLER_H
























