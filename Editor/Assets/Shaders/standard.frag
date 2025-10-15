#version 460 core

out vec4 FragColor;

const float PI = 3.14159265359;

in VS_OUT
{
	vec3 FragPos;
	vec2 TexCoord;
	vec3 Normal;
} fs_in;

struct Material {
    vec3 albedo;
    float metallic;
    float roughness;
    float ao;
};

struct DirectionalLight 
{
    vec3 direction;
    vec3 color; 
    float intensity; 
};  
uniform DirectionalLight dirLights[10];
uniform int nbDirectionalLights;

struct PointLight  
{
    vec3 position;
    vec3 color; 
    float intensity; 
};  
uniform PointLight pointLights[10];
uniform int nbPointLights;

struct SpotLight  
{
    vec3 position;
    vec3 direction;
    vec3 color; 
    float spotAngle;
    float spotSmoothAngle;
    float intensity; 
};
uniform SpotLight spotLights[10];
uniform int nbSpotLights;

uniform Material material;
uniform vec3 viewPos;

uniform sampler2D albedoMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;
uniform sampler2D normalMap;

uniform bool hasAlbedoMap;
uniform bool hasMetallicMap;
uniform bool hasRoughnessMap;
uniform bool hasAOMap;
uniform bool hasNormalMap;

uniform bool useToonShading; 
uniform bool useOutline;

float toonShade(float NdotL)
{
    if (NdotL > 0.95)
        return 1.0;
    else if (NdotL > 0.5)
        return 0.7;
    else if (NdotL > 0.25)
        return 0.4;
    else
        return 0.1;
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}   

vec3 computeDirectionalLight(vec3 N, vec3 V, vec3 F0, DirectionalLight dLight, vec3 albedo, float roughness, float metallic)
{
    vec3 L = -normalize(dLight.direction); 
    vec3 H = normalize(V + L); 

    float NDF = DistributionGGX(N, H, roughness);   
    float G = GeometrySmith(N, V, L, roughness);      
    vec3 F = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);

    vec3 numerator = NDF * G * F; 
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;

    float NdotL = max(dot(N, L), 0.0);
    if (useToonShading)
        NdotL = toonShade(NdotL);

    return (kD * albedo / PI + specular) * dLight.color * dLight.intensity * NdotL;
}

vec3 computePointLight(vec3 N, vec3 V, vec3 F0, PointLight dLight, vec3 albedo, float roughness, float metallic)
{
    vec3 L = normalize(dLight.position - fs_in.FragPos); 
    vec3 H = normalize(V + L); 

    float distance = length(dLight.position - fs_in.FragPos);
    float attenuation = 1.0 / (distance * distance); 
    vec3 radiance = dLight.color * attenuation * dLight.intensity; 
        
    float NDF = DistributionGGX(N, H, roughness);   
    float G = GeometrySmith(N, V, L, roughness);      
    vec3 F = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);

    vec3 numerator = NDF * G * F; 
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;

    float NdotL = max(dot(N, L), 0.0);
    if (useToonShading)
        NdotL = toonShade(NdotL);

    return (kD * albedo / PI + specular) * radiance * NdotL;
}

vec3 computeSpotLight(vec3 N, vec3 V, vec3 F0, SpotLight dLight, vec3 albedo, float roughness, float metallic)
{
    vec3 L = normalize(dLight.position - fs_in.FragPos); 
    vec3 H = normalize(V + L);

    float distance = length(dLight.position - fs_in.FragPos);
    float attenuation = 1.0 / (distance * distance); 
    vec3 radiance = dLight.color * attenuation * dLight.intensity;

    float theta = dot(L, normalize(-dLight.direction));
    float epsilon = dLight.spotAngle - dLight.spotSmoothAngle;
    float intensity = clamp((theta - dLight.spotSmoothAngle) / epsilon, 0.0, 1.0);
    radiance *= intensity;

    float NDF = DistributionGGX(N, H, roughness);   
    float G = GeometrySmith(N, V, L, roughness);      
    vec3 F = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);

    vec3 numerator = NDF * G * F; 
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;

    float NdotL = max(dot(N, L), 0.0);
    if (useToonShading)
        NdotL = toonShade(NdotL);

    return (kD * albedo / PI + specular) * radiance * NdotL;
}

vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(normalMap, fs_in.TexCoord).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(fs_in.FragPos);
    vec3 Q2  = dFdy(fs_in.FragPos);
    vec2 st1 = dFdx(fs_in.TexCoord);
    vec2 st2 = dFdy(fs_in.TexCoord);

    vec3 N   = normalize(fs_in.Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

void main()
{    
    vec3 albedo = hasAlbedoMap ? pow(texture(albedoMap, fs_in.TexCoord).rgb, vec3(2.2)) : material.albedo;
    float metallic = hasMetallicMap ? texture(metallicMap, fs_in.TexCoord).r : material.metallic;
    float roughness = hasRoughnessMap ? texture(roughnessMap, fs_in.TexCoord).r : material.roughness;
    float ao = hasAOMap ? texture(aoMap, fs_in.TexCoord).r : material.ao;

    vec3 N = hasNormalMap ? getNormalFromMap() : normalize(fs_in.Normal);;
    vec3 V = normalize(viewPos - fs_in.FragPos);

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);

    for(int i = 0; i < nbDirectionalLights; i++)
        Lo += computeDirectionalLight(N, V, F0, dirLights[i], albedo, roughness, metallic);

    for(int i = 0; i < nbPointLights; i++)
        Lo += computePointLight(N, V, F0, pointLights[i], albedo, roughness, metallic);

    for(int i = 0; i < nbSpotLights; i++)
        Lo += computeSpotLight(N, V, F0, spotLights[i], albedo, roughness, metallic);

    vec3 ambient = vec3(0.03) * albedo * ao;    
    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2)); 

    FragColor = vec4(color, 1.0);
}