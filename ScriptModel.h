#ifndef  SCRIPTMODEL_H
#define  SCRIPTMODEL_H
#include "CustomModel.h"

namespace Game
{
	class ButterflyScript :public CustomModel
	{
	public:
		//这里直接继承基类的构造函数
		using CustomModel::CustomModel;
		void UpdateVariant(glm::mat4 view, glm::mat4 projection) override;
	
	private:

	};




}


#endif // ! SCRIPTMODEL_H

