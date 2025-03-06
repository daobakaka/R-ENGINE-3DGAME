#ifndef  SCRIPTMODEL_H
#define  SCRIPTMODEL_H
#include "CustomModel.h"
#include <vector>  // ȷ��ʹ���� std::vector

namespace Game
{


#pragma region ����ģ��
	//ȫ��Ψһ��ʶ
	//extern int TGUID;
	class CustomModel;
	/// <summary>
	/// ��ʼ����ģ��
	/// </summary>
	class ButterflyScript :public CustomModel
	{
	public:
		//����ֱ�Ӽ̳л���Ĺ��캯��
		using CustomModel::CustomModel;
		void UpdateVariant(glm::mat4 view, glm::mat4 projection) override;

	private:

	};
	/// <summary>
	/// �ο�����ϵģ��
	/// </summary>
	class CoordinateSystemCus : public CustomModel
	{

	public:
		using CustomModel::CustomModel;

		void Update(glm::mat4 view, glm::mat4 projection) override;

	private:

	};
	/// <summary>
	/// �ƹ��ʶģ��
	/// </summary>
	class LightModel :public CustomModel
	{
		using CustomModel::CustomModel;
	public:

		void SteLightParameters(glm::vec3 color, float intensity, glm::vec3 direction = glm::vec3(0));

		void RenderingTexture() override;

	private:
		glm::vec3 myColor;
		float myIntensity;
		glm::vec3 myDirection;
	};
	/// <summary>
	/// �Զ�������
	/// </summary>

	class CustomizeRay : public CustomModel
	{
		using CustomModel::CustomModel;
	public:
		void Update(glm::mat4 view, glm::mat4 projection) override;
		bool DrawLine(glm::mat4 view, glm::mat4 projection)override;
		void SetRayPar(glm::vec3 color, float intensity);


	protected:
		glm::vec3 myColor;
		float myIntensity;
		glm::vec3 myDirection;


	};

	/// <summary>
	/// ��Ӱ��ͼ
	/// </summary>
	class ShadowTexture :public CustomModel
	{
		using CustomModel::CustomModel;
	public:
		void DrawDepthPic(glm::mat4 lightSpaceMatrix, GLuint shader) override;
		void DrawDepthPicDynamical(glm::mat4 lightSpaceMatrix, GLuint shader) override;
	protected:
		GLuint _depthMap;
	};
#pragma endregion

#pragma region ͨ��Shaderģ��
	/// <summary>
	/// ͨ��ģ��
	/// </summary>
	class CustomModelShader :public CustomModel
	{

	public:
		/// <summary>
		/// ����ֵ����shader���ⲿ����Ҫ��ʾ���ã����ٶ���shader���ظ�����
		/// </summary>
		/// <param name="shader"></param>
		/// <param name="modelData"></param>
		/// <param name="isSkinnedMesh"></param>
		/// <param name="ifLight"></param>
		/// <param name="ifShadow"></param>
		CustomModelShader(const std::string& name, const ModelData& modelData, bool isSkinnedMesh, bool ifLight = false, bool ifShadow = false);

		CustomModelShader();

		virtual bool Draw(glm::mat4 view, glm::mat4 projection) override;//��̬���ƿ���д

		//virtual void UpdateVariant(glm::mat4 view, glm::mat4 projection) override;//�����ƶ�����д

		virtual bool DrawDynamical(glm::mat4 view, glm::mat4 projection) override;//��̬���ƿ���д����IntergtatedAnimatior����

		virtual void RenderingTexture()override;

	protected:

	};


#pragma endregion


#pragma region ʵ����ģ��

	/// <summary>
	/// ʵ����ģ��
	/// </summary>
	class CustomModelInstance :public CustomModel
	{
	public:
		CustomModelInstance();

		CustomModelInstance(const std::string& name,
			const ModelData& modelData,
			bool isSkinnedMesh,
			bool ifLightIn,
			bool ifShadow,
			int instanceCount = 100,  // ʵ����������Ĭ��Ϊ 100
			glm::vec3 positionOffset = glm::vec3(0.0f),  // ƽ��ƫ�ƣ�Ĭ��Ϊ (0, 0, 0)
			glm::vec3 rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f)); // ��ת������ͨ�� rotationAxis ����


		virtual bool Draw(glm::mat4 view, glm::mat4 projection) override;//��̬���ƿ���д

		//virtual void UpdateVariant(glm::mat4 view, glm::mat4 projection) override;//�����ƶ�����д

		virtual bool DrawDynamical(glm::mat4 view, glm::mat4 projection) override;//��̬���ƿ���д����IntergtatedAnimatior����


		virtual void RenderingTexture()override;
		virtual void GenerateInstanceMatrices();

	protected:
		int _instanceCount;  // �洢ʵ������
		glm::vec3 _positionOffset;  // �洢ƽ��ƫ��
		glm::vec3 _rotationAxis;  // �洢��ת������ÿ�������ת������
		GLuint _instanceBuffer;
		std::vector<glm::mat4> _modelMatrices;  // ʹ�� std::vector �洢��̬������ʵ������

	};

#pragma endregion

#pragma region  ���Ժ���ģ��
	/// <summary>
	/// ���ڲ��Եĺ��������ڿɸ���
	/// </summary>
	class ButterflyScriptShader :public CustomModelShader
	{
	public:
		//����ֱ�Ӽ̳л���Ĺ��캯��
		using CustomModelShader::CustomModelShader;
		void UpdateVariant(glm::mat4 view, glm::mat4 projection) override;

	private:


	};

#pragma endregion


#pragma region ���ģ��
	/// <summary>
	/// �½���ң���������ư��ڵ������ӽǵ�ģʽ
	/// </summary>

	class GamePlayer :public CustomModelShader
	{
	public:
		using CustomModelShader::CustomModelShader;

		void UpdateVariant(glm::mat4 view, glm::mat4 projection) override;

		void Start() override;//��д��ʼ������
		
	private:
		LifecycleManager<CustomModel>* _manager;
		Controller* _controller;

		void PlayerController(GLFWwindow* window );


	};




#pragma endregion
#pragma region ��Ϸ���߰��


	/// <summary>
/// �ӵ�
/// </summary>
	class GameBullet :public CustomModelShader
	{

	public:
		using CustomModelShader::CustomModelShader;
		void UpdateVariant(glm::mat4 view, glm::mat4 projection) override;



	};
#pragma endregion

	
}

#endif // ! SCRIPTMODEL_H