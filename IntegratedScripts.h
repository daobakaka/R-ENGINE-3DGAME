#ifndef INTERGRATED_SCRIPTS_H
#define INTERGRATED_SCRIPTS_H
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "cube.h"
#include <random>
#include <chrono>
#include <thread>
namespace Game {
	class IntegratedScripts
	{
	public:
		static IntegratedScripts* GetInstance();//指针方法，返回一个指向该方法的指针
		~IntegratedScripts();
		//具体类型的方法模块
		float TGetRandomFloat(float min, float max);
		//综合变换随机数方法
		void TChangeRandom(float min, float max);
		// 获取一个范围在[min, max]之间的随机整数
		int TGetRandomInt(int min, int max);
		// 获取一个范围在[min, max]之间的随机浮动数
		//帧率更新		
		int TUpdateFPS();
		//帧率控制
		void TControlFrameRate(float targetFrameRate);
		//平行光旋转
		void TParallelLightRotation(GameObject *object);
		//测试加速度作用信息
		void TPosition(GameObject* object);

	private:

		static IntegratedScripts* instance;
		IntegratedScripts();
		std::mt19937 gen;  // 随机数生成器
		float randomFloat;
		int randomInt;

	public:
		//方块儿测试方式
		void CubeUpdateFun(GameObject* cube);
		void TestUpdateFun(GameObject* gameObject);
		//其他对象方法
		void ActorButtfly(GameObject *obj);
	     
		//玩家控制方法，综合控制器
		void PlayerControl(GLFWwindow* window, CustomModel* other);
		
	private:
		//玩家控制模块
		float _shootPrepare;
		bool _enableShoot;


	};

	

}


#endif