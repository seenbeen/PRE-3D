#version 330 core
layout (location = 0) in vec3 iPos;
layout (location = 1) in vec3 iNorm;
layout (location = 2) in vec2 iUV;
layout (location = 3) in ivec4 iBoneIndices;
layout (location = 4) in vec4 iBoneWeights;

uniform mat4 PRE_MVP;

out vec2 TexCoord;

void main()
{
    gl_Position = PRE_MVP * vec4(iPos, 1.0);
    TexCoord = iUV;
}