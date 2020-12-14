#version 460

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texc;
layout(location = 2) in vec3 norm;

layout(binding = 0) uniform UniformBufferObject {
	mat4 model;
	mat4 view;
	mat4 proj;
};

layout(location = 0) out vec2 UV;

void main()
{
	gl_Position = proj * view * model * vec4(position, 1.0f);
	UV = texc;
}
