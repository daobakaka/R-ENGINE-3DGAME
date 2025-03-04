#ifndef ENUM_TOTAL_H
#define ENUM_TOTAL_H
#include <glm/glm.hpp>
#include <unordered_map>
#include <unordered_set>
#include "Octree.h";
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

	enum SpecailType
	{
		OriginalT,//ԭʼ����
		BasePlane,//��������
		Player,//���

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
		OctreeNode* currentNode;
		int layer;
		//������ײ��
		SpecailType sType;
		// ��ײ������
		CollisionType collider;
		glm::vec3 &position;    // ����λ��
		glm::vec3 &velocity;    // �����ٶ�
		glm::vec3 &acceleration;  // ������ٶ�
		glm::vec3 _collisionMin;
		glm::vec3 _collisionMax;
		//�������
		//����Ħ��ϵ��
		float friction;
		//�������Ħ��ϵ��
		float otherFriction;
		float mass;
		glm::vec3 gravity;
		float elasticity;//����ϵ��
		// ��ײ��������¼��ǰ����Щ���巢����ײ
		std::unordered_set<int> collidingBodies;

		//��ײ�а뾶
		float radius;
		//��ײ����״����
		glm::vec3 ratio;
		//��ײ״̬�ж�
		bool isCollision;
		//������
		bool trigger;
		//��̬����
		bool staticObj;
		//�����������ٶ�
		bool gravityEnabled;

		//������ײ��ת�߼�
		glm::vec3 angularVelocity;
		glm::quat &rotation;        // ��ת�Ƕȣ���Ԫ����
		float rotationDamping; //��ת����
		//����������������ȫ������ײ�д�С
		CollisionProperties(glm::vec3 &pos,glm::vec3 &vel,glm::vec3 &acc,glm::quat &rot)
			: position(pos), velocity(vel), acceleration(acc), mass(1),friction(0.05f),_collisionMin(0),_collisionMax(0),isCollision(false),trigger(false),rotation(rot) {}
	};

	

}
#endif













































