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
uniform float metallic; // 金属度
uniform float roughness; // 粗糙度
uniform float opacity; // 透明度
uniform float IOR; // 折射率
uniform float ao; // 环境光遮蔽率


//特效参数
uniform float dissolveThreshold; // 溶解阈值，控制溶解的进度

// 颜色
uniform vec3 baseColor;   // 模型固有色
uniform vec3 emission;    // 自发光（可选）

// 贴图
uniform sampler2D baseTexture;  // 基础纹理采样器
uniform sampler2D normalTexture; // 法线纹理采样器
uniform sampler2D heightTexture; // 高度贴图
uniform sampler2D roughnessTexture; // 糙度贴图
uniform samplerCube specularTexture; // 高光反射贴图（原名 reflectionTexture）
uniform sampler2D aoTexture;  // 环境光遮蔽贴图

//--平行光贴图
uniform sampler2D autoParallelShadowMap;//平行光光照贴图，用于生成主阴影
// 接收光源的阴影矩阵
uniform mat4 lightSpaceMatrix;  // 从光源视角计算的矩阵

// 点光源参数
const int MAX_POINT_LIGHTS = 4;
uniform int numPointLights;
uniform vec3 lightPos[MAX_POINT_LIGHTS];
uniform vec3 lightColor[MAX_POINT_LIGHTS];
uniform float lightIntensity[MAX_POINT_LIGHTS];
float lightAttenuation = 0.01F; // 距离衰减系数
uniform float lightDistanceSquared[MAX_POINT_LIGHTS]; // 传入的距离平方

// 平行光（方向光）参数
uniform vec3 parallelLightDirection;
uniform vec3 parallelLightColor;
uniform float parallelLightIntensity;

// 手电筒（聚光灯）参数
const int MAX_FLASHLIGHTS = 4;
uniform int numFlashLights;
uniform vec3 flashLightPos[MAX_FLASHLIGHTS];
uniform vec3 flashLightDirection[MAX_FLASHLIGHTS];
uniform vec3 flashLightColor[MAX_FLASHLIGHTS];
uniform float flashLightIntensity[MAX_FLASHLIGHTS];
uniform float flashLightCutoff[MAX_FLASHLIGHTS]; // cutoff 值为余弦值
float flashlightAttenuation = 0.01F; // 手电筒光的衰减系数
uniform float flashLightDistanceSquared[MAX_FLASHLIGHTS]; // 传入的聚光灯距离平方

// PBR 光照计算函数
vec3 calculatePBR(vec3 lightDir, vec3 viewDir, vec3 norm, vec3 lightColor, float lightIntensity, float metallicValue, float roughnessValue) {
    vec3 halfDir = normalize(lightDir + viewDir);

    // 基础反射率（F0）：金属度决定
    vec3 F0 = mix(vec3(0.04), baseColor, metallicValue);

    // Fresnel 项（Schlick 近似）
    float HdotV = max(dot(halfDir, viewDir), 0.0);
    vec3 F = F0 + (1.0 - F0) * pow(1.0 - HdotV, 5.0);

    // 漫反射部分
    float NdotL = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * NdotL * (1.0 - metallicValue);

    // 镜面反射部分（Cook-Torrance BRDF）
    float NdotV = max(dot(norm, viewDir), 0.0);
    float NdotH = max(dot(norm, halfDir), 0.0);

    float alpha = roughnessValue * roughnessValue;
    float alphaSq = alpha * alpha;

    // 几何函数（Smith-Schlick GGX）
    float G1 = NdotV / (NdotV * (1.0 - alphaSq) + alphaSq);
    float G2 = NdotL / (NdotL * (1.0 - alphaSq) + alphaSq);
    float G = G1 * G2;

    // 法线分布函数（Trowbridge-Reitz GGX）
    float denom = (NdotH * NdotH * (alphaSq - 1.0) + 1.0);
    float D = alphaSq / (3.14159 * denom * denom);

    // 镜面反射
    vec3 specular = (D * G * F) / (4.0 * NdotV * NdotL + 0.0001);

    return (diffuse + specular) * lightIntensity;
}

// 计算光线损失（折射率仅在透明度低于 0.6 时生效）
float calculateLightLoss(float opacityValue, float IORValue) {
    if (opacityValue < 0.6) {
        // 透明度低于 0.6 时，根据折射率计算光线损失
        return 1.0 - pow((IORValue - 1.0) / (IORValue + 1.0), 2.0);
    } else {
        // 否则，光线损失为 0
        return 0.0;
    }
}

void main() {


// 计算视图方向，必须在使用 viewDir 前计算
    vec3 viewDir = normalize(ViewPos - FragPos);
    // 采样法线纹理并调整法线
    vec3 normalMap = texture(normalTexture, TexCoord).rgb;
    normalMap = normalMap * 2.0 - 1.0;  // 映射到 [-1, 1]
    vec3 norm = normalize(Normal + normalMap);  // 结合原始法线

    // 采样高度纹理并调整纹理坐标
    float height = texture(heightTexture, TexCoord).r;
    vec2 parallaxOffset = viewDir.xy * (height * 0.01f);  // 简单的视差偏移,这是针对高度图来进行计算的，如果没有则设置为0
    vec2 adjustedTexCoord = TexCoord + parallaxOffset;//这里就调整了纹理坐标的偏移量

    // 采样粗糙度纹理并调整粗糙度
    float roughnessValue = texture(roughnessTexture, adjustedTexCoord).r * roughness;

    // 采样环境光遮蔽纹理并调整 AO
    float aoValue = texture(aoTexture, adjustedTexCoord).r * ao;

    // 采样高光反射纹理
    vec3 reflectionDir = reflect(-viewDir, norm);
    vec3 specularColor = texture(specularTexture, reflectionDir).rgb;

    // 采样基础纹理并调整基础颜色
    vec4 texColor = texture(baseTexture, adjustedTexCoord);
    vec3 finalBaseColor = baseColor * texColor.rgb;

    // 初始化光照贡献
    vec3 ambientTotal = vec3(0.0);
    vec3 diffuseTotal = vec3(0.0);
    vec3 specularTotal = vec3(0.0);


    // --- 点光源部分 ---
    for (int i = 0; i < numPointLights; i++) {
        if (lightIntensity[i] > 0.0001) {
            float attenuation = 1.0 / (1.0 + lightAttenuation * lightDistanceSquared[i]);
            ambientTotal += 0.1 * lightColor[i];
            vec3 lightDir = normalize(lightPos[i] - FragPos);
            vec3 pbrLighting = calculatePBR(lightDir, viewDir, norm, lightColor[i], lightIntensity[i], metallic, roughnessValue);
            diffuseTotal += pbrLighting * attenuation;
        }
    }

    // --- 手电筒（聚光灯）部分 ---
    for (int j = 0; j < numFlashLights; j++) {
        if (flashLightIntensity[j] > 0.0001) {
            vec3 lightDir = normalize(flashLightPos[j] - FragPos);
            float attenuation = 1.0 / (1.0 + flashlightAttenuation * flashLightDistanceSquared[j]);
            float theta = dot(lightDir, normalize(flashLightDirection[j]));
            if (theta > flashLightCutoff[j]) {
                ambientTotal += 0.1 * flashLightColor[j];
                vec3 pbrLighting = calculatePBR(lightDir, viewDir, norm, flashLightColor[j], flashLightIntensity[j], metallic, roughnessValue);
                diffuseTotal += pbrLighting * attenuation;
            }
        }
    }
// --- 平行光（方向光）部分 ---
if (parallelLightIntensity > 0.0001) {
    ambientTotal += 0.2f * parallelLightColor;
    vec3 lightDir = normalize(-parallelLightDirection);
    vec3 pbrLighting = calculatePBR(lightDir, viewDir, norm, parallelLightColor, parallelLightIntensity, metallic, roughnessValue);
    diffuseTotal += pbrLighting;

    // 阴影计算
    vec4 fragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    // 定义边缘阈值
    const float edgeThreshold = 0.01; // 边缘阈值，可以根据需要调整

    // 判断是否临近阴影贴图边缘
    bool isNearEdge = (projCoords.x < edgeThreshold || projCoords.x > 1.0 - edgeThreshold ||
                       projCoords.y < edgeThreshold || projCoords.y > 1.0 - edgeThreshold);

    // 从阴影贴图中获取深度值
    float closestDepth = texture(autoParallelShadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    // 动态调整阴影偏差
    float bias = max(0.005 * (1.0 - dot(norm, lightDir)), 0.001);

    // 阴影判断：比较当前片段的深度与从阴影贴图中取出的深度
    float shadow = currentDepth > (closestDepth + bias) ? 1.0 : 0.0;

    // 使用 PCF 平滑阴影边缘
    float shadowPCF = 0.0f;
    vec2 texelSize = 1.0 / textureSize(autoParallelShadowMap, 0);
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcf = texture(autoParallelShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadowPCF += (currentDepth - bias > pcf) ? 1.0 : 0.0;
        }
    }
    shadowPCF /= 9.0f;  // 平均结果

    // 如果在阴影中，漫反射和镜面反射会变暗
    if (isNearEdge) {
        // 临近边缘时，禁用阴影计算
        diffuseTotal *= 1.0;
    } else {
        // 正常应用阴影
        diffuseTotal *= (1.0 - shadowPCF);
    }
}

    // 最终光照
    vec3 lighting = ambientTotal * aoValue + diffuseTotal + specularColor * metallic;

    // 计算光线损失
    float lightLoss = calculateLightLoss(opacity, IOR);

    // 最终颜色 = (光照效果 × 物体固有色) + 自发光
    vec3 result = lighting * finalBaseColor * (1.0 - lightLoss) + emission;
    //溶解度阈值丢弃像素
    if(texColor.r<dissolveThreshold)
     { discard;  }

    // 输出颜色
    FragColor = vec4(result, opacity) * texColor;
}
)";
/// <summary>
/// 无光照基础着色器
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
uniform sampler2D baseTexture;  // 纹理


//--平行光贴图
uniform sampler2D autoParallelShadowMap;//平行光光照贴图，用于生成主阴影
// 接收光源的阴影矩阵
uniform mat4 lightSpaceMatrix;  // 从光源视角计算的矩阵

void main()
{
    // 如果 baseColor == (0,0,0)，则默认使用(0.1, 0.1, 0.1)
    // 注意：对向量进行 == 比较在 GLSL 4.5 中是允许的，但如果要判断是否近似为零，可以改用 length(baseColor) < 1e-6 等。
    vec3 colorToUse = (baseColor == vec3(0.0)) ? vec3(0.1, 0.1, 0.1) : baseColor;

    // 无需把 baseColor 加到纹理颜色，可改为乘法叠加
    FragColor = (texture(baseTexture, TexCoord) + vec4(colorToUse, 1.0)) ;
  // FragColor = vec4(1,1,1,1);

} 
)";
/// <summary>
/// 无光照通用渲染着色器
/// </summary>
const char* CustomNoLightShadowShaderVertexSource = R"(
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


const char* CustomNoLightShadowShaderFragmentSource = R"(
#version 450 core

in vec3 FragPos;  
in vec3 Normal;
in vec2 TexCoord;  
in vec3 ViewPos;  
out vec4 FragColor;

// 基本参数（保留接口，后续可扩展）
uniform float metallic;      // 金属度
uniform float roughness;     // 粗糙度
uniform float opacity;       // 透明度
uniform float IOR;           // 折射率
uniform float ao;            // 环境光遮蔽率

//特效参数
uniform float dissolveThreshold; // 溶解阈值，控制溶解的进度
// 颜色
uniform vec3 baseColor;      // 模型固有色
uniform vec3 emission;       // 自发光

// 贴图采样器
uniform sampler2D baseTexture;      // 基础纹理
uniform sampler2D normalTexture;     // 法线纹理
uniform sampler2D heightTexture;     // 高度贴图（用于视差映射）
uniform sampler2D roughnessTexture;   // 粗糙度贴图
uniform samplerCube specularTexture;  // 反光贴图（cube map）
uniform sampler2D aoTexture;          // 环境光遮蔽贴图

// 阴影相关（保留方向光阴影计算）
uniform sampler2D autoParallelShadowMap; // 平行光阴影贴图
uniform mat4 lightSpaceMatrix;           // 从光源视角计算的矩阵
uniform vec3 parallelLightDirection;     // 平行光方向，用于计算阴影偏置

// 以下点光源、聚光等均取消，不参与计算

void main()
{
      vec3 viewDir = normalize(ViewPos - FragPos);
    // 采样法线纹理并调整法线
    vec3 normalMap = texture(normalTexture, TexCoord).rgb;
    normalMap = normalMap * 2.0 - 1.0;  // 映射到 [-1, 1]
    vec3 norm = normalize(Normal + normalMap);  // 结合原始法线

    // 采样高度纹理并调整纹理坐标
    float height = texture(heightTexture, TexCoord).r;
    vec2 parallaxOffset = viewDir.xy * (height * 0.01f);  // 简单的视差偏移,这是针对高度图来进行计算的，如果没有则设置为0
    vec2 adjustedTexCoord = TexCoord + parallaxOffset;//这里就调整了纹理坐标的偏移量

    // 采样粗糙度纹理并调整粗糙度
    float roughnessValue = texture(roughnessTexture, adjustedTexCoord).r * roughness;

    // 采样环境光遮蔽纹理并调整 AO
    float aoValue = texture(aoTexture, adjustedTexCoord).r * ao;

    // 采样高光反射纹理
    vec3 reflectionDir = reflect(-viewDir, norm);
    vec3 specularColor = texture(specularTexture, reflectionDir).rgb;

    // 采样基础纹理并调整基础颜色
    vec4 texColor = texture(baseTexture, adjustedTexCoord);
    vec3 finalBaseColor = baseColor * texColor.rgb;

 // ---- 阴影计算（方向光阴影） ----
vec4 fragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
projCoords = projCoords * 0.5 + 0.5; // 将坐标映射到 [0, 1]

// 定义边缘阈值
const float edgeThreshold = 0.01; // 边缘阈值，可以根据需要调整

// 判断是否临近阴影贴图边缘
bool isNearEdge = (projCoords.x < edgeThreshold || projCoords.x > 1.0 - edgeThreshold ||
                   projCoords.y < edgeThreshold || projCoords.y > 1.0 - edgeThreshold);

// 使用平行光方向计算阴影偏置
vec3 lightDir = normalize(-parallelLightDirection);
float bias = max(0.005 * (1.0 - dot(norm, lightDir)), 0.005); // 保留 bias 值，最小 0.005
float currentDepth = projCoords.z;

// 阴影判断：比较当前片段的深度与从阴影贴图中取出的深度
float shadow = 0.0;
if (!isNearEdge) {
    // 使用 PCF 软阴影采样（3x3 邻域）
    vec2 texelSize = 1.0 / textureSize(autoParallelShadowMap, 0);
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(autoParallelShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += (currentDepth - bias > pcfDepth) ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0; // 平均结果
}

// 将阴影因子应用到基础颜色
finalBaseColor *= (1.0 - shadow);
// ---- 阴影计算结束 ----

    // 最终输出颜色：基础调制结果加上自发光
     vec3 result = finalBaseColor*aoValue*roughnessValue  +specularColor*metallic+ emission;
       //溶解度阈值丢弃像素
    if(texColor.r<dissolveThreshold)
     { discard;  }
     FragColor = vec4(result, opacity) * texColor;


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
uniform sampler2D baseTexture;  // 纹理
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
    FragColor = (texture(baseTexture, TexCoord) *vec4(colorToUse, 1.0))* 0.5f* tempIntensity;
} 
)";


/// <summary>
/// 字体渲染着色器
/// </summary>
const char* textRenderVertex = R"(
#version 450 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;
uniform mat4 projection;
void main()
{
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}
)";

const char* textRenderFragment = R"(
#version 450 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    color = vec4(textColor, 1.0) * sampled;
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
/// <summary>
/// 深度图测试着色器
/// </summary>
const char* depthTestShaderVertexShaderSource = R"(
#version 450 core
layout(location = 0) in vec3 aPos;   // 顶点位置

uniform mat4 model;                  // 物体模型矩阵
uniform mat4 lightSpaceMatrix;       // 从光源视角计算的矩阵

void main() {
    // 将顶点从世界空间转换到光源视角空间
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);  // model 矩阵用于世界空间转换
}
)";
/// <summary>
/// 可以不渲染，因为深度值将会储存在深度缓冲区中，OPENGL 储存深度值是非线性的，这里需要进行线性转换才能看见
/// </summary>
const char* depthTestShaderFragmentShaderSource = R"(
#version 450 core

out vec4 FragColor;

// 直接在着色器中定义近裁剪面和远裁剪面
const float near = 0.1f;  // 近裁剪面
const float far = 100.0f;   // 远裁剪面

// 线性化深度函数
float LinearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0;  // 将深度值从 [0, 1] 转换到 NDC (-1, 1) 区间
    return (2.0 * near * far) / (far + near - z * (far - near));  // 计算线性深度
}

void main() {
    // 获取当前片段的深度值
    float depth = gl_FragCoord.z;

    // 线性化深度值
    float linearDepth = LinearizeDepth(depth) / far; // 除以 far，得到深度值在 [0, 1] 范围内

    // 渲染深度，显示线性化的深度值（灰度）
    FragColor = vec4(vec3(linearDepth), 1.0);  // 使用线性深度作为颜色值



}
)";

/// <summary>
/// 深度图平行光阴影使用着色器
/// </summary>
const char* depthShaderVertexShaderSource = R"(
#version 450 core
layout(location = 0) in vec3 aPos;   // 顶点位置

uniform mat4 model;                  // 物体模型矩阵
uniform mat4 lightSpaceMatrix;       // 从光源视角计算的矩阵

void main() {
    // 将顶点从世界空间转换到光源视角空间
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);  // model 矩阵用于世界空间转换
}
)";
/// <summary>
/// 可以不渲染
/// </summary>
const char* depthShaderFragmentShaderSource = R"(
#version 450 core

out vec4 FragColor;

void main() {

}
)";
/// <summary>
/// 深度图视口后处理使用着色器
/// </summary>
const char* depthViewPortShaderVertexShaderSource = R"(
#version 450 core
layout(location = 0) in vec3 aPos;   // 顶点位置

uniform mat4 model;                  // 物体模型矩阵
uniform mat4 view;       // 视口
uniform mat4 projection; //透视

void main() {
    //  gl_Position = projection *  model * vec4(aPos, 1.0);
       gl_Position = projection * view* model * vec4(aPos, 1.0);
}
)";
/// <summary>
/// 可以不渲染
/// </summary>
const char* depthViewPortShaderFragmentShaderSource = R"(
#version 450 core

out vec4 FragColor;

void main() {

}
)";
/// <summary>
/// 深度图可视化顶点着色器
/// </summary>
const char* depthVisualShaderVertexShaderSource = R"(
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 1.0);
}
)";
/// <summary>
/// 深度图正交可视化片元着色器
/// </summary>
const char* depthVisualShaderFragmentShaderSource = R"(
#version 450 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D depthMap;   // 深度贴图
const float near_plane=0.1f;      // 近裁剪面
const float far_plane=100.0f;       // 远裁剪面

void main()
{             

    float depthValue = texture(depthMap, TexCoords).r;
    FragColor = vec4(vec3(depthValue), 1.0); // orthographic
}
)";
/// <summary>
/// 投影深度图线性可视化片元着色器
/// </summary>
const char* depthLinerVisualShaderFragmentShaderSource = R"(
#version 450 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D depthMap;   // 深度贴图
const float near_plane=0.1f;      // 近裁剪面
const float far_plane=1000.0f;       // 远裁剪面

// 线性化深度值
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // 转换到NDC空间
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

void main()
{             
    // 获取当前纹理中的深度值
    float depthValue = texture(depthMap, TexCoords).r;

    // 线性化深度值
    float linearDepth = LinearizeDepth(depthValue);

    // 将线性化深度值映射到[0,1]范围进行可视化
    FragColor = vec4(vec3(linearDepth / far_plane), 1.0);  // 使用线性深度值来显示深度


}
)";
/// <summary>
/// 无光照实例化简单实例化着色器
/// </summary>
const char* instanceNoLightingVertexShaderSource = R"(
#version 450 core
layout(location = 0) in vec3 aPos;           // 顶点位置
layout(location = 1) in vec2 aTexCoord;      // 纹理坐标
layout(location = 2) in vec3 aNormal;        // 法线

// 实例化矩阵
layout(location = 3) in mat4 instanceMatrix;

out vec2 TexCoord;                          // 传递纹理坐标到片段着色器
out vec3 FragPos;                           // 传递片段位置到片段着色器
out vec3 Normal;                            // 传递法线到片段着色器
out vec4 FragPosClip;                       // 传递裁剪空间坐标到片段着色器
// 传递实例ID，GL 中内置的
flat out int instanceID;

uniform mat4 projection;                    // 投影矩阵
uniform mat4 view;                          // 视图矩阵
uniform mat4 transform;                     // 模型变换矩阵

void main()
{
    // 计算片段位置和纹理坐标
    FragPos = vec3(instanceMatrix * vec4(aPos, 1.0)); // 使用实例矩阵进行变换
    TexCoord = aTexCoord;
    Normal = mat3(transpose(inverse(instanceMatrix))) * aNormal; // 法线变换

    // 计算裁剪空间坐标
    FragPosClip = projection * view * transform * instanceMatrix * vec4(aPos, 1.0);
    
    instanceID = gl_InstanceID;

    // 计算最终位置
    gl_Position = FragPosClip;
}
)";


/// <summary>
///实例化着色器基础版
/// </summary>
const char* instanceNoLightingFragmentShaderSource = R"(
#version 450 core

in vec3 FragPos;    // 传入片段位置
in vec3 Normal;     // 传入法线
in vec2 TexCoord;   // 传入纹理坐标

out vec4 FragColor; // 输出颜色

uniform vec3 baseColor;      // 模型固有色
uniform vec3 emission;      //自发光
uniform sampler2D baseTexture;  // 基础纹理采样器

void main()
{
    
  // 自发光默认值(可选逻辑)
    vec3 emissionDefault = (emission == vec3(0.0)) ? vec3(0.3) : emission;

    // 如果 baseColor 没设置，默认给一个大概颜色
    vec3 colorToUse = (baseColor == vec3(0.0)) ? vec3(0.5, 0.5, 0.5) : baseColor;

     // 纹理采样
    vec4 texColor = texture(baseTexture, TexCoord);

    // 使用基础颜色与纹理进行混合
    vec3 color = colorToUse * texColor.rgb+emission;

     FragColor = vec4(color, 1.0f);

}
)";
/// <summary>
///实例化片元萤光版
/// </summary>
const char* instanceNoLightingFireFlyFragmentShaderSource = R"(
#version 450 core

in vec3 FragPos;    // 传入片段位置
in vec3 Normal;     // 传入法线
in vec2 TexCoord;   // 传入纹理坐标
flat in int instanceID; // 传递的实例ID

out vec4 FragColor; // 输出颜色

uniform vec3 baseColor;         // 模型固有色（取值范围 0~1）
uniform vec3 emission;          // 自发光参数（取值范围 0~1，所有实例传入相同）
uniform sampler2D baseTexture;  // 基础纹理采样器

void main()
{
    // 采样基础纹理
    vec4 texColor = texture(baseTexture, TexCoord);

    // 利用 instanceID 生成一个随机偏移量
    // 这里采用经典的哈希函数方式：sin(instanceID * 常数) 再乘上一个大数，然后取小数部分
    float instanceOffset = fract(sin(float(instanceID) * 12.9898) * 43758.5453);

    // 使用正弦函数实现全周期映射，确保结果始终在 0～1 范围内
    // 这样无论传入的 emission/baseColor 值如何，都能产生完整的 0～1 周期性变化
    vec3 modulatedEmission = 0.5 + 0.5 * sin(2.0 * 3.14159 * (emission + instanceOffset));
    vec3 modulatedBaseColor = 0.5 + 0.5 * sin(2.0 * 3.14159 * (baseColor + instanceOffset));

    // 结合基础纹理采样颜色与调制后的自发光和基础颜色
    vec3 finalColor = modulatedBaseColor * texColor.rgb + modulatedEmission;

    FragColor = vec4(finalColor, finalColor.r);
}

)";
/// <summary>
/// 实例化片元荧光彩色版
/// </summary>
const char* instanceNoLightingFireFlyColorFragmentShaderSource = R"(

#version 450 core

in vec3 FragPos;    
in vec3 Normal;     
in vec2 TexCoord;   
flat in int instanceID; 

out vec4 FragColor; 

uniform vec3 baseColor;         // 输入的基础颜色（0~1），这里用 R 分量作为相位输入
uniform vec3 emission;          // 输入的自发光参数（0~1），这里用 R 分量作为相位输入
uniform sampler2D baseTexture;  // 基础纹理采样器

// 辅助函数：将 HSV 转换为 RGB
vec3 hsv2rgb(vec3 c) {
    vec3 rgb = clamp(abs(mod(c.x * 6.0 + vec3(0.0, 4.0, 2.0), 6.0) - 3.0) - 1.0, 0.0, 1.0);
    rgb = rgb * c.y + (1.0 - c.y);
    return rgb * c.z;
}

void main()
{
    // 采样基础纹理
    vec4 texColor = texture(baseTexture, TexCoord);
    
    // 利用 instanceID 生成 0～1 之间的随机偏移量
    float instanceOffset = fract(sin(float(instanceID) * 12.9898) * 43758.5453);
    
    // 计算多彩的自发光颜色：将 emission.r 与 instanceOffset 相加作为 hue（色相），饱和度与亮度均设为 1
    float emissionHue = fract(emission.r + instanceOffset);
    vec3 modulatedEmission = hsv2rgb(vec3(emissionHue, 1.0, 1.0));
    
    // 计算多彩的基础颜色：将 baseColor.r 与 instanceOffset 相加作为 hue，同样使用饱和度和亮度为 1
    float baseHue = fract(baseColor.r + instanceOffset);
    vec3 modulatedBaseColor = hsv2rgb(vec3(baseHue, 1.0, 1.0));
    
    // 结合基础纹理采样的颜色与调制后的颜色产生最终颜色
    vec3 finalColor = modulatedBaseColor * texColor.rgb + modulatedEmission;
    
    FragColor = vec4(finalColor, finalColor.r);
}

)";
/// <summary>
///实例化着色器用于使用视口深度图进行后处理测试的片元部分
/// </summary>
const char* instanceNoLightingForViewPortDepthMapFragmentShaderSource = R"(
#version 450 core

in vec3 FragPos;    // 传入片段位置
in vec3 Normal;     // 传入法线
in vec2 TexCoord;   // 传入纹理坐标
in vec4 FragPosClip; // 传入裁剪空间坐标

out vec4 FragColor; // 输出颜色

uniform vec3 baseColor;      // 模型固有色
uniform vec3 emission;      // 自发光
uniform sampler2D baseTexture;  // 基础纹理采样器
uniform sampler2D depthTexture;  // 深度纹理,用于实例化树木实现遮挡半透明

void main()
{
    // 自发光默认值(可选逻辑)
    vec3 emissionDefault = (emission == vec3(0.0)) ? vec3(0.3) : emission;

    // 如果 baseColor 没设置，默认给一个大概颜色
    vec3 colorToUse = (baseColor == vec3(0.0)) ? vec3(0.5, 0.5, 0.5) : baseColor;

    // 纹理采样
    vec4 texColor = texture(baseTexture, TexCoord);

    // 使用基础颜色与纹理进行混合
    vec3 color = colorToUse * texColor.rgb;

    // 将裁剪空间坐标转换为屏幕空间坐标
    vec3 fragPosScreen = FragPosClip.xyz / FragPosClip.w;  // 透视除法
    fragPosScreen = fragPosScreen * 0.5 + 0.5;  // 转换到 [0, 1] 范围

    // 采样深度图
    float depthFromTexture = texture(depthTexture, fragPosScreen.xy).r;

    // 比较深度值,只要有采集到深度
    if (fragPosScreen.z < depthFromTexture&&depthFromTexture<1.0f) {
        FragColor = vec4(color, 0.1f);  // 如果当前片段在前面，设置为半透明
    } else {
        FragColor = vec4(color, 1.0);  // 否则不透明
    }
}
)";
/// <summary>
/// 波浪顶点着色器，用于制造波浪效果
/// </summary>
const char* waveVertexShaderSource = R"(

#version 450 core
layout(location = 0) in vec3 aPos;       // 顶点位置
layout(location = 1) in vec2 aTexCoord;  // 纹理坐标
layout(location = 2) in vec3 aNormal;    // 顶点法线

// 输出到片段着色器
out vec2 TexCoord;   // 传递纹理坐标
out vec3 FragPos;    // 世界空间中的片段位置
out vec3 Normal;     // 世界空间中的法线
out vec3 ViewPos;    // 相机位置

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec2 textureScale; // 纹理缩放因子
uniform float waveTime;        // 时间变量，用于波浪效果
uniform float waveAmplitude; // 波浪幅度
uniform float waveFrequency; // 波浪频率
uniform float waveSpeed;     // 波浪速度

void main()
{
    // 计算波浪效果
    vec3 waveOffset = vec3(0.0, sin(aPos.x * waveFrequency + waveTime * waveSpeed) * waveAmplitude, 0.0);
    vec4 worldPos = model * vec4(aPos + waveOffset, 1.0);
    FragPos = worldPos.xyz;

    // 对纹理坐标应用波浪效果
    vec2 texWaveOffset = vec2(0.0, sin(aTexCoord.x * waveFrequency + waveTime * waveSpeed) * waveAmplitude * 0.1);
    TexCoord = (aTexCoord + texWaveOffset) * textureScale;

    // 正确变换法线，防止非均匀缩放问题
    Normal = mat3(transpose(inverse(model))) * aNormal;

    // 计算相机位置 (viewPos)，视图矩阵是从世界空间转换到相机空间的
    ViewPos = -mat3(view) * vec3(view[3]);

    // 计算最终屏幕坐标
    gl_Position = projection * view * worldPos;
}


)";

/// <summary>
/// 模板测试纯色着色器
/// </summary>
const char* stencilTestVertexShaderSource = R"(
#version 450 core
layout(location = 0) in vec3 aPos;   // 顶点位置
layout(location = 1) in vec2 aTexCoord;   // 纹理坐标 
layout(location = 2) in vec3 aNormal;   // 法线

//这里对应初始化时，设置的顶点属性
out vec3 FragPos;   // 传递给片段着色器的片段位置

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)";

const char* stencilTestFragmentShaderSource = R"(
    #version 450 core
    out vec4 FragColor;  // 输出颜色
    
    void main() {
        FragColor = vec4(1,1,1,0.5F);  // 白色着色器
    }
)";
#endif