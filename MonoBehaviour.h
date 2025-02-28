#ifndef MONOBEHAVIOUR_H
#define MONOBEHAVIOUR_H
#include "EnumTotal.h"

namespace Game {
    //���ֽӿ���ʵ���ϾͲ���Ҫ������.CPP�ļ�ȥʵ���ˣ�ֱ���ڼ̳����CPP�ļ���ȥʵ��
    class MonoBehaviour {
    public:
        virtual void Start() = 0;    // ��Ϸ��ʼʱ����
        virtual void Update(glm::mat4 view, glm::mat4 projection) = 0;   // ÿ֡����ʱ���ã���Ϊ��ͼ��ͶӰ����û�����ɾ�̬������������Ҫ���룬����ÿ��ʵ����ȥ��ȡ
        //�о�ʵ�ʵ������ڲ����������ɾ�̬�ģ����߿���Զ����뵥������        
        virtual void Destroy() = 0;  // ����ʱ����
        virtual void SetVariant(ModelClass par) = 0;
        virtual ModelClass GetVariant() = 0;
        virtual void UpdateVariant(glm::mat4 view, glm::mat4 projection) = 0;
        virtual void UpdateDepthPic(glm::mat4 lightSpaceMatrix,GLuint shader)=0;

        virtual bool GetIfCollision() = 0;
        virtual ~MonoBehaviour() = default;  // ����������
    };
}
#endif // MONOBEHAVIOUR_H
#pragma once
