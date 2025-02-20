#ifndef SHADERS_Hero
#define SHADERS_Hero
//这种shader 常量只能被一个文件引用，在其他文件中使用则需要使用声明 extern 等声明参数

/// <summary>
/// 基础着色器
/// </summary>
const char* cubeVertexShaderSource = R"(
    #version 450 core
    layout (location = 0) in vec3 aPos;   // 顶点位置
    out vec3 ourColor;                    // 传递给片段着色器的颜色
    uniform mat4 model;                   // 模型变换矩阵
    uniform mat4 view;                    // 视图变换矩阵
    uniform mat4 projection;              // 投影矩阵
    uniform vec3 color;                   // 颜色 (通过 uniform 传递)
    
    void main() {
        gl_Position = projection * view * model * vec4(aPos, 1.0f);
        ourColor = color;  // 将颜色传递给片段着色器
    }
)";

const char* cubeFragmentShaderSource = R"(
    #version 450 core
    out vec4 FragColor;  // 输出颜色
    in vec3 ourColor;    // 从顶点着色器传递过来的颜色
    
    void main() {
        // 默认浅蓝色，带有透明度
        vec3 defaultColor = vec3(1.0f, 1.0f, 1.0f);  // 淡蓝色
        FragColor = vec4(ourColor+defaultColor, 0.5f);  // 使用传递的颜色，并设置透明度
    }
)";
/// <summary>
/// 参考坐标轴旋转着色器
/// </summary>
const char* axisVertexShaderSource = R"(
    #version 450 core
layout (location = 0) in vec3 aPos;     // 顶点位置
layout (location = 1) in vec3 aColor;   // 顶点颜色
out vec3 ourColor;                      // 输出到片元着色器的颜色

uniform mat4 model;       // 模型变换矩阵
uniform mat4 view;        // 视图矩阵
uniform mat4 projection;  // 投影矩阵

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    ourColor = aColor;
}

)";
const char* axisFragmentShaderSource = R"(
    #version 450 core
    in vec3 ourColor;       // 从顶点着色器传入的颜色
    out vec4 FragColor;     // 输出的片元颜色

    void main() {
        FragColor = vec4(ourColor, 1.0);
    }
)";



/// <summary>
/// 射线着色器
/// </summary>
const char* rayVertexShaderSource = R"(
#version 450 core
layout (location = 0) in vec3 aPos;  // 顶点位置

uniform mat4 model;       // 模型变换矩阵
uniform mat4 view;        // 视图矩阵
uniform mat4 projection;  // 投影矩阵

void main()
{
    // 标准 MVP 变换
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)";
const char* rayFragmentShaderSource = R"(
#version 450 core
out vec4 FragColor;

// 射线颜色由 baseColor 控制，如果未传则默认(0.1,0.1,0.1)
uniform vec3 baseColor;

void main()
{
    // 如果 baseColor == vec3(0.0), 默认用(0.1,0.1,0.1)
    vec3 colorUsed = (baseColor == vec3(0.0)) ? vec3(1, 1, 1) : baseColor;
    
    FragColor = vec4(colorUsed, 1.0);
}
)";
/// <summary>
/// 基础光照着色器
/// </summary>
const char* lightSourceVertexShaderSource = R"(
    #version 450 core
    layout (location = 0) in vec3 aPos;   // 顶点位置
    out vec3 fragPos;                     // 传递给片段着色器的顶点位置
    uniform mat4 model;                   // 模型变换矩阵
    uniform mat4 view;                    // 视图变换矩阵
    uniform mat4 projection;              // 投影矩阵
    
    void main() {
        fragPos = vec3(model * vec4(aPos, 1.0f));  // 计算变换后的顶点位置
        gl_Position = projection * view * model * vec4(aPos, 1.0f);  // 计算最终位置
    }
)";

const char* lightSourceFragmentShaderSource = R"(
    #version 450 core
    out vec4 FragColor;  // 输出颜色
    in vec3 fragPos;    // 从顶点着色器传递过来的顶点位置,这里事实上，就是物体在世界坐标的位置

    // 光源属性
    uniform vec3 lightPos;      // 光源位置,uniform 是设计全局的东西
    uniform vec3 lightColor;    // 光源颜色
    uniform float lightIntensity; // 光源强度
    uniform vec3 viewPos;        // 视点位置
    uniform vec3 baseColor;//基础色
    uniform vec3 emission;//自发光，通常加到颜色输出的末尾

    void main() {
        // 自发光（Emission）
       vec3 emissionDefault=(emission == vec3(0.0f,0.0f,0.0f))?vec3(0.1f,0.1f,0.1f):emission;  // 淡蓝色自发光
       vec3 colorToUse = (baseColor == vec3(0.0f,0.0f,0.0f))?vec3(0.3f, 0.3f, 0.0f):baseColor;      
        // 光照部分
        // 环境光
        vec3 ambient = 0.1 * lightColor;  //处理光照衰减、环境光、漫反射、镜面反射等通常使用乘法，乘法通常情况下模拟的是衰减效果

        // 计算光源位置和物体表面法线之间的角度 
        vec3 lightDir = normalize(lightPos - fragPos);  
        float diff = max(dot(normalize(fragPos), lightDir), 0.0);  // 漫反射
        vec3 diffuse = diff * lightColor * lightIntensity;  // 漫反射光照

        // 镜面反射
        vec3 reflectDir = reflect(-lightDir, normalize(fragPos));  // 反射向量
        vec3 viewDir = normalize(viewPos - fragPos);  // 从物体到视点的向量
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);  // 镜面反射
        vec3 specular = spec * lightColor * lightIntensity*0.1f;

        // 合并所有光照成分
        vec3 result = (ambient + diffuse + specular);

        // 最终颜色 = 自发光 + 光照效果
        FragColor = vec4(result + emissionDefault + colorToUse, 0.5f);  // 使用光照结果和自发光结果
    }
)";
/// <summary>
/// 基础附加颜色光照着色器
/// </summary>
const char* colorlightSourceVertexShaderSource = R"(
  #version 450 core

layout (location = 0) in vec3 aPos;      // 顶点位置
layout (location = 1) in vec3 aNormal;   // 顶点法线

out vec3 fragPos;   // 世界空间坐标
out vec3 Normal;    // 世界空间法线

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // 计算世界空间坐标
    vec4 worldPos = model * vec4(aPos, 1.0);
    fragPos = worldPos.xyz;

    // 如果 model 包含非均匀缩放，请改成:
    // Normal = mat3(transpose(inverse(model))) * aNormal;
    // 这里只做演示, 假设 model 不含非均匀缩放:
    Normal = mat3(model) * aNormal;

    // 最终位置
    gl_Position = projection * view * worldPos;
}

)";

const char* colorlightSourceFragmentShaderSource = R"(
#version 450 core

in vec3 fragPos;  
in vec3 Normal;
out vec4 FragColor;

// uniform: 传入的光照、相机、材质等参数
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightIntensity;
uniform vec3 viewPos;
uniform vec3 baseColor;   // 可以当作模型的固有色
uniform vec3 emission;    // 自发光（可选）

void main()
{
    // 自发光默认值(可选逻辑)
    vec3 emissionDefault = (emission == vec3(0.0)) ? vec3(0.1) : emission;

    // 如果 baseColor 没设置，默认给一个大概颜色
    vec3 colorToUse = (baseColor == vec3(0.0)) ? vec3(0.1, 0.1, 0.1) : baseColor;

    // 1. 环境光
    vec3 ambient = 0.1 * lightColor;

    // 2. 漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * lightIntensity;

    // 3. 镜面反射(Phong)
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = spec * lightColor * lightIntensity * 0.1;

    // 合并
    vec3 lighting = ambient + diffuse + specular;

    // 最终颜色 = (光照 × 物体固有色) + 自发光
    vec3 result = lighting * colorToUse + emissionDefault;
    FragColor = vec4(result, 1.0);
}
)";
/// <summary>
/// 有色光照通用着色器
/// </summary>
const char* colorlightsArrayVertexShaderSource = R"(
#version 450 core
layout(location = 0) in vec3 aPos;       // 顶点位置
layout(location = 1) in vec2 aTexCoord;    // 纹理坐标
layout(location = 2) in vec3 aNormal;      // 顶点法线

// 输出到片段着色器
out vec2 TexCoord;   // 传递纹理坐标
out vec3 FragPos;    // 世界空间中的片段位置
out vec3 Normal;     // 世界空间中的法线
out vec3 ViewPos;   // 相机位置

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec2 textureScale ; // uniform 变量设计赋值非法，只能外部传递纹理缩放因子,改变纹理的缩放方式

void main()
{
    // 计算世界空间坐标
    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos = worldPos.xyz;

    // 传递纹理坐标
    TexCoord = aTexCoord * textureScale;
  
    // 正确变换法线，防止非均匀缩放问题
    Normal = mat3(transpose(inverse(model))) * aNormal;

 // 计算相机位置 (viewPos)，视图矩阵是从世界空间转换到相机空间的
    ViewPos = -mat3(view) * vec3(view[3]);

    // 计算最终屏幕坐标
    gl_Position = projection * view * worldPos;
}
)";


const char* colorlightsArraySourceFragmentShaderSource = R"(
#version 450 core

in vec3 FragPos;  
in vec3 Normal;
in vec2 TexCoord;  // 传入纹理坐标
in vec3 ViewPos;   // 相机位置传入（从顶点着色器） 
out vec4 FragColor;

// 基本参数
uniform float metallic = 0.5f; // 金属度
uniform float roughness = 0.5f; // 粗糙度
uniform float opacity = 1.0f; // 透明度
uniform float IOR = 1.0f; // 折射率
uniform float ao = 1.0f; // 环境光遮蔽率

// 颜色
uniform vec3 baseColor;   // 模型固有色
uniform vec3 emission;    // 自发光（可选）

// 贴图
uniform sampler2D baseTexture;  // 基础纹理采样器
uniform sampler2D normalTexture; // 法线纹理采样器
uniform sampler2D heightTexture; // 高度贴图
uniform sampler2D roughnessTexture; // 糙度贴图
uniform samplerCube reflectionTexture; // 高光反射贴图
uniform sampler2D aoTexture;  // 环境光遮蔽贴图

//--平行光贴图
uniform sampler2D autoParallelShadowMap;//平行光光照贴图，用于生成主阴影
// 接收光源的阴影矩阵
uniform mat4 lightSpaceMatrix;  // 从光源视角计算的矩阵

// 点光源参数
const int MAX_POINT_LIGHTS =4;
uniform int numPointLights=0;
uniform vec3 lightPos[MAX_POINT_LIGHTS];
uniform vec3 lightColor[MAX_POINT_LIGHTS];
uniform float lightIntensity[MAX_POINT_LIGHTS];
uniform float lightAttenuation=1; // 距离衰减系数
uniform float lightDistanceSquared[MAX_POINT_LIGHTS]; // 传入的距离平方

// 平行光（方向光）参数
uniform vec3 parallelLightDirection;
uniform vec3 parallelLightColor;
uniform float parallelLightIntensity;

// 手电筒（聚光灯）参数
const int MAX_FLASHLIGHTS =4;//数组计算不能是动态值，只能是空值，参照上下文索引符号
uniform int numFlashLights=0;//可以采用传入数据变量来进行更改
uniform vec3 flashLightPos[MAX_FLASHLIGHTS];
uniform vec3 flashLightDirection[MAX_FLASHLIGHTS];
uniform vec3 flashLightColor[MAX_FLASHLIGHTS];
uniform float flashLightIntensity[MAX_FLASHLIGHTS];
uniform float flashLightCutoff[MAX_FLASHLIGHTS]; // cutoff 值为余弦值
uniform float flashlightAttenuation=1; // 手电筒光的衰减系数
uniform float flashLightDistanceSquared[MAX_FLASHLIGHTS]; // 传入的聚光灯距离平方

void main() {
    vec3 emissionDefault = (emission == vec3(0.0)) ? vec3(0.1) : emission;

    vec3 colorToUse = (baseColor == vec3(0.0)) ? vec3(0.1, 0.1, 0.1) : baseColor;

    vec3 norm = normalize(Normal);

    vec3 ambientTotal = vec3(0.0);
    vec3 diffuseTotal = vec3(0.0);
    vec3 specularTotal = vec3(0.0);
    
    // --- 点光源部分 ---
    for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
        if (lightIntensity[i] > 0.0001) {
            // 使用传入的距离平方进行衰减
            float attenuation = 1.0 / (1.0 + lightAttenuation * lightDistanceSquared[i]);

            // 环境光：每个点光源均贡献 10%
            ambientTotal += 0.1 * lightColor[i];
            
            // 漫反射
            vec3 lightDir = normalize(lightPos[i] - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            diffuseTotal += diff * lightColor[i] * lightIntensity[i] * attenuation;
            
            // 镜面反射（Phong 模型）
            vec3 viewDir = normalize(ViewPos - FragPos);
            vec3 reflectDir = reflect(-lightDir, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
            specularTotal += spec * lightColor[i] * lightIntensity[i] * 0.1 * attenuation;
        }
    }
    
    // --- 平行光（方向光）部分 ---
    if (parallelLightIntensity > 0.0001) {
        ambientTotal += 0.2f * parallelLightColor;//平行光单独贡献20%
        
        vec3 lightDir = normalize(-parallelLightDirection);
        float diff = max(dot(norm, lightDir), 0.0);
        diffuseTotal += diff * parallelLightColor * parallelLightIntensity;
        
        vec3 viewDir = normalize(ViewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
        specularTotal += spec * parallelLightColor * parallelLightIntensity * 0.1;

        // 计算阴影
        vec4 fragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
        vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
        projCoords = projCoords * 0.5 + 0.5;  // 将坐标从[-1, 1]映射到[0, 1]

        // 从阴影贴图中获取深度值
        float closestDepth = texture(autoParallelShadowMap, projCoords.xy).r;
        float currentDepth = projCoords.z;

        // 阴影判断：比较当前片段的深度与从阴影贴图中取出的深度
        float bias = 0.005;  // 阴影偏差，用来避免自阴影问题
        float shadow = currentDepth > (closestDepth + bias) ? 1.0 : 0.0;

        // 如果在阴影中，漫反射和镜面反射会变暗
        diffuseTotal *= (1.0 - shadow);
        specularTotal *= (1.0 - shadow);

        // 计算阴影平滑（PCF）
        //float shadowPCF = 0.0f;
        //for(int x = -1; x <= 1; ++x) {
        //    for(int y = -1; y <= 1; ++y) {
        //        float pcf = texture(autoParallelShadowMap, projCoords.xy + vec2(x, y) * 0.005).r;
        //        shadowPCF += pcf;
        //    }
        //}
        //shadowPCF /= 9.0f;  // 平均结果
        //diffuseTotal *= (1.0 - shadowPCF);
        //specularTotal *= (1.0 - shadowPCF);

    }
    
    // --- 手电筒（聚光灯）部分 ---
    for (int j = 0; j < MAX_FLASHLIGHTS; j++) {
        if (flashLightIntensity[j] > 0.0001) {
            vec3 lightDir = normalize(flashLightPos[j] - FragPos);
            
            // 使用传入的聚光灯距离平方进行衰减
            float attenuation = 1.0 / (1.0 + flashlightAttenuation * flashLightDistanceSquared[j]);
            
            // 计算聚光效果
            float theta = dot(lightDir, normalize(flashLightDirection[j]));
            if (theta > flashLightCutoff[j]) {
                ambientTotal += 0.1 * flashLightColor[j];
                
                float diff = max(dot(norm, lightDir), 0.0);
                diffuseTotal += diff * flashLightColor[j] * flashLightIntensity[j] * attenuation;
                
                vec3 viewDir = normalize(ViewPos - FragPos);
                vec3 reflectDir = reflect(-lightDir, norm);
                float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
                specularTotal += spec * flashLightColor[j] * flashLightIntensity[j] * 0.1 * attenuation;
            }
        }
    }
    
    vec3 lighting = ambientTotal + diffuseTotal + specularTotal;
    
    // 最终颜色 = (光照效果 × 物体固有色) + 自发光
    vec3 result = lighting * colorToUse + emissionDefault;
    
    // 采样纹理颜色，并与光照结果相乘
    vec4 texColor = texture(baseTexture, TexCoord);
    FragColor = vec4(result, 1.0) * texColor;
}

)";

/// <summary>
/// 无光照通用着色器
/// </summary>

const char* noneLightVertexShaderSource = R"(
#version 450 core
layout(location = 0) in vec3 aPos;   // 顶点位置
layout(location = 1) in vec2 aTexCoord;   // 纹理坐标 
layout(location = 2) in vec3 aNormal;   // 法线
//这里对应初始化时，设置的顶点属性
out vec2 TexCoord;  // 传递给片段着色器的纹理坐标
out vec3 FragPos;   // 传递给片段着色器的片段位置

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    TexCoord = aTexCoord;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)";

const char* noneLightFragmentShaderSource = R"(
#version 450 core

out vec4 FragColor;

in vec2 TexCoord;   // 从顶点着色器接收的纹理坐标
in vec3 FragPos;    // 从顶点着色器接收的片段位置

uniform vec3 baseColor;      // 可以当作模型的固有色
uniform sampler2D texture1;  // 纹理

void main()
{
    // 如果 baseColor == (0,0,0)，则默认使用(0.1, 0.1, 0.1)
    // 注意：对向量进行 == 比较在 GLSL 4.5 中是允许的，但如果要判断是否近似为零，可以改用 length(baseColor) < 1e-6 等。
    vec3 colorToUse = (baseColor == vec3(0.0)) ? vec3(0.1, 0.1, 0.1) : baseColor;

    // 无需把 baseColor 加到纹理颜色，可改为乘法叠加
    FragColor = (texture(texture1, TexCoord) + vec4(colorToUse, 1.0)) ;
} 
)";
/// <summary>
/// 无光照光源渲染着色器
/// </summary>
const char* noneLightLightVertexShaderSource = R"(
#version 450 core
layout(location = 0) in vec3 aPos;   // 顶点位置
layout(location = 1) in vec2 aTexCoord;   // 纹理坐标 
layout(location = 2) in vec3 aNormal;   // 法线
//这里对应初始化时，设置的顶点属性
out vec2 TexCoord;  // 传递给片段着色器的纹理坐标
out vec3 FragPos;   // 传递给片段着色器的片段位置

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    TexCoord = aTexCoord;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)";

const char* noneLightLightFragmentShaderSource = R"(
#version 450 core

out vec4 FragColor;

in vec2 TexCoord;   // 从顶点着色器接收的纹理坐标
in vec3 FragPos;    // 从顶点着色器接收的片段位置

uniform vec3 baseColor;      // 可以当作模型的固有色
uniform sampler2D texture1;  // 纹理
uniform float lightIntensity; // 光强度可视化
uniform float lightDirection;//光照方向，暂时不用
void main()
{
    // 如果 baseColor == (0,0,0)，则默认使用(0.1, 0.1, 0.1)
    // 注意：对向量进行 == 比较在 GLSL 4.5 中是允许的，但如果要判断是否近似为零，可以改用 length(baseColor) < 1e-6 等。
    vec3 colorToUse = (baseColor == vec3(0.0)) ? vec3(0.1, 0.1, 0.1) : baseColor;

    // 如果 lightIntensity==0，则默认值为0.1
    float tempIntensity = (lightIntensity == 0.0) ? 0.1f : lightIntensity;

    // 纹理取样 + baseColor，再乘以光强度
    // 无需把 baseColor 加到纹理颜色，可改为乘法叠加
    FragColor = (texture(texture1, TexCoord) *vec4(colorToUse, 1.0))* 0.5f* tempIntensity;
} 
)";


/// <summary>
/// 字体渲染着色器
/// </summary>
const char* textRenderVertex = R"(
#version 450 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;
out vec2 TexCoordText;
uniform mat4 projection;  // 正交矩阵
void main()
{
    gl_Position = projection * vec4(aPos, 1.0, 1.0);  // 应用正交投影，设置Z值为1 最靠前，opengl 中Z值范围为[-1,1],由远到近
    TexCoordText = aTexCoord;
}
)";

const char* textRenderFragment = R"(

#version 450 core
in vec2 TexCoordText;  // 从顶点着色器传来的纹理坐标
out vec4 FragColor; // 最终颜色输出
uniform sampler2D text; // 字体纹理
uniform vec3 textColor; // 文本颜色
void main()
{
    vec4 sampled = texture(text, TexCoordText);  // 获取纹理颜色
    if (sampled.r == 0)  // 如红色等于0，则不渲染
        discard;
   /* else
        sampled.r=1;*/
    FragColor = vec4(textColor,1)*sampled.r;
       // FragColor = vec4(textColor * sampled.r, 1);  // 乘以红色通道的灰度值
       // FragColor = vec4(textColor,1.0f)*sampled;
}
)";
/// <summary>
/// 天空盒着色器
/// </summary>
const char* skyboxVertexShaderSource = R"(
    #version 450 core
    layout (location = 0) in vec3 aPos;  // 顶点位置

    out vec3 TexCoords;  // 传递给片段着色器的纹理坐标

    uniform mat4 projection;  // 投影矩阵
    uniform mat4 view;  // 视图矩阵

    void main() {
        gl_Position = projection * view * vec4(aPos, 1.0f);  // 计算最终的屏幕坐标
        TexCoords = aPos;  // 将顶点坐标传递给片段着色器
    }
)";

const char* skyboxFragmentShaderSource = R"(
    #version 450 core
    out vec4 FragColor;  // 输出颜色
    in vec3 TexCoords;   // 从顶点着色器传递过来的纹理坐标

    uniform samplerCube skybox;  // 立方体纹理

    void main() {
        FragColor = texture(skybox, TexCoords);  // 从立方体纹理中取样并输出颜色
    }
)";


#endif