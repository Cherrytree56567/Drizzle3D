#version 330 core
#define MAX_LIGHTS 10

in vec2 texCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

struct Light {
    vec3 direction;
    vec3 position;
    vec3 color;
    float strength;
    float SpecularStrength;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    int type;
    int id;

    float constant;
    float linear;
    float quadratic;
};

uniform sampler2D textureSampler;
uniform vec3 viewPos;
uniform Light lights[MAX_LIGHTS];
uniform int numLights;
uniform bool IsSkyBox;
uniform float red;

void main() {
    FragColor = vec4(red, 1.0f, 1.0f, 1.0f);
}
