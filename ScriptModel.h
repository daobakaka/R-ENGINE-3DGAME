#ifndef  SCRIPTMODEL_H
#define  SCRIPTMODEL_H
#include "CustomModel.h"

namespace Game
{

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
		
		 void SteLightParameters(glm::vec3 color, float intensity,glm::vec3 direction=glm::vec3(0));

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
		void SetRayPar(glm::vec3 color,float intensity);


	protected:
		glm::vec3 myColor;
		float myIntensity;
		glm::vec3 myDirection;


	};


}




#endif // ! SCRIPTMODEL_H

