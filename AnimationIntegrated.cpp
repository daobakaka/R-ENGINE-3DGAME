#include "AnimationIntegrated.h"
#include <iostream> // �����ڵ���
#include "CustomModel.h"
using namespace Game;
// Animation �๹�캯��ʵ��
Animation::Animation(const std::vector<std::vector<Vertex>>& keyFrames, const std::vector<float>& keyFrameTimes, bool loop, float speed)
    : keyFrames(keyFrames), keyFrameTimes(keyFrameTimes), loop(loop), currentTime(0.0f),
    isPlaying(false), currentKeyFrameIndex(0), nextKeyFrameIndex(1), speed(speed) {}



Animation::Animation( AnimationData animationData)
    :
    isPlaying(false), currentKeyFrameIndex(0), nextKeyFrameIndex(1), speed(speed) 
{
    keyFrames = animationData.keyFrames;
    keyFrameTimes = animationData.keyFrameTimes;
    loop = animationData.loop;
    speed = animationData.speed;
}


// ���¶���״̬
void Animation::Update(float deltaTime) {
    if (isPlaying) {
        currentTime += deltaTime * speed;  // ���ݲ����ٶȵ�����ǰʱ��
        if (currentTime >= keyFrameTimes.back()) {
            if (loop) {
                currentTime = 0.0f;  // ѭ������
            }
            else {
                currentTime = keyFrameTimes.back();  // ��������
                isPlaying = false;
            }
        }

        // ����ʱ����µ�ǰ�ؼ�֡������
        for (size_t i = 0; i < keyFrameTimes.size(); ++i) {
            if (currentTime < keyFrameTimes[i]) {
                currentKeyFrameIndex = i - 1;
                nextKeyFrameIndex = i;
                break;
            }
        }
    }
}
// ��ȡ��ֵ��Ķ�������
std::vector<Vertex> Animation::GetInterpolatedVertices() const {
    // ȷ�������������ؼ�֡
    if (keyFrames.size() < 2) {
        return {}; // ���ؿյĶ�������
    }

    const auto& startFrame = keyFrames[currentKeyFrameIndex];
    const auto& endFrame = keyFrames[nextKeyFrameIndex];

    // ����ؼ�֡����������һ�£�ѡ���С���������������ȱʧ�Ķ���
    size_t numVertices = std::min(startFrame.size(), endFrame.size());

    // �����ֵ���ӣ���ǰʱ���������ؼ�֮֡��Ľ���
    float frameProgress = (currentTime - keyFrameTimes[currentKeyFrameIndex]) /
        (keyFrameTimes[nextKeyFrameIndex] - keyFrameTimes[currentKeyFrameIndex]);

    std::vector<Vertex> interpolatedVertices;
    for (size_t i = 0; i < numVertices; ++i) {
        Vertex interpolatedVertex;
        interpolatedVertex.Position = glm::mix(startFrame[i].Position, endFrame[i].Position, frameProgress);
        interpolatedVertex.Normal = glm::mix(startFrame[i].Normal, endFrame[i].Normal, frameProgress);
        interpolatedVertex.TexCoords = glm::mix(startFrame[i].TexCoords, endFrame[i].TexCoords, frameProgress);
        interpolatedVertices.push_back(interpolatedVertex);
    }

    return interpolatedVertices;
}



// ���Ŷ���
void Animation::Play() {
    isPlaying = true;
    currentTime = 0.0f;
}

// ���ö���
void Animation::Reset() {
    currentTime = 0.0f;
    isPlaying = false;
}


// �ж϶����Ƿ��ڲ���
bool Animation::IsPlaying() const {
    return isPlaying;
}




// AnimationController �๹�캯��ʵ��
AnimationController::AnimationController() : currentAnimationIndex(-1) {}

// ��Ӷ���
void AnimationController::AddAnimation( Animation* animation) {
    animations.push_back(*animation);  // ���һ����������

    std::cout <<"��������" << animations.size() << std::endl;

}

// ����ָ�������Ķ���������ͬʱ������������
void AnimationController::PlayAnimation(int index, float deltaTime,CustomModel* model)
{
   //std::cout << "��ʼ���ţ�" << index << std::endl;
        if (!ifplay)
        {
            if (index >= 0 && index < animations.size()) 
            {
                if (currentAnimationIndex != -1)
                    animations[currentAnimationIndex].Reset();

            currentAnimationIndex = index;
            animations[currentAnimationIndex].Play();
            ifplay = true;//��update�����н������ã���ֹ����ѭ��
            }
         }
        if (currentAnimationIndex != -1)
        {

            Update(deltaTime);

           model->UpdateVerticesForAnimation(GetCurrentAnimationVertices());

        }

}

// ֹͣ��ǰ����
void AnimationController::StopAnimation(int index) {
  
    if (ifplay)
    {
        if (currentAnimationIndex != -1)
            animations[currentAnimationIndex].Reset();
        ifplay = false;//��update�����н������ã���ֹ����ѭ��
    }
}

// ���¶���״̬
void AnimationController::Update(float deltaTime) {
   
        animations[currentAnimationIndex].Update(deltaTime);
    
}

// ��ȡ��ǰ�����Ĳ�ֵ��������
 std::vector<Vertex> AnimationController::GetCurrentAnimationVertices()  {
    
       
        return animations[currentAnimationIndex].GetInterpolatedVertices();
   
 
}

 bool AnimationController::Interface()
 {
     return false;
 }
