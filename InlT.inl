//这是一个.inl文件，这个文件是对头文件的补充，为了提升头文件的可读性，将.inl文件用于扩展头文件中方法的部分实现，如内联函数或者模板类<T>等

#ifndef InlT_inl
#define InlT_inl
#include <iostream>

inline void InlTest::SayHelloInl() //内联函数，因为其编译的特性，可以不需要引用相关的头文件就可以编译直接使用InlTest::SayHelloInl() 不会报错
{
	std::cout << "this is a inl fun " << std::endl;

}

#endif

