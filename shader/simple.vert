#version 300 es

layout(location = 0) in vec3 vertPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(vertPos, 1.0);
}