#ifndef Custom_Model_H
#define Custom_Model_H
#include "AnimationIntergrated.h"

namespace Game {
    extern std::unordered_map<int, CollisionProperties*> CollisionProps;//������ײ���ڲ���������ڲ����м��
    class LightSpawner;
    class CustomModel : public GameObject
    {
    public:
        CustomModel();
        CustomModel(const char* vertexShaderSourceIn, const char* fragmentShaderSourceIn, GLfloat* vertices, GLuint* indices, size_t vertexCount, size_t indexCount, bool ifLight = false);
        CustomModel(const char* vertexShaderSourceIn, const char* fragmentShaderSourceIn, GLfloat* vertices, GLuint* indices, size_t vertexCount, size_t indexCount, int ifVaraint, bool ifLight = false);
        //--���·���
        CustomModel(const char* vertexShaderSourceIn, const char* fragmentShaderSourceIn, std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& normals, bool ifLight = false);
        CustomModel(const char* vertexShaderSourceIn, const char* fragmentShaderSourceIn, std::vector<Vertex>& vertices, std::vector<unsigned int>& finalIndicesm, bool ifLight = false);
        CustomModel(const char* vertexShaderSourceIn, const char* fragmentShaderSourceIn, std::vector<Vertex>& vertices, std::vector<unsigned int>& finalIndices, bool isSkinnedMesh, bool ifLight = false);
       //--��÷���
        CustomModel(const char* vertexShaderSourceIn, const char* fragmentShaderSourceIn, const ModelData& modelData, bool isSkinnedMesh, bool ifLight = false,bool ifShadow=false);
        
        void UpdateVerticesForAnimation(size_t animationFrame);
        void UpdateVerticesForAnimation(const std::vector<Vertex>& vertex);
        virtual void Start() override;
        virtual void SpecialMethod();//���ⷽ���������������ⲿ������д
        virtual void SelfIns(); //�����ʼ��������������������д
        //--�̳�GameObject ������
        virtual bool Draw(glm::mat4 view, glm::mat4 projection) override;//��̬���ƿ���д
        virtual bool DrawLine(glm::mat4 view, glm::mat4 projection);//����������������custom ����ӵ��·���������������ƣ�����д
        virtual bool DrawLineFixedWidget(glm::mat4 view, glm::mat4 projection);//��������ϵ�������̶���һ����Ļ���棬������д
       virtual ~CustomModel();
        //--�̳�MonoBehaviour������
        virtual void Update(glm::mat4 view, glm::mat4 projection) override;//�����ƶ�����д
        virtual void UpdateVariant(glm::mat4 view, glm::mat4 projection) override;//�����ƶ�����д
        virtual bool DrawDynamical(glm::mat4 view, glm::mat4 projection) override;//��̬���ƿ���д����IntergtatedAnimatior����
        //--����ģ�飬��Ϊÿ���������Ķ�������Ҳ��᲻һ����Ŀǰ���÷��뷽��
        virtual void AttachAnimationController(AnimationData animaitonData);//���Ӷ������������ɴ���һ��Ĭ�϶���
        virtual void AttachAnimationController(); //���Ӷ�������������Ĭ�ϲ�������ȫ�ڲ����죬����д
        virtual void PlayAnimation(int index, float frame = 0.0167f);//���󲥷Ŷ��������������ⲿ������󣬿���д
        virtual void StopPlayAnimation();//����ֹͣ���Ŷ�������������д
        //���ͼģ��,��д����
        virtual void DrawDepthPic(glm::mat4 lightSpaceMatrix,GLuint shader) override; //��̬�������ͼ������д
        virtual void DrawDepthPicDynamical(glm::mat4 lightSpaceMatrix,GLuint shader) override;//��̬�������ͼ������д
        virtual void UpdateDepthPic(glm::mat4 lightSpaceMatrix,GLuint shader) override;//���ͼ���·���������д
        //�ӿ����ͼģ�飬���ڶ�ȡ�ӿ����ֵ
        virtual void DrawDepthViewPortPic(glm::mat4 view, glm::mat4 projection, GLuint shader) ; //��̬�������ͼ������д
        virtual void DrawDepthViewPortPicDynamical(glm::mat4 view, glm::mat4 projection, GLuint shader) ;//��̬�������ͼ������д
        virtual void UpdateDepthViewPortPic(glm::mat4 view, glm::mat4 projection, GLuint shader) ;//���ͼ���·���������д
        //��������ģ��
        virtual void UpdatePhysics();
        //�����ײ
        virtual void UpdateCollisionAndPhysics(std::unordered_map<int, CollisionProperties*>& cop);
        //�����������������ѹ������
        virtual bool AttachTexture(const std::unordered_map<PictureTpye, GLuint>& textureData, int order=0, glm::vec2 textureScale= glm::vec2(1));
        virtual void UniformParametersIns(glm::vec3 baseColor=glm::vec3(0.3f), glm::vec3 emission=glm::vec3(0.05f), float metallic=0, float roughness=1, float opacity=1, float IOR=1.33f, float ao=0.6f, float dissolveThreshold = 0);//shadner ����������ʼ��
        virtual void RenderingLight(LightSpawner* lightSpawner);      
        //��������������ʼ����������
        virtual bool AttachPhysicalEngine(bool staticObj = false,float mass=1, float friction=0.05f, glm::vec3 velocity = glm::vec3(0), glm::vec3 acceleration = glm::vec3(0, -9.8f, 0),
            float elasticity = 0.5f, bool lockXZAxi = true, float rotationDampping = 0.15f,  bool trigger = false, int layer = 1, float rotationAdjust = 0.382f
            ); 
        virtual bool AttachCollider(CollisionType collider=CollisionType::Box, SpecialType type = SpecialType::OriginalT,float radius=1);//������ײ�����д
        //�ж��Ƿ���Ҫ��ײ����ײ�Ķ��󣬷�װ�ڷ��ͻ����е�vector������
        virtual bool GetIfCollision() override;  
        //����
         void DestroySelf();
        //������ʧ��
         bool SetActive(bool active);
        //����ID�����߼�
        int GetID();
        int SetID(int id);
        //��ȡ״̬
        bool GetActiveState()const ;
        //������Ϸ����
        float timeAccumulator=0;//������ʱ�������,�ⲿ��ֱ�ӷ���
        

    protected:
#pragma region   �����ʼ��ԭ��{}ֻ��������ʱ���г�ʼ��

        std::vector<Vertex> verticesTras = {};
        //  GLfloat verticesIns[];
          //�����������
        AnimationController* _animator = nullptr;
        PhysicalEngine* _physicsBody = nullptr;
        CollisionBody* _collider = nullptr;

    protected:
        //��ͼ���,���ڼ̳й�ϵ������������Ҫ����
        std::unordered_map<PictureTpye, GLuint> _textures; // ��������
        int _textureOrder;//����ʹ�õ�����Ԫ
        bool _drawTexture;
        glm::vec2 _textureScale;
        size_t index;
        size_t vertexCount;
#pragma endregion

        //��ײ�弰����ģ�����
    protected:
        bool _ifCollision;
        glm::vec3 _collisionMin;
        glm::vec3 _collisionMax;
        bool _ifPhysics;
        bool _ifActive = true;
        //��Ϸ����ȫ��Ψһ��ʶ��
        int ID;
        //������Ⱦ�ӿ�͸�����ͼ������ͨ��CustomModelShader������
        bool _enableDepth;
        float _timeAccumulator=0;//������ʱ�������,���ڲ�ʹ��



    protected:
        //����İ󶨼����Σ������ڲ�����
        virtual void BindTexture();//��Ⱦ����������д
        virtual void BindTextureAdditional();//���ӵ���Ⱦ������
        virtual void UniformParametersInput();//ȫ��shader����������д
        //���� consterxpr  �ƹ���Ч�����飬 static_assert ���� ���б���׶μ��
    public:
        template <typename T>
        T* GetComponent() {
            if constexpr (std::is_same<T, AnimationController>::value) {
                return _animator;
            }
            else if constexpr (std::is_same<T, PhysicalEngine>::value) {
                return _physicsBody;
            }
            else if constexpr (std::is_same<T, CollisionBody>::value) {
                return _collider;
            }
            else {
                static_assert(false, "Unsupported component type");
            }
        }
    };
}
#endif // CUBE_H
