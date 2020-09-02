#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightView;
uniform mat4 lightProjection;
//uniform mat3 normalMatrix;

out VS_OUT
{
    vec3 fragPos;
    vec2 texCoords;
    vec3 normal;
    vec4 fragPosLightSpace;
} vs_out;

void main()
{
    vs_out.fragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.texCoords = aTexCoords;
    // TODO: Fix the normal matrix here
//    normal = normalMatrix * aNormal;
    vs_out.normal = aNormal;
    vs_out.fragPosLightSpace = lightProjection * lightView * model * vec4(aPos, 1.0);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
