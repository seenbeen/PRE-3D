#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D diffuseSampler;
uniform sampler2D emissionSampler;
uniform sampler2D normalSampler;
uniform sampler2D specularSampler;

void main()
{
    FragColor = texture(diffuseSampler, TexCoord);
}