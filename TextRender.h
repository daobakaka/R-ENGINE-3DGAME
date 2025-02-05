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
//全局变量只能在一个头文件或源文件中引用，头文件中的全局变量，是引用的时候进行编译，多个引用多个编译，所以会报错
namespace Game {

    struct TextCharacter {
        GLuint TextureID;  // 字符纹理ID
        glm::ivec2 Size;   // 字符宽高
        glm::ivec2 Bearing; // 字符的偏移
        GLuint Advance;    // 字符的水平位移
    };
    class TextRender {
    public:
        // 获取单例实例
        static TextRender* GetInstance();

        // 删除拷贝构造函数和赋值操作符，确保单例的唯一性
        TextRender(const TextRender&) = delete;
        void operator=(const TextRender&) = delete;

        // 初始化字体
        void MakeFronts();

        // 渲染文本
        void RenderText(const std::string& text, float x, float y, float scale, glm::vec3 color);

    private:
        // 构造函数和析构函数
        TextRender();
        ~TextRender();
        static  TextRender* instance;
        // 初始化VBO和VAO
        void InitializeTextRender(const char* textRenderVertex, const char* textRenderFragment);
        void CheckShaderCompilation(GLuint shader, const std::string& type);

        // 加载纹理
        GLuint LoadTextureFromBitmap(FT_Bitmap& bitmap);

        // 存储字体字符信息的unordered_map
        std::unordered_map<GLubyte, TextCharacter> TextMapDic;

        // VBO和VAO
        GLuint VBO, VAO,EBO;

        // shader程序
        GLuint shaderProgram;

        void PrintBitmap(const FT_Bitmap& bitmap);
    };
}
#endif // TEXTRENDER_H
#pragma once
