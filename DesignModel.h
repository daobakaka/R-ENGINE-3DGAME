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

		virtual bool Draw(glm::mat4 view, glm::mat4 projection) override;//��̬���ƿ���д;
		virtual bool DrawDynamical(glm::mat4 view, glm::mat4 projection) override;//��̬���ƿ���д����IntegtatedAnimatior����

		virtual void DrawDepthPicDynamical(glm::mat4 lightSpaceMatrix, GLuint shader) override;//��̬�������ͼ������д
		virtual void DrawDepthPic(glm::mat4 lightSpaceMatrix, GLuint shader) override; //��̬�������ͼ������д

		virtual void  UniformParametersInput() override;//��дͨ��shader�Ĵ��Σ��ı�״̬



	protected:
		std::vector<GLuint> _meshVAOs;  // �洢ÿ�� Mesh �� VAO
		std::vector<GLuint> _meshVBOs;  // �洢ÿ�� Mesh �� VBO
		std::vector<GLuint> _meshEBOs;  // �洢ÿ�� Mesh �� EBO
		Model _modelData;




	};




}
#endif // !DESIGN_MODEL_H

