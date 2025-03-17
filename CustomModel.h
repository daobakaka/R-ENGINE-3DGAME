#ifndef Custom_Model_H
#define Custom_Model_H
#include "AnimationIntergrated.h"

namespace Game {
    extern std::unordered_map<int, CollisionProperties*> CollisionProps;//就在碰撞体内部这个大类内部进行检查
    class LightSpawner;
    class CustomModel : public GameObject
    {
    public:
        CustomModel();
        CustomModel(const char* vertexShaderSourceIn, const char* fragmentShaderSourceIn, GLfloat* vertices, GLuint* indices, size_t vertexCount, size_t indexCount, bool ifLight = false);
        CustomModel(const char* vertexShaderSourceIn, const char* fragmentShaderSourceIn, GLfloat* vertices, GLuint* indices, size_t vertexCount, size_t indexCount, int ifVaraint, bool ifLight = false);
        //--更新方法
        CustomModel(const char* vertexShaderSourceIn, const char* fragmentShaderSourceIn, std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& normals, bool ifLight = false);
        CustomModel(const char* vertexShaderSourceIn, const char* fragmentShaderSourceIn, std::vector<Vertex>& vertices, std::vector<unsigned int>& finalIndicesm, bool ifLight = false);
        CustomModel(const char* vertexShaderSourceIn, const char* fragmentShaderSourceIn, std::vector<Vertex>& vertices, std::vector<unsigned int>& finalIndices, bool isSkinnedMesh, bool ifLight = false);
       //--最常用方法
        CustomModel(const char* vertexShaderSourceIn, const char* fragmentShaderSourceIn, const ModelData& modelData, bool isSkinnedMesh, bool ifLight = false,bool ifShadow=false);
        
        void UpdateVerticesForAnimation(size_t animationFrame);
        void UpdateVerticesForAnimation(const std::vector<Vertex>& vertex);
        virtual void Start() override;
        virtual void SpecialMethod();//特殊方法，用于派生类外部访问重写
        virtual void SelfIns(); //特殊初始化方法，用于派生类重写
        //--继承GameObject 的声明
        virtual bool Draw(glm::mat4 view, glm::mat4 projection) override;//静态绘制可重写
        virtual bool DrawLine(glm::mat4 view, glm::mat4 projection);//绘制线条方法，在custom 类添加的新方法，针对线条绘制，可重写
        virtual bool DrawLineFixedWidget(glm::mat4 view, glm::mat4 projection);//绘制坐标系方法，固定在一个屏幕里面，可以重写
       virtual ~CustomModel();
        //--继承MonoBehaviour的声明
        virtual void Update(glm::mat4 view, glm::mat4 projection) override;//常规移动可重写
        virtual void UpdateVariant(glm::mat4 view, glm::mat4 projection) override;//变体移动可重写
        virtual bool DrawDynamical(glm::mat4 view, glm::mat4 projection) override;//动态绘制可重写，与IntergtatedAnimatior联动
        //--动画模块，因为每个对象具体的动画操作也许会不一样，目前采用分离方法
        virtual void AttachAnimationController(AnimationData animaitonData);//附加动画控制器，可传入一个默认动画
        virtual void AttachAnimationController(); //附加动画控制器，无默认参数，完全内部构造，可重写
        virtual void PlayAnimation(int index, float frame = 0.0167f);//对象播放动画方法，避免外部传入对象，可重写
        virtual void StopPlayAnimation();//对象停止播放动画方法，可重写
        //深度图模块,重写基类
        virtual void DrawDepthPic(glm::mat4 lightSpaceMatrix,GLuint shader) override; //静态绘制深度图，可重写
        virtual void DrawDepthPicDynamical(glm::mat4 lightSpaceMatrix,GLuint shader) override;//动态绘制深度图，可重写
        virtual void UpdateDepthPic(glm::mat4 lightSpaceMatrix,GLuint shader) override;//深度图更新方法，可重写
        //视口深度图模块，用于读取视口深度值
        virtual void DrawDepthViewPortPic(glm::mat4 view, glm::mat4 projection, GLuint shader) ; //静态绘制深度图，可重写
        virtual void DrawDepthViewPortPicDynamical(glm::mat4 view, glm::mat4 projection, GLuint shader) ;//动态绘制深度图，可重写
        virtual void UpdateDepthViewPortPic(glm::mat4 view, glm::mat4 projection, GLuint shader) ;//深度图更新方法，可重写
        //物理引擎模块
        virtual void UpdatePhysics();
        //检查碰撞
        virtual void UpdateCollisionAndPhysics(std::unordered_map<int, CollisionProperties*>& cop);
        //附加纹理方法，添加了压缩因子
        virtual bool AttachTexture(const std::unordered_map<PictureTpye, GLuint>& textureData, int order=0, glm::vec2 textureScale= glm::vec2(1));
        virtual void UniformParametersIns(glm::vec3 baseColor=glm::vec3(0.3f), glm::vec3 emission=glm::vec3(0.05f), float metallic=0, float roughness=1, float opacity=1, float IOR=1.33f, float ao=0.6f, float dissolveThreshold = 0);//shadner 基本参数初始化
        virtual void RenderingLight(LightSpawner* lightSpawner);      
        //添加物理组件，初始化基本参数
        virtual bool AttachPhysicalEngine(bool staticObj = false,float mass=1, float friction=0.05f, glm::vec3 velocity = glm::vec3(0), glm::vec3 acceleration = glm::vec3(0, -9.8f, 0),
            float elasticity = 0.5f, bool lockXZAxi = true, float rotationDampping = 0.15f,  bool trigger = false, int layer = 1, float rotationAdjust = 0.382f
            ); 
        virtual bool AttachCollider(CollisionType collider=CollisionType::Box, SpecialType type = SpecialType::OriginalT,float radius=1);//附件碰撞体可重写
        //判断是否需要碰撞，碰撞的对象，封装在泛型基类中的vector容器里
        virtual bool GetIfCollision() override;  
        //销毁
         void DestroySelf();
        //激活与失活
         bool SetActive(bool active);
        //对象ID处理逻辑
        int GetID();
        int SetID(int id);
        //获取状态
        bool GetActiveState()const ;
        //额外游戏参数
        float timeAccumulator=0;//独立的时间计数器,外部可直接访问
        

    protected:
#pragma region   数组初始化原则：{}只能在声明时进行初始化

        std::vector<Vertex> verticesTras = {};
        //  GLfloat verticesIns[];
          //动画控制组件
        AnimationController* _animator = nullptr;
        PhysicalEngine* _physicsBody = nullptr;
        CollisionBody* _collider = nullptr;

    protected:
        //贴图组件,存在继承关系，这里子类需要访问
        std::unordered_map<PictureTpye, GLuint> _textures; // 纹理容器
        int _textureOrder;//绘制使用的纹理单元
        bool _drawTexture;
        glm::vec2 _textureScale;
        size_t index;
        size_t vertexCount;
#pragma endregion

        //碰撞体及物理模块参数
    protected:
        bool _ifCollision;
        glm::vec3 _collisionMin;
        glm::vec3 _collisionMax;
        bool _ifPhysics;
        bool _ifActive = true;
        //游戏对象全局唯一标识符
        int ID;
        //允许渲染视口透视深度图，必须通过CustomModelShader类生成
        bool _enableDepth;
        float _timeAccumulator=0;//独立的时间计数器,供内部使用



    protected:
        //纹理的绑定及传参，在类内部进行
        virtual void BindTexture();//渲染纹理方法可重写
        virtual void BindTextureAdditional();//附加的渲染纹理方法
        virtual void UniformParametersInput();//全局shader参数输入重写
        //采用 consterxpr  绕过无效编译检查， static_assert 断言 进行编译阶段检查
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
