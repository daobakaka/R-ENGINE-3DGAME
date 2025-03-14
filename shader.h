#ifndef SHADERS_Hero
#define SHADERS_Hero
//����shader ����ֻ�ܱ�һ���ļ����ã��������ļ���ʹ������Ҫʹ������ extern ����������

/// <summary>
/// ������ɫ��
/// </summary>
const char* cubeVertexShaderSource = R"(
    #version 450 core
    layout (location = 0) in vec3 aPos;   // ����λ��
    out vec3 ourColor;                    // ���ݸ�Ƭ����ɫ������ɫ
    uniform mat4 model;                   // ģ�ͱ任����
    uniform mat4 view;                    // ��ͼ�任����
    uniform mat4 projection;              // ͶӰ����
    uniform vec3 color;                   // ��ɫ (ͨ�� uniform ����)
    
    void main() {
        gl_Position = projection * view * model * vec4(aPos, 1.0f);
        ourColor = color;  // ����ɫ���ݸ�Ƭ����ɫ��
    }
)";

const char* cubeFragmentShaderSource = R"(
    #version 450 core
    out vec4 FragColor;  // �����ɫ
    in vec3 ourColor;    // �Ӷ�����ɫ�����ݹ�������ɫ
    
    void main() {
        // Ĭ��ǳ��ɫ������͸����
        vec3 defaultColor = vec3(1.0f, 1.0f, 1.0f);  // ����ɫ
        FragColor = vec4(ourColor+defaultColor, 0.5f);  // ʹ�ô��ݵ���ɫ��������͸����
    }
)";
/// <summary>
/// �ο���������ת��ɫ��
/// </summary>
const char* axisVertexShaderSource = R"(
    #version 450 core
layout (location = 0) in vec3 aPos;     // ����λ��
layout (location = 1) in vec3 aColor;   // ������ɫ
out vec3 ourColor;                      // �����ƬԪ��ɫ������ɫ

uniform mat4 model;       // ģ�ͱ任����
uniform mat4 view;        // ��ͼ����
uniform mat4 projection;  // ͶӰ����

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    ourColor = aColor;
}

)";
const char* axisFragmentShaderSource = R"(
    #version 450 core
    in vec3 ourColor;       // �Ӷ�����ɫ���������ɫ
    out vec4 FragColor;     // �����ƬԪ��ɫ

    void main() {
        FragColor = vec4(ourColor, 1.0);
    }
)";



/// <summary>
/// ������ɫ��
/// </summary>
const char* rayVertexShaderSource = R"(
#version 450 core
layout (location = 0) in vec3 aPos;  // ����λ��

uniform mat4 model;       // ģ�ͱ任����
uniform mat4 view;        // ��ͼ����
uniform mat4 projection;  // ͶӰ����

void main()
{
    // ��׼ MVP �任
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)";
const char* rayFragmentShaderSource = R"(
#version 450 core
out vec4 FragColor;

// ������ɫ�� baseColor ���ƣ����δ����Ĭ��(0.1,0.1,0.1)
uniform vec3 baseColor;

void main()
{
    // ��� baseColor == vec3(0.0), Ĭ����(0.1,0.1,0.1)
    vec3 colorUsed = (baseColor == vec3(0.0)) ? vec3(1, 1, 1) : baseColor;
    
    FragColor = vec4(colorUsed, 1.0);
}
)";
/// <summary>
/// ����������ɫ��
/// </summary>
const char* lightSourceVertexShaderSource = R"(
    #version 450 core
    layout (location = 0) in vec3 aPos;   // ����λ��
    out vec3 fragPos;                     // ���ݸ�Ƭ����ɫ���Ķ���λ��
    uniform mat4 model;                   // ģ�ͱ任����
    uniform mat4 view;                    // ��ͼ�任����
    uniform mat4 projection;              // ͶӰ����
    
    void main() {
        fragPos = vec3(model * vec4(aPos, 1.0f));  // ����任��Ķ���λ��
        gl_Position = projection * view * model * vec4(aPos, 1.0f);  // ��������λ��
    }
)";

const char* lightSourceFragmentShaderSource = R"(
    #version 450 core
    out vec4 FragColor;  // �����ɫ
    in vec3 fragPos;    // �Ӷ�����ɫ�����ݹ����Ķ���λ��,������ʵ�ϣ��������������������λ��

    // ��Դ����
    uniform vec3 lightPos;      // ��Դλ��,uniform �����ȫ�ֵĶ���
    uniform vec3 lightColor;    // ��Դ��ɫ
    uniform float lightIntensity; // ��Դǿ��
    uniform vec3 viewPos;        // �ӵ�λ��
    uniform vec3 baseColor;//����ɫ
    uniform vec3 emission;//�Է��⣬ͨ���ӵ���ɫ�����ĩβ

    void main() {
        // �Է��⣨Emission��
       vec3 emissionDefault=(emission == vec3(0.0f,0.0f,0.0f))?vec3(0.1f,0.1f,0.1f):emission;  // ����ɫ�Է���
       vec3 colorToUse = (baseColor == vec3(0.0f,0.0f,0.0f))?vec3(0.3f, 0.3f, 0.0f):baseColor;      
        // ���ղ���
        // ������
        vec3 ambient = 0.1 * lightColor;  //�������˥���������⡢�����䡢���淴���ͨ��ʹ�ó˷����˷�ͨ�������ģ�����˥��Ч��

        // �����Դλ�ú�������淨��֮��ĽǶ� 
        vec3 lightDir = normalize(lightPos - fragPos);  
        float diff = max(dot(normalize(fragPos), lightDir), 0.0);  // ������
        vec3 diffuse = diff * lightColor * lightIntensity;  // ���������

        // ���淴��
        vec3 reflectDir = reflect(-lightDir, normalize(fragPos));  // ��������
        vec3 viewDir = normalize(viewPos - fragPos);  // �����嵽�ӵ������
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);  // ���淴��
        vec3 specular = spec * lightColor * lightIntensity*0.1f;

        // �ϲ����й��ճɷ�
        vec3 result = (ambient + diffuse + specular);

        // ������ɫ = �Է��� + ����Ч��
        FragColor = vec4(result + emissionDefault + colorToUse, 0.5f);  // ʹ�ù��ս�����Է�����
    }
)";
/// <summary>
/// ����������ɫ������ɫ��
/// </summary>
const char* colorlightSourceVertexShaderSource = R"(
  #version 450 core

layout (location = 0) in vec3 aPos;      // ����λ��
layout (location = 1) in vec3 aNormal;   // ���㷨��

out vec3 fragPos;   // ����ռ�����
out vec3 Normal;    // ����ռ䷨��

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // ��������ռ�����
    vec4 worldPos = model * vec4(aPos, 1.0);
    fragPos = worldPos.xyz;

    // ��� model �����Ǿ������ţ���ĳ�:
    // Normal = mat3(transpose(inverse(model))) * aNormal;
    // ����ֻ����ʾ, ���� model �����Ǿ�������:
    Normal = mat3(model) * aNormal;

    // ����λ��
    gl_Position = projection * view * worldPos;
}

)";

const char* colorlightSourceFragmentShaderSource = R"(
#version 450 core

in vec3 fragPos;  
in vec3 Normal;
out vec4 FragColor;

// uniform: ����Ĺ��ա���������ʵȲ���
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightIntensity;
uniform vec3 viewPos;
uniform vec3 baseColor;   // ���Ե���ģ�͵Ĺ���ɫ
uniform vec3 emission;    // �Է��⣨��ѡ��

void main()
{
    // �Է���Ĭ��ֵ(��ѡ�߼�)
    vec3 emissionDefault = (emission == vec3(0.0)) ? vec3(0.1) : emission;

    // ��� baseColor û���ã�Ĭ�ϸ�һ�������ɫ
    vec3 colorToUse = (baseColor == vec3(0.0)) ? vec3(0.1, 0.1, 0.1) : baseColor;

    // 1. ������
    vec3 ambient = 0.1 * lightColor;

    // 2. ������
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * lightIntensity;

    // 3. ���淴��(Phong)
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = spec * lightColor * lightIntensity * 0.1;

    // �ϲ�
    vec3 lighting = ambient + diffuse + specular;

    // ������ɫ = (���� �� �������ɫ) + �Է���
    vec3 result = lighting * colorToUse + emissionDefault;
    FragColor = vec4(result, 1.0);
}
)";
/// <summary>
/// ��ɫ����ͨ����ɫ��
/// </summary>
const char* colorlightsArrayVertexShaderSource = R"(
#version 450 core
layout(location = 0) in vec3 aPos;       // ����λ��
layout(location = 1) in vec2 aTexCoord;    // ��������
layout(location = 2) in vec3 aNormal;      // ���㷨��

// �����Ƭ����ɫ��
out vec2 TexCoord;   // ������������
out vec3 FragPos;    // ����ռ��е�Ƭ��λ��
out vec3 Normal;     // ����ռ��еķ���
out vec3 ViewPos;   // ���λ��

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec2 textureScale ; // uniform ������Ƹ�ֵ�Ƿ���ֻ���ⲿ����������������,�ı���������ŷ�ʽ

void main()
{
    // ��������ռ�����
    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos = worldPos.xyz;

    // ������������
    TexCoord = aTexCoord * textureScale;
  
    // ��ȷ�任���ߣ���ֹ�Ǿ�����������
    Normal = mat3(transpose(inverse(model))) * aNormal;

 // �������λ�� (viewPos)����ͼ�����Ǵ�����ռ�ת��������ռ��
    ViewPos = -mat3(view) * vec3(view[3]);

    // ����������Ļ����
    gl_Position = projection * view * worldPos;
}
)";

const char* colorlightsArraySourceFragmentShaderSource = R"(
#version 450 core

in vec3 FragPos;  
in vec3 Normal;
in vec2 TexCoord;  // ������������
in vec3 ViewPos;   // ���λ�ô��루�Ӷ�����ɫ���� 
out vec4 FragColor;

// ��������
uniform float metallic; // ������
uniform float roughness; // �ֲڶ�
uniform float opacity; // ͸����
uniform float IOR; // ������
uniform float ao; // �������ڱ���


//��Ч����
uniform float dissolveThreshold; // �ܽ���ֵ�������ܽ�Ľ���

// ��ɫ
uniform vec3 baseColor;   // ģ�͹���ɫ
uniform vec3 emission;    // �Է��⣨��ѡ��

// ��ͼ
uniform sampler2D baseTexture;  // �������������
uniform sampler2D normalTexture; // �������������
uniform sampler2D heightTexture; // �߶���ͼ
uniform sampler2D roughnessTexture; // �ڶ���ͼ
uniform samplerCube specularTexture; // �߹ⷴ����ͼ��ԭ�� reflectionTexture��
uniform sampler2D aoTexture;  // �������ڱ���ͼ

//--ƽ�й���ͼ
uniform sampler2D autoParallelShadowMap;//ƽ�й������ͼ��������������Ӱ
// ���չ�Դ����Ӱ����
uniform mat4 lightSpaceMatrix;  // �ӹ�Դ�ӽǼ���ľ���

// ���Դ����
const int MAX_POINT_LIGHTS = 4;
uniform int numPointLights;
uniform vec3 lightPos[MAX_POINT_LIGHTS];
uniform vec3 lightColor[MAX_POINT_LIGHTS];
uniform float lightIntensity[MAX_POINT_LIGHTS];
float lightAttenuation = 0.01F; // ����˥��ϵ��
uniform float lightDistanceSquared[MAX_POINT_LIGHTS]; // ����ľ���ƽ��

// ƽ�й⣨����⣩����
uniform vec3 parallelLightDirection;
uniform vec3 parallelLightColor;
uniform float parallelLightIntensity;

// �ֵ�Ͳ���۹�ƣ�����
const int MAX_FLASHLIGHTS = 4;
uniform int numFlashLights;
uniform vec3 flashLightPos[MAX_FLASHLIGHTS];
uniform vec3 flashLightDirection[MAX_FLASHLIGHTS];
uniform vec3 flashLightColor[MAX_FLASHLIGHTS];
uniform float flashLightIntensity[MAX_FLASHLIGHTS];
uniform float flashLightCutoff[MAX_FLASHLIGHTS]; // cutoff ֵΪ����ֵ
float flashlightAttenuation = 0.01F; // �ֵ�Ͳ���˥��ϵ��
uniform float flashLightDistanceSquared[MAX_FLASHLIGHTS]; // ����ľ۹�ƾ���ƽ��

// PBR ���ռ��㺯��
vec3 calculatePBR(vec3 lightDir, vec3 viewDir, vec3 norm, vec3 lightColor, float lightIntensity, float metallicValue, float roughnessValue) {
    vec3 halfDir = normalize(lightDir + viewDir);

    // ���������ʣ�F0���������Ⱦ���
    vec3 F0 = mix(vec3(0.04), baseColor, metallicValue);

    // Fresnel �Schlick ���ƣ�
    float HdotV = max(dot(halfDir, viewDir), 0.0);
    vec3 F = F0 + (1.0 - F0) * pow(1.0 - HdotV, 5.0);

    // �����䲿��
    float NdotL = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * NdotL * (1.0 - metallicValue);

    // ���淴�䲿�֣�Cook-Torrance BRDF��
    float NdotV = max(dot(norm, viewDir), 0.0);
    float NdotH = max(dot(norm, halfDir), 0.0);

    float alpha = roughnessValue * roughnessValue;
    float alphaSq = alpha * alpha;

    // ���κ�����Smith-Schlick GGX��
    float G1 = NdotV / (NdotV * (1.0 - alphaSq) + alphaSq);
    float G2 = NdotL / (NdotL * (1.0 - alphaSq) + alphaSq);
    float G = G1 * G2;

    // ���߷ֲ�������Trowbridge-Reitz GGX��
    float denom = (NdotH * NdotH * (alphaSq - 1.0) + 1.0);
    float D = alphaSq / (3.14159 * denom * denom);

    // ���淴��
    vec3 specular = (D * G * F) / (4.0 * NdotV * NdotL + 0.0001);

    return (diffuse + specular) * lightIntensity;
}

// ���������ʧ�������ʽ���͸���ȵ��� 0.6 ʱ��Ч��
float calculateLightLoss(float opacityValue, float IORValue) {
    if (opacityValue < 0.6) {
        // ͸���ȵ��� 0.6 ʱ�����������ʼ��������ʧ
        return 1.0 - pow((IORValue - 1.0) / (IORValue + 1.0), 2.0);
    } else {
        // ���򣬹�����ʧΪ 0
        return 0.0;
    }
}

void main() {


// ������ͼ���򣬱�����ʹ�� viewDir ǰ����
    vec3 viewDir = normalize(ViewPos - FragPos);
    // ��������������������
    vec3 normalMap = texture(normalTexture, TexCoord).rgb;
    normalMap = normalMap * 2.0 - 1.0;  // ӳ�䵽 [-1, 1]
    vec3 norm = normalize(Normal + normalMap);  // ���ԭʼ����

    // �����߶�����������������
    float height = texture(heightTexture, TexCoord).r;
    vec2 parallaxOffset = viewDir.xy * (height * 0.01f);  // �򵥵��Ӳ�ƫ��,������Ը߶�ͼ�����м���ģ����û��������Ϊ0
    vec2 adjustedTexCoord = TexCoord + parallaxOffset;//����͵��������������ƫ����

    // �����ֲڶ����������ֲڶ�
    float roughnessValue = texture(roughnessTexture, adjustedTexCoord).r * roughness;

    // �����������ڱ��������� AO
    float aoValue = texture(aoTexture, adjustedTexCoord).r * ao;

    // �����߹ⷴ������
    vec3 reflectionDir = reflect(-viewDir, norm);
    vec3 specularColor = texture(specularTexture, reflectionDir).rgb;

    // ����������������������ɫ
    vec4 texColor = texture(baseTexture, adjustedTexCoord);
    vec3 finalBaseColor = baseColor * texColor.rgb;

    // ��ʼ�����չ���
    vec3 ambientTotal = vec3(0.0);
    vec3 diffuseTotal = vec3(0.0);
    vec3 specularTotal = vec3(0.0);


    // --- ���Դ���� ---
    for (int i = 0; i < numPointLights; i++) {
        if (lightIntensity[i] > 0.0001) {
            float attenuation = 1.0 / (1.0 + lightAttenuation * lightDistanceSquared[i]);
            ambientTotal += 0.1 * lightColor[i];
            vec3 lightDir = normalize(lightPos[i] - FragPos);
            vec3 pbrLighting = calculatePBR(lightDir, viewDir, norm, lightColor[i], lightIntensity[i], metallic, roughnessValue);
            diffuseTotal += pbrLighting * attenuation;
        }
    }

    // --- �ֵ�Ͳ���۹�ƣ����� ---
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
// --- ƽ�й⣨����⣩���� ---
if (parallelLightIntensity > 0.0001) {
    ambientTotal += 0.2f * parallelLightColor;
    vec3 lightDir = normalize(-parallelLightDirection);
    vec3 pbrLighting = calculatePBR(lightDir, viewDir, norm, parallelLightColor, parallelLightIntensity, metallic, roughnessValue);
    diffuseTotal += pbrLighting;

    // ��Ӱ����
    vec4 fragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    // �����Ե��ֵ
    const float edgeThreshold = 0.01; // ��Ե��ֵ�����Ը�����Ҫ����

    // �ж��Ƿ��ٽ���Ӱ��ͼ��Ե
    bool isNearEdge = (projCoords.x < edgeThreshold || projCoords.x > 1.0 - edgeThreshold ||
                       projCoords.y < edgeThreshold || projCoords.y > 1.0 - edgeThreshold);

    // ����Ӱ��ͼ�л�ȡ���ֵ
    float closestDepth = texture(autoParallelShadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    // ��̬������Ӱƫ��
    float bias = max(0.005 * (1.0 - dot(norm, lightDir)), 0.001);

    // ��Ӱ�жϣ��Ƚϵ�ǰƬ�ε���������Ӱ��ͼ��ȡ�������
    float shadow = currentDepth > (closestDepth + bias) ? 1.0 : 0.0;

    // ʹ�� PCF ƽ����Ӱ��Ե
    float shadowPCF = 0.0f;
    vec2 texelSize = 1.0 / textureSize(autoParallelShadowMap, 0);
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcf = texture(autoParallelShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadowPCF += (currentDepth - bias > pcf) ? 1.0 : 0.0;
        }
    }
    shadowPCF /= 9.0f;  // ƽ�����

    // �������Ӱ�У�������;��淴���䰵
    if (isNearEdge) {
        // �ٽ���Եʱ��������Ӱ����
        diffuseTotal *= 1.0;
    } else {
        // ����Ӧ����Ӱ
        diffuseTotal *= (1.0 - shadowPCF);
    }
}

    // ���չ���
    vec3 lighting = ambientTotal * aoValue + diffuseTotal + specularColor * metallic;

    // ���������ʧ
    float lightLoss = calculateLightLoss(opacity, IOR);

    // ������ɫ = (����Ч�� �� �������ɫ) + �Է���
    vec3 result = lighting * finalBaseColor * (1.0 - lightLoss) + emission;
    //�ܽ����ֵ��������
    if(texColor.r<dissolveThreshold)
     { discard;  }

    // �����ɫ
    FragColor = vec4(result, opacity) * texColor;
}
)";
/// <summary>
/// �޹��ջ�����ɫ��
/// </summary>

const char* noneLightVertexShaderSource = R"(
#version 450 core
layout(location = 0) in vec3 aPos;   // ����λ��
layout(location = 1) in vec2 aTexCoord;   // �������� 
layout(location = 2) in vec3 aNormal;   // ����
//�����Ӧ��ʼ��ʱ�����õĶ�������
out vec2 TexCoord;  // ���ݸ�Ƭ����ɫ������������
out vec3 FragPos;   // ���ݸ�Ƭ����ɫ����Ƭ��λ��

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

in vec2 TexCoord;   // �Ӷ�����ɫ�����յ���������
in vec3 FragPos;    // �Ӷ�����ɫ�����յ�Ƭ��λ��

uniform vec3 baseColor;      // ���Ե���ģ�͵Ĺ���ɫ
uniform sampler2D baseTexture;  // ����


//--ƽ�й���ͼ
uniform sampler2D autoParallelShadowMap;//ƽ�й������ͼ��������������Ӱ
// ���չ�Դ����Ӱ����
uniform mat4 lightSpaceMatrix;  // �ӹ�Դ�ӽǼ���ľ���

void main()
{
    // ��� baseColor == (0,0,0)����Ĭ��ʹ��(0.1, 0.1, 0.1)
    // ע�⣺���������� == �Ƚ��� GLSL 4.5 ��������ģ������Ҫ�ж��Ƿ����Ϊ�㣬���Ը��� length(baseColor) < 1e-6 �ȡ�
    vec3 colorToUse = (baseColor == vec3(0.0)) ? vec3(0.1, 0.1, 0.1) : baseColor;

    // ����� baseColor �ӵ�������ɫ���ɸ�Ϊ�˷�����
    FragColor = (texture(baseTexture, TexCoord) + vec4(colorToUse, 1.0)) ;
  // FragColor = vec4(1,1,1,1);

} 
)";
/// <summary>
/// �޹���ͨ����Ⱦ��ɫ��
/// </summary>
const char* CustomNoLightShadowShaderVertexSource = R"(
#version 450 core
layout(location = 0) in vec3 aPos;       // ����λ��
layout(location = 1) in vec2 aTexCoord;    // ��������
layout(location = 2) in vec3 aNormal;      // ���㷨��

// �����Ƭ����ɫ��
out vec2 TexCoord;   // ������������
out vec3 FragPos;    // ����ռ��е�Ƭ��λ��
out vec3 Normal;     // ����ռ��еķ���
out vec3 ViewPos;   // ���λ��

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec2 textureScale ; // uniform ������Ƹ�ֵ�Ƿ���ֻ���ⲿ����������������,�ı���������ŷ�ʽ

void main()
{
    // ��������ռ�����
    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos = worldPos.xyz;

    // ������������
    TexCoord = aTexCoord * textureScale;
  
    // ��ȷ�任���ߣ���ֹ�Ǿ�����������
    Normal = mat3(transpose(inverse(model))) * aNormal;

 // �������λ�� (viewPos)����ͼ�����Ǵ�����ռ�ת��������ռ��
    ViewPos = -mat3(view) * vec3(view[3]);

    // ����������Ļ����
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

// ���������������ӿڣ���������չ��
uniform float metallic;      // ������
uniform float roughness;     // �ֲڶ�
uniform float opacity;       // ͸����
uniform float IOR;           // ������
uniform float ao;            // �������ڱ���

//��Ч����
uniform float dissolveThreshold; // �ܽ���ֵ�������ܽ�Ľ���
// ��ɫ
uniform vec3 baseColor;      // ģ�͹���ɫ
uniform vec3 emission;       // �Է���

// ��ͼ������
uniform sampler2D baseTexture;      // ��������
uniform sampler2D normalTexture;     // ��������
uniform sampler2D heightTexture;     // �߶���ͼ�������Ӳ�ӳ�䣩
uniform sampler2D roughnessTexture;   // �ֲڶ���ͼ
uniform samplerCube specularTexture;  // ������ͼ��cube map��
uniform sampler2D aoTexture;          // �������ڱ���ͼ

// ��Ӱ��أ������������Ӱ���㣩
uniform sampler2D autoParallelShadowMap; // ƽ�й���Ӱ��ͼ
uniform mat4 lightSpaceMatrix;           // �ӹ�Դ�ӽǼ���ľ���
uniform vec3 parallelLightDirection;     // ƽ�йⷽ�����ڼ�����Ӱƫ��

// ���µ��Դ���۹�Ⱦ�ȡ�������������

void main()
{
      vec3 viewDir = normalize(ViewPos - FragPos);
    // ��������������������
    vec3 normalMap = texture(normalTexture, TexCoord).rgb;
    normalMap = normalMap * 2.0 - 1.0;  // ӳ�䵽 [-1, 1]
    vec3 norm = normalize(Normal + normalMap);  // ���ԭʼ����

    // �����߶�����������������
    float height = texture(heightTexture, TexCoord).r;
    vec2 parallaxOffset = viewDir.xy * (height * 0.01f);  // �򵥵��Ӳ�ƫ��,������Ը߶�ͼ�����м���ģ����û��������Ϊ0
    vec2 adjustedTexCoord = TexCoord + parallaxOffset;//����͵��������������ƫ����

    // �����ֲڶ����������ֲڶ�
    float roughnessValue = texture(roughnessTexture, adjustedTexCoord).r * roughness;

    // �����������ڱ��������� AO
    float aoValue = texture(aoTexture, adjustedTexCoord).r * ao;

    // �����߹ⷴ������
    vec3 reflectionDir = reflect(-viewDir, norm);
    vec3 specularColor = texture(specularTexture, reflectionDir).rgb;

    // ����������������������ɫ
    vec4 texColor = texture(baseTexture, adjustedTexCoord);
    vec3 finalBaseColor = baseColor * texColor.rgb;

 // ---- ��Ӱ���㣨�������Ӱ�� ----
vec4 fragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
projCoords = projCoords * 0.5 + 0.5; // ������ӳ�䵽 [0, 1]

// �����Ե��ֵ
const float edgeThreshold = 0.01; // ��Ե��ֵ�����Ը�����Ҫ����

// �ж��Ƿ��ٽ���Ӱ��ͼ��Ե
bool isNearEdge = (projCoords.x < edgeThreshold || projCoords.x > 1.0 - edgeThreshold ||
                   projCoords.y < edgeThreshold || projCoords.y > 1.0 - edgeThreshold);

// ʹ��ƽ�йⷽ�������Ӱƫ��
vec3 lightDir = normalize(-parallelLightDirection);
float bias = max(0.005 * (1.0 - dot(norm, lightDir)), 0.005); // ���� bias ֵ����С 0.005
float currentDepth = projCoords.z;

// ��Ӱ�жϣ��Ƚϵ�ǰƬ�ε���������Ӱ��ͼ��ȡ�������
float shadow = 0.0;
if (!isNearEdge) {
    // ʹ�� PCF ����Ӱ������3x3 ����
    vec2 texelSize = 1.0 / textureSize(autoParallelShadowMap, 0);
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(autoParallelShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += (currentDepth - bias > pcfDepth) ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0; // ƽ�����
}

// ����Ӱ����Ӧ�õ�������ɫ
finalBaseColor *= (1.0 - shadow);
// ---- ��Ӱ������� ----

    // ���������ɫ���������ƽ�������Է���
     vec3 result = finalBaseColor*aoValue*roughnessValue  +specularColor*metallic+ emission;
       //�ܽ����ֵ��������
    if(texColor.r<dissolveThreshold)
     { discard;  }
     FragColor = vec4(result, opacity) * texColor;


}
)";


/// <summary>
/// �޹��չ�Դ��Ⱦ��ɫ��
/// </summary>
const char* noneLightLightVertexShaderSource = R"(
#version 450 core
layout(location = 0) in vec3 aPos;   // ����λ��
layout(location = 1) in vec2 aTexCoord;   // �������� 
layout(location = 2) in vec3 aNormal;   // ����
//�����Ӧ��ʼ��ʱ�����õĶ�������
out vec2 TexCoord;  // ���ݸ�Ƭ����ɫ������������
out vec3 FragPos;   // ���ݸ�Ƭ����ɫ����Ƭ��λ��

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

in vec2 TexCoord;   // �Ӷ�����ɫ�����յ���������
in vec3 FragPos;    // �Ӷ�����ɫ�����յ�Ƭ��λ��

uniform vec3 baseColor;      // ���Ե���ģ�͵Ĺ���ɫ
uniform sampler2D baseTexture;  // ����
uniform float lightIntensity; // ��ǿ�ȿ��ӻ�
uniform float lightDirection;//���շ�����ʱ����
void main()
{
    // ��� baseColor == (0,0,0)����Ĭ��ʹ��(0.1, 0.1, 0.1)
    // ע�⣺���������� == �Ƚ��� GLSL 4.5 ��������ģ������Ҫ�ж��Ƿ����Ϊ�㣬���Ը��� length(baseColor) < 1e-6 �ȡ�
    vec3 colorToUse = (baseColor == vec3(0.0)) ? vec3(0.1, 0.1, 0.1) : baseColor;

    // ��� lightIntensity==0����Ĭ��ֵΪ0.1
    float tempIntensity = (lightIntensity == 0.0) ? 0.1f : lightIntensity;

    // ����ȡ�� + baseColor���ٳ��Թ�ǿ��
    // ����� baseColor �ӵ�������ɫ���ɸ�Ϊ�˷�����
    FragColor = (texture(baseTexture, TexCoord) *vec4(colorToUse, 1.0))* 0.5f* tempIntensity;
} 
)";


/// <summary>
/// ������Ⱦ��ɫ��
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
/// ��պ���ɫ��
/// </summary>
const char* skyboxVertexShaderSource = R"(
    #version 450 core
    layout (location = 0) in vec3 aPos;  // ����λ��

    out vec3 TexCoords;  // ���ݸ�Ƭ����ɫ������������

    uniform mat4 projection;  // ͶӰ����
    uniform mat4 view;  // ��ͼ����

    void main() {
        gl_Position = projection * view * vec4(aPos, 1.0f);  // �������յ���Ļ����
        TexCoords = aPos;  // ���������괫�ݸ�Ƭ����ɫ��
    }
)";

const char* skyboxFragmentShaderSource = R"(
    #version 450 core
    out vec4 FragColor;  // �����ɫ
    in vec3 TexCoords;   // �Ӷ�����ɫ�����ݹ�������������

    uniform samplerCube skybox;  // ����������

    void main() {
        FragColor = texture(skybox, TexCoords);  // ��������������ȡ���������ɫ
    }
)";
/// <summary>
/// ���ͼ������ɫ��
/// </summary>
const char* depthTestShaderVertexShaderSource = R"(
#version 450 core
layout(location = 0) in vec3 aPos;   // ����λ��

uniform mat4 model;                  // ����ģ�;���
uniform mat4 lightSpaceMatrix;       // �ӹ�Դ�ӽǼ���ľ���

void main() {
    // �����������ռ�ת������Դ�ӽǿռ�
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);  // model ������������ռ�ת��
}
)";
/// <summary>
/// ���Բ���Ⱦ����Ϊ���ֵ���ᴢ������Ȼ������У�OPENGL �������ֵ�Ƿ����Եģ�������Ҫ��������ת�����ܿ���
/// </summary>
const char* depthTestShaderFragmentShaderSource = R"(
#version 450 core

out vec4 FragColor;

// ֱ������ɫ���ж�����ü����Զ�ü���
const float near = 0.1f;  // ���ü���
const float far = 100.0f;   // Զ�ü���

// ���Ի���Ⱥ���
float LinearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0;  // �����ֵ�� [0, 1] ת���� NDC (-1, 1) ����
    return (2.0 * near * far) / (far + near - z * (far - near));  // �����������
}

void main() {
    // ��ȡ��ǰƬ�ε����ֵ
    float depth = gl_FragCoord.z;

    // ���Ի����ֵ
    float linearDepth = LinearizeDepth(depth) / far; // ���� far���õ����ֵ�� [0, 1] ��Χ��

    // ��Ⱦ��ȣ���ʾ���Ի������ֵ���Ҷȣ�
    FragColor = vec4(vec3(linearDepth), 1.0);  // ʹ�����������Ϊ��ɫֵ



}
)";

/// <summary>
/// ���ͼƽ�й���Ӱʹ����ɫ��
/// </summary>
const char* depthShaderVertexShaderSource = R"(
#version 450 core
layout(location = 0) in vec3 aPos;   // ����λ��

uniform mat4 model;                  // ����ģ�;���
uniform mat4 lightSpaceMatrix;       // �ӹ�Դ�ӽǼ���ľ���

void main() {
    // �����������ռ�ת������Դ�ӽǿռ�
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);  // model ������������ռ�ת��
}
)";
/// <summary>
/// ���Բ���Ⱦ
/// </summary>
const char* depthShaderFragmentShaderSource = R"(
#version 450 core

out vec4 FragColor;

void main() {

}
)";
/// <summary>
/// ���ͼ�ӿں���ʹ����ɫ��
/// </summary>
const char* depthViewPortShaderVertexShaderSource = R"(
#version 450 core
layout(location = 0) in vec3 aPos;   // ����λ��

uniform mat4 model;                  // ����ģ�;���
uniform mat4 view;       // �ӿ�
uniform mat4 projection; //͸��

void main() {
    //  gl_Position = projection *  model * vec4(aPos, 1.0);
       gl_Position = projection * view* model * vec4(aPos, 1.0);
}
)";
/// <summary>
/// ���Բ���Ⱦ
/// </summary>
const char* depthViewPortShaderFragmentShaderSource = R"(
#version 450 core

out vec4 FragColor;

void main() {

}
)";
/// <summary>
/// ���ͼ���ӻ�������ɫ��
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
/// ���ͼ�������ӻ�ƬԪ��ɫ��
/// </summary>
const char* depthVisualShaderFragmentShaderSource = R"(
#version 450 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D depthMap;   // �����ͼ
const float near_plane=0.1f;      // ���ü���
const float far_plane=100.0f;       // Զ�ü���

void main()
{             

    float depthValue = texture(depthMap, TexCoords).r;
    FragColor = vec4(vec3(depthValue), 1.0); // orthographic
}
)";
/// <summary>
/// ͶӰ���ͼ���Կ��ӻ�ƬԪ��ɫ��
/// </summary>
const char* depthLinerVisualShaderFragmentShaderSource = R"(
#version 450 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D depthMap;   // �����ͼ
const float near_plane=0.1f;      // ���ü���
const float far_plane=1000.0f;       // Զ�ü���

// ���Ի����ֵ
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // ת����NDC�ռ�
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

void main()
{             
    // ��ȡ��ǰ�����е����ֵ
    float depthValue = texture(depthMap, TexCoords).r;

    // ���Ի����ֵ
    float linearDepth = LinearizeDepth(depthValue);

    // �����Ի����ֵӳ�䵽[0,1]��Χ���п��ӻ�
    FragColor = vec4(vec3(linearDepth / far_plane), 1.0);  // ʹ���������ֵ����ʾ���


}
)";
/// <summary>
/// �޹���ʵ������ʵ������ɫ��
/// </summary>
const char* instanceNoLightingVertexShaderSource = R"(
#version 450 core
layout(location = 0) in vec3 aPos;           // ����λ��
layout(location = 1) in vec2 aTexCoord;      // ��������
layout(location = 2) in vec3 aNormal;        // ����

// ʵ��������
layout(location = 3) in mat4 instanceMatrix;

out vec2 TexCoord;                          // �����������굽Ƭ����ɫ��
out vec3 FragPos;                           // ����Ƭ��λ�õ�Ƭ����ɫ��
out vec3 Normal;                            // ���ݷ��ߵ�Ƭ����ɫ��
out vec4 FragPosClip;                       // ���ݲü��ռ����굽Ƭ����ɫ��
// ����ʵ��ID��GL �����õ�
flat out int instanceID;

uniform mat4 projection;                    // ͶӰ����
uniform mat4 view;                          // ��ͼ����
uniform mat4 transform;                     // ģ�ͱ任����

void main()
{
    // ����Ƭ��λ�ú���������
    FragPos = vec3(instanceMatrix * vec4(aPos, 1.0)); // ʹ��ʵ��������б任
    TexCoord = aTexCoord;
    Normal = mat3(transpose(inverse(instanceMatrix))) * aNormal; // ���߱任

    // ����ü��ռ�����
    FragPosClip = projection * view * transform * instanceMatrix * vec4(aPos, 1.0);
    
    instanceID = gl_InstanceID;

    // ��������λ��
    gl_Position = FragPosClip;
}
)";


/// <summary>
///ʵ������ɫ��������
/// </summary>
const char* instanceNoLightingFragmentShaderSource = R"(
#version 450 core

in vec3 FragPos;    // ����Ƭ��λ��
in vec3 Normal;     // ���뷨��
in vec2 TexCoord;   // ������������

out vec4 FragColor; // �����ɫ

uniform vec3 baseColor;      // ģ�͹���ɫ
uniform vec3 emission;      //�Է���
uniform sampler2D baseTexture;  // �������������

void main()
{
    
  // �Է���Ĭ��ֵ(��ѡ�߼�)
    vec3 emissionDefault = (emission == vec3(0.0)) ? vec3(0.3) : emission;

    // ��� baseColor û���ã�Ĭ�ϸ�һ�������ɫ
    vec3 colorToUse = (baseColor == vec3(0.0)) ? vec3(0.5, 0.5, 0.5) : baseColor;

     // �������
    vec4 texColor = texture(baseTexture, TexCoord);

    // ʹ�û�����ɫ��������л��
    vec3 color = colorToUse * texColor.rgb+emission;

     FragColor = vec4(color, 1.0f);

}
)";
/// <summary>
///ʵ����ƬԪө���
/// </summary>
const char* instanceNoLightingFireFlyFragmentShaderSource = R"(
#version 450 core

in vec3 FragPos;    // ����Ƭ��λ��
in vec3 Normal;     // ���뷨��
in vec2 TexCoord;   // ������������
flat in int instanceID; // ���ݵ�ʵ��ID

out vec4 FragColor; // �����ɫ

uniform vec3 baseColor;         // ģ�͹���ɫ��ȡֵ��Χ 0~1��
uniform vec3 emission;          // �Է��������ȡֵ��Χ 0~1������ʵ��������ͬ��
uniform sampler2D baseTexture;  // �������������

void main()
{
    // ������������
    vec4 texColor = texture(baseTexture, TexCoord);

    // ���� instanceID ����һ�����ƫ����
    // ������þ���Ĺ�ϣ������ʽ��sin(instanceID * ����) �ٳ���һ��������Ȼ��ȡС������
    float instanceOffset = fract(sin(float(instanceID) * 12.9898) * 43758.5453);

    // ʹ�����Һ���ʵ��ȫ����ӳ�䣬ȷ�����ʼ���� 0��1 ��Χ��
    // �������۴���� emission/baseColor ֵ��Σ����ܲ��������� 0��1 �����Ա仯
    vec3 modulatedEmission = 0.5 + 0.5 * sin(2.0 * 3.14159 * (emission + instanceOffset));
    vec3 modulatedBaseColor = 0.5 + 0.5 * sin(2.0 * 3.14159 * (baseColor + instanceOffset));

    // ��ϻ������������ɫ����ƺ���Է���ͻ�����ɫ
    vec3 finalColor = modulatedBaseColor * texColor.rgb + modulatedEmission;

    FragColor = vec4(finalColor, finalColor.r);
}

)";
/// <summary>
/// ʵ����ƬԪӫ���ɫ��
/// </summary>
const char* instanceNoLightingFireFlyColorFragmentShaderSource = R"(

#version 450 core

in vec3 FragPos;    
in vec3 Normal;     
in vec2 TexCoord;   
flat in int instanceID; 

out vec4 FragColor; 

uniform vec3 baseColor;         // ����Ļ�����ɫ��0~1���������� R ������Ϊ��λ����
uniform vec3 emission;          // ������Է��������0~1���������� R ������Ϊ��λ����
uniform sampler2D baseTexture;  // �������������

// ������������ HSV ת��Ϊ RGB
vec3 hsv2rgb(vec3 c) {
    vec3 rgb = clamp(abs(mod(c.x * 6.0 + vec3(0.0, 4.0, 2.0), 6.0) - 3.0) - 1.0, 0.0, 1.0);
    rgb = rgb * c.y + (1.0 - c.y);
    return rgb * c.z;
}

void main()
{
    // ������������
    vec4 texColor = texture(baseTexture, TexCoord);
    
    // ���� instanceID ���� 0��1 ֮������ƫ����
    float instanceOffset = fract(sin(float(instanceID) * 12.9898) * 43758.5453);
    
    // �����ʵ��Է�����ɫ���� emission.r �� instanceOffset �����Ϊ hue��ɫ�ࣩ�����Ͷ������Ⱦ���Ϊ 1
    float emissionHue = fract(emission.r + instanceOffset);
    vec3 modulatedEmission = hsv2rgb(vec3(emissionHue, 1.0, 1.0));
    
    // �����ʵĻ�����ɫ���� baseColor.r �� instanceOffset �����Ϊ hue��ͬ��ʹ�ñ��ͶȺ�����Ϊ 1
    float baseHue = fract(baseColor.r + instanceOffset);
    vec3 modulatedBaseColor = hsv2rgb(vec3(baseHue, 1.0, 1.0));
    
    // ��ϻ��������������ɫ����ƺ����ɫ����������ɫ
    vec3 finalColor = modulatedBaseColor * texColor.rgb + modulatedEmission;
    
    FragColor = vec4(finalColor, finalColor.r);
}

)";
/// <summary>
///ʵ������ɫ������ʹ���ӿ����ͼ���к�����Ե�ƬԪ����
/// </summary>
const char* instanceNoLightingForViewPortDepthMapFragmentShaderSource = R"(
#version 450 core

in vec3 FragPos;    // ����Ƭ��λ��
in vec3 Normal;     // ���뷨��
in vec2 TexCoord;   // ������������
in vec4 FragPosClip; // ����ü��ռ�����

out vec4 FragColor; // �����ɫ

uniform vec3 baseColor;      // ģ�͹���ɫ
uniform vec3 emission;      // �Է���
uniform sampler2D baseTexture;  // �������������
uniform sampler2D depthTexture;  // �������,����ʵ������ľʵ���ڵ���͸��

void main()
{
    // �Է���Ĭ��ֵ(��ѡ�߼�)
    vec3 emissionDefault = (emission == vec3(0.0)) ? vec3(0.3) : emission;

    // ��� baseColor û���ã�Ĭ�ϸ�һ�������ɫ
    vec3 colorToUse = (baseColor == vec3(0.0)) ? vec3(0.5, 0.5, 0.5) : baseColor;

    // �������
    vec4 texColor = texture(baseTexture, TexCoord);

    // ʹ�û�����ɫ��������л��
    vec3 color = colorToUse * texColor.rgb;

    // ���ü��ռ�����ת��Ϊ��Ļ�ռ�����
    vec3 fragPosScreen = FragPosClip.xyz / FragPosClip.w;  // ͸�ӳ���
    fragPosScreen = fragPosScreen * 0.5 + 0.5;  // ת���� [0, 1] ��Χ

    // �������ͼ
    float depthFromTexture = texture(depthTexture, fragPosScreen.xy).r;

    // �Ƚ����ֵ,ֻҪ�вɼ������
    if (fragPosScreen.z < depthFromTexture&&depthFromTexture<1.0f) {
        FragColor = vec4(color, 0.1f);  // �����ǰƬ����ǰ�棬����Ϊ��͸��
    } else {
        FragColor = vec4(color, 1.0);  // ����͸��
    }
}
)";
/// <summary>
/// ���˶�����ɫ�����������첨��Ч��
/// </summary>
const char* waveVertexShaderSource = R"(

#version 450 core
layout(location = 0) in vec3 aPos;       // ����λ��
layout(location = 1) in vec2 aTexCoord;  // ��������
layout(location = 2) in vec3 aNormal;    // ���㷨��

// �����Ƭ����ɫ��
out vec2 TexCoord;   // ������������
out vec3 FragPos;    // ����ռ��е�Ƭ��λ��
out vec3 Normal;     // ����ռ��еķ���
out vec3 ViewPos;    // ���λ��

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec2 textureScale; // ������������
uniform float waveTime;        // ʱ����������ڲ���Ч��
uniform float waveAmplitude; // ���˷���
uniform float waveFrequency; // ����Ƶ��
uniform float waveSpeed;     // �����ٶ�

void main()
{
    // ���㲨��Ч��
    vec3 waveOffset = vec3(0.0, sin(aPos.x * waveFrequency + waveTime * waveSpeed) * waveAmplitude, 0.0);
    vec4 worldPos = model * vec4(aPos + waveOffset, 1.0);
    FragPos = worldPos.xyz;

    // ����������Ӧ�ò���Ч��
    vec2 texWaveOffset = vec2(0.0, sin(aTexCoord.x * waveFrequency + waveTime * waveSpeed) * waveAmplitude * 0.1);
    TexCoord = (aTexCoord + texWaveOffset) * textureScale;

    // ��ȷ�任���ߣ���ֹ�Ǿ�����������
    Normal = mat3(transpose(inverse(model))) * aNormal;

    // �������λ�� (viewPos)����ͼ�����Ǵ�����ռ�ת��������ռ��
    ViewPos = -mat3(view) * vec3(view[3]);

    // ����������Ļ����
    gl_Position = projection * view * worldPos;
}


)";

/// <summary>
/// ģ����Դ�ɫ��ɫ��
/// </summary>
const char* stencilTestVertexShaderSource = R"(
#version 450 core
layout(location = 0) in vec3 aPos;   // ����λ��
layout(location = 1) in vec2 aTexCoord;   // �������� 
layout(location = 2) in vec3 aNormal;   // ����

//�����Ӧ��ʼ��ʱ�����õĶ�������
out vec3 FragPos;   // ���ݸ�Ƭ����ɫ����Ƭ��λ��

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
    out vec4 FragColor;  // �����ɫ
    
    void main() {
        FragColor = vec4(1,1,1,0.5F);  // ��ɫ��ɫ��
    }
)";
#endif