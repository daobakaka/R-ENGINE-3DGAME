#ifndef  SCRIPTMODEL_H
#define  SCRIPTMODEL_H
#include "CustomModel.h"
#include <vector>  // 确保使用了 std::vector

namespace Game
{

	class LightRender;
#pragma region 特殊模型
	//全局唯一标识
	//extern int TGUID;
	class CustomModel;
	class ShaderManager;
	/// <summary>
	/// 初始蝴蝶模型
	/// </summary>
	class ButterflyScript :public CustomModel
	{
	public:
		//这里直接继承基类的构造函数
		using CustomModel::CustomModel;
		void UpdateVariant(glm::mat4 view, glm::mat4 projection) override;

	private:

	};
	/// <summary>
	/// 参考坐标系模型
	/// </summary>
	class CoordinateSystemCus : public CustomModel
	{

	public:
		using CustomModel::CustomModel;

		void Update(glm::mat4 view, glm::mat4 projection) override;

	private:

	};
	/// <summary>
	/// 灯光标识模型
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
	/// 自定义射线
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
	/// 阴影贴图
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

#pragma region 通用Shader模型
	/// <summary>
	/// 通用模型
	/// </summary>
	class CustomModelShader :public CustomModel
	{

	public:
		/// <summary>
		/// 采用值传递shader，外部不需要显示调用，减少独立shader的重复计算
		/// </summary>
		/// <param name="shader"></param>
		/// <param name="modelData"></param>
		/// <param name="isSkinnedMesh"></param>
		/// <param name="ifLight"></param>
		/// <param name="ifShadow"></param>
		CustomModelShader(const std::string& name, const ModelData& modelData, bool isSkinnedMesh, bool ifLight = false, bool ifShadow = false,bool enableDepth=false);
		virtual ~CustomModelShader() override;
		CustomModelShader();

		virtual bool Draw(glm::mat4 view, glm::mat4 projection) override;//静态绘制可重写

		//virtual void UpdateVariant(glm::mat4 view, glm::mat4 projection) override;//变体移动可重写
		//允许采用统一shader绘制开关
		virtual bool DrawDynamical(glm::mat4 view, glm::mat4 projection) override;//动态绘制可重写，与IntergtatedAnimatior联动

		virtual void RenderingLight();//通过着色器的光照渲染
		virtual void SelfIns() override;
	protected:
		LightSpawner* lightSpawner;
		virtual void BindTexture()override;//
		virtual void UniformParametersInput() override;//全局shader参数输入重写
		virtual void BindTextureAdditional() override;//通用shader类重写基类额外渲染方法，以渲染完整的7张贴图
		virtual void UniformParametersChange() override;


	};


#pragma endregion


#pragma region 实例化模型

	/// <summary>
	/// 实例化模型
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
			int instanceCount = 100,  // 实例化数量，默认为 100
			glm::vec3 positionOffset = glm::vec3(0.0f),  // 平移偏移，默认为 (0, 0, 0)
			glm::vec3 rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f),
			ModelClass type = ModelClass::InstanceCube); // 旋转增量，通过 rotationAxis 控制

		virtual bool Draw(glm::mat4 view, glm::mat4 projection) override;//静态绘制可重写
		//virtual void UpdateVariant(glm::mat4 view, glm::mat4 projection) override;//变体移动可重写
		virtual bool DrawDynamical(glm::mat4 view, glm::mat4 projection) override;//动态绘制可重写，与IntergtatedAnimatior联动
		virtual void SpecialMethod()override;
		//视口深度图绘制
		virtual void UpdateDepthViewPortPic(glm::mat4 view, glm::mat4 projection, GLuint shader) override;
		//允许测试深度
		void EnableDepthcal();

	protected:
		int _instanceCount;  // 存储实例数量
		glm::vec3 _positionOffset;  // 存储平移偏移
		glm::vec3 _rotationAxis;  // 存储旋转增量（每个轴的旋转增量）
		GLuint _instanceBuffer;
		std::vector<glm::mat4> _modelMatrices;  // 使用 std::vector 存储动态数量的实例矩阵
		LightRender* _lightRender;
		bool _useViewPortDepthMap;
		virtual void GenerateInstanceMatrices(ModelClass type = ModelClass::InstanceCube);
		virtual void UniformParametersInput() override;
		virtual void BindTexture()override;
		//实例化类重写额外绑定纹理方法为空方法，因为目前只支持基础纹理
		virtual void BindTextureAdditional() override;
		//实例化变换
		virtual void UpadeInstanceMatrices();
	};

	
	/// <summary>
	/// 实例化继承 萤火虫
	/// </summary>

	class FireflyInstance :public CustomModelInstance
	{
	public:
		using CustomModelInstance :: CustomModelInstance;
		void SelfIns() override;
		void UpdateVariant(glm::mat4 view, glm::mat4 projection) override;//重写变体更新方法，构建萤火虫自身运动
	protected:
		std::vector<InstanceData> _instanceData; // 存储每个实例的随机扰动值
		void InitializeInstanceData(); // 初始化每个实例的随机扰动值
		//萤火虫动态发光
		virtual void  UniformParametersChange() override;

	};

#pragma endregion

#pragma region  蝴蝶模型
	/// <summary>
	/// 蝴蝶，后期可更改
	/// </summary>
	class ButterflyScriptShader :public CustomModelShader
	{
	public:
		//这里直接继承基类的构造函数
		using CustomModelShader::CustomModelShader;
		void UpdateVariant(glm::mat4 view, glm::mat4 projection) override;
		void SelfIns() override;
	private:
		// 内部飞行参数
		float _circleRadius;    // 水平圆周运动半径（随机范围：[5,15]）
		float _horizontalSpeed; // 水平线速度（单位/秒，随机范围例如 [3,6]）
		float _xzStartAngle;    // 初始角度（弧度）
		float _verticalSpeed;   // 垂直上升速度（单位/秒，随机范围：[0.5,2.0]）
		float _baseY;           // 基准 Y 坐标（通常为 0）

		// 振荡参数（用于垂直方向的波动）
		float _oscStartTime;           // 振荡开始时间（秒，随机范围：[10,20]）
		float _oscillationAmplitude;   // 振荡振幅（随机范围：[0.5,2.0]）
		float _oscillationFrequency;   // 振荡频率（弧度/秒，随机范围：[1,3]）
		float _dt;
	};

#pragma endregion
#pragma region  黑洞
	/// <summary>
	/// 巨物天体
	/// </summary>
	class BlackHole :public CustomModelShader
	{
	public:
		//这里直接继承基类的构造函数
		using CustomModelShader::CustomModelShader;
		void UpdateVariant(glm::mat4 view, glm::mat4 projection) override;
	protected:
		void UniformParametersChange() override;//黑洞类在这里重写传参

	};

#pragma endregion

#pragma region 玩家模型
	/// <summary>
	/// 新建玩家，拟采用类似暗黑的锁定视角的模式
	/// </summary>

	class GamePlayer :public CustomModelShader
	{
	public:
		using CustomModelShader::CustomModelShader;

		void UpdateVariant(glm::mat4 view, glm::mat4 projection) override;

		void Start() override;//重写初始化代码
		//特殊方法
		void SpecialMethod() override;
	private:
		LifecycleManager<CustomModel>* _manager;
		Controller* _controller;
		ShaderManager* _shaderManager;
	protected:	
		void UniformParametersChange() override;//玩家类在这里重写传参
		void RenderingStencilTest();
	};


#pragma endregion
#pragma region 游戏道具板块

/// <summary>
/// 气泡子弹
/// </summary>
	class GameBullet :public CustomModelShader
	{

	public:
		using CustomModelShader::CustomModelShader;
		void UpdateVariant(glm::mat4 view, glm::mat4 projection) override;
		void SelfIns() override;//初始化随机参数
	protected:

		float _waveTime ; // 获取当前时间
		float _waveAmplitude ; // 波浪幅度
		float _waveFrequency; // 波浪频率
		float _waveSpeed ;     // 波浪速度
		void UniformParametersChange() override;//重写通用shader的传参，改变状态

	};
#pragma endregion
	/// <summary>
	/// 石头怪
	/// </summary>
	class GameStoneMonser :public CustomModelShader
	{
	public:
		using CustomModelShader::CustomModelShader;
	    void UpdateVariant(glm::mat4 view, glm::mat4 projection) override; //自主移动
		void SelfIns() override;//初始化随机参数


	protected:
		CustomModel* _player;
		float _speed;
		bool _alive;
		float _deathTime;
		//动画控制参数
		bool _isRunA;
		bool _isAttackA;
		bool _isDeathA;
		void UniformParametersChange()override;//重写通用shader的传参，改变状态
	};
	

	
}

#endif // ! SCRIPTMODEL_H