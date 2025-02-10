#ifndef Custom_Model_H
#define Custom_Model_H
//！！！--即使头文件中有相互包含，一定要注意顺序，这是很可能有隐藏的编译错误的地方，检查不到，程序也不能运行
//#include "GameObject.h"
#include "AnimationIntergrated.h"

namespace Game {
    class CustomModel : public GameObject
    {
    public:
        CustomModel(const char* vertexShaderSourceIn, const char* fragmentShaderSourceIn, GLfloat* vertices, GLuint* indices, size_t vertexCount, size_t indexCount, bool ifLight = false);
        CustomModel(const char* vertexShaderSourceIn, const char* fragmentShaderSourceIn, GLfloat* vertices, GLuint* indices, size_t vertexCount, size_t indexCount, int ifVaraint, bool ifLight = false);
        //--更新方法
        CustomModel(const char* vertexShaderSourceIn, const char* fragmentShaderSourceIn, std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& normals, bool ifLight = false);
        CustomModel(const char* vertexShaderSourceIn, const char* fragmentShaderSourceIn, std::vector<Vertex>& vertices, std::vector<unsigned int>& finalIndicesm, bool ifLight = false);
        CustomModel(const char* vertexShaderSourceIn, const char* fragmentShaderSourceIn, std::vector<Vertex>& vertices, std::vector<unsigned int>& finalIndices, bool isSkinnedMesh, bool ifLight = false);
        CustomModel(const char* vertexShaderSourceIn, const char* fragmentShaderSourceIn, const ModelData& modelData, bool isSkinnedMesh, bool ifLight = false);
        void UpdateVerticesForAnimation(size_t animationFrame);
        void UpdateVerticesForAnimation(const std::vector<Vertex>& vertex);

        //--继承GameObject 的声明
        virtual bool Draw(glm::mat4 view, glm::mat4 projection) override;//静态绘制可重写
        virtual bool DrawLine(glm::mat4 view, glm::mat4 projection);//绘制线条方法，在custom 类添加的新方法，针对线条绘制，可重写
        virtual bool DrawLineFixedWidget(glm::mat4 view);//绘制坐标系方法，固定在一个屏幕里面，可以重写
        ~CustomModel();
        //--继承MonoBehaviour的声明
        virtual void Update(glm::mat4 view, glm::mat4 projection) override;//常规移动可重写
        virtual void UpdateVariant(glm::mat4 view, glm::mat4 projection) override;//变体移动可重写
        virtual bool DrawDynamical(glm::mat4 view, glm::mat4 projection) override;//动态绘制可重写，与IntergtatedAnimatior联动
        //--动画模块，因为每个对象具体的动画操作也许会不一样，目前采用分离方法
        virtual void AttachAnimationController(AnimationData animaitonData);//附加动画控制器，可传入一个默认动画
        virtual void AttachAnimationController(); //附加动画控制器，无默认参数，完全内部构造，可重写
        virtual void PlayAnimation(int index, float frame = 0.0167f);//对象播放动画方法，避免外部传入对象，可重写
        virtual void StopPlayAnimation();//对象停止播放动画方法，可重写
        //
        virtual bool AttachTexture() override;//附加纹理处理可重写
        //
        virtual bool AttachPhysicalEngine();//附加物理引擎可重写
        virtual bool AttachCollider();//附件碰撞体可重写


    private:
#pragma region   数组初始化原则：{}只能在声明时进行初始化
        size_t index;
        size_t vertexCount;
        std::vector<Vertex> verticesTras = {};
        //  GLfloat verticesIns[];
          //动画控制组件
        AnimationController* animator = nullptr;
        PhysicalEngine* physicsBody = nullptr;
        CollisionBody* collider = nullptr;
#pragma endregion



        //采用 consterxpr  绕过无效编译检查， static_assert 断言 进行编译阶段检查
    public:
        template <typename T>
        T* GetComponent() {
            if constexpr (std::is_same<T, AnimationController>::value) {
                return animator;
            }
            else if constexpr (std::is_same<T, PhysicalEngine>::value) {
                return physicsBody;
            }
            else if constexpr (std::is_same<T, CollisionBody>::value) {
                return collider;
            }
            else {
                static_assert(false, "Unsupported component type");
            }
        }
    };
}
#endif // CUBE_H
