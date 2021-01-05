#version 320 es

layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 out_color;
void main() {
    gl_Position = projection * view * model * vec4(vertPos, 1.0);
    out_color = color;
}