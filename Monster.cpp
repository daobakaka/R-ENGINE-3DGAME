#include "Monster.h"
#include <iostream>



using namespace Game;

bool Monster::couldhavebenn = true;
Monster::Monster(std::string name, float health, int leavel)
{
	myName = name;
	myHealth = health;
	myleavel = leavel;
	_testnum = 33;
}


Monster::~Monster()
{
	std::cout << myName<<" :is dispose" << std::endl;
} 
 
void Monster::Attack()
{
	std::cout << myName << " attacks!" << std::endl;
	
}
void Monster::TakeDamage(float damage)
{
	myHealth -= damage;
	std::cout << myName << "hurt by damage" << damage << std::endl;
	std::cout << "health is" << myHealth << std::endl;

}



MonsterSon::MonsterSon(std::string name, float health, int leavel): Monster(name, health, leavel) //实现的方法中不用重新添加Public等声明，在头文件中已经声明了
{

	myHealth = 2 * health;

}


void MonsterSon::MagicSon()
{
	std::cout << myName << ": take the special son  magic" << std::endl;
	
}



MonsterGrandSon::MonsterGrandSon(std::string name, float health, int leavel) :MonsterSon(name, health, leavel) 
{

	myName = "grand" + name;


}

void MonsterGrandSon::MagicGrandSon()

{

	std::cout << myName << ": take the special grand son  magic" << std::endl;
}

void MonsterGrandSon::MagicSon()
{

	std::cout << myName << ":grandSon  take the special  overird son  magic" << std::endl;
}

MonsterAbstractSon::MonsterAbstractSon(std::string name, float health, int leavel) :MonsterGrandSon(name, health, leavel)
	
{
}

MonsterDeepSon::MonsterDeepSon(std::string name, float health, int leavel):MonsterAbstractSon(name, health, leavel)
{
}

void MonsterDeepSon::AbstractFunc()
{
	std::cout << "this is a pure virtual func!"<<std::endl;
}

 void MonsterDeepSon::TestFuncinline()
{
	std::cout << "this is a inline func!" << std::endl;
}
