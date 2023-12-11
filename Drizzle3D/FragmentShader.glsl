#version 330 core

in vec3 fragColor; // Input color from the vertex shader

out vec4 FragColor; // Output color to the framebuffer

struct Light {
    vec3 pos_or_dir;
    vec3 color;
    float intensity;

    vec3 ambient;
    vec3 specular;
    vec3 diffuse;

    bool isAmbient;
    bool isDiffuse;
    bool isSpecular;
    bool isCombined;

    bool isDirectional;
    bool isPoint;

    float constantPoint;
    float linearPoint;
    float quadraticPoint;

    float ID;
};

uniform Light lights[3]; // Assuming there are three lights, you can adjust the array size
uniform vec3 viewPos; // Camera/view position
uniform float materialShininess; // Shininess of the material
uniform vec3 normal; // Surface normal

void main() {
    // Define an output color value
    vec3 output = vec3(0.0);

    // Calculate lighting contributions for each light
    for (int i = 0; i < 3; ++i) {
        vec3 ambient = vec3(0.0);
        vec3 diffuse = vec3(0.0);
        vec3 specular = vec3(0.0);

        vec3 lightDir;
        if (lights[i].isDirectional) {
            lightDir = normalize(-lights[i].pos_or_dir);
        } else if (lights[i].isPoint) {
            lightDir = normalize(lights[i].pos_or_dir - gl_FragCoord.xyz);
        }

        if (lights[i].isAmbient) {
            ambient = lights[i].ambient * fragColor;
        }

        if (lights[i].isDiffuse) {
            float diff = max(dot(normalize(lightDir), normalize(normal)), 0.0);
            diffuse = lights[i].diffuse * diff * fragColor;
        }

        if (lights[i].isSpecular) {
            vec3 reflectDir = reflect(-lightDir, normalize(normal));
            float spec = pow(max(dot(normalize(viewPos - gl_FragCoord.xyz), reflectDir), 0.0), materialShininess);
            specular = lights[i].specular * spec;
        }

        output += ambient + diffuse + specular;
    }

    FragColor = vec4(output, 1.0);
}
