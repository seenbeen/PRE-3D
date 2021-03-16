#version 330 core
layout (location = 0) in vec3 iPos;
layout (location = 1) in vec3 iNorm;
layout (location = 2) in vec3 iTangent;
layout (location = 3) in vec3 iBiTangent;
layout (location = 4) in vec2 iUV;

uniform mat4 PRE_MODEL_MATRIX;
uniform mat4 PRE_VIEW_MATRIX;
uniform mat4 PRE_PROJECTION_MATRIX;

out vec3 iFragNorm;
out vec3 iFragPos;

out vec2 iTexCoord;
out vec2 iAccumCoord;

void main()
{
    gl_Position = PRE_PROJECTION_MATRIX * PRE_VIEW_MATRIX * PRE_MODEL_MATRIX * vec4(iPos, 1.0f);

    iFragNorm = mat3(transpose(inverse(PRE_MODEL_MATRIX))) * iNorm;
    iFragPos = vec3(PRE_MODEL_MATRIX * vec4(iPos, 1.0));

    iTexCoord = iUV;
    iAccumCoord = (gl_Position.xy / gl_Position.w + vec2(1.0f, 1.0f)) / 2.0f;
}