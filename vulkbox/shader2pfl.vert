#version 450
#extension GL_ARB_separate_shader_objects : enable

// model matrix
// view matrix
// projection matrix
// upos - user position
layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec4 upos;
    vec4 verts[40];
    vec4 texcs[40];
    vec4 norms[40];
} ubo;

// inputs
// inIndex = the index to the vertex in the arrys above
// inTrans = the translate params for the current vertex
//           the cubes can't be rotated or scaled - only translated
layout(location = 0) in uint inIndex;
layout(location = 1) in vec3 inTrans;
layout(location = 2) in float texOffset;

// the color from the colors array
layout(location = 0) out vec3 fragColor;
// the texture coordinates
layout(location = 1) out vec2 fragTexCoord;
// normal to the face
layout(location = 2) out vec3 fragNormal;
// transformed vertex
layout(location = 3) out vec3 fragPos;
// pass upos to the frag shader (untransformed)
layout(location = 4) out vec3 viewPos;

layout(location = 5) out vec3 eyeNormal;

layout(location = 6) out vec3 eyePosition;

layout(location = 7) out vec3 eyeLightPos;

layout(location = 8) out vec3 fragLightPos;

void main() {

    // light position (one static light)
    vec3 lightPos = { 100.0f, 100.0f, 100.0f };

    // translate the cube vertex using inTrans
    mat4 translate1 = mat4(1.0, 0.0, 0.0, 0.0, 
					       0.0, 1.0, 0.0, 0.0, 
					       0.0, 0.0, 1.0, 0.0,  
					       inTrans[0], inTrans[1], inTrans[2], 1.0);

    // compute position from proj/view/(model/translate)
    gl_Position = ubo.proj * ubo.view * (ubo.model * translate1) * ubo.verts[inIndex];

    // pass on to frag shader
    fragColor = vec3(1,1,1);
    fragTexCoord = vec2(ubo.texcs[inIndex]);
    fragTexCoord[1] += texOffset;
    fragNormal = vec3(ubo.norms[inIndex]);
    fragPos = vec3((ubo.model * translate1) * ubo.verts[inIndex]);
    viewPos = vec3(ubo.upos);

    // Transform the normal's orientation into eye space.
    eyeNormal = vec3(ubo.view * (ubo.model * translate1) * ubo.norms[inIndex]);

    // Transform the vertex into eye space.
    eyePosition = vec3(ubo.view * (ubo.model * translate1) * ubo.verts[inIndex]);

    // Transform eye light pos to eye space.
    eyeLightPos = vec3(ubo.view * (ubo.model * translate1) * vec4(lightPos, 1.0));

    fragLightPos = lightPos;
}