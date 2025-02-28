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
		static IntergratedScripts* GetInstance();//ָ�뷽��������һ��ָ��÷�����ָ��
		~IntergratedScripts();
		//�������͵ķ���ģ��
		float TGetRandomFloat(float min, float max);
		//�ۺϱ任���������
		void TChangeRandom(float min, float max);
		// ��ȡһ����Χ��[min, max]֮����������
		int TGetRandomInt(int min, int max);
		// ��ȡһ����Χ��[min, max]֮������������
		//֡�ʸ���		
		int TUpdateFPS();
		//֡�ʿ���
		void TControlFrameRate(float targetFrameRate);
		//ƽ�й���ת
		void TParallelLightRotation(GameObject *object);
		//���Լ��ٶ�������Ϣ
		void TPosition(GameObject* object);

	private:

		static IntergratedScripts* instance;
		IntergratedScripts();
		std::mt19937 gen;  // �����������
		float randomFloat;
		int randomInt;

	public:
		//��������Է�ʽ
		void CubeUpdateFun(GameObject* cube);
		void TestUpdateFun(GameObject* gameObject);
		//�������󷽷�
		void ActorButtfly(GameObject *obj);
		


	};

	

}


#endif