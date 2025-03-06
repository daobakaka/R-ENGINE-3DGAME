#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <iostream>
#include "Monster.h"
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "LifecycleManager.h"

#include "Octree.h"//�˲������ɻ������������
namespace Game {

//ǰ������--

	class CustomModel;
	class MeshDataManager;
	template <typename T>
	class LifecycleManager;
	class CoordinateSystemCus;
	//��ʼ����̬������0ֵ
	
//---
	class  Controller
	{
	public:

		static Controller* GetInstance();
		// �����״̬
		glm::vec3 position;  // �����λ��
		glm::vec3 front;     // �����ǰ������
		glm::vec3 up;        // �����������
		float pitch, yaw;    // ������ƫ��
		glm::vec3 right;//������
		float zoom;//���ַŴ���С


		// ͸��ͶӰ����Ĳ���
		float fov;//�ӿ�
		float aspect;//��߱�
		float nearPlane;//������
		float farPlane;//Զ����



		// ���ͼ��̿���
		bool rightMousePressed;
		double lastX, lastY;  // ���λ��
		double tempX, tempY;   // ��ʱƫ��λ��
		bool mouseMovesContinuously;

		// ��Ļ��С
		int windowWidth ;
		int windowHeight ;

		//��������
		void ProcessInput(GLFWwindow* window, glm::vec3& objectPosition);
		//���ص�ע��
		void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		//����ƶ�����
		void MouseButtonFun(GLFWwindow* window);
		//���������ת�߼�
		void ProcessMouseInput(GLFWwindow* window, float& pitch, float& yaw, bool& rightMousePressed,bool pitchRotation=true);


		void GetPlayerPosition(CustomModel* player,glm::vec3 offset);

		//������������߼�
		
		//�ۺϷ�����֡�ʿ���
		void FrameControlMethod(GLFWwindow* window);

		void ProcessMouseScrollGlobal(float yoffset);
	
		void SetScrollCallback(GLFWwindow* window);

		//����ѡ��ת�ǶȲο�
		void RenderWidget(const char* widgetVertexShaderSource, const char* widgetFragmentShaderSource,
			GLuint widgetVAO, int widgetSize, int widgetPosX, int widgetPosY);

		// ��ȡ�������ͼ����
		glm::mat4 GetViewMatrix();
		glm::mat4 GetFixedViewMatrix();
		glm::mat4 GetProjectionMatrix();
		//��������ϵ��ȫ�����ͼshader
		void BuildWidgetShader(MeshDataManager* meshData, LifecycleManager<CustomModel>* customModle);

		//��Ϸ���ݽṹ����
		static Octree* OCTREET;
	
	private:
		static Controller* instance;
		glm::mat4 view;
		// ͶӰ����
		glm::mat4 projection;
		Controller();
		~Controller();
		//������������ϵshader
		GLuint widgetShaderProgram;
		GLuint _depthShaderProgram;
			//���������ɫ��
		void BuildDepthShader();
		//����ȫ����ɫ��
		void UseDepthShaderProgram();
		//��ȡshader
		GLint GetDepthShaderProgram();
	

#pragma region �������ģ��
	public:
		//�����˳����
		void FrameControlMethodPlayer(GLFWwindow* window, CustomModel* player);
		//�����˳�����ӽǾ���
		glm::mat4 GetPlayerViewMatrix(CustomModel* palyer, glm::vec3 offset = glm::vec3(0));

	private:

		//�����˳����������������
		void ProcessInputPhysics(GLFWwindow* window, CustomModel* player);
#pragma endregion


	};
}

#endif