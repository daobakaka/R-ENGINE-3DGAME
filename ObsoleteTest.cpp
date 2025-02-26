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

    //栈创建，类似C#的普通变量，但没有new
    Monster monster("daobakaka", 100, 10);
    monster.Attack();
    monster.TakeDamage(10);
    //堆创建，以指针模式创建
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
#pragma region 宏定义模块,C++中的宏替换，内联函数等
    double x = 10;
    x = x + PICUS;
    std::cout << "hello world:" << PIConst << std::endl;
    SquareFun(PIConst);
   // DEBUGLOG(SQUARE(x));
    TestOne();//仅仅在头文件中直接声明一个方法，未包含在类里面，独立的方法，且未包含作用域修饰符 public，但是源文件中有实现
    TestGlobal();//仅仅在源文件中有实现，也没有修饰符，应该是默认public,

    InlTest* obj = new InlTest(); //这种包含在类里面的内联函数，也必须通过对象来声明，而不能像上面的裸内联直接声明
    obj->SayHelloInl();


#pragma endregion

}