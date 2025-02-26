#ifndef LIGHT_H
#define LIGHT_H

#include <vector>
#include <glm/glm.hpp>
#include "GameObject.h"

namespace Game {


    enum ShaderClass;
    
    // ������Դ�ṹ��
    struct CustomPointLight {
        glm::vec3 position;
        glm::vec3 color;       // RGB ��Դ��ɫ
        float intensity;
    };

    // ����ƽ�й⣨����⣩��ȫ��ֻ��һ��
    struct CustomParallelLight {
        glm::vec3 direction;   // ���߷���
        glm::vec3 color;
        float intensity;
    };

    // �����ֵ�Ͳ�⣨�۹�ƣ�
    struct CustomFlashLight {
        glm::vec3 position;
        glm::vec3 direction;
        glm::vec3 color;
        float intensity;
        //�۹�ǻ���
        float cutoff;       
    };

    class LightSpawner {
    public:
        // ��ȡ����ʵ��
        static LightSpawner* GetInstance();

        // �������Դ����ֵ���Σ������洢���ڲ�������
        CustomPointLight SpawPointLight(glm::vec3 position = glm::vec3(0.0f),
            glm::vec3 color = glm::vec3(1.0f),
            float intensity = 1.0f);

        // ��ʼ�������ȫ��ƽ�й�
        CustomParallelLight SpawParallelLight(glm::vec3 direction = glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3 color = glm::vec3(1.0f),
            float intensity = 10.0f);
        // �����ֵ�Ͳ�⣨�۹�ƣ���Ĭ��λ��Ϊ (1, 1, 1)��Ĭ�Ϸ���ָ��ԭ��
        CustomFlashLight SpawFlashLight(
            glm::vec3 position = glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3 direction = glm::vec3(-0.57735f, -0.57735f, -0.57735f), // ָ��ԭ��Ĺ�һ������
            glm::vec3 color = glm::vec3(1.0f, .0f, .0f),//Ĭ�Ϻ�ɫ
            float intensity = 10.0f,
            float cutoff = cos(glm::radians(12.5f))
        );

        //ƽ�й������
        void ParalletLightController(glm::vec3 dirction=glm::vec3(-1), glm::vec3 color = glm::vec3(1), float intensity = 10.0f);
        //���Դ��������Ŀǰ֧��4��
        void PointLightController( glm::vec3 color = glm::vec3(1), float intensity = 1.0f,int lightNumber=0);
        //�ֵ�Ͳ���������Ŀǰ֧��4��
        void FlashLightController(glm::vec3 dirction = glm::vec3(-1), glm::vec3 color = glm::vec3(1), float intensity = 20.0f,float cutoff=12.5f,int lightNumber=0);


        // ��ȡ���е��Դ������,ǰ��const ��ֹ����������const ȷ���ⲿ������ø÷����������������const���ͣ��ؿ�Χ
        const std::vector<CustomPointLight>& GetPointLights() const { return pointLights; }

        // ��ȡȫ��ƽ�й�����
        const CustomParallelLight& GetParallelLight() const { return parallelLight; }

        // ��ȡ�����ֵ�Ͳ�������
        const std::vector<CustomFlashLight>& GetFlashLights() const { return flashLights; }

        //�Ƿ񿪵ƹ�ģ�ͱ�ʶ
        bool modelIdentification;

    private:
        LightSpawner();
        ~LightSpawner();
        LightSpawner(const LightSpawner&) = delete;
        LightSpawner& operator=(const LightSpawner&) = delete;

        // �ڲ�����
        std::vector<CustomPointLight> pointLights;
        CustomParallelLight parallelLight;
        std::vector<CustomFlashLight> flashLights;

        static LightSpawner* instance;
       
    };

    class LightRender {
    public:
        static LightRender* GetInstance();

        // RenderLights: �����й�Դ�������� shaderProgram �У�
        // controllerPtr Ϊ������ָ�룬spawnerPtr Ϊ LightSpawner ָ��
        void RenderLights(GLuint shaderProgram, const Controller* controllerPtr, const Game::LightSpawner* spawnerPtr,glm::vec3 position);
        /// <summary>
        /// ����ƽ�й���Ӱ��ͼ,������Ӱ�����ɫ��
        /// </summary>
        /// <returns></returns>
        GLuint CreateShadowMapForParallelLight();
        /// <summary>
        ///����ƽ�й��ӽǾ���
        /// </summary>
        void RenderDepthMapForParallelLight( glm::vec3 lightDirection);
        glm::mat4 GetLightMatrix();

        GLuint CompileShader(const char* ver, const char* fra);
        //��ȡ�����ɫ��
        GLuint GetDepthShaderProgram(ShaderClass shader);
        //ʹ�������ɫ��
        void UseDepthShaderProgram(ShaderClass shader);
        //�����Ȼ�����
        void UnbindFramebuffer();
        //����Ȼ�����
        void BindFramebuffer();
        //��Ⱦ��Ӱͼ
        void RenderShadowTexture(CustomModel* obj,glm::mat4 crossView);
        //��Ⱦ��Ӱͼ������,������Ҫʹ��NDC����׼�豸����ռ䣬����������ͶӰ��
        void RenderShadowTexture(GLuint shader);

        
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
        GLuint _depthTestShader;
        GLuint _depthVisualShader;
        glm::mat4 _lightSpaceMatrix;
        

    private:
        GLuint quadVAO = 0;
        GLuint quadVBO;
    };




} // namespace Game

#endif // LIGHT_H
