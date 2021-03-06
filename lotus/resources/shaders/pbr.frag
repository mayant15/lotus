#version 450 core

#define M_PI 3.1415926535897932384626433832795
#define MAX_DIR_LIGHTS 10
#define MAX_POINT_LIGHTS 10
#define MAX_SPOT_LIGHTS 10

struct Material {
    vec3 vAlbedo;
    sampler2D tAlbedo;
    bool bUseAlbedoTex;

    sampler2D tNormal;
    bool bUseNormalTex;

    float fAO;
    sampler2D tAO;
    bool bUseAOTex;

    float fRoughness;
    float fMetallic;

};

struct DirectionalLight {
    vec4 color;
    vec3 direction;
};

struct Spotlight {
    vec4 color;
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
    vec4 color;
    float constant;
    float linear;
    float quadratic;
};

out vec4 fragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 WorldPos;
// in vec4 FragPosLightSpace;
in mat3 TBN;

// uniform sampler2D shadowMap;
uniform samplerCube irradianceMap;
uniform bool bUseIrradianceMap;

uniform vec3 camPos;

uniform DirectionalLight dirLight[MAX_DIR_LIGHTS];
uniform Spotlight spotlight[MAX_SPOT_LIGHTS];
uniform PointLight pointLight[MAX_POINT_LIGHTS];

uniform int numDirLight;
uniform int numSpotlight;
uniform int numPointLight;

uniform Material material;


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

    return max(nom, 0.001) / max(denom, 0.001);// prevent divide by zero for roughness=0.0 and NdotH=1.0
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
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    float NdotV = max(dot(N, V), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);

    return ggx1 * ggx2;
}


// Everything has Fresnel http://filmicworlds.com/blog/everything-has-fresnel/
// Specular reflections that sharpen when viewed at an angle. F0 depends on IOR and the metalness of the material.
vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

float getAO()
{
    return (material.bUseAOTex) ? texture(material.tAO, TexCoords).r : material.fAO;
}

vec3 getAlbedo()
{
    // Must tonemap this here for things to look good
    vec3 color = (material.bUseAlbedoTex) ? texture(material.tAlbedo, TexCoords).rgb : material.vAlbedo;
    return pow(color, vec3 (2.2));
}


// Solve the complete reflectance equation given a radiance
vec3 calculateIrradiance(vec3 radiance, vec3 N, vec3 L, vec3 V, vec3 F0)
{
    vec3 H = normalize(L + V);

    float D = DistributionGGX(N, H, material.fRoughness);
    float G = GeometrySmith(N, V, L, material.fRoughness);
    vec3 F = FresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);

    // Calculate Cook Torrance specular component
    float denom = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
    vec3 specular = D * G * F / max(denom, 0.001);// to avoid division by zero

    // Calculate diffuse and specular contributions
    vec3 ks = F;
    vec3 kd = vec3(1.0) - ks;
    kd = kd * (1.0 - material.fMetallic);// nullify the diffuse component if material is metallic

    vec3 albedo = getAlbedo();

    if (material.bUseNormalTex)
    {
        // NOTE: Sometimes I get pure white specular highlights when using normal maps
        // Clamping them for now to remove that effect;
        specular = clamp(specular, 0.0, 1.0) * albedo;
    }

    return ((kd * albedo / M_PI) + specular) * radiance * max(dot(N, L), 0.0);
}


// Check if the current fragment is in shadow
// float calculateShadow(vec3 N, vec3 L)
// {
//     // To [-1, 1]
//     vec3 projCoord = FragPosLightSpace.xyz / FragPosLightSpace.w;
//
//     // To [0, 1]
//     projCoord = projCoord * 0.5 + 0.5;
//
//     // For points outside the light frustum's far plane
//     if (projCoord.z > 1.0)
//     {
//         return 0.0;
//     }
//
//     float closestDepth = texture(shadowMap, projCoord.xy).r;
//     float currentDepth = projCoord.z;
//
//
//     // To treat shadow acne https://stackoverflow.com/questions/36908835/what-causes-shadow-acne
//     float bias = max(0.05 * (1.0 - dot(N, L)), 0.005);
//
//     // Percentage Closer Filtering
//     float shadow = 0.0;
//     vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
//     for (int x = -1; x <= 1; ++x)
//     {
//         for (int y = -1; y <= 1; ++y)
//         {
//             float pcfDepth = texture(shadowMap, projCoord.xy + vec2(x, y) * texelSize).r;
//             shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
//         }
//     }
//     shadow /= 9.0;
//
//     return shadow;
// }


vec3 calculatePointLight(PointLight light, vec3 N, vec3 V, vec3 F0)
{
    vec3 L = normalize(light.position - WorldPos);

    float distance = length(light.position - WorldPos);
    float attenuation = 1.0f / (distance * distance);
    vec3 radiance = (light.color * attenuation).rgb;

    return calculateIrradiance(radiance, N, L, V, F0);
}


vec3 calculateDirLight(DirectionalLight light, vec3 N, vec3 V, vec3 F0)
{
    vec3 L = normalize(-light.direction);
    vec3 H = normalize(L + V);
    vec3 radiance = light.color.rgb;

    return calculateIrradiance(radiance, N, L, V, F0);
}


vec3 calculateSpotlight(Spotlight light, vec3 N, vec3 V, vec3 F0)
{
    vec3 L = normalize(light.position - WorldPos);
    vec3 H = normalize(L + V);
    vec3 lightDir = normalize(light.direction);

    float cosTheta = dot(L, -lightDir);

    float angle1 = light.innerCutOff * M_PI / 180;
    float angle2 = light.outerCutOff * M_PI / 180;

    float diffCos = cos(angle1) - cos(angle2);
    float intensity = clamp((cosTheta - cos(angle2)) / diffCos, 0.0, 1.0);

    float distance = length(light.position - WorldPos);
    float attenuation = 1.0f / (distance * distance);

    vec3 radiance = (light.color * intensity * attenuation).rgb;

    return calculateIrradiance(radiance, N, L, V, F0);
}


vec3 getNormal()
{
    // obtain normal from normal map in range [0,1]
    if (material.bUseNormalTex)
    {
        vec3 normal = texture(material.tNormal, TexCoords).rgb;
        // transform normal vector to range [-1,1]
        normal = normal * 2.0 - 1.0;

        // NOTE: matrix multiplications in the frag shader are usually not a good idea.
        // See this for an alternative https://learnopengl.com/Advanced-Lighting/Normal-Mapping
        return TBN * normal;
    }
    else
    {
        return Normal;
    }
}

vec3 getIrradiance(vec3 N)
{
    return (bUseIrradianceMap) ? texture(irradianceMap, N).rgb : vec3(0.3f);
}

void main()
{
    vec3 N = normalize(getNormal());
    vec3 V = normalize(camPos - WorldPos);
    vec3 albedo = getAlbedo();

    // F0 for the Fresnel Schlick approximation that we're using is 0.04 for dielectrics
    // and tinted with the materials albedo for metals
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, material.fMetallic);

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

    // Ambient component
    vec3 kS = FresnelSchlickRoughness(max(dot(N, V), 0.0), F0, material.fRoughness);
    vec3 kD = 1.0 - kS;
    vec3 irradiance = getIrradiance(N);
    vec3 diffuse    = irradiance * albedo;
    vec3 ambient    = (kD * diffuse) * getAO();

    vec3 color = L0 + ambient;

    // HDR/Gamma correction
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0f / 2.2f));

    // TODO: Improve shadows
    // float shadow = calculateShadow(N, dirLight[0].direction);

    fragColor = vec4(color, 1.0f);
}
