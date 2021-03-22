#version 330 core

in vec3 iFragPos;
in vec4 iFragPosLightSpace;

in vec3 iTangentViewPos;
in vec3 iTangentFragPos;
in vec3 iTangentLightPos;
in vec3 iTangentLightDirection;

in vec2 iTexCoord;

const float MATERIAL_SHININESS = 32.0f;

uniform int PRE_IS_FIRST_LIGHT_PASS;

uniform vec2 PRE_LIGHT_ACCUMULATOR_SAMPLER_SIZE;
uniform sampler2D PRE_LIGHT_ACCUMULATOR_SAMPLER;

uniform sampler2D PRE_SHADOW_MAP_SAMPLER;
uniform samplerCube PRE_SHADOW_CUBE_MAP_SAMPLER;
uniform float PRE_SHADOW_MAP_FAR_CLIPPING_PLANE;

uniform int PRE_AMBIENT_LIGHT_FLAG;
uniform int PRE_POINT_LIGHT_FLAG;
uniform int PRE_SPOT_LIGHT_FLAG;
uniform int PRE_DIRECTIONAL_LIGHT_FLAG;

uniform vec3 PRE_LIGHT_POSITION;
uniform vec3 PRE_LIGHT_DIRECTION;
uniform vec3 PRE_LIGHT_COLOR;
uniform float PRE_LIGHT_ATTENUATION_LINEAR;
uniform float PRE_LIGHT_ATTENUATION_QUADRATIC;
uniform float PRE_LIGHT_INNER_RADIUS;
uniform float PRE_LIGHT_OUTER_RADIUS;

uniform sampler2D diffuseSampler;
uniform sampler2D emissionSampler;
uniform sampler2D normalSampler;
uniform sampler2D specularSampler;

out vec4 FragColor;

float ShadowCalculation2D(vec4 fragPosLightSpace)
{
    vec3 projCoords = (fragPosLightSpace.xyz / fragPosLightSpace.w) / 2.0f + 0.5f;
    float bias = 0.002f;

    float shadow = 0.0f;
    vec2 texelSize = 1.0f / textureSize(PRE_SHADOW_MAP_SAMPLER, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(PRE_SHADOW_MAP_SAMPLER, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += projCoords.z - bias > pcfDepth  ? 1.0f : 0.0f;
        }
    }
    shadow /= 9.0;

    if (projCoords.z > 1.0)
    {
		shadow = 0;
    }
    return 1.0f - shadow;
}

float ShadowCalculation3D(vec3 fragPos)
{
	vec3 fragToLight = fragPos - PRE_LIGHT_POSITION;

    float bias = 0.15f;

	float shadowDepth = texture(PRE_SHADOW_CUBE_MAP_SAMPLER, fragToLight).r * PRE_SHADOW_MAP_FAR_CLIPPING_PLANE;

    float shadow = 0.0f;

	if (length(fragToLight) - bias > shadowDepth)
	{
		shadow = 1.0f;
	}

	// TODO: Sampling for soft shadows

	return 1.0f - shadow;
}

void main()
{
	vec4 accumulatorColor = texture(PRE_LIGHT_ACCUMULATOR_SAMPLER, gl_FragCoord.xy / PRE_LIGHT_ACCUMULATOR_SAMPLER_SIZE) * (1 - PRE_IS_FIRST_LIGHT_PASS);

	vec4 diffuseColor = texture(diffuseSampler, iTexCoord);
	vec4 specularColor = texture(specularSampler, iTexCoord);
	vec4 emissionColor = texture(emissionSampler, iTexCoord);

	vec3 fragNormal = normalize(vec3(texture(normalSampler, iTexCoord)) * 2.0f - 1.0f);
	vec3 viewDirection = normalize(iTangentViewPos - iTangentFragPos);

	vec3 lightDelta = PRE_LIGHT_POSITION - iFragPos;
	float lightDistance = length(lightDelta);
	float attenuation = 1.0f / (1.0f + PRE_LIGHT_ATTENUATION_LINEAR * lightDistance + PRE_LIGHT_ATTENUATION_QUADRATIC * (lightDistance * lightDistance));

	// ambient light
	vec4 ambientDiffuse = emissionColor;
	vec4 ambientLight = PRE_AMBIENT_LIGHT_FLAG * ambientDiffuse * attenuation;

	// point light
    vec3 pointLightDirection = normalize(iTangentLightPos - iTangentFragPos);
    float pointDiffuseIntensity = max(dot(fragNormal, pointLightDirection), 0.0f);
    vec4 pointDiffuse = pointDiffuseIntensity * diffuseColor;

    vec3 reflectDirection = reflect(-pointLightDirection, fragNormal);
    vec4 pointSpecular = pow(max(dot(viewDirection, reflectDirection), 0.0f), MATERIAL_SHININESS) * specularColor;

    vec4 pointLightTotal = (pointDiffuse + pointSpecular) * attenuation;

    vec4 pointLight = PRE_POINT_LIGHT_FLAG * pointLightTotal;

    // spot light
    float spotLightTheta = dot(PRE_LIGHT_DIRECTION, normalize(-lightDelta));
    float spotLightEpsilon = PRE_LIGHT_INNER_RADIUS - PRE_LIGHT_OUTER_RADIUS;
    float spotLightIntensity = clamp((spotLightTheta - PRE_LIGHT_OUTER_RADIUS) / spotLightEpsilon, 0.0, 1.0);

    vec4 spotLight = PRE_SPOT_LIGHT_FLAG * pointLightTotal * spotLightIntensity;

    // directional light
    vec4 directionalDiffuse = max(dot(fragNormal, -iTangentLightDirection), 0.0f) * diffuseColor;
    vec3 directionalReflectDirection = reflect(iTangentLightDirection, fragNormal);
    vec4 directionalSpecular = pow(max(dot(viewDirection, directionalReflectDirection), 0.0f), MATERIAL_SHININESS) * specularColor;

    vec4 directionalLight = PRE_DIRECTIONAL_LIGHT_FLAG * (directionalDiffuse + directionalSpecular);

    vec4 litColor = (ambientLight + pointLight * ShadowCalculation3D(iFragPos) + (spotLight + directionalLight) * ShadowCalculation2D(iFragPosLightSpace)) * vec4(PRE_LIGHT_COLOR, 1.0f);

    FragColor = accumulatorColor + litColor;
}