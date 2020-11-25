#version 450
#extension GL_ARB_separate_shader_objects : enable

// color from vshader
layout(location = 0) in vec3 fragColor;
// tex coord from vshader
layout(location = 1) in vec2 fragTexCoord;
// face normal from vshader
layout(location = 2) in vec3 fragNormal;
// transformed position from vshader
layout(location = 3) in vec3 fragPos;
// view position (untransformed)
layout(location = 4) in vec3 viewPos;

// the final color
layout(location = 0) out vec4 outColor;

// array of texture samplers
layout(binding = 1) uniform sampler2D texSampler[2];

void main() {

    // old code - just passes texture color
    //outColor = texture(texSampler[0], fragTexCoord);

    // light color
    vec3 lightColor = { 1, 1, 1 };

    // light position (one static light)
    vec3 lightPos = { 100.0f, 100.0f, 100.0f };

    // specular light strength 0 to 1
    float specularStrength = 0.5;

    // ambient light strength 0 to 1
    float ambientStrength = 0.5;

    // normalize the face normal
    vec3 norm = normalize(fragNormal);

    // get dir vector from model transformed pos to light pos
    vec3 lightDir = normalize(lightPos - fragPos);

    // calculate diffuse factor using the face normal and light direction
    // and multiply by the light color to get a final diffuse color
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // calculate the ambient light color from the ambient strength
    vec3 ambient = ambientStrength * lightColor;

    // get the color from the texture
    vec4 objectColor = texture(texSampler[0], fragTexCoord);

    // get direction vector from frag to viewer
    vec3 viewDir = normalize(viewPos - fragPos);

    // reflect the opposite of the light dir
    // (light to frag) across the face normal
    vec3 reflectDir = reflect(-lightDir, fragNormal);  

    // use the reflection dir to calculate the specular factor
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    // use the specular factor and spec strength to calulate the specular light
    vec3 specular = specularStrength * spec * lightColor;  

    // calculate the final color
    outColor = vec4((ambient + diffuse + specular),1.0f) * objectColor;
}