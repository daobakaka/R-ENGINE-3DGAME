#ifndef  SCRIPTMODEL_H
#define  SCRIPTMODEL_H
#include "CustomModel.h"
#include <vector>  // ȷ��ʹ���� std::vector

namespace Game
{

	class LightRender;
#pragma region ����ģ��
	//ȫ��Ψһ��ʶ
	//extern int TGUID;
	class CustomModel;
	class ShaderManager;
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

		void SetLightParameters(glm::vec3 color, float intensity, glm::vec3 direction = glm::vec3(0));

		void BindTexture() override;

	private:
		glm::vec3 _myColor;
		float _myIntensity;
		glm::vec3 _myDirection;
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
		glm::vec3 _myColor;
		float _myIntensity;
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
		CustomModelShader(const std::string& name, const ModelData& modelData, bool isSkinnedMesh, bool ifLight = false, bool ifShadow = false,bool enableDepth=false);
		virtual ~CustomModelShader() override;
		CustomModelShader();

		virtual bool Draw(glm::mat4 view, glm::mat4 projection) override;//��̬���ƿ���д

		//virtual void UpdateVariant(glm::mat4 view, glm::mat4 projection) override;//�����ƶ�����д
		//�������ͳһshader���ƿ���
		virtual bool DrawDynamical(glm::mat4 view, glm::mat4 projection) override;//��̬���ƿ���д����IntergtatedAnimatior����

		virtual void RenderingLight();//ͨ����ɫ���Ĺ�����Ⱦ
		virtual void SelfIns() override;
	protected:
		LightSpawner* lightSpawner;
		virtual void BindTexture()override;//
		virtual void UniformParametersInput() override;//ȫ��shader����������д
		virtual void BindTextureAdditional() override;//ͨ��shader����д���������Ⱦ����������Ⱦ������7����ͼ
		virtual void UniformParametersChange() override;


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
		virtual ~CustomModelInstance() override ;
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
		virtual void SpecialMethod()override;
		//�ӿ����ͼ����
		virtual void UpdateDepthViewPortPic(glm::mat4 view, glm::mat4 projection, GLuint shader) override;
		//����������
		void EnableDepthcal();

	protected:
		int _instanceCount;  // �洢ʵ������
		glm::vec3 _positionOffset;  // �洢ƽ��ƫ��
		glm::vec3 _rotationAxis;  // �洢��ת������ÿ�������ת������
		GLuint _instanceBuffer;
		std::vector<glm::mat4> _modelMatrices;  // ʹ�� std::vector �洢��̬������ʵ������
		LightRender* _lightRender;
		bool _useViewPortDepthMap;
		virtual void GenerateInstanceMatrices(ModelClass type = ModelClass::InstanceCube);
		virtual void UniformParametersInput() override;
		virtual void BindTexture()override;
		//ʵ��������д�����������Ϊ�շ�������ΪĿǰֻ֧�ֻ�������
		virtual void BindTextureAdditional() override;
		//ʵ�����任
		virtual void UpadeInstanceMatrices();
	};

	
	/// <summary>
	/// ʵ�����̳� ө���
	/// </summary>

	class FireflyInstance :public CustomModelInstance
	{
	public:
		using CustomModelInstance :: CustomModelInstance;
		void SelfIns() override;
		void UpdateVariant(glm::mat4 view, glm::mat4 projection) override;//��д������·���������ө��������˶�
	protected:
		std::vector<InstanceData> _instanceData; // �洢ÿ��ʵ��������Ŷ�ֵ
		void InitializeInstanceData(); // ��ʼ��ÿ��ʵ��������Ŷ�ֵ
		//ө��涯̬����
		virtual void  UniformParametersChange() override;

	};

#pragma endregion

#pragma region  ����ģ��
	/// <summary>
	/// ���������ڿɸ���
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
#pragma region  �ڶ�
	/// <summary>
	/// ��������
	/// </summary>
	class BlackHole :public CustomModelShader
	{
	public:
		//����ֱ�Ӽ̳л���Ĺ��캯��
		using CustomModelShader::CustomModelShader;
		void UpdateVariant(glm::mat4 view, glm::mat4 projection) override;
	protected:
		void UniformParametersChange() override;//�ڶ�����������д����

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
		//���ⷽ��
		void SpecialMethod() override;
	private:
		LifecycleManager<CustomModel>* _manager;
		Controller* _controller;
		ShaderManager* _shaderManager;
	protected:	
		void UniformParametersChange() override;//�������������д����
		void RenderingStencilTest();
	};


#pragma endregion
#pragma region ��Ϸ���߰��

/// <summary>
/// �����ӵ�
/// </summary>
	class GameBullet :public CustomModelShader
	{

	public:
		using CustomModelShader::CustomModelShader;
		void UpdateVariant(glm::mat4 view, glm::mat4 projection) override;
		void SelfIns() override;//��ʼ���������
	protected:

		float _waveTime ; // ��ȡ��ǰʱ��
		float _waveAmplitude ; // ���˷���
		float _waveFrequency; // ����Ƶ��
		float _waveSpeed ;     // �����ٶ�
		void UniformParametersChange() override;//��дͨ��shader�Ĵ��Σ��ı�״̬

	};
#pragma endregion
	/// <summary>
	/// ʯͷ��
	/// </summary>
	class GameStoneMonser :public CustomModelShader
	{
	public:
		using CustomModelShader::CustomModelShader;
	    void UpdateVariant(glm::mat4 view, glm::mat4 projection) override; //�����ƶ�
		void SelfIns() override;//��ʼ���������


	protected:
		CustomModel* _player;
		float _speed;
		bool _alive;
		float _deathTime;
		//�������Ʋ���
		bool _isRunA;
		bool _isAttackA;
		bool _isDeathA;
		void UniformParametersChange()override;//��дͨ��shader�Ĵ��Σ��ı�״̬
	};
	

	
}

#endif // ! SCRIPTMODEL_H