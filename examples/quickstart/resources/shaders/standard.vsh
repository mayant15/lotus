#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

out vec3 fragPos;
out vec2 texCoords;
out vec3 normal;

void main()
{
    fragPos = vec3(model * vec4(aPos, 1.0));
    texCoords = aTexCoords;
    // TODO: Fix the normal matrix here
//    normal = normalMatrix * aNormal;
    normal = aNormal;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
