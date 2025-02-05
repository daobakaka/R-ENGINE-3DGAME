#ifndef COMPONENT_H
#define COMPONENT_H
namespace Game {
	template <typename T>
	class IComponent
	{
	public:

		virtual bool Interface() = 0;
		virtual ~IComponent() = default; // 虚析构函数，确保派生类能够正确析构
	private:

	};
}






































#endif
