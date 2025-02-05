#include "LifecycleManager.h"
#include "MonoBehaviour.h"
#include <iostream>
#include "GameObject.h"
#include "CustomModel.h"
using namespace Game;
template <typename T>
LifecycleManager<T>* LifecycleManager<T>::instance = nullptr;


template <typename T>
void LifecycleManager<T>::RegisterObject(T* object) {
 
           nativeObjects.push_back(object);
           if (object->GetVariant() != 0)
               variantObjects.push_back(object);
}

template<typename T>
std::vector<T*> LifecycleManager<T>::GetNativeObjects()
{
    return nativeObjects;
}

template<typename T>
std::vector<T*> LifecycleManager<T>::GetVariantObjects()
{
    return variantObjects;
}

template<typename T>
LifecycleManager<T>* LifecycleManager<T>::GetInstance()
{
    if (instance == nullptr) {
        instance = new LifecycleManager<T>();
    }
    return instance;
}



//目前还没有用到变体的初始化方法，暂时不分离方法
template <typename T>
void LifecycleManager<T>::StartAll() {
    for (T* object : nativeObjects) {
        object->Start();  // 调用每个对象的 Start 方法
    }
    //for (T* object : variantObjects) {
    //    object->Start();  // 调用每个变体对象的 Start 方法
    //}
}

template <typename T>
void LifecycleManager<T>::UpdateAll(glm::mat4 view, glm::mat4 projection) {
    for (T* object : nativeObjects) {
        object->Update(view, projection);  // 调用每个对象的 Update 方法
    }
}

template <typename T>
void LifecycleManager<T>::DestroyAll() {
    for (T* object : nativeObjects) {
        object->Destroy();  // 调用每个对象的 Destroy 方法
    }
    //for (T* object : variantObjects) {
    //    object->Destroy();  // 调用每个变体对象的 Destory 方法
    //}
}

template<typename T>
void LifecycleManager<T>::UpdateAllVariant(glm::mat4 view, glm::mat4 projection)
{
    for (T* object:variantObjects)
    {
        object->UpdateVariant(view,projection);
    }

}









// 明确实例化模板类，避免链接错误，这个放在源文件中的实例化模板，有1个T则添加一个具体的类在这里进行声明
//如果模板类完全放在头文件中，则不需要显示实例化，会自动的根据代码情况编译！！通常采用头文件的方式（声明、宏、内联函数、模板类和一些常量）
//编译器需要在编译源文件时为模板类生成代码，所以放在源文件中需要显示声明，头文件的实际编译，会放在CPP引用.h的位置
template class LifecycleManager<MonoBehaviour>;
template class LifecycleManager<GameObject>; // 显式实例化 GameObject
template class LifecycleManager<CustomModel>;//显示实例化 CustomModel

