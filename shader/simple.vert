#version 320 es

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 normal;
out vec3 fragPos;
out vec2 texCoords;

void main() {
    gl_Position = projection * view * model * vec4(aPosition, 1.0);
    normal = mat3(transpose(inverse(model))) * aNormal;
    fragPos = vec3(model * vec4(aPosition, 1.0));
    texCoords = aTexCoords;
}