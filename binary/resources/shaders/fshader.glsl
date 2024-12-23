#version 330 core

struct Light {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float innerCutOff;
    float outerCutOff;
    float c0;
    float c1;
    float c2;
};

struct Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 emissive;
    float shininess;
};

struct Textures {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emissive;
    sampler2D normal;
    float shininess;
};

#define NLD 1
//#define NLP 1
#define NLF 6

uniform Light uLightG;
uniform Light uLightD[NLD];
//uniform Light uLightP[NLP];
uniform Light uLightF[NLF];
uniform Material uMaterial;
uniform Textures uTextures;
uniform bool uWithMaterials;
uniform bool uWithNormals;
uniform vec3 uEye;

in vec3 vNor;
in vec3 vPos;
in vec2 vTex;

out vec4 outColor;

vec3 funDirectional(Light light, Material material, vec3 N, vec3 V);
//vec3 funPositional(Light light, Material material, vec3 N, vec3 V);
vec3 funFocal(Light light, Material material, vec3 N, vec3 V);

void main() {
    vec3 N = normalize(vNor);
    vec3 V = normalize(uEye - vPos);
    
    Material material;
    
    if (uWithMaterials) {
        material.ambient = uMaterial.ambient;
        material.diffuse = uMaterial.diffuse;
        material.specular = uMaterial.specular;
        material.emissive = uMaterial.emissive;
        material.shininess = uMaterial.shininess;
    }else {
        material.ambient = texture(uTextures.diffuse , vTex) * 0.3;
        material.diffuse = texture(uTextures.diffuse , vTex);
        material.specular = texture(uTextures.specular, vTex);
        material.emissive = texture(uTextures.emissive, vTex);
        material.shininess = uTextures.shininess;
        if (uWithNormals)N = normalize(texture(uTextures.normal, vTex).rgb - 0.5);
    }
    
    vec3 color = material.emissive.rgb + uLightG.ambient * material.ambient.rgb;
    for(int i = 0; i < NLD; i ++ ) {
        color += funDirectional(uLightD[i], material, N, V);
    }
    /*
    for(int i = 0; i < NLP; i ++ ) {
        color += funPositional(uLightP[i], material, N, V);
    }
    */
    for(int i = 0; i < NLF; i ++ ) {
        color += funFocal(uLightF[i], material, N, V);
    }
    
    outColor = vec4(color, material.diffuse.a);
}

vec3 funDirectional(Light light, Material material, vec3 N, vec3 V) {
    vec3 L = normalize(-light.direction);
    vec3 R = normalize(reflect(-L, N));
    
    float dotLN = dot(L, N);
    float dotRV = 0.0;
    if (dotLN < 0.0) {
        dotLN = 0.0;
    }else {
        dotRV = max(dot(R, V), 0.0);
    }
    
    vec3 ambient = light.ambient * material.ambient.rgb;
    vec3 diffuse = light.diffuse * material.diffuse.rgb * dotLN;
    vec3 specular = light.specular * material.specular.rgb * pow(dotRV, material.shininess);
    
    vec3 color = ambient + diffuse + specular;
    
    return color;
}

/*
vec3 funPositional(Light light, Material material, vec3 N, vec3 V) {
    vec3 L = normalize(light.position - vPos);
    vec3 R = normalize(reflect(-L, N));
    
    float dotLN = dot(L, N);
    float dotRV = 0.0;
    if (dotLN < 0.0) {
        dotLN = 0.0;
    }else {
        dotRV = max(dot(R, V), 0.0);
    }
    
    vec3 ambient = light.ambient * material.ambient.rgb;
    vec3 diffuse = light.diffuse * material.diffuse.rgb * dotLN;
    vec3 specular = light.specular * material.specular.rgb * pow(dotRV, material.shininess);
    
    float distance = length(light.position - vPos);
    float attenuation = 1.0 / (light.c0 + light.c1 * distance + light.c2 * pow(distance, 2));
    
    vec3 color = attenuation * (ambient + diffuse + specular);
    
    return color;
}
*/

vec3 funFocal(Light light, Material material, vec3 N, vec3 V) {
    vec3 L = normalize(light.position - vPos);
    vec3 R = normalize(reflect(-L, N));
    
    float dotLN = dot(L, N);
    float dotRV = 0.0;
    if (dotLN < 0.0) {
        dotLN = 0.0;
    }else {
        dotRV = max(dot(R, V), 0.0);
    }
    
    vec3 ambient = light.ambient * material.ambient.rgb;
    vec3 diffuse = light.diffuse * material.diffuse.rgb * dotLN;
    vec3 specular = light.specular * material.specular.rgb * pow(dotRV, material.shininess);
    
    float distance = length(light.position - vPos);
    float attenuation = 1.0 / (light.c0 + light.c1 * distance + light.c2 * pow(distance, 2));
    
    vec3 direction = normalize(-light.direction);
    float intensity = clamp((light.outerCutOff - dot(L, direction)) / (light.outerCutOff - light.innerCutOff), 0.0, 1.0);
    
    vec3 color = attenuation * (ambient + intensity * (diffuse + specular));
    
    return color;
}
