#include <iostream>
#include "Monster.h"
#include <vector>
#include "MicroCpp.h"
#include "Test.h"
#include <glm/fwd.hpp>

using namespace Game;

void TestCpp()
{
    std::cout << "Fuck you bitch!\n";

    //ջ����������C#����ͨ��������û��new
    Monster monster("daobakaka", 100, 10);
    monster.Attack();
    monster.TakeDamage(10);
    //�Ѵ�������ָ��ģʽ����
    Monster* monsterPtr = new Monster("kakadaoba", 50, 5);
    monsterPtr->Attack();
    monsterPtr->TakeDamage(5);
    delete(monsterPtr);

    std::vector<Monster> monsters;
    monsters.push_back(Monster("Monster 1", 101, 10));
    monsters.push_back(Monster("Monster 2", 81, 8));
    monsters.push_back(Monster("Monster 3", 51, 5));
    monsters[1].Attack();

    Monster* monstersa = new Monster[3]{
        Monster("Monster a1", 101, 10),
        Monster("Monster a2", 81, 8),
        Monster("Monster a3", 51, 5),
    };
    monstersa[2].TakeDamage(3);
    monstersa->TakeDamage(1);
    delete[] monstersa;

    MonsterSon* monsterSonPtr = new MonsterSon("son", 888, 1);
    monsterSonPtr->TakeDamage(20);
    monsterSonPtr->MagicSon();
    delete(monsterSonPtr);

    MonsterGrandSon* monsterGrandSonPtr = new MonsterGrandSon("grandSon", 999, 2);
    monsterGrandSonPtr->MagicSon();
    monsterGrandSonPtr->TakeDamage(30);
    monsterGrandSonPtr->MagicGrandSon();
    delete(monsterGrandSonPtr);

    MonsterDeepSon* monsterDeepSon = new MonsterDeepSon("DeepSon", 111, 2);
    monsterDeepSon->TakeDamage(100);
    monsterDeepSon->AbstractFunc();
    monsterDeepSon->TestFuncinline();
    delete(monsterDeepSon);


}
extern void TestGlobal();
void MicroTest()
{
#pragma region �궨��ģ��,C++�еĺ��滻������������
    double x = 10;
    x = x + PICUS;
    std::cout << "hello world:" << PIConst << std::endl;
    SquareFun(PIConst);
   // DEBUGLOG(SQUARE(x));
    TestOne();//������ͷ�ļ���ֱ������һ��������δ�����������棬�����ķ�������δ�������������η� public������Դ�ļ�����ʵ��
    TestGlobal();//������Դ�ļ�����ʵ�֣�Ҳû�����η���Ӧ����Ĭ��public,

    InlTest* obj = new InlTest(); //���ְ����������������������Ҳ����ͨ���������������������������������ֱ������
    obj->SayHelloInl();


#pragma endregion

}