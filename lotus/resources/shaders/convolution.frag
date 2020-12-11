#version 450 core

#define M_PI 3.1415926535897932384626433832795

out vec4 fragColor;
in vec3 localPos;
uniform samplerCube envMap;

void main()
{
    // the sample direction equals the hemisphere's orientation 
    vec3 normal = normalize(localPos);
    vec3 irradiance = vec3(0.0f);

    // Convolution
    vec3 up    = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(up, normal);
    up = cross(normal, right);

    float sampleDelta = 0.025;
    float nrSamples = 0.0;
    for (float phi = 0.0; phi < 2.0 * M_PI; phi += sampleDelta)
    {
        for (float theta = 0.0; theta < 0.5 * M_PI; theta += sampleDelta)
        {
            vec3 tangentSample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;
            irradiance += texture(envMap, sampleVec).rgb * cos(theta) * sin(theta);
            nrSamples++;
        }
    }
    irradiance = M_PI * irradiance * (1.0 / float(nrSamples));

    fragColor = vec4(irradiance, 1.0);
}