//这个类用来测试一些全局的方法，如extern 或者 基于测试.inl 文件产的基础类等

#pragma once
#include <iostream>
#include<string>
void TestOne();//这种方法 也可以包含为单独的实现
	class  InlTest
	{
	public:
		InlTest();

		void SayHelloInl();

		~InlTest();

	private:

	};

#include "InlT.inl"//这种情况下要注意位置，这里的实现必须放在声明的类后面才行！

