#version 300 es

#ifdef GL_ES
precision highp float;
#endif

uniform vec4 uniformColor;

out vec4 fragColor;

void main(){
    fragColor = uniformColor;
}