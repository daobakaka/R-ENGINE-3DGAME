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
		virtual void AbstractFunc() = 0;//=0 ��ʾΪ�ú���Ϊ���麯��


	};


	class  MonsterDeepSon :public MonsterAbstractSon
	{
	public:
		MonsterDeepSon(std::string name, float health, int leavel);
		void AbstractFunc() override;
		void TestFuncinline();


	};
	//�����ı�ʾ��ʽ���ǽӿ�
	class IMonster
	{

	public:
		virtual void IMonsterFun1() = 0;
		virtual void IMonsterFun2() = 0;

	};
}


