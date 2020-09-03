#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightView;
uniform mat4 lightProjection;

out vec3 Normal;
out vec2 TexCoords;
out vec3 WorldPos;
out vec4 FragPosLightSpace;

void main()
{
    Normal = mat3(model) * aNormal;
    TexCoords = aTexCoords;
    WorldPos = vec3(model * vec4(aPos, 1.0));
    FragPosLightSpace = lightProjection * lightView * vec4(WorldPos, 1.0);

    gl_Position = projection * view * vec4(WorldPos, 1.0);
}
