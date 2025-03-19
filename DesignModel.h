#ifndef DESIGN_MODEL_H
#define DESIGN_MODEL_H
#include "ScriptModel.h"
namespace Game {


	class CustomModel;
	class DesignModel :public CustomModelShader

	{
	public:

		DesignModel(const std::string& name, const Model& model, bool isSkinnedMesh, bool ifLight = false, bool ifShadow = false);		
		~DesignModel();

		virtual bool Draw(glm::mat4 view, glm::mat4 projection) override;//静态绘制可重写;
		virtual bool DrawDynamical(glm::mat4 view, glm::mat4 projection) override;//动态绘制可重写，与IntegtatedAnimatior联动

		virtual void DrawDepthPicDynamical(glm::mat4 lightSpaceMatrix, GLuint shader) override;//动态绘制深度图，可重写
		virtual void DrawDepthPic(glm::mat4 lightSpaceMatrix, GLuint shader) override; //静态绘制深度图，可重写

		virtual void  UniformParametersInput() override;//重写通用shader的传参，改变状态



	protected:
		std::vector<GLuint> _meshVAOs;  // 存储每个 Mesh 的 VAO
		std::vector<GLuint> _meshVBOs;  // 存储每个 Mesh 的 VBO
		std::vector<GLuint> _meshEBOs;  // 存储每个 Mesh 的 EBO
		Model _modelData;




	};




}
#endif // !DESIGN_MODEL_H

