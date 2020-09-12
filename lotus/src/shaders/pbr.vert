#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightView;
uniform mat4 lightProjection;

out vec3 Normal;
out vec2 TexCoords;
out vec3 WorldPos;
out vec4 FragPosLightSpace;
out mat3 TBN;

void main()
{
    Normal = mat3(model) * aNormal;
    TexCoords = aTexCoords;
    WorldPos = vec3(model * vec4(aPos, 1.0));
    FragPosLightSpace = lightProjection * lightView * vec4(WorldPos, 1.0);

    vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
    // re-orthogonalize T with respect to N
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    TBN = mat3(T, B, N);

    gl_Position = projection * view * vec4(WorldPos, 1.0);
}
