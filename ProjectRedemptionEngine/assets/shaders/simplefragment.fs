#version 330 core
out vec4 FragColor;

in vec3 iFragNorm;
in vec3 iFragPos;

in vec2 iTexCoord;
in vec2 iAccumCoord;

const float MATERIAL_SHININESS = 64.0f;

uniform int PRE_IS_FIRST_LIGHT_PASS;

uniform sampler2D PRE_LIGHT_ACCUMULATOR_SAMPLER;

uniform int PRE_AMBIENT_LIGHT_FLAG;
uniform int PRE_POINT_LIGHT_FLAG;
uniform int PRE_SPOT_LIGHT_FLAG;
uniform int PRE_DIRECTIONAL_LIGHT_FLAG;

uniform vec3 PRE_VIEW_POSITION;

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

	vec4 diffuseColor = texture(diffuseSampler, iTexCoord);
	vec4 specularColor = texture(specularSampler, iTexCoord);

	vec3 fragNormal = normalize(iFragNorm);
	vec3 viewDirection = normalize(PRE_VIEW_POSITION - iFragPos);

	// ambient light

	// point light
    vec3 pointLightDirection = normalize(PRE_LIGHT_POSITION - iFragPos);
    float pointDiffuseIntensity = max(dot(fragNormal, pointLightDirection), 0.0f) *  PRE_LIGHT_LUMINOSITY;
    vec4 pointDiffuse = pointDiffuseIntensity * diffuseColor;

    vec3 reflectDirection = reflect(-pointLightDirection, fragNormal);
    vec4 pointSpecular = pow(max(dot(viewDirection, reflectDirection), 0.0), MATERIAL_SHININESS) * specularColor;

    vec4 pointLight = PRE_POINT_LIGHT_FLAG * (pointDiffuse + pointSpecular);

    // directional light


    // spot light


    vec4 litColor = (pointLight) * vec4(PRE_LIGHT_COLOR, 1.0f);

    FragColor = accumulatorColor + litColor;
}