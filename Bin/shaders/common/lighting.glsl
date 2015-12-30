// constants
const int MAX_NR_LIGHTS = 16;
const vec3 COLOR_AMBIENT = vec3(0.03, 0.03, 0.03);
// const vec3 COLOR_AMBIENT = vec3(0.05, 0.05, 0.05);

struct Light {
    // properties
    vec3 Position;
    vec3 Diffuse;
    vec3 Specular;
    // attenuation
    float Linear;
};
uniform Light Lights[MAX_NR_LIGHTS];
uniform int NrActiveLights;

// calculated in world coordinates
float CalcDiffuse_L(vec3 FragPos, vec3 Normal, vec3 LightPos)
{
    vec3 lightDir = normalize(LightPos - FragPos);
    float diffuse = max(dot(lightDir, Normal), 0.0);
    return diffuse;
}

float CalcSpecular_BP(vec3 FragPos, vec3 Normal, vec3 LightPos, float exp)
{
    vec3 lightDir = normalize(LightPos - FragPos);
    vec3 viewDir = vec3(400.0f, 300.0f, -50.0f) - FragPos; // constant as camera has no specific position (project 3D -> 2D)
    vec3 halfwayDir = normalize(lightDir + viewDir);
    
    float spec = pow(max(dot(Normal, halfwayDir), 0.0), exp);
    return spec;
}

// attenuation
float CalcAttenuation(vec3 FragPos, vec3 LightPos, float Attenuation)
{
    float distance = length(FragPos - LightPos);
    float value = clamp(1.0f - distance / Attenuation, 0.0, 1.0f);
    value = value;
    value = sqrt(value);
    // return 1.0;
    return value;
}