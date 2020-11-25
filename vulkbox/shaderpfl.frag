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
layout(location = 5) in vec3 eyeNormal;
layout(location = 6) in vec3 eyePosition;
layout(location = 7) in vec3 eyeLightPos;
layout(location = 8) in vec3 fragLightPos;

// the final color
layout(location = 0) out vec4 outColor;

// array of texture samplers
layout(binding = 1) uniform sampler2D texSampler[2];

void main() {

    // old code - just passes texture color
    //outColor = texture(texSampler[0], fragTexCoord);

    // light color
    vec3 lightColor = { 1, 1, 1 };

    //** BEGIN new code for per frag shading

    // Will be used for attenuation.
    float distance = length(eyeLightPos - eyePosition);

    // Get a lighting direction vector from the light to the vertex.
    //vec3 lightVector = normalize(eyeLightPos - eyePosition);
    // make this a constant direction
    vec3 lightVector = normalize(eyeLightPos);

    // Calculate the dot product of the light vector and vertex normal. If the normal and light vector are
    // pointing in the same direction then it will get max illumination.
    float diffuseStrength = max(dot(eyeNormal, lightVector), 0.1);

    // Add attenuation.
    //vec3 diffuse = diffuseStrength * (1.0 / (1.0 + (0.25 * distance * distance))) * lightColor;
    vec3 diffuse = diffuseStrength * lightColor;

    //** END new code for per frag shading

    // specular light strength 0 to 1
    float specularStrength = 0.5;

    // ambient light strength 0 to 1
    float ambientStrength = 0.2;

    // normalize the face normal
    //vec3 norm = normalize(fragNormal);

    // get dir vector from model transformed pos to light pos
    vec3 lightDir = normalize(fragLightPos - fragPos);

    // calculate diffuse factor using the face normal and light direction
    // and multiply by the light color to get a final diffuse color
    //float diff = max(dot(norm, lightDir), 0.0);
    //vec3 diffuse = diff * lightColor;

    // calculate the ambient light color from the ambient strength
    vec3 ambient = ambientStrength * lightColor;

    // get the color from the texture
    vec4 objectColor = texture(texSampler[0], fragTexCoord);

    // get direction vector from frag to viewer
    //vec3 viewDir = normalize(viewPos - fragPos);

    // reflect the opposite of the light dir
    // (light to frag) across the face normal
    //vec3 reflectDir = reflect(-lightDir, fragNormal);  

    // use the reflection dir to calculate the specular factor
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    // use the specular factor and spec strength to calulate the specular light
    //vec3 specular = specularStrength * spec * lightColor;  

    // calculate the final color
    //outColor = vec4((ambient + diffuse + specular),1.0f) * objectColor;
    outColor = vec4((ambient + diffuse),1.0f) * objectColor;
}