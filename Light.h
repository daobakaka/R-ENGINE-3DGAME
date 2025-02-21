#ifndef LIGHT_H
#define LIGHT_H

#include <vector>
#include <glm/glm.hpp>
#include "GameObject.h"

namespace Game {

    // 定义点光源结构体
    struct CustomPointLight {
        glm::vec3 position;
        glm::vec3 color;       // RGB 光源颜色
        float intensity;
    };

    // 定义平行光（方向光），全局只有一个
    struct CustomParallelLight {
        glm::vec3 direction;   // 光线方向
        glm::vec3 color;
        float intensity;
    };

    // 定义手电筒光（聚光灯）
    struct CustomFlashLight {
        glm::vec3 position;
        glm::vec3 direction;
        glm::vec3 color;
        float intensity;
        //聚光角弧度
        float cutoff;       
    };

    class LightSpawner {
    public:
        // 获取单例实例
        static LightSpawner* GetInstance();

        // 创建点光源（按值传参），并存储到内部容器中
        CustomPointLight SpawPointLight(glm::vec3 position = glm::vec3(0.0f),
            glm::vec3 color = glm::vec3(1.0f),
            float intensity = 1.0f);

        // 初始化或更新全局平行光
        CustomParallelLight SpawParallelLight(glm::vec3 direction = glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3 color = glm::vec3(1.0f),
            float intensity = 10.0f);
        // 创建手电筒光（聚光灯），默认位置为 (1, 1, 1)，默认方向指向原点
        CustomFlashLight SpawFlashLight(
            glm::vec3 position = glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3 direction = glm::vec3(-0.57735f, -0.57735f, -0.57735f), // 指向原点的归一化向量
            glm::vec3 color = glm::vec3(1.0f, .0f, .0f),//默认红色
            float intensity = 10.0f,
            float cutoff = cos(glm::radians(12.5f))
        );

        //平行光控制器
        void ParalletLightController(glm::vec3 dirction=glm::vec3(-1), glm::vec3 color = glm::vec3(1), float intensity = 10.0f);
        //点光源控制器，目前支持4个
        void PointLightController( glm::vec3 color = glm::vec3(1), float intensity = 1.0f,int lightNumber=0);
        //手电筒光控制器，目前支持4个
        void FlashLightController(glm::vec3 dirction = glm::vec3(-1), glm::vec3 color = glm::vec3(1), float intensity = 20.0f,float cutoff=12.5f,int lightNumber=0);


        // 获取所有点光源的容器,前置const 防止拷贝，后置const 确保外部对象调用该方法，对象本身可以是const类型，拓宽范围
        const std::vector<CustomPointLight>& GetPointLights() const { return pointLights; }

        // 获取全局平行光数据
        const CustomParallelLight& GetParallelLight() const { return parallelLight; }

        // 获取所有手电筒光的容器
        const std::vector<CustomFlashLight>& GetFlashLights() const { return flashLights; }

        //是否开灯光模型标识
        bool modelIdentification;

    private:
        LightSpawner();
        ~LightSpawner();
        LightSpawner(const LightSpawner&) = delete;
        LightSpawner& operator=(const LightSpawner&) = delete;

        // 内部容器
        std::vector<CustomPointLight> pointLights;
        CustomParallelLight parallelLight;
        std::vector<CustomFlashLight> flashLights;

        static LightSpawner* instance;
       
    };

    class LightRender {
    public:
        static LightRender* GetInstance();

        // RenderLights: 将所有光源参数传入 shaderProgram 中，
        // controllerPtr 为控制器指针，spawnerPtr 为 LightSpawner 指针
        void RenderLights(GLuint shaderProgram, const Controller* controllerPtr, const Game::LightSpawner* spawnerPtr,glm::vec3 position);
        /// <summary>
        /// 创建平行光阴影贴图,编译阴影深度着色器
        /// </summary>
        /// <returns></returns>
        GLuint CreateShadowMapForParallelLight();
        /// <summary>
        ///计算平行光视角矩阵
        /// </summary>
        void RenderDepthMapForParallelLight( glm::vec3 lightDirection);
        glm::mat4 GetLightMatrix();
        //获取深度着色器
        GLuint GetDepthShaderProgram();
        //使用深度着色器
        void UseDepthShaderProgram();

        void UnbindFramebuffer();

        void BindFramebuffer();
    private:
        LightRender();
        ~LightRender();
        LightRender(const LightRender&) = delete;
        LightRender& operator=(const LightRender&) = delete;

        static LightRender* instance;

    public:
        GLuint _depthMapParallelFBO;
    protected:
        GLuint _depthMapParallel;
        GLuint _depthShaderProgram;
        glm::mat4 _lightSpaceMatrix;
        
    };




} // namespace Game

#endif // LIGHT_H
