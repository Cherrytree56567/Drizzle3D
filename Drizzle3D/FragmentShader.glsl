#version 330 core

in vec3 fragColor;  // Input color from vertex shader
in vec3 fragPos;  // Input position from vertex shader
in vec3 fragNormal;  // Input normal from vertex shader

out vec4 FragColor;

const int MAX_LIGHTS = 300; // Maximum number of lights
struct Light {
    vec3 pos_or_dir;
    vec3 color;
    float intensity;

    vec3 ambient;
    vec3 specular;
    vec3 diffuse;

    bool isDirectional;
    bool isPoint;

    float constantPoint;
    float linearPoint;
    float quadraticPoint;

    float ID;
};

uniform Light lights[MAX_LIGHTS];
uniform int numLights; // Number of active lights

void main() {
    vec3 result = vec3(0.0);

    for (int i = 0; i < MAX_LIGHTS && i < numLights; ++i) {
        vec3 lightDir;
        float distance = 1.0;

        if (lights[i].isDirectional) {
            lightDir = normalize(-lights[i].pos_or_dir);
        } else {
            lightDir = normalize(lights[i].pos_or_dir - fragPos);
            distance = length(lights[i].pos_or_dir - fragPos);
        }

        // Ambient
        vec3 ambient = lights[i].ambient;

        // Diffuse
        vec3 norm = normalize(fragNormal);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lights[i].diffuse;

        // Specular
        float specularStrength = 0.5;
        vec3 viewDir = normalize(-fragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
        vec3 specular = specularStrength * spec * lights[i].specular;

        // Attenuation for point lights
        float attenuation = 1.0;
        if (lights[i].isPoint) {
            attenuation = 1.0 / (lights[i].constantPoint + lights[i].linearPoint * distance +
                                 lights[i].quadraticPoint * distance * distance);
        }

        result += attenuation * (ambient + diffuse + specular) * lights[i].color * lights[i].intensity;
    }

    FragColor = vec4(result * fragColor, 1.0);
}
