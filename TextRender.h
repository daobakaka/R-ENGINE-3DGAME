#ifndef TEXTRENDER_H
#define TEXTRENDER_H
#include <iostream>
#include <string>
#include <unordered_map>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H


extern const char* textRenderVertex;
extern const char* textRenderFragment;
namespace Game {

    struct TextCharacter {
        GLuint TextureID;  // �ַ�����ID
        glm::ivec2 Size;   // �ַ����
        glm::ivec2 Bearing; // �ַ���ƫ��
        GLuint Advance;    // �ַ���ˮƽλ��
    };
    class TextRender {
    public:
        // ��ȡ����ʵ��
        static TextRender* GetInstance();

        // ɾ���������캯���͸�ֵ��������ȷ��������Ψһ��
        TextRender(const TextRender&) = delete;
        void operator=(const TextRender&) = delete;

        // ��ʼ������
        void MakeFronts();

        // ��Ⱦ�ı�
        void RenderText(const std::string& text, float x, float y, float scale, glm::vec3 color);

    private:
        // ���캯������������
        TextRender();
        ~TextRender();
        static  TextRender* instance;
        // ��ʼ��VBO��VAO
        void InitializeTextRender(const char* textRenderVertex, const char* textRenderFragment);
        void CheckShaderCompilation(GLuint shader, const std::string& type);

        // ��������
        GLuint LoadTextureFromBitmap(FT_Bitmap& bitmap);

        // �洢�����ַ���Ϣ��unordered_map
        std::unordered_map<GLubyte, TextCharacter> _TextMapDic;

        // VBO��VAO
        GLuint _VBO, _VAO,_EBO;

        // shader����
        GLuint _shaderProgram;

        void PrintBitmap(const FT_Bitmap& bitmap);
    };
}
#endif // TEXTRENDER_H
#pragma once
