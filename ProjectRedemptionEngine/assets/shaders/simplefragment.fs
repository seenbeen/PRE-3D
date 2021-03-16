#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec2 AccumCoord;

uniform int PRE_IS_FIRST_LIGHT_PASS;

uniform sampler2D PRE_LIGHT_ACCUMULATOR_SAMPLER;

uniform sampler2D diffuseSampler;
uniform sampler2D emissionSampler;
uniform sampler2D normalSampler;
uniform sampler2D specularSampler;

void main()
{
	vec4 accumulatorColor = texture(PRE_LIGHT_ACCUMULATOR_SAMPLER, AccumCoord) * (1 - PRE_IS_FIRST_LIGHT_PASS);
    vec4 lightColor = texture(diffuseSampler, TexCoord);
    FragColor = accumulatorColor + lightColor;
}