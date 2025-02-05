#pragma once
#include<string>
namespace Game {
	class Monster
	{
	public:

		Monster(std::string name, float health, int leavel);
		virtual ~Monster();


		static bool couldhavebenn;
		float myHealth;
		int myleavel;
		std::string myName;
		void Attack();
		void TakeDamage(float damage);
	private:
		int _testnum;
	};

	class MonsterSon :public Monster
	{
	public:
		MonsterSon(std::string name, float health, int leavel);
		virtual void MagicSon();
	};

	class MonsterGrandSon :public MonsterSon
	{
	public:
		MonsterGrandSon(std::string name, float health, int leavel);
		void MagicGrandSon();
		void MagicSon() override;


	};



	class  MonsterAbstractSon :public MonsterGrandSon
	{
	public:
		MonsterAbstractSon(std::string name, float health, int leavel);
		virtual void AbstractFunc() = 0;//=0 表示为该函数为纯虚函数，等同c#中的抽象函数，如何一个类里面全是纯虚函数，则为C#中的接口，被重写的函数前面必须加关键字virtual


	};


	class  MonsterDeepSon :public MonsterAbstractSon
	{
	public:
		MonsterDeepSon(std::string name, float health, int leavel);
		void AbstractFunc() override;
		void TestFuncinline();


	};
	//这样的表示方式就是接口
	class IMonster
	{

	public:
		virtual void IMonsterFun1() = 0;
		virtual void IMonsterFun2() = 0;

	};
}


