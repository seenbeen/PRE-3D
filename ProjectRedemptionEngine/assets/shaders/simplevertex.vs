#version 330 core
layout (location = 0) in vec3 iPos;
layout (location = 1) in vec3 iNorm;
layout (location = 2) in vec3 iTangent;
layout (location = 3) in vec3 iBiTangent;
layout (location = 4) in vec2 iUV;
layout (location = 5) in ivec4 iBoneIndices;
layout (location = 6) in vec4 iBoneWeights;

uniform mat4 PRE_MODEL_VIEW_MATRIX;
uniform mat4 PRE_PROJECTION_MATRIX;

uniform mat4 PRE_BONETRANSFORMS[MAX_BONES];

uniform int PRE_AMBIENT_LIGHT_FLAG;
uniform int PRE_POINT_LIGHT_FLAG;
uniform int PRE_SPOT_LIGHT_FLAG;
uniform int PRE_DIRECTIONAL_LIGHT_FLAG;

uniform vec3 PRE_LIGHT_POSITION;
uniform vec3 PRE_LIGHT_DIRECTION;
uniform vec3 PRE_LIGHT_COLOR;
uniform float PRE_LIGHT_LUMINOSITY;
uniform float PRE_LIGHT_SIZE;

out vec2 TexCoord;

void main()
{
    gl_Position = PRE_PROJECTION_MATRIX * PRE_MODEL_VIEW_MATRIX * vec4(iPos, 1.0);
    TexCoord = iUV;
}