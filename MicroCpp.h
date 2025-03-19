#ifndef MICROCPP_H
#define MICROCPP_H
#define PICUS 3.14159
#define SQUARE(x) ((x) * (x))
//pragma once 现代推荐方法，但类库多了之后，可能造成不支持的情况，单次编译宏定义，避免了嵌套包的情况  
//这种方式的宏 ，是弱类型的，容易引发一起问题;
//无法在调试器中跟踪调用，宏替换发生在预处理阶段，调试器无法识别。这两种方式建议使用inline 或 constexpr 替代
//两种建议替代模式
//1.内联函数，编译器直接编译，函数在编译位置直接展开，没有跳转，无常规函数的压栈、跳转、恢复等开销
inline float SquareFun(float x)
{
	return x * x;
}
//2.constexpr auto PI = 3.14159  常量  constexpr 常量表达式：在编译的时候就知道结果的表示式，定义的值在编译的时候就确定，不能修改，因此效率更高
constexpr auto PIConst = 3.14159;
constexpr int SquareInt(int x)
{

	return x * x;

}
#endif // MICROCPP_H