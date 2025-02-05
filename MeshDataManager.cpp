#include "MeshDataManager.h"
using namespace Game;
MeshDataManager* MeshDataManager::instance = nullptr;
MeshDataManager* MeshDataManager::GetInstance()
{
    if (instance == nullptr) {
        instance = new MeshDataManager();
    }
    return instance;
}



