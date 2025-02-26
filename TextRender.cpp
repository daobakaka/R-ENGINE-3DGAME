#include "TextRender.h"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace Game;
TextRender* TextRender::instance = nullptr;
// 单例实例的获取方法
TextRender* TextRender::GetInstance() {

    if (instance == nullptr)
    {
        instance = new TextRender();
    }

    return instance;
}

// 构造函数
TextRender::TextRender() {

}

// 析构函数
TextRender::~TextRender() {
    // 清理OpenGL资源等
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void TextRender::PrintBitmap(const FT_Bitmap& bitmap) {
    // 输出位图的通道信息
    auto pixelMode = bitmap.pixel_mode;
    if (pixelMode == FT_PIXEL_MODE_MONO) {
        std::cout << "Bitmap is in MONO (1 bit per pixel) mode." << std::endl;
    }
    else if (pixelMode == FT_PIXEL_MODE_GRAY) {
        std::cout << "Bitmap is in GRAY (8 bits per pixel) mode." << std::endl;
    }
    else if (pixelMode == FT_PIXEL_MODE_BGRA) {
        std::cout << "Bitmap is in BGRA (32 bits per pixel) mode." << std::endl;
    }
    else {
        std::cout << "Unknown pixel mode." << std::endl;
    }


    // 遍历并打印位图数据
    for (unsigned int i = 0; i < bitmap.rows; ++i) {
        for (unsigned int j = 0; j < bitmap.width; ++j) {
            // 输出每个像素的灰度值
            std::cout << (bitmap.buffer[i * bitmap.width + j] ? '*' : ' ');
        }
        std::cout << std::endl;
    }
}

// 初始化字体
void TextRender::MakeFronts() {

    const char* fontPath = "E:\\C++\\FirstOne\\C++Steam52\\Assets\\Fronts\\2.ttf";
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "Could not initialize FreeType Library" << std::endl;
        return;
    }

    FT_Face face;
    if (FT_New_Face(ft, fontPath, 0, &face)) {
        std::cerr << "Could not load font" << std::endl;
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);  // 设置字体大小
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);//！！！！！设置字节像素对齐格式，因为是字体，所以需要特殊的对准方式！！！！
    // 加载所有字符
    for (GLubyte c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER | FT_LOAD_DEFAULT)) {
            std::cerr << "Could not load character " << c << std::endl;
            continue;
        }

        // 打印当前字符的位图数据
       // PrintBitmap(face->glyph->bitmap);

        GLuint texture = LoadTextureFromBitmap(face->glyph->bitmap);

        TextCharacter character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };

        // 插入字符到unordered_map
        TextMapDic[c] = character;

    }
   // glPixelStorei(GL_UNPACK_ALIGNMENT, 4);//恢复默认状态
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    // 初始化VBO和VAO
    InitializeTextRender(textRenderVertex, textRenderFragment);
}

// 加载纹理
GLuint TextRender::LoadTextureFromBitmap(FT_Bitmap& bitmap) {
    GLuint textureID;
    glGenTextures(1, &textureID);//在GPU中为 textureID 生成唯一的标识符号，这个标识符在系统内部自动处理，像hash标识符一样
    glBindTexture(GL_TEXTURE_2D, textureID);//绑定当前处理的纹理对象，建立CPU到GPU 的纹理传输
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, bitmap.width, bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap.buffer);//以通道采样等不同方式，将数据传输到GPU
    //这里上传完毕之后，就可以在GPU中通过shader对数据进行使用

    //随后对GPU中的数据进行处理，对纹理的处理方式，比如失真，抗锯齿等
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //检查打印位图 加载之后的纹理


    return textureID;
}

// 初始化VBO和VAO
void TextRender::InitializeTextRender(const char* textRenderVertex, const char* textRenderFragment)
{
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &textRenderVertex, nullptr); // 直接传递着色器代码字符串
    glCompileShader(vertexShader);
    CheckShaderCompilation(vertexShader, "VERTEX");

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &textRenderFragment, nullptr); // 直接传递着色器代码字符串
    glCompileShader(fragmentShader);
    CheckShaderCompilation(fragmentShader, "FRAGMENT");

    // 创建着色器程序并链接
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    //这里实际上是检查链接，使用一个方法进行统一
    CheckShaderCompilation(shaderProgram, "PROGRAM");

    // 删除着色器对象
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


}

// 渲染文本
void TextRender::RenderText(const std::string& text, float x, float y, float scale, glm::vec3 color) {


    //--关于UI 的渲染  
#pragma region 状态处理区域，处理完成之后需要归还状态

    glUseProgram(shaderProgram);

    // 创建正交投影矩阵
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(2400), 0.0f, static_cast<float>(1200));

    // 传递正交矩阵到着色器
    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));


    // 设置文本颜色,从外部传入参数
    GLint textColorLocation = glGetUniformLocation(shaderProgram, "textColor");
    glUniform3f(textColorLocation, color.x, color.y, color.z);

    glActiveTexture(GL_TEXTURE0);

    // 绑定 VAO
    glBindVertexArray(VAO);

    // 使用OpenGL绘制每个字符
    for (std::string::const_iterator c = text.begin(); c != text.end(); c++) {
        TextCharacter ch = TextMapDic[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;

        // 设置字符纹理,通过传入的字符来映射相关的纹理坐标
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        // 定义字符的顶点数据，这里实际上就是取人渲染的位置
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        // 更新VBO数据
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // 缓冲区分支更新，处理字体时的优化方式

        // 绘制字符
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // 更新字符位置,水平偏移位置
        x += (ch.Advance >> 6) * scale;  // 以像素为单位的宽度
    }
    glBindVertexArray(0);
#pragma endregion

}


void TextRender::CheckShaderCompilation(GLuint shader, const std::string& type)
{
    GLint success;
    GLchar infoLog[512];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n";
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 512, nullptr, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n";
        }
    }
}