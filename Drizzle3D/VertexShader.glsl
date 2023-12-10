#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor; // Color attribute

out vec3 fragColor; // Output color to fragment shader

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main() {
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0);
    fragColor = aColor; // Pass color to fragment shader
}
