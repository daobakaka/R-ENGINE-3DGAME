#ifndef ENUM_TOTAL_H
#define ENUM_TOTAL_H
#include <glm/glm.hpp>
#include <unordered_map>
namespace Game {


	enum ModelClass
	{
		OriginalE,
		//����ģ��
		CubeE,
		CubeTestE,
		CubeEVariant1,
		CubeEVarinat2,
		ActorButterfly,
		TsetButterfly,
		LightColorTestCube,
		TestPhysics,
		//����ģ��
		Ray,//����
		AxisWidget,//������ģ��
		PointLight,//���Դ
		FlashLight,//�ֵ�Ͳ
		ParallelLight,//ƽ�й�
		Skybox,//��պ�
		StaticPlane,//��̬�ذ�
		//����ģ��
		
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
		int ID;//ȫ��Ψһ��ݱ�ʶ
		int layer;
		
		glm::vec3 &position;    // ����λ��
		glm::vec3 &velocity;    // �����ٶ�
		glm::vec3 &acceleration;  // ������ٶ�
		glm::vec3 _collisionMin;
		glm::vec3 _collisionMax;
		//�������
		float friction;
		float mass;

		//��ײ������
		CollisionType type;
		//��ײ�а뾶
		float radius;
		//��ײ����״����
		glm::vec3 ratio;
		//��ײ״̬�ж�
		bool isCollision;
		//������
		bool trigger;
		//����������������ȫ������ײ�д�С
		CollisionProperties(glm::vec3 &pos,glm::vec3 &vel,glm::vec3 &acc)
			: position(pos), velocity(vel), acceleration(acc), mass(1),friction(0.05f),_collisionMin(0),_collisionMax(0),isCollision(false),trigger(false) {}
	};

	

}
#endif













































