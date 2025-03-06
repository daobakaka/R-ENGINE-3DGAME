#ifndef  SCRIPTMODEL_H
#define  SCRIPTMODEL_H
#include "CustomModel.h"
#include <vector>  // 确保使用了 std::vector

namespace Game
{


#pragma region 特殊模型
	//全局唯一标识
	//extern int TGUID;
	class CustomModel;
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

		void SteLightParameters(glm::vec3 color, float intensity, glm::vec3 direction = glm::vec3(0));

		void RenderingTexture() override;

	private:
		glm::vec3 myColor;
		float myIntensity;
		glm::vec3 myDirection;
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
		glm::vec3 myColor;
		float myIntensity;
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
		CustomModelShader(const std::string& name, const ModelData& modelData, bool isSkinnedMesh, bool ifLight = false, bool ifShadow = false);

		CustomModelShader();

		virtual bool Draw(glm::mat4 view, glm::mat4 projection) override;//静态绘制可重写

		//virtual void UpdateVariant(glm::mat4 view, glm::mat4 projection) override;//变体移动可重写

		virtual bool DrawDynamical(glm::mat4 view, glm::mat4 projection) override;//动态绘制可重写，与IntergtatedAnimatior联动

		virtual void RenderingTexture()override;

	protected:

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

		CustomModelInstance(const std::string& name,
			const ModelData& modelData,
			bool isSkinnedMesh,
			bool ifLightIn,
			bool ifShadow,
			int instanceCount = 100,  // 实例化数量，默认为 100
			glm::vec3 positionOffset = glm::vec3(0.0f),  // 平移偏移，默认为 (0, 0, 0)
			glm::vec3 rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f)); // 旋转增量，通过 rotationAxis 控制


		virtual bool Draw(glm::mat4 view, glm::mat4 projection) override;//静态绘制可重写

		//virtual void UpdateVariant(glm::mat4 view, glm::mat4 projection) override;//变体移动可重写

		virtual bool DrawDynamical(glm::mat4 view, glm::mat4 projection) override;//动态绘制可重写，与IntergtatedAnimatior联动


		virtual void RenderingTexture()override;
		virtual void GenerateInstanceMatrices();

	protected:
		int _instanceCount;  // 存储实例数量
		glm::vec3 _positionOffset;  // 存储平移偏移
		glm::vec3 _rotationAxis;  // 存储旋转增量（每个轴的旋转增量）
		GLuint _instanceBuffer;
		std::vector<glm::mat4> _modelMatrices;  // 使用 std::vector 存储动态数量的实例矩阵

	};

#pragma endregion

#pragma region  测试蝴蝶模型
	/// <summary>
	/// 用于测试的蝴蝶，后期可更改
	/// </summary>
	class ButterflyScriptShader :public CustomModelShader
	{
	public:
		//这里直接继承基类的构造函数
		using CustomModelShader::CustomModelShader;
		void UpdateVariant(glm::mat4 view, glm::mat4 projection) override;

	private:


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
		
	private:
		LifecycleManager<CustomModel>* _manager;
		Controller* _controller;

		void PlayerController(GLFWwindow* window );


	};




#pragma endregion
#pragma region 游戏道具板块


	/// <summary>
/// 子弹
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