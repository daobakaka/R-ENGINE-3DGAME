#ifndef ENUM_TOTAL_H
#define ENUM_TOTAL_H
#include <glm/glm.hpp>
namespace Game {
	enum ModelClass
	{
		OriginalE,
		CubeE,
		CubeTestE,
		CubeEVariant1,
		CubeEVarinat2,
		ActorButterfly,
		TsetButterfly,
		LightColorTestCube,
		Ray,//射线
		AxisWidget,//坐标轴模型
		PointLight,//点光源
		FlashLight,//手电筒
		ParallelLight,//平行光
		Skybox,//天空盒
		StaticPlane,//静态地板
		
	};
	enum ShaderClass
	{
		OriginalS,
		DepthCalculate,
		DepthRender,
		DepthTest,
	};
	enum CollisionType
	{
		Box,
		Sphere,
	};
	//步长结构体
	struct StepVector3
	{
		glm::vec3 position = glm::vec3(0, 0, 0);
		glm::vec3 rotation = glm::vec3(0, 0, 0);
		glm::vec3 scale = glm::vec3(1);
	};
	//碰撞体结构体
	struct CollisionProperties {
		glm::vec3 position;    // 物体位置
		glm::vec3 velocity;    // 物体速度
		glm::vec3 acceleration;  // 物体加速度
		float mass;            // 物体质量

		CollisionProperties()
			: position(0.0f), velocity(0.0f), acceleration(0.0f), mass(1.0f) {}
	};
}
#endif













































