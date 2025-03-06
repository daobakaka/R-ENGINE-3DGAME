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

#include "Octree.h"//八叉树交由基本控制类控制
namespace Game {

//前向声明--

	class CustomModel;
	class MeshDataManager;
	template <typename T>
	class LifecycleManager;
	class CoordinateSystemCus;
	//初始化静态变量的0值
	
//---
	class  Controller
	{
	public:

		static Controller* GetInstance();
		// 摄像机状态
		glm::vec3 position;  // 摄像机位置
		glm::vec3 front;     // 摄像机前向向量
		glm::vec3 up;        // 摄像机上向量
		float pitch, yaw;    // 俯仰与偏航
		glm::vec3 right;//右向量
		float zoom;//滚轮放大缩小


		// 透视投影矩阵的参数
		float fov;//视宽
		float aspect;//宽高比
		float nearPlane;//近裁面
		float farPlane;//远裁面



		// 鼠标和键盘控制
		bool rightMousePressed;
		double lastX, lastY;  // 鼠标位置
		double tempX, tempY;   // 临时偏移位置
		bool mouseMovesContinuously;

		// 屏幕大小
		int windowWidth ;
		int windowHeight ;

		//键盘输入
		void ProcessInput(GLFWwindow* window, glm::vec3& objectPosition);
		//鼠标回调注册
		void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		//鼠标移动测试
		void MouseButtonFun(GLFWwindow* window);
		//鼠标输入旋转逻辑
		void ProcessMouseInput(GLFWwindow* window, float& pitch, float& yaw, bool& rightMousePressed,bool pitchRotation=true);


		void GetPlayerPosition(CustomModel* player,glm::vec3 offset);

		//处理滚轮缩放逻辑
		
		//综合方法，帧率控制
		void FrameControlMethod(GLFWwindow* window);

		void ProcessMouseScrollGlobal(float yoffset);
	
		void SetScrollCallback(GLFWwindow* window);

		//坐标选旋转角度参考
		void RenderWidget(const char* widgetVertexShaderSource, const char* widgetFragmentShaderSource,
			GLuint widgetVAO, int widgetSize, int widgetPosX, int widgetPosY);

		// 获取摄像机视图矩阵
		glm::mat4 GetViewMatrix();
		glm::mat4 GetFixedViewMatrix();
		glm::mat4 GetProjectionMatrix();
		//构建坐标系，全局深度图shader
		void BuildWidgetShader(MeshDataManager* meshData, LifecycleManager<CustomModel>* customModle);

		//游戏数据结构管理
		static Octree* OCTREET;
	
	private:
		static Controller* instance;
		glm::mat4 view;
		// 投影矩阵
		glm::mat4 projection;
		Controller();
		~Controller();
		//构建特殊坐标系shader
		GLuint widgetShaderProgram;
		GLuint _depthShaderProgram;
			//编译深度着色器
		void BuildDepthShader();
		//调用全局着色器
		void UseDepthShaderProgram();
		//获取shader
		GLint GetDepthShaderProgram();
	

#pragma region 新增玩家模块
	public:
		//第三人称玩家
		void FrameControlMethodPlayer(GLFWwindow* window, CustomModel* player);
		//第三人称玩家视角矩阵
		glm::mat4 GetPlayerViewMatrix(CustomModel* palyer, glm::vec3 offset = glm::vec3(0));

	private:

		//第三人称物理调整键盘输入
		void ProcessInputPhysics(GLFWwindow* window, CustomModel* player);
#pragma endregion


	};
}

#endif