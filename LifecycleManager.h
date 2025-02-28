#ifndef LIFECYCLE_MANAGER_H
#define LIFECYCLE_MANAGER_H

#include <vector> //����
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>  // ������ת

namespace Game {

    //ģ������������͵�������
    template <typename T>
    class LifecycleManager
    {
    public:
        // ע�� MonoBehaviour �����������ڹ�����
        void RegisterObject(T* object);
        std::vector<T*> GetNativeObjects();
        std::vector<T*> GetVariantObjects();
        std::vector<T*> GetColloderObjects();
        static LifecycleManager<T>* GetInstance();
        // ����ѭ���е���ÿ��������������ڷ���
        void StartAll();
        void UpdateAll(glm::mat4 view, glm::mat4 projection);
        void DestroyAll();
        //--������·���
        void UpdateAllVariant(glm::mat4 view, glm::mat4 projection);

        //��Դ���ģ�ͻ���
        void UpdateDepthPic(glm::mat4 projection, GLuint shader);

        //�����������
        void PhysicalUpdateAll();

    private:
        std::vector<T*> nativeObjects;   // �洢ע��Ķ���
        std::vector<T*> variantObjects;  // �洢�������
        std::vector<T*> colliderObjects; //������ײ����
        LifecycleManager() {};
        static LifecycleManager<T>* instance ;

    public:
        //��������
    };

    // ��ͷ�ļ���ʵ��ģ�巽��,���ﶼ����������д�����ⱨ��
    //ԭʼ�����װ��������ִ�����и����߼�����������װ��ִ�б����߼������Ż�����ײ�����װ��ר��ִ����ײ�߼�
    template <typename T>
    void LifecycleManager<T>::RegisterObject(T* object) {
        nativeObjects.push_back(object);
        if (object->GetVariant() != 0)
            variantObjects.push_back(object);
        if (object->GetIfCollision() == true)
            colliderObjects.push_back(object);
    }

    template <typename T>
    std::vector<T*> LifecycleManager<T>::GetNativeObjects() {
        return nativeObjects;
    }

    template <typename T>
    std::vector<T*> LifecycleManager<T>::GetVariantObjects() {
        return variantObjects;
    }

    template<typename T>
    inline std::vector<T*> LifecycleManager<T>::GetColloderObjects()
    {
        return colliderObjects;
    }

    template <typename T>
    LifecycleManager<T>* LifecycleManager<T>::GetInstance() {
        if (instance == nullptr) {
            instance = new LifecycleManager<T>();
        }
        return instance;
    }

    template <typename T>
    void LifecycleManager<T>::StartAll() {
        for (T* object : nativeObjects) {
            object->Start();  // ����ÿ������� Start ����
        }
    }

    template <typename T>
    void LifecycleManager<T>::UpdateAll(glm::mat4 view, glm::mat4 projection) {
        for (T* object : nativeObjects) {
            object->Update(view, projection);  // ����ÿ������� Update ����
        }
    }

    template <typename T>
    void LifecycleManager<T>::DestroyAll() {
        for (T* object : nativeObjects) {
            object->Destroy();  // ����ÿ������� Destroy ����
        }
    }

    template <typename T>
    void LifecycleManager<T>::UpdateAllVariant(glm::mat4 view, glm::mat4 projection) {
        for (T* object : variantObjects) {
            object->UpdateVariant(view, projection);
        }
    }

    template <typename T>
    void LifecycleManager<T>::UpdateDepthPic(glm::mat4 projection, GLuint shader) {
        for (T* object : variantObjects) {
            object->UpdateDepthPic(projection, shader);
        }
    }




} // namespace Game

#endif // LIFECYCLE_MANAGER_H
