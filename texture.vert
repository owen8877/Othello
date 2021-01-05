#version 300 es

layout(location = 0) in vec3 vertPos;

uniform mat4 mvpMatrix;

void main() {
    gl_Position = mvpMatrix * vec4(vertPos, 1.0);
}