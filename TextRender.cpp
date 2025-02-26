#include "TextRender.h"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace Game;
TextRender* TextRender::instance = nullptr;
// ����ʵ���Ļ�ȡ����
TextRender* TextRender::GetInstance() {

    if (instance == nullptr)
    {
        instance = new TextRender();
    }

    return instance;
}

// ���캯��
TextRender::TextRender() {

}

// ��������
TextRender::~TextRender() {
    // ����OpenGL��Դ��
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void TextRender::PrintBitmap(const FT_Bitmap& bitmap) {
    // ���λͼ��ͨ����Ϣ
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


    // ��������ӡλͼ����
    for (unsigned int i = 0; i < bitmap.rows; ++i) {
        for (unsigned int j = 0; j < bitmap.width; ++j) {
            // ���ÿ�����صĻҶ�ֵ
            std::cout << (bitmap.buffer[i * bitmap.width + j] ? '*' : ' ');
        }
        std::cout << std::endl;
    }
}

// ��ʼ������
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

    FT_Set_Pixel_Sizes(face, 0, 48);  // ���������С
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);//���������������ֽ����ض����ʽ����Ϊ�����壬������Ҫ����Ķ�׼��ʽ��������
    // ���������ַ�
    for (GLubyte c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER | FT_LOAD_DEFAULT)) {
            std::cerr << "Could not load character " << c << std::endl;
            continue;
        }

        // ��ӡ��ǰ�ַ���λͼ����
       // PrintBitmap(face->glyph->bitmap);

        GLuint texture = LoadTextureFromBitmap(face->glyph->bitmap);

        TextCharacter character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };

        // �����ַ���unordered_map
        TextMapDic[c] = character;

    }
   // glPixelStorei(GL_UNPACK_ALIGNMENT, 4);//�ָ�Ĭ��״̬
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    // ��ʼ��VBO��VAO
    InitializeTextRender(textRenderVertex, textRenderFragment);
}

// ��������
GLuint TextRender::LoadTextureFromBitmap(FT_Bitmap& bitmap) {
    GLuint textureID;
    glGenTextures(1, &textureID);//��GPU��Ϊ textureID ����Ψһ�ı�ʶ���ţ������ʶ����ϵͳ�ڲ��Զ�������hash��ʶ��һ��
    glBindTexture(GL_TEXTURE_2D, textureID);//�󶨵�ǰ�����������󣬽���CPU��GPU ��������
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, bitmap.width, bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap.buffer);//��ͨ�������Ȳ�ͬ��ʽ�������ݴ��䵽GPU
    //�����ϴ����֮�󣬾Ϳ�����GPU��ͨ��shader�����ݽ���ʹ��

    //����GPU�е����ݽ��д���������Ĵ���ʽ������ʧ�棬����ݵ�
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //����ӡλͼ ����֮�������


    return textureID;
}

// ��ʼ��VBO��VAO
void TextRender::InitializeTextRender(const char* textRenderVertex, const char* textRenderFragment)
{
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &textRenderVertex, nullptr); // ֱ�Ӵ�����ɫ�������ַ���
    glCompileShader(vertexShader);
    CheckShaderCompilation(vertexShader, "VERTEX");

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &textRenderFragment, nullptr); // ֱ�Ӵ�����ɫ�������ַ���
    glCompileShader(fragmentShader);
    CheckShaderCompilation(fragmentShader, "FRAGMENT");

    // ������ɫ����������
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    //����ʵ�����Ǽ�����ӣ�ʹ��һ����������ͳһ
    CheckShaderCompilation(shaderProgram, "PROGRAM");

    // ɾ����ɫ������
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

// ��Ⱦ�ı�
void TextRender::RenderText(const std::string& text, float x, float y, float scale, glm::vec3 color) {


    //--����UI ����Ⱦ  
#pragma region ״̬�������򣬴������֮����Ҫ�黹״̬

    glUseProgram(shaderProgram);

    // ��������ͶӰ����
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(2400), 0.0f, static_cast<float>(1200));

    // ��������������ɫ��
    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));


    // �����ı���ɫ,���ⲿ�������
    GLint textColorLocation = glGetUniformLocation(shaderProgram, "textColor");
    glUniform3f(textColorLocation, color.x, color.y, color.z);

    glActiveTexture(GL_TEXTURE0);

    // �� VAO
    glBindVertexArray(VAO);

    // ʹ��OpenGL����ÿ���ַ�
    for (std::string::const_iterator c = text.begin(); c != text.end(); c++) {
        TextCharacter ch = TextMapDic[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;

        // �����ַ�����,ͨ��������ַ���ӳ����ص���������
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        // �����ַ��Ķ������ݣ�����ʵ���Ͼ���ȡ����Ⱦ��λ��
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        // ����VBO����
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // ��������֧���£���������ʱ���Ż���ʽ

        // �����ַ�
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // �����ַ�λ��,ˮƽƫ��λ��
        x += (ch.Advance >> 6) * scale;  // ������Ϊ��λ�Ŀ��
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