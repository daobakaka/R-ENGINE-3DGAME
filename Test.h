//�������������һЩȫ�ֵķ�������extern ���� ���ڲ���.inl �ļ����Ļ������

#pragma once
#include <iostream>
#include<string>
void TestOne();//���ַ��� Ҳ���԰���Ϊ������ʵ��
	class  InlTest
	{
	public:
		InlTest();

		void SayHelloInl();

		~InlTest();

	private:

	};

#include "InlT.inl"//���������Ҫע��λ�ã������ʵ�ֱ�������������������У�

