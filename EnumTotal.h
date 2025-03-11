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
		//ʵ����ģ��
		InstanceCube,
		InstanceCircle,
		InstanceSphere,
		InstanceRound,
		//����ģ��
		Ray,//����
		AxisWidget,//������ģ��
		PointLight,//���Դ
		FlashLight,//�ֵ�Ͳ
		ParallelLight,//ƽ�й�
		Skybox,//��պ�
		StaticPlane,//��̬�ذ�
		//����ģ��
		Player,//��Ϸ����
		PlayerBullet,//�����ӵ�

		StoneMonser,//ʯͷ��
		
	};

	enum PictureTpye
	{
		BaseP,
		NormalP,
		SpecularP,
		HightP,
		RoughnessP,
		AOP,
		OtherP,

	};


	enum SpecialType
	{
		OriginalT,//ԭʼ����
		BasePlane,//��������
		SPlayer,//���

	};
	enum ShaderClass
	{
		OriginalS,
		DepthCalculate,
		DepthRender,
		DepthTest,
		DepthMapParallel,//���ƽ�й���Ӱ��ͼID
		DepthMapTest,//��Ȳ��Ժ�����ͼID
		KAKATK,
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
	//��Ϸ�ṹ��
	struct GameProperties
	{
		float health = 100;
		float damage = 3;
		float speed = 1;


	};

	//��ײ��ṹ��
	struct CollisionProperties {
		int ID;//ȫ��Ψһ��ݱ�ʶ
		OctreeNode* currentNode;
		bool &isActive;
		int &layer;
		//������ײ��
		SpecialType sType;
		//�߼�����
		ModelClass logicType;
		//ʱ���¼
		float timer;
		// ��ײ������
		CollisionType collider;
		glm::vec3 &position;    // ����λ��
		glm::vec3 &velocity;    // �����ٶ�
		glm::vec3 &acceleration;  // ������ٶ�
		glm::vec3 _collisionMin;
		glm::vec3 _collisionMax;
		//�������
		//����Ħ��ϵ��
		float &friction;
		//�������Ħ��ϵ��
		float otherFriction;
		float &mass;
		glm::vec3 gravity;
		float &elasticity;//����ϵ��
		// ��ײ��������¼��ǰ����Щ���巢����ײ
		std::unordered_set<int> collidingBodies;

		//��ײ�а뾶
		float radius;
		//��ײ����״����
		glm::vec3 &ratio;
		//��ײ״̬�ж�
		bool isCollision;
		//�Ƿ��ڵ���
		bool isOnGround;
		//������
		bool &trigger;
		//��̬����
		bool staticObj;
		//�����������ٶ�
		bool gravityEnabled;

		//������ײ��ת�߼�
		glm::vec3 angularVelocity;
		glm::quat &rotation;        // ��ת�Ƕȣ���Ԫ����
		float &rotationDamping; //��ת����
		bool &lockXZAxi;//XZ������
		float &rotationAdjust;//��ת����ϵ��


		GameProperties gameProperties;
		//����������������ȫ������ײ�д�С
		CollisionProperties(glm::vec3 &pos,glm::vec3 &vel,glm::vec3 &acc,glm::quat &rot,glm::vec3 &ratioIN,bool &ifActiveIN,float &rotationDampingIN,bool &lockSXAxiIN ,
			float &massIN,float &frictionIN,float &elasticityIN,int &layerIN,bool &triggerIN,float &rotationAdjustIN)
			: position(pos), velocity(vel), acceleration(acc),ratio(ratioIN), mass(massIN), friction(frictionIN), _collisionMin(0), _collisionMax(0), isCollision(false), trigger(triggerIN), rotation(rot)
		,isActive(ifActiveIN),rotationDamping(rotationDampingIN),lockXZAxi(lockSXAxiIN),elasticity(elasticityIN),layer(layerIN),rotationAdjust(rotationAdjustIN)
		{}
	};

}
#endif













































