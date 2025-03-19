#ifndef MICROCPP_H
#define MICROCPP_H
#define PICUS 3.14159
#define SQUARE(x) ((x) * (x))
//pragma once �ִ��Ƽ���������������֮�󣬿�����ɲ�֧�ֵ���������α���궨�壬������Ƕ�װ������  
//���ַ�ʽ�ĺ� ���������͵ģ���������һ������;
//�޷��ڵ������и��ٵ��ã����滻������Ԥ����׶Σ��������޷�ʶ�������ַ�ʽ����ʹ��inline �� constexpr ���
//���ֽ������ģʽ
//1.����������������ֱ�ӱ��룬�����ڱ���λ��ֱ��չ����û����ת���޳��溯����ѹջ����ת���ָ��ȿ���
inline float SquareFun(float x)
{
	return x * x;
}
//2.constexpr auto PI = 3.14159  ����  constexpr �������ʽ���ڱ����ʱ���֪������ı�ʾʽ�������ֵ�ڱ����ʱ���ȷ���������޸ģ����Ч�ʸ���
constexpr auto PIConst = 3.14159;
constexpr int SquareInt(int x)
{

	return x * x;

}
#endif // MICROCPP_H