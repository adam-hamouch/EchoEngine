#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aNormal;  

out VS_OUT
{
	vec3 FragPos;
	vec2 TexCoord;
	vec3 Normal;
} vs_out;

uniform mat4 mvp;
uniform mat4 model;

void main()
{
    gl_Position = mvp * vec4(aPos, 1.0);
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));  
    vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;  
    vs_out.TexCoord = aUV;
}