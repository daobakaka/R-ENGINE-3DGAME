#ifndef PHYSICAL_ENGINE_H
#define PHYSICAL_ENGINE_H
#include"GameObject.h"
namespace Game {
	class PhysicalEngine :IComponent<PhysicalEngine>
	{
	public:
		PhysicalEngine();

		bool Interface() override;

	private:

	};

}
#endif // !PHYSICAL_ENGINE_H

