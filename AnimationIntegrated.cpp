#include "AnimationIntegrated.h"
#include <iostream> // 仅用于调试
#include "CustomModel.h"
using namespace Game;
// Animation 类构造函数实现
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


// 更新动画状态
void Animation::Update(float deltaTime) {
    if (isPlaying) {
        currentTime += deltaTime * speed;  // 根据播放速度调整当前时间
        if (currentTime >= keyFrameTimes.back()) {
            if (loop) {
                currentTime = 0.0f;  // 循环播放
            }
            else {
                currentTime = keyFrameTimes.back();  // 动画结束
                isPlaying = false;
            }
        }

        // 根据时间更新当前关键帧的索引
        for (size_t i = 0; i < keyFrameTimes.size(); ++i) {
            if (currentTime < keyFrameTimes[i]) {
                currentKeyFrameIndex = i - 1;
                nextKeyFrameIndex = i;
                break;
            }
        }
    }
}
// 获取插值后的顶点数据
std::vector<Vertex> Animation::GetInterpolatedVertices() const {
    // 确保有至少两个关键帧
    if (keyFrames.size() < 2) {
        return {}; // 返回空的顶点数据
    }

    const auto& startFrame = keyFrames[currentKeyFrameIndex];
    const auto& endFrame = keyFrames[nextKeyFrameIndex];

    // 如果关键帧顶点数量不一致，选择较小的数量，或者填充缺失的顶点
    size_t numVertices = std::min(startFrame.size(), endFrame.size());

    // 计算插值因子：当前时间在两个关键帧之间的进度
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



// 播放动画
void Animation::Play() {
    isPlaying = true;
    currentTime = 0.0f;
}

// 重置动画
void Animation::Reset() {
    currentTime = 0.0f;
    isPlaying = false;
}


// 判断动画是否在播放
bool Animation::IsPlaying() const {
    return isPlaying;
}




// AnimationController 类构造函数实现
AnimationController::AnimationController() : currentAnimationIndex(-1) {}

// 添加动画
void AnimationController::AddAnimation( Animation* animation) {
    animations.push_back(*animation);  // 添加一个动画对象

    std::cout <<"动画长度" << animations.size() << std::endl;

}

// 播放指定索引的动画，不能同时播放两个动画
void AnimationController::PlayAnimation(int index, float deltaTime,CustomModel* model)
{
   //std::cout << "开始播放：" << index << std::endl;
        if (!ifplay)
        {
            if (index >= 0 && index < animations.size()) 
            {
                if (currentAnimationIndex != -1)
                    animations[currentAnimationIndex].Reset();

            currentAnimationIndex = index;
            animations[currentAnimationIndex].Play();
            ifplay = true;//在update函数中进行设置，防止持续循环
            }
         }
        if (currentAnimationIndex != -1)
        {

            Update(deltaTime);

           model->UpdateVerticesForAnimation(GetCurrentAnimationVertices());

        }

}

// 停止当前动画
void AnimationController::StopAnimation(int index) {
  
    if (ifplay)
    {
        if (currentAnimationIndex != -1)
            animations[currentAnimationIndex].Reset();
        ifplay = false;//在update函数中进行设置，防止持续循环
    }
}

// 更新动画状态
void AnimationController::Update(float deltaTime) {
   
        animations[currentAnimationIndex].Update(deltaTime);
    
}

// 获取当前动画的插值顶点数据
 std::vector<Vertex> AnimationController::GetCurrentAnimationVertices()  {
    
       
        return animations[currentAnimationIndex].GetInterpolatedVertices();
   
 
}

 bool AnimationController::Interface()
 {
     return false;
 }
