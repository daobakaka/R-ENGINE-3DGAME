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
	class IntergratedScripts
	{
	public:
		static IntergratedScripts* GetInstance();//指针方法，返回一个指向该方法的指针
		~IntergratedScripts();
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

	private:

		static IntergratedScripts* instance;
		IntergratedScripts();
		std::mt19937 gen;  // 随机数生成器
		float randomFloat;
		int randomInt;

	public:
		//方块儿测试方式
		void CubeUpdateFun(GameObject* cube);
		void TestUpdateFun(GameObject* gameObject);
		//其他对象方法
		void ActorButtfly(GameObject *obj);
		


	};

	

}


#endif