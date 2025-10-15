#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 vp;
uniform float outlineThickness; 

void main()
{
    vec3 offset = aNormal * outlineThickness;
    vec4 worldPos = model * vec4(aPos + offset, 1.0);
    gl_Position = vp * worldPos;
}
