#include "DesignModel.h"
#include "ShaderManager.h"
#include "LifecycleManager.h"
#include "light.h"
using namespace Game;
Game::DesignModel::DesignModel(const std::string& name, const Model& modelData, bool isSkinnedMesh, bool ifLightIn, bool ifShadow)
{
    _modelData = modelData;//�������õ�ֵ
    IsSkinnedMesh = isSkinnedMesh;
    ifLight = ifLightIn;
    _ifShadow = ifShadow;

    // ��ȡ����ʵ��
    lightSpawner = LightSpawner::GetInstance();

    // ��ȡ��ɫ������
    shaderProgram = ShaderManager::GetInstance()->GetShader(name);

    // ���� Model �е����� Mesh
    for (size_t i = 0; i < modelData.meshes.size(); i++) {
        const Mesh& mesh = modelData.meshes[i];

        // Ϊÿ�� Mesh ���� VAO��VBO �� EBO
        GLuint VAO, VBO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // �� VAO
        glBindVertexArray(VAO);

        // �����Ƿ�Ϊ��̬������������������񣩾���ʹ�ö�̬��̬������
        GLuint bufferUsage = isSkinnedMesh ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;

        // VBO: ���뵱ǰ Mesh �����ж���
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER,
            mesh.vertices.size() * sizeof(MeshVertex), // ʹ�� MeshVertex �ṹ��Ĵ�С
            mesh.vertices.data(), // ʹ�õ�ǰ Mesh �Ķ�������
            bufferUsage);  // �����Ƿ�Ϊ��̬����ѡ�񻺳�������

        // EBO: ���뵱ǰ Mesh ������
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            mesh.indices.size() * sizeof(unsigned int), // ʹ���������ݵĴ�С
            mesh.indices.data(), // ʹ�õ�ǰ Mesh ����������
            bufferUsage);  // ͬ�������Ƿ�Ϊ��̬����ѡ�񻺳�������

        // ���ö�������ָ��
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

        // ���ù���������Ȩ������
        glEnableVertexAttribArray(5);
        glVertexAttribIPointer(5, 4, GL_INT, sizeof(MeshVertex),
            (void*)offsetof(MeshVertex, BoneIDs));

        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE,
            sizeof(MeshVertex),
            (void*)offsetof(MeshVertex, Weights));

        // ��� VAO
        glBindVertexArray(0);

        // �� VAO��VBO �� EBO �洢�������Ա������Ⱦʹ��
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
    // �������� Mesh ����Ⱦ
    for (size_t i = 0; i < _meshVAOs.size(); i++) {

        //����������Ⱦ����,Ҫ��draw֮ǰ���ã�������Ч
        BindTexture();
        BindTextureAdditional();
        // ����ģ�;���
        GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));

        // �󶨵�ǰ Mesh �� VAO
        glBindVertexArray(_meshVAOs[i]);

        // ���Ƶ�ǰ Mesh
        if (justDrawVerteies) {
            glDrawArrays(GL_TRIANGLES, 0, _modelData.meshes[i].vertices.size());
        }
        else {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _meshEBOs[i]);
            glDrawElements(GL_TRIANGLES, _modelData.meshes[i].indices.size(), GL_UNSIGNED_INT, 0);
        }

        // ��� VAO
        glBindVertexArray(0);
    }
    UniformParametersInput();//������������ÿշ���
    RenderingLight();//ͨ��shader�������Ⱦ���������

    return true;
}

bool Game::DesignModel::DrawDynamical(glm::mat4 view, glm::mat4 projection)
{
    glUseProgram(shaderProgram);
    // �������� Mesh ����Ⱦ
    for (size_t i = 0; i < _meshVAOs.size(); i++) {
        //����������Ⱦ����,Ҫ��draw֮ǰ���ã�������Ч
        BindTexture();
        BindTextureAdditional();
        // ����ģ�;���
        GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));

        // �󶨵�ǰ Mesh �� VAO �� VBO
        glBindVertexArray(_meshVAOs[i]);
        glBindBuffer(GL_ARRAY_BUFFER, _meshVBOs[i]);

        // ���¶�̬��������
        glBufferData(GL_ARRAY_BUFFER,
            _modelData.meshes[i].vertices.size() * sizeof(MeshVertex),
            _modelData.meshes[i].vertices.data(),
            GL_DYNAMIC_DRAW);

        // ���Ƶ�ǰ Mesh
        if (justDrawVerteies) {
            glDrawArrays(GL_TRIANGLES, 0, _modelData.meshes[i].vertices.size());
        }
        else {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _meshEBOs[i]);
            glDrawElements(GL_TRIANGLES, _modelData.meshes[i].indices.size(), GL_UNSIGNED_INT, 0);
        }

        // ��� VAO
        glBindVertexArray(0);
    }
    UniformParametersInput();//������������ÿշ���
    RenderingLight();//ͨ��shader�������Ⱦ���������

    return true;

}
void DesignModel::DrawDepthPic(glm::mat4 lightSpaceMatrix, GLuint shader)
{
    // �������� Mesh ����Ⱦ
    for (size_t i = 0; i < _meshVAOs.size(); i++) {
        // ����ģ�;���
        GLuint modelLoc = glGetUniformLocation(shader, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));

        // �󶨵�ǰ Mesh �� VAO
        glBindVertexArray(_meshVAOs[i]);

        // ���Ƶ�ǰ Mesh
        if (justDrawVerteies) {
            glDrawArrays(GL_TRIANGLES, 0, _modelData.meshes[i].vertices.size());
        }
        else {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _meshEBOs[i]);
            glDrawElements(GL_TRIANGLES, _modelData.meshes[i].indices.size(), GL_UNSIGNED_INT, 0);
        }

        // ��� VAO
        glBindVertexArray(0);
    }


}
void Game::DesignModel::UniformParametersInput()
{
    //������
    GLuint metallicLoc = glGetUniformLocation(shaderProgram, "metallic");
    glUniform1f(metallicLoc, 0.0f);

    //�ڶ�
    GLuint roughnessLoc = glGetUniformLocation(shaderProgram, "roughness");
    glUniform1f(roughnessLoc, 1.0f);
    //͸����
    GLuint opacityLoc = glGetUniformLocation(shaderProgram, "opacity");
    glUniform1f(opacityLoc, 1.0f);
    //������
    GLuint IORLoc = glGetUniformLocation(shaderProgram, " IOR");
    glUniform1f(IORLoc, 1.330f);
    //�����⹱����
    GLuint aoLoc = glGetUniformLocation(shaderProgram, "ao");
    glUniform1f(aoLoc, 1.0f);


    // �Է���
    GLuint emissionLoc = glGetUniformLocation(shaderProgram, "emission");
    glUniform3f(emissionLoc, .05f, .05f, .05f); // �����Է�����ɫ

    // ����ɫ
    GLuint baseColorLoc = glGetUniformLocation(shaderProgram, "baseColor");
    glUniform3f(baseColorLoc, 0.1f, 0.1f, 0.1f); // �������ɫ����ɫ��





}
void DesignModel::DrawDepthPicDynamical(glm::mat4 lightSpaceMatrix, GLuint shader)
{

    for (size_t i = 0; i < _meshVAOs.size(); i++) {
        // ����ģ�;���
        GLuint modelLoc = glGetUniformLocation(shader, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));

        // �󶨵�ǰ Mesh �� VAO �� VBO
        glBindVertexArray(_meshVAOs[i]);
        glBindBuffer(GL_ARRAY_BUFFER, _meshVBOs[i]);

        // ���¶�̬��������
        glBufferData(GL_ARRAY_BUFFER,
            _modelData.meshes[i].vertices.size() * sizeof(MeshVertex),
            _modelData.meshes[i].vertices.data(),
            GL_DYNAMIC_DRAW);

        // ���Ƶ�ǰ Mesh
        if (justDrawVerteies) {
            glDrawArrays(GL_TRIANGLES, 0, _modelData.meshes[i].vertices.size());
        }
        else {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _meshEBOs[i]);
            glDrawElements(GL_TRIANGLES, _modelData.meshes[i].indices.size(), GL_UNSIGNED_INT, 0);
        }

        // ��� VAO
        glBindVertexArray(0);
    }

}