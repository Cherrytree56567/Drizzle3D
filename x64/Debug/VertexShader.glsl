#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;  // Added attribute for color

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out vec3 fragColor;  // Output color to fragment shader

void main() {
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0);
    fragColor = aColor;  // Pass color to fragment shader
}
