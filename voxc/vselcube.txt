#version 460

layout (location=0) in vec3 position;
layout (location=1) in vec2 texc;
layout (location=2) in vec3 norm;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec2 UV;

void main()
{
	gl_Position = proj * view * model * vec4(position, 1.0f);
	UV = texc;
}