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

		virtual void RenderingTextureAdditional() override;//��д����������;

		virtual void RenderingLight();//ͨ����ɫ���Ĺ�����Ⱦ
		virtual void SelfIns();
	protected:
		LightSpawner* lightSpawner;



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
			glm::vec3 rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f),
			ModelClass type = ModelClass::InstanceCube); // ��ת������ͨ�� rotationAxis ����


		virtual bool Draw(glm::mat4 view, glm::mat4 projection) override;//��̬���ƿ���д

		//virtual void UpdateVariant(glm::mat4 view, glm::mat4 projection) override;//�����ƶ�����д

		virtual bool DrawDynamical(glm::mat4 view, glm::mat4 projection) override;//��̬���ƿ���д����IntergtatedAnimatior����


		virtual void RenderingTexture()override;
		virtual void GenerateInstanceMatrices(ModelClass type=ModelClass::InstanceCube);

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
		void SelfIns() override;
	private:
		// �ڲ����в���
		float _circleRadius;    // ˮƽԲ���˶��뾶�������Χ��[5,15]��
		float _horizontalSpeed; // ˮƽ���ٶȣ���λ/�룬�����Χ���� [3,6]��
		float _xzStartAngle;    // ��ʼ�Ƕȣ����ȣ�
		float _verticalSpeed;   // ��ֱ�����ٶȣ���λ/�룬�����Χ��[0.5,2.0]��
		float _baseY;           // ��׼ Y ���꣨ͨ��Ϊ 0��

		// �񵴲��������ڴ�ֱ����Ĳ�����
		float _oscStartTime;           // �񵴿�ʼʱ�䣨�룬�����Χ��[10,20]��
		float _oscillationAmplitude;   // ������������Χ��[0.5,2.0]��
		float _oscillationFrequency;   // ��Ƶ�ʣ�����/�룬�����Χ��[1,3]��
		float _dt;
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

		void  UniformParametersInput() override;//�������������д����
		
	private:
		LifecycleManager<CustomModel>* _manager;
		Controller* _controller;



	};

	class NoneLightModel :public CustomModelShader
	{

	public :
		using CustomModelShader::CustomModelShader;
		void   UniformParametersInput() override;//�������������д����







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

		void  UniformParametersInput();//��дͨ��shader�Ĵ��Σ��ı�״̬

	};
#pragma endregion

	
}

#endif // ! SCRIPTMODEL_H