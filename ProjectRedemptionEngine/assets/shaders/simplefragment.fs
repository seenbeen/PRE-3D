#version 330 core
out vec4 FragColor;

in vec3 iFragNorm;
in vec3 iFragPos;

in vec2 iTexCoord;
in vec2 iAccumCoord;

uniform int PRE_IS_FIRST_LIGHT_PASS;

uniform sampler2D PRE_LIGHT_ACCUMULATOR_SAMPLER;

uniform int PRE_AMBIENT_LIGHT_FLAG;
uniform int PRE_POINT_LIGHT_FLAG;
uniform int PRE_SPOT_LIGHT_FLAG;
uniform int PRE_DIRECTIONAL_LIGHT_FLAG;

uniform vec3 PRE_LIGHT_POSITION;
uniform vec3 PRE_LIGHT_DIRECTION;
uniform vec3 PRE_LIGHT_COLOR;
uniform float PRE_LIGHT_LUMINOSITY;
uniform float PRE_LIGHT_SIZE;

uniform sampler2D diffuseSampler;
uniform sampler2D emissionSampler;
uniform sampler2D normalSampler;
uniform sampler2D specularSampler;

void main()
{
	vec4 accumulatorColor = texture(PRE_LIGHT_ACCUMULATOR_SAMPLER, iAccumCoord) * (1 - PRE_IS_FIRST_LIGHT_PASS);

	// point light
    vec3 pointDiffuseDirection = normalize(PRE_LIGHT_POSITION - iFragPos);
    float pointDiffuseIntensity = max(dot(iFragNorm, pointDiffuseDirection), 0.0f) *  PRE_LIGHT_LUMINOSITY;
    vec3 pointDiffuse = PRE_POINT_LIGHT_FLAG * pointDiffuseIntensity * PRE_LIGHT_COLOR;

    vec4 litColor = vec4(pointDiffuse, 1.0f) * texture(diffuseSampler, iTexCoord);

    FragColor = accumulatorColor + litColor;
}