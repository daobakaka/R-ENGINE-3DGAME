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
		//测试模块
		CubeE,
		CubeTestE,
		CubeEVariant1,
		CubeEVarinat2,
		ActorButterfly,
		TsetButterfly,
		LightColorTestCube,
		TestPhysics,
		//特殊模块
		Ray,//射线
		AxisWidget,//坐标轴模型
		PointLight,//点光源
		FlashLight,//手电筒
		ParallelLight,//平行光
		Skybox,//天空盒
		StaticPlane,//静态地板
		//独立模块
		
	};

	enum SpecailType
	{
		OriginalT,//原始类型
		BasePlane,//基础地面
		Player,//玩家

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
		int ID;//全局唯一身份标识
		OctreeNode* currentNode;
		int layer;
		//特殊碰撞体
		SpecailType sType;
		// 碰撞盒类型
		CollisionType collider;
		glm::vec3 &position;    // 物体位置
		glm::vec3 &velocity;    // 物体速度
		glm::vec3 &acceleration;  // 物体加速度
		glm::vec3 _collisionMin;
		glm::vec3 _collisionMax;
		//定向参数
		//自身摩擦系数
		float friction;
		//解除对象摩擦系数
		float otherFriction;
		float mass;
		glm::vec3 gravity;
		float elasticity;//弹性系数
		// 碰撞容器，记录当前与哪些物体发生碰撞
		std::unordered_set<int> collidingBodies;

		//碰撞盒半径
		float radius;
		//碰撞盒形状比率
		glm::vec3 ratio;
		//碰撞状态判断
		bool isCollision;
		//触发器
		bool trigger;
		//静态物体
		bool staticObj;
		//开启重力加速度
		bool gravityEnabled;

		//增加碰撞旋转逻辑
		glm::vec3 angularVelocity;
		glm::quat &rotation;        // 旋转角度（四元数）
		float rotationDamping; //旋转阻尼
		//以上三个量可以完全控制碰撞盒大小
		CollisionProperties(glm::vec3 &pos,glm::vec3 &vel,glm::vec3 &acc,glm::quat &rot)
			: position(pos), velocity(vel), acceleration(acc), mass(1),friction(0.05f),_collisionMin(0),_collisionMax(0),isCollision(false),trigger(false),rotation(rot) {}
	};

	

}
#endif













































