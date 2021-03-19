#version 330 core

layout (location = 0) in vec3 iPos;
layout (location = 1) in vec3 iNorm;
layout (location = 2) in vec3 iTangent;
layout (location = 3) in vec3 iBiTangent;
layout (location = 4) in vec2 iUV;
layout (location = 5) in ivec4 iBoneIndices;
layout (location = 6) in vec4 iBoneWeights;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;

uniform mat4 PRE_MODEL_MATRIX;
uniform mat4 PRE_VIEW_MATRIX;
uniform mat4 PRE_PROJECTION_MATRIX;

uniform mat4 PRE_BONE_TRANSFORMS[MAX_BONES];

uniform vec3 PRE_VIEW_POSITION;
uniform vec3 PRE_LIGHT_POSITION;
uniform vec3 PRE_LIGHT_DIRECTION;

out vec3 iFragPos;

out vec3 iTangentViewPos;
out vec3 iTangentFragPos;
out vec3 iTangentLightPos;
out vec3 iTangentLightDirection;

out vec2 iTexCoord;

void main()
{
    mat3 normalMatrix = mat3(transpose(inverse(PRE_MODEL_MATRIX)));

    vec3 T = normalize(normalMatrix * iTangent);
    vec3 B = normalize(normalMatrix * iBiTangent);
    vec3 N = normalize(normalMatrix * iNorm);
    mat3 TBN = transpose(mat3(T, B, N));

    iTangentViewPos = TBN * PRE_VIEW_POSITION;
    iFragPos = vec3(PRE_MODEL_MATRIX * vec4(iPos, 1.0));
    iTangentFragPos = TBN * iFragPos;
    iTangentLightPos = TBN * PRE_LIGHT_POSITION;
    iTangentLightDirection = TBN * PRE_LIGHT_DIRECTION;

    gl_Position = PRE_PROJECTION_MATRIX * PRE_VIEW_MATRIX * PRE_MODEL_MATRIX * vec4(iPos, 1.0);

    iTexCoord = iUV;
}