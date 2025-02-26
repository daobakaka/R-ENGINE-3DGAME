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
		Ray,//����
		AxisWidget,//������ģ��
		PointLight,//���Դ
		FlashLight,//�ֵ�Ͳ
		ParallelLight,//ƽ�й�
		Skybox,//��պ�
		StaticPlane,//��̬�ذ�
		
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
	//�����ṹ��
	struct StepVector3
	{
		glm::vec3 position = glm::vec3(0, 0, 0);
		glm::vec3 rotation = glm::vec3(0, 0, 0);
		glm::vec3 scale = glm::vec3(1);
	};
	//��ײ��ṹ��
	struct CollisionProperties {
		glm::vec3 position;    // ����λ��
		glm::vec3 velocity;    // �����ٶ�
		glm::vec3 acceleration;  // ������ٶ�
		float mass;            // ��������

		CollisionProperties()
			: position(0.0f), velocity(0.0f), acceleration(0.0f), mass(1.0f) {}
	};
}
#endif













































