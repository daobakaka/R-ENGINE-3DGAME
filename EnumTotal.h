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
		StaticPlan,//静态地板
		
	};

	//步长结构体
	struct StepVector3
	{
		glm::vec3 position = glm::vec3(0, 0, 0);
		glm::vec3 rotation = glm::vec3(0, 0, 0);
		glm::vec3 scale = glm::vec3(1);

	};
}
#endif













































