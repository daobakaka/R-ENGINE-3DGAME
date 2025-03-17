#include "DesignModel.h"
#include "ShaderManager.h"
#include "LifecycleManager.h"
#include "light.h"
using namespace Game;
Game::DesignModel::DesignModel(const std::string& name, const Model& modelData, bool isSkinnedMesh, bool ifLightIn, bool ifShadow)
{
    _modelData = modelData;//传递引用的值
    IsSkinnedMesh = isSkinnedMesh;
    ifLight = ifLightIn;
    _ifShadow = ifShadow;

    // 获取光照实例
    lightSpawner = LightSpawner::GetInstance();

    // 获取着色器程序
    shaderProgram = ShaderManager::GetInstance()->GetShader(name);

    // 遍历 Model 中的所有 Mesh
    for (size_t i = 0; i < modelData.meshes.size(); i++) {
        const Mesh& mesh = modelData.meshes[i];

        // 为每个 Mesh 生成 VAO、VBO 和 EBO
        GLuint VAO, VBO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // 绑定 VAO
        glBindVertexArray(VAO);

        // 根据是否为动态网格（例如骨骼动画网格）决定使用动态或静态缓冲区
        GLuint bufferUsage = isSkinnedMesh ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;

        // VBO: 传入当前 Mesh 的所有顶点
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER,
            mesh.vertices.size() * sizeof(MeshVertex), // 使用 MeshVertex 结构体的大小
            mesh.vertices.data(), // 使用当前 Mesh 的顶点数据
            bufferUsage);  // 根据是否为动态网格选择缓冲区类型

        // EBO: 传入当前 Mesh 的索引
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            mesh.indices.size() * sizeof(unsigned int), // 使用索引数据的大小
            mesh.indices.data(), // 使用当前 Mesh 的索引数据
            bufferUsage);  // 同样根据是否为动态网格选择缓冲区类型

        // 设置顶点属性指针
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
            sizeof(MeshVertex),
            (void*)offsetof(MeshVertex, Position));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
            sizeof(MeshVertex),
            (void*)offsetof(MeshVertex, Normal));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
            sizeof(MeshVertex),
            (void*)offsetof(MeshVertex, TexCoords));

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE,
            sizeof(MeshVertex),
            (void*)offsetof(MeshVertex, Tangent));

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE,
            sizeof(MeshVertex),
            (void*)offsetof(MeshVertex, Bitangent));

        // 设置骨骼索引和权重属性
        glEnableVertexAttribArray(5);
        glVertexAttribIPointer(5, 4, GL_INT, sizeof(MeshVertex),
            (void*)offsetof(MeshVertex, BoneIDs));

        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE,
            sizeof(MeshVertex),
            (void*)offsetof(MeshVertex, Weights));

        // 解绑 VAO
        glBindVertexArray(0);

        // 将 VAO、VBO 和 EBO 存储起来，以便后续渲染使用
        _meshVAOs.push_back(VAO);
        _meshVBOs.push_back(VBO);
        _meshEBOs.push_back(EBO);
    }

}

Game::DesignModel::~DesignModel()
{
}

bool Game::DesignModel::Draw(glm::mat4 view, glm::mat4 projection)
{
    glUseProgram(shaderProgram);
    // 遍历所有 Mesh 并渲染
    for (size_t i = 0; i < _meshVAOs.size(); i++) {

        //集成纹理渲染方法,要在draw之前调用，才能生效
        BindTexture();
        BindTextureAdditional();
        // 设置模型矩阵
        GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));

        // 绑定当前 Mesh 的 VAO
        glBindVertexArray(_meshVAOs[i]);

        // 绘制当前 Mesh
        if (justDrawVerteies) {
            glDrawArrays(GL_TRIANGLES, 0, _modelData.meshes[i].vertices.size());
        }
        else {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _meshEBOs[i]);
            glDrawElements(GL_TRIANGLES, _modelData.meshes[i].indices.size(), GL_UNSIGNED_INT, 0);
        }

        // 解绑 VAO
        glBindVertexArray(0);
    }
    UniformParametersInput();//父类在这里调用空方法
    RenderingLight();//通用shader，光的渲染在这里进行

    return true;
}

bool Game::DesignModel::DrawDynamical(glm::mat4 view, glm::mat4 projection)
{
    glUseProgram(shaderProgram);
    // 遍历所有 Mesh 并渲染
    for (size_t i = 0; i < _meshVAOs.size(); i++) {
        //集成纹理渲染方法,要在draw之前调用，才能生效
        BindTexture();
        BindTextureAdditional();
        // 设置模型矩阵
        GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));

        // 绑定当前 Mesh 的 VAO 和 VBO
        glBindVertexArray(_meshVAOs[i]);
        glBindBuffer(GL_ARRAY_BUFFER, _meshVBOs[i]);

        // 更新动态顶点数据
        glBufferData(GL_ARRAY_BUFFER,
            _modelData.meshes[i].vertices.size() * sizeof(MeshVertex),
            _modelData.meshes[i].vertices.data(),
            GL_DYNAMIC_DRAW);

        // 绘制当前 Mesh
        if (justDrawVerteies) {
            glDrawArrays(GL_TRIANGLES, 0, _modelData.meshes[i].vertices.size());
        }
        else {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _meshEBOs[i]);
            glDrawElements(GL_TRIANGLES, _modelData.meshes[i].indices.size(), GL_UNSIGNED_INT, 0);
        }

        // 解绑 VAO
        glBindVertexArray(0);
    }
    UniformParametersInput();//父类在这里调用空方法
    RenderingLight();//通用shader，光的渲染在这里进行

    return true;

}
void DesignModel::DrawDepthPic(glm::mat4 lightSpaceMatrix, GLuint shader)
{
    // 遍历所有 Mesh 并渲染
    for (size_t i = 0; i < _meshVAOs.size(); i++) {
        // 设置模型矩阵
        GLuint modelLoc = glGetUniformLocation(shader, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));

        // 绑定当前 Mesh 的 VAO
        glBindVertexArray(_meshVAOs[i]);

        // 绘制当前 Mesh
        if (justDrawVerteies) {
            glDrawArrays(GL_TRIANGLES, 0, _modelData.meshes[i].vertices.size());
        }
        else {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _meshEBOs[i]);
            glDrawElements(GL_TRIANGLES, _modelData.meshes[i].indices.size(), GL_UNSIGNED_INT, 0);
        }

        // 解绑 VAO
        glBindVertexArray(0);
    }


}
void Game::DesignModel::UniformParametersInput()
{
    //金属度
    GLuint metallicLoc = glGetUniformLocation(shaderProgram, "metallic");
    glUniform1f(metallicLoc, 0.0f);

    //糙度
    GLuint roughnessLoc = glGetUniformLocation(shaderProgram, "roughness");
    glUniform1f(roughnessLoc, 1.0f);
    //透明度
    GLuint opacityLoc = glGetUniformLocation(shaderProgram, "opacity");
    glUniform1f(opacityLoc, 1.0f);
    //折射率
    GLuint IORLoc = glGetUniformLocation(shaderProgram, " IOR");
    glUniform1f(IORLoc, 1.330f);
    //环境光贡献率
    GLuint aoLoc = glGetUniformLocation(shaderProgram, "ao");
    glUniform1f(aoLoc, 1.0f);


    // 自发光
    GLuint emissionLoc = glGetUniformLocation(shaderProgram, "emission");
    glUniform3f(emissionLoc, .05f, .05f, .05f); // 传入自发光颜色

    // 基本色
    GLuint baseColorLoc = glGetUniformLocation(shaderProgram, "baseColor");
    glUniform3f(baseColorLoc, 0.1f, 0.1f, 0.1f); // 传入基本色（暗色）





}
void DesignModel::DrawDepthPicDynamical(glm::mat4 lightSpaceMatrix, GLuint shader)
{

    for (size_t i = 0; i < _meshVAOs.size(); i++) {
        // 设置模型矩阵
        GLuint modelLoc = glGetUniformLocation(shader, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));

        // 绑定当前 Mesh 的 VAO 和 VBO
        glBindVertexArray(_meshVAOs[i]);
        glBindBuffer(GL_ARRAY_BUFFER, _meshVBOs[i]);

        // 更新动态顶点数据
        glBufferData(GL_ARRAY_BUFFER,
            _modelData.meshes[i].vertices.size() * sizeof(MeshVertex),
            _modelData.meshes[i].vertices.data(),
            GL_DYNAMIC_DRAW);

        // 绘制当前 Mesh
        if (justDrawVerteies) {
            glDrawArrays(GL_TRIANGLES, 0, _modelData.meshes[i].vertices.size());
        }
        else {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _meshEBOs[i]);
            glDrawElements(GL_TRIANGLES, _modelData.meshes[i].indices.size(), GL_UNSIGNED_INT, 0);
        }

        // 解绑 VAO
        glBindVertexArray(0);
    }

}