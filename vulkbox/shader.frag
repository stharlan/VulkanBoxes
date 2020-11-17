#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragNormal;
layout(location = 3) in vec3 fragPos;
layout(location = 4) in vec3 viewPos;

layout(location = 0) out vec4 outColor;

// now this is an array
layout(binding = 1) uniform sampler2D texSampler[2];

void main() {
    //outColor = texture(texSampler[0], fragTexCoord);

    vec3 lightColor = { 1.0f, 1.0f, 1.0f };
    vec3 lightPos = { 100.0f, 100.0f, 100.0f };
    float specularStrength = 0.5;
    float ambientStrength = 0.1;

    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(lightPos - fragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 ambient = ambientStrength * lightColor;

    vec4 objectColor = texture(texSampler[0], fragTexCoord);

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, fragNormal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  

    outColor = vec4((ambient + diffuse + specular),1.0) * objectColor;
}