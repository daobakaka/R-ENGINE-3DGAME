#ifndef ANIMATION_INTERGRATED_H
#define ANIMATION_INTERGRATED_H
#include"GameObject.h"
#include "FileLoadIO.h"
namespace Game {

    class CustomModel; // ǰ������ CustomModel ��

    class Animation
    {
    public:
        Animation(const std::vector<std::vector<Vertex>>& keyFrames, const std::vector<float>& keyFrameTimes, bool loop, float speed = 1.0f);
        Animation(AnimationData animationData);
        void Update(float deltaTime);
        std::vector<Vertex> GetInterpolatedVertices() const;//��ȡ��ֵ������󣬼������еĹؼ�֡
        void Play();
        void Reset();
        bool IsPlaying() const;//read only ������ �����ڲ����ܶ���ĳ�Ա���κ��޸ģ����� ֻ�����ݵı����Ż�
        // const std::string& GetName() const ;//ǰ��������1 �����ڲ����ܶ���ĳ�Ա���κ��޸ģ�2 ���ص�����������ֻ���ģ�����ͨ�������޸����õ��ڴ�,������������������Ҫ����ʹ��
                                             //ǰ�治��const����ɷ����������ض����ܱ��޸ĵ��߼���ͻ�����ǰ������const, ��������ǰ����ԣ�Ч����һ����            

    private:
        bool loop;  // �Ƿ�ѭ������
        float currentTime;
        bool isPlaying;
        size_t currentKeyFrameIndex;
        size_t nextKeyFrameIndex;
        float speed;  // �����ٶȿ���
        std::vector<std::vector<Vertex>> keyFrames;       // �洢�ؼ�֡��������
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

        bool Interface() override;//ʵ�ֽӿ� ������Ϊobject ��Componenet���

    private:
        std::vector<Animation> animations;
        int currentAnimationIndex;
        bool ifplay = false;
    };

}
#endif // ANIMATION_CONTROLLER_H
























