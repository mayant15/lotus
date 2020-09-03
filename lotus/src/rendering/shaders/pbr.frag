#version 450 core

#define M_PI 3.1415926535897932384626433832795
#define MAX_DIR_LIGHTS 10
#define MAX_POINT_LIGHTS 10
#define MAX_SPOT_LIGHTS 10

struct DirectionalLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 direction;
};

struct Spotlight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
    vec3 direction;

    float innerCutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

out vec4 fragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 WorldPos;
in vec4 FragPosLightSpace;

uniform sampler2D shadowMap;

uniform vec3 camPos;

uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;

uniform DirectionalLight dirLight[MAX_DIR_LIGHTS];
uniform Spotlight spotlight[MAX_SPOT_LIGHTS];
uniform PointLight pointLight[MAX_POINT_LIGHTS];

uniform int numDirLight;
uniform int numSpotlight;
uniform int numPointLight;


// Implements a Trowbridge-Reitz GGX for Cook Torrance specular function
// Represents the probability of the microfacets in this fragment to be aligned with the halfway vector
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    // Turns out roughness^2 looks better
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = M_PI * denom * denom;

    return nom / max(denom, 0.001); // prevent divide by zero for roughness=0.0 and NdotH=1.0
}


// These two functions below simulate geometric self-occlusion on the surface, i.e. the effect of overlapping
// microfacets that shadow each other. Implements Smith's method with Schlick GGX
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

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


// Everything has Fresnel http://filmicworlds.com/blog/everything-has-fresnel/
// Specular reflections that sharpen when viewed at an angle. F0 depends on IOR and the metalness of the material.
vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}


float calculateShadow(vec3 N, vec3 L)
{
    // To [-1, 1]
    vec3 projCoord = FragPosLightSpace.xyz / FragPosLightSpace.w;

    // To [0, 1]
    projCoord = projCoord * 0.5 + 0.5;

    // For points outside the light frustum's far plane
    if(projCoord.z > 1.0)
    {
        return 0.0;
    }

    float closestDepth = texture(shadowMap, projCoord.xy).r;
    float currentDepth = projCoord.z;

    
    // To treat shadow acne https://stackoverflow.com/questions/36908835/what-causes-shadow-acne
    float bias = max(0.05 * (1.0 - dot(N, L)), 0.005);

    // Percentage Closer Filtering
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoord.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    return shadow;
}


vec3 calculatePointLight(PointLight light, vec3 N, vec3 V, vec3 F0)
{
    // Attenuation
    vec3 L = normalize(light.position - WorldPos);
    vec3 H = normalize(L + V);

    float distance = length(light.position - WorldPos);
    float attenuation = 1.0f / (distance * distance);
    vec3 radiance = light.diffuse * attenuation;
        
    float D = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    vec3 F = FresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);

    // Calculate Cook Torrance specular component
    float denom = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
    vec3 specular = D * F * G / max(denom, 0.001); // to avoid division by zero

    // Calculate diffuse and specular contributions
    vec3 ks = F;
    vec3 kd = vec3(1.0) - ks;
    kd = kd * (1.0 - metallic); // nullify the diffuse component if material is metallic

    return ((kd * albedo / M_PI) + specular) * radiance * max(dot(N, L), 0.0);
    // L0 += radiance * max(dot(N, L), 0.0);
}


vec3 calculateDirLight(DirectionalLight light, vec3 N, vec3 V, vec3 F0)
{
    vec3 L = normalize(-1.0 * light.direction);
    vec3 H = normalize(L + V);
    vec3 radiance = light.diffuse;

    float D = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    vec3 F = FresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);

    // Calculate Cook Torrance specular component
    float denom = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
    vec3 specular = D * F * G / max(denom, 0.001); // to avoid division by zero

    // Calculate diffuse and specular contributions
    vec3 ks = F;
    vec3 kd = vec3(1.0) - ks;
    kd = kd * (1.0 - metallic); // nullify the diffuse component if material is metallic

    return ((kd * albedo / M_PI) + specular) * radiance * max(dot(N, L), 0.0);
}


vec3 calculateSpotlight(Spotlight light, vec3 N, vec3 V, vec3 F0)
{
    vec3 L = normalize(light.position - WorldPos);
    vec3 H = normalize(L + V);
    vec3 lightDir = normalize(light.direction);
    
    float cosTheta = dot(L, -lightDir);
    float diffCos = cos(light.innerCutOff) - cos(light.outerCutOff);
    float intensity = clamp((cosTheta - cos(light.outerCutOff)) / diffCos, 0.0, 1.0);

    float distance = length(light.position - WorldPos);
    float attenuation = 1.0f / (distance * distance);
    // float linear = light.linear * distance;
    // float quadratic = light.quadratic * distance * distance;
    // float attenuation = 1.0f / (light.constant + linear + quadratic);

    vec3 radiance = light.diffuse * attenuation * intensity;

    float D = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    vec3 F = FresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);

    // Calculate Cook Torrance specular component
    float denom = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
    vec3 specular = D * F * G / max(denom, 0.001); // to avoid division by zero

    // Calculate diffuse and specular contributions
    vec3 ks = F;
    vec3 kd = vec3(1.0) - ks;
    kd = kd * (1.0 - metallic); // nullify the diffuse component if material is metallic

    return ((kd * albedo / M_PI) + specular) * radiance * max(dot(N, L), 0.0);
}


void main()
{
    vec3 N = normalize(Normal);
    vec3 V = normalize(camPos - WorldPos);

    // F0 for the Fresnel Schlick approximation that we're using is 0.04 for dielectrics
    // and tinted with the materials albedo for metals
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);
    
    vec3 L0 = vec3(0.0f);
    for (int i = 0; i < numPointLight; i++)
    {
        L0 += calculatePointLight(pointLight[i], N, V, F0);
    }

    for (int i = 0; i < numDirLight; i++)
    {
        L0 += calculateDirLight(dirLight[i], N, V, F0);
    }

    for (int i = 0; i < numSpotlight; i++)
    {
        L0 += calculateSpotlight(spotlight[i], N, V, F0);
    }

    // TODO: Raytraced GI?
    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + L0;

    // HDR/Gamma correction
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0f / 2.2f));

    // TODO: Improve shadows
    float shadow = calculateShadow(N, dirLight[0].direction);
    fragColor = vec4(color, 1.0f) * (1.0f - shadow);
}
