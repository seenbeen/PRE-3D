#version 330 core

in vec3 iTangentViewPos;
in vec3 iTangentFragPos;
in vec3 iTangentLightPos;

in vec2 iTexCoord;
in vec2 iAccumCoord;

const float MATERIAL_SHININESS = 64.0f;

uniform int PRE_IS_FIRST_LIGHT_PASS;

uniform sampler2D PRE_LIGHT_ACCUMULATOR_SAMPLER;

uniform int PRE_AMBIENT_LIGHT_FLAG;
uniform int PRE_POINT_LIGHT_FLAG;
uniform int PRE_SPOT_LIGHT_FLAG;
uniform int PRE_DIRECTIONAL_LIGHT_FLAG;

uniform vec3 PRE_LIGHT_DIRECTION;
uniform vec3 PRE_LIGHT_COLOR;
uniform float PRE_LIGHT_LUMINOSITY;
uniform float PRE_LIGHT_SIZE;

uniform sampler2D diffuseSampler;
uniform sampler2D emissionSampler;
uniform sampler2D normalSampler;
uniform sampler2D specularSampler;

out vec4 FragColor;

void main()
{
	vec4 accumulatorColor = texture(PRE_LIGHT_ACCUMULATOR_SAMPLER, iAccumCoord) * (1 - PRE_IS_FIRST_LIGHT_PASS);

	vec4 diffuseColor = texture(diffuseSampler, iTexCoord);
	vec4 specularColor = texture(specularSampler, iTexCoord);
	vec4 emissionColor = texture(emissionSampler, iTexCoord);

	vec3 fragNormal = normalize(vec3(texture(normalSampler, iTexCoord)) * 2.0 - 1.0);
	vec3 viewDirection = normalize(iTangentViewPos - iTangentFragPos);

	// ambient light

	// point light
    vec3 pointLightDirection = normalize(iTangentLightPos - iTangentFragPos);
    float pointDiffuseIntensity = max(dot(fragNormal, pointLightDirection), 0.0f) *  PRE_LIGHT_LUMINOSITY;
    vec4 pointDiffuse = pointDiffuseIntensity * diffuseColor;

    vec3 reflectDirection = reflect(-pointLightDirection, fragNormal);
    vec4 pointSpecular = pow(max(dot(viewDirection, reflectDirection), 0.0), MATERIAL_SHININESS) * specularColor;

    vec4 pointLight = PRE_POINT_LIGHT_FLAG * (pointDiffuse + pointSpecular);

    // directional light

    // spot light

    vec4 litColor = (pointLight) * vec4(PRE_LIGHT_COLOR, 1.0f);

    FragColor = PRE_IS_FIRST_LIGHT_PASS * emissionColor + accumulatorColor + litColor;
}