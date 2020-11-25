#version 450
#extension GL_ARB_separate_shader_objects : enable

// cube vertices
vec3 locs[32] = vec3[] (
    // +z
    vec3(0, 0, 1),
    vec3(1, 0, 1),
    vec3(1, 1, 1),
    vec3(0, 1, 1),

    // +x
    vec3(1, 0, 0),
    vec3(1, 1, 0),
    vec3(1, 1, 1),
    vec3(1, 0, 1),

    // -x
    vec3(0, 0, 0),
    vec3(0, 0, 1),
    vec3(0, 1, 1),
    vec3(0, 1, 0),

    // +y
    vec3(0, 1, 0),
    vec3(0, 1, 1),
    vec3(1, 1, 1),
    vec3(1, 1, 0),

    // -y
    vec3(0, 0, 0),
    vec3(1, 0, 0),
    vec3(1, 0, 1),
    vec3(0, 0, 1),

    // -z
    vec3(0, 0, 0),
    vec3(0, 1, 0),
    vec3(1, 1, 0),
    vec3(1, 0, 0),

    // diag 1a
    vec3(0, 0, 0),
    vec3(1, 1, 0),
    vec3(1, 1, 1),
    vec3(0, 0, 1),

    // diag 1b
    vec3(0, 0, 0),
    vec3(0, 0, 1),
    vec3(1, 1, 1),
    vec3(1, 1, 0)

);

// cube colors (not used)
vec3 clrs[32] = vec3[] (
    vec3(1.0f, 0.0f, 0.0f),
    vec3(0.0f, 1.0f, 0.0f),
    vec3(0.0f, 0.0f, 1.0f),
    vec3(1.0f, 1.0f, 1.0f),
    vec3(1.0f, 0.0f, 0.0f),
    vec3(0.0f, 1.0f, 0.0f),
    vec3(0.0f, 0.0f, 1.0f),
    vec3(1.0f, 1.0f, 1.0f),
    vec3(1.0f, 0.0f, 0.0f),
    vec3(0.0f, 1.0f, 0.0f),
    vec3(0.0f, 0.0f, 1.0f),
    vec3(1.0f, 1.0f, 1.0f),
    vec3(1.0f, 0.0f, 0.0f),
    vec3(0.0f, 1.0f, 0.0f),
    vec3(0.0f, 0.0f, 1.0f),
    vec3(1.0f, 1.0f, 1.0f),
    vec3(1.0f, 0.0f, 0.0f),
    vec3(0.0f, 1.0f, 0.0f),
    vec3(0.0f, 0.0f, 1.0f),
    vec3(1.0f, 1.0f, 1.0f),
    vec3(1.0f, 0.0f, 0.0f),
    vec3(0.0f, 1.0f, 0.0f),
    vec3(0.0f, 0.0f, 1.0f),
    vec3(1.0f, 1.0f, 1.0f),
    vec3(1.0f, 0.0f, 0.0f),
    vec3(0.0f, 1.0f, 0.0f),
    vec3(0.0f, 0.0f, 1.0f),
    vec3(1.0f, 1.0f, 1.0f),
    vec3(1.0f, 0.0f, 0.0f),
    vec3(0.0f, 1.0f, 0.0f),
    vec3(0.0f, 0.0f, 1.0f),
    vec3(1.0f, 1.0f, 1.0f)
);

// cube texture coords
// based on an 8x8 tiled image
// all in the first row of tiles
// six sides of cube (last two tiles in row no used)
vec2 texcrds[32] = vec2[] (

    vec2(0.125f, 0.0f),
    vec2(0.249f, 0.0f),
    vec2(0.249f, 0.124f),
    vec2(0.125f, 0.124f),

    vec2(0.25f, 0.0f),
    vec2(0.374f, 0.0f),
    vec2(0.374f, 0.124f),
    vec2(0.25f, 0.124f),

    vec2(0.0f, 0.0f),
    vec2(0.124f, 0.0f),
    vec2(0.124f, 0.124f),
    vec2(0.0f, 0.124f),

    vec2(0.625f, 0.0f),
    vec2(0.749f, 0.0f),
    vec2(0.749f, 0.124f),
    vec2(0.625f, 0.124f),

    vec2(0.5f, 0.0f),
    vec2(0.624f, 0.0f),
    vec2(0.624f, 0.124f),
    vec2(0.5f, 0.124f),

    vec2(0.375f, 0.0f),
    vec2(0.499f, 0.0f),
    vec2(0.499f, 0.124f),
    vec2(0.375f, 0.124f),

    vec2(0.75f, 0.124f),
    vec2(0.874f, 0.124f),
    vec2(0.874f, 0.0f),
    vec2(0.75f, 0.0f),

    vec2(0.75f, 0.124f),
    vec2(0.874f, 0.124f),
    vec2(0.874f, 0.0f),
    vec2(0.75f, 0.0f)
);

// normals for cube sides
vec3 normals[32] = vec3[] (
    vec3(0.0f, 0.0f, 1.0f),
    vec3(0.0f, 0.0f, 1.0f),
    vec3(0.0f, 0.0f, 1.0f),
    vec3(0.0f, 0.0f, 1.0f),
    vec3(1.0f, 0.0f, 0.0f),
    vec3(1.0f, 0.0f, 0.0f),
    vec3(1.0f, 0.0f, 0.0f),
    vec3(1.0f, 0.0f, 0.0f),
    vec3(-1.0f, 0.0f, 0.0f),
    vec3(-1.0f, 0.0f, 0.0f),
    vec3(-1.0f, 0.0f, 0.0f),
    vec3(-1.0f, 0.0f, 0.0f),
    vec3(0.0f, 1.0f, 0.0f),
    vec3(0.0f, 1.0f, 0.0f),
    vec3(0.0f, 1.0f, 0.0f),
    vec3(0.0f, 1.0f, 0.0f),
    vec3(0.0f, -1.0f, 0.0f),
    vec3(0.0f, -1.0f, 0.0f),
    vec3(0.0f, -1.0f, 0.0f),
    vec3(0.0f, -1.0f, 0.0f),
    vec3(0.0f, 0.0f, -1.0f),
    vec3(0.0f, 0.0f, -1.0f),
    vec3(0.0f, 0.0f, -1.0f),
    vec3(0.0f, 0.0f, -1.0f),
    vec3(1.0f, -1.0f, 0.0f),
    vec3(1.0f, -1.0f, 0.0f),
    vec3(1.0f, -1.0f, 0.0f),
    vec3(1.0f, -1.0f, 0.0f),
    vec3(-1.0f, 1.0f, 0.0f),
    vec3(-1.0f, 1.0f, 0.0f),
    vec3(-1.0f, 1.0f, 0.0f),
    vec3(-1.0f, 1.0f, 0.0f)
);

// model matrix
// view matrix
// projection matrix
// upos - user position
layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec4 upos;
} ubo;

// inputs
// inIndex = the index to the vertex in the arrys above
// inTrans = the translate params for the current vertex
//           the cubes can't be rotated or scaled - only translated
layout(location = 0) in uint inIndex;
layout(location = 1) in vec3 inTrans;

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
    gl_Position = ubo.proj * ubo.view * (ubo.model * translate1) * vec4(locs[inIndex], 1.0);

    // pass on to frag shader
    fragColor = clrs[inIndex];
    fragTexCoord = texcrds[inIndex];
    fragNormal = normals[inIndex];
    fragPos = vec3((ubo.model * translate1) * vec4(locs[inIndex], 1.0));
    viewPos = vec3(ubo.upos);

    // Transform the normal's orientation into eye space.
    eyeNormal = vec3(ubo.view * (ubo.model * translate1) * vec4(normals[inIndex], 0.0));

    // Transform the vertex into eye space.
    eyePosition = vec3(ubo.view * (ubo.model * translate1) * vec4(locs[inIndex], 1.0));

    // Transform eye light pos to eye space.
    eyeLightPos = vec3(ubo.view * (ubo.model * translate1) * vec4(lightPos, 1.0));

    fragLightPos = lightPos;
}