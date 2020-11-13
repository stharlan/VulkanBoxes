#version 450
#extension GL_ARB_separate_shader_objects : enable

vec3 locs[8] = vec3[] (
    vec3(-0.5, -0.5, 0.5),
    vec3(0.5, -0.5, 0.5),
    vec3(0.5, 0.5, 0.5),
    vec3(-0.5, 0.5, 0.5),
    vec3(-0.5, -0.5, -0.5),
    vec3(0.5, -0.5, -0.5),
    vec3(0.5, 0.5, -0.5),
    vec3(-0.5, 0.5, -0.5)
);

vec3 clrs[8] = vec3[] (
    vec3(1.0f, 0.0f, 0.0f),
    vec3(0.0f, 1.0f, 0.0f),
    vec3(0.0f, 0.0f, 1.0f),
    vec3(1.0f, 1.0f, 1.0f),
    vec3(1.0f, 0.0f, 0.0f),
    vec3(0.0f, 1.0f, 0.0f),
    vec3(0.0f, 0.0f, 1.0f),
    vec3(1.0f, 1.0f, 1.0f)
);

vec2 texcrds[8] = vec2[] (
    vec2(0.25f, 0.25f),
    vec2(0.49f, 0.25f),
    vec2(0.49f, 0.49f),
    vec2(0.25f, 0.49f),
    vec2(0.0f, 0.0f),
    vec2(1.0f, 0.0f),
    vec2(1.0f, 1.0f),
    vec2(0.0f, 1.0f)
);

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in uint inIndex;
layout(location = 1) in vec3 inTrans;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

void main() {

    mat4 translate1 = mat4(1.0, 0.0, 0.0, 0.0, 
					       0.0, 1.0, 0.0, 0.0, 
					       0.0, 0.0, 1.0, 0.0,  
					       inTrans[0], inTrans[1], inTrans[2], 1.0);
    gl_Position = ubo.proj * ubo.view * (ubo.model * translate1) * vec4(locs[inIndex], 1.0);
    fragColor = clrs[inIndex];
    fragTexCoord = texcrds[inIndex];
}