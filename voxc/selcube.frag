#version 460

in vec2 UV;

out vec4 outColor;

void main()
{
	outColor = vec4(0.0f,0.0f,0.0f,0.5
	f);
	if(UV.x > 0.05f && UV.x < 0.95f && UV.y > 0.05f && UV.y < 0.95f) discard;
}