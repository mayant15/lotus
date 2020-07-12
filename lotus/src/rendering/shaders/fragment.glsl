#version 450 core

in vec3 fragPos;
in vec2 texCoords;
in vec3 normal;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
};

uniform Light light;
uniform Material material;
uniform sampler2D texture_diffuse1;
uniform float ambientStrength = 0.2;
uniform float specularStrength = 0.5;
uniform vec3 viewPos;

out vec4 fragColor;

void main()
{
    vec3 ambient = light.ambient * material.ambient;

    // diffuse lighting calculations
    // 1. Calculate light direction vector
    vec3 lightDir = normalize(fragPos - light.position);
    vec3 norm = normalize(normal);

    // 2. Intensity as dot product with the normal
    float diff = max(dot(norm, -lightDir), 0.0f);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // Specular lighting calculations
    // 1. Calculate the view vector
    vec3 viewDir = normalize(fragPos - viewPos);
    vec3 reflectDir = reflect(lightDir, norm);

    // 2. Specular component
    float spec = pow(max(dot(-viewDir, reflectDir), 0.0f), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);


    vec3 result = ambient + diffuse + specular;
    fragColor = vec4(result, 1.0f);
}
