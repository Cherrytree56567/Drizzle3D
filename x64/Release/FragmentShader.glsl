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

void main() {
    vec3 ambient = vec3(0.0);
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    for (int i = 0; i < numLights; ++i) {
        vec3 lightDir;

        float attenuation = 1.0;
        if (lights[i].type == 0) {
            lightDir = normalize(-lights[i].direction);
        } else if (lights[i].type == 1) {
            lightDir = normalize(lights[i].position - FragPos);
            float distance = length(lights[i].position - FragPos);
            attenuation = 1.0 / (lights[i].constant + lights[i].linear * distance + lights[i].quadratic * (distance * distance));
        }

        float adjustedStrength = lights[i].strength * attenuation; // Adjust strength based on attenuation

        ambient += adjustedStrength * lights[i].color * lights[i].ambient;

        float diff = max(dot(norm, lightDir), 0.0);
        diffuse += diff * lights[i].diffuse * lights[i].color; // Remove attenuation from diffuse


        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        specular += attenuation * lights[i].SpecularStrength * spec * lights[i].specular * lights[i].color;
    }

    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0) * texture(textureSampler, texCoord);
}
