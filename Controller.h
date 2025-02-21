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
namespace Game {

//前向声明--

	class CustomModel;
	class MeshDataManager;
	template <typename T>
	class LifecycleManager;
	class CoordinateSystemCus;

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
		void ProcessMouseInput(GLFWwindow* window, float& pitch, float& yaw, bool& rightMousePressed);

		//综合方法，帧率控制
		void FrameControlMethod(GLFWwindow* window);


		//坐标选旋转角度参考
		void RenderWidget(const char* widgetVertexShaderSource, const char* widgetFragmentShaderSource,
			GLuint widgetVAO, int widgetSize, int widgetPosX, int widgetPosY);

		// 获取摄像机视图矩阵
		glm::mat4 GetViewMatrix();
		glm::mat4 GetFixedViewMatrix();
		glm::mat4 GetProjectionMatrix();
		//构建坐标系，全局深度图shader
		void BuildWidgetShader(MeshDataManager* meshData, LifecycleManager<CustomModel>* customModle);
	
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
		
	};
}

#endif