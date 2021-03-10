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

uniform mat4 PRE_MVP;

uniform mat4 PRE_BONETRANSFORMS[MAX_BONES];

out vec2 TexCoord;

void main()
{
    vec4 composedPos = vec4(iPos, 1.0f);//vec4(0.0f);
    vec3 composedNorm = vec3(0.0f);
    /*
    for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
    {
        if (iBoneIndices[i] == -1)
        {
            break;
        }
        if (iBoneIndices[i] >= MAX_BONES) 
        {
            composedPos = vec4(iPos, 1.0f);
            composedNorm = iNorm;
            break;
        }
        mat4 boneInfluence = PRE_BONETRANSFORMS[iBoneIndices[i]] * iBoneWeights[i];
        composedPos += boneInfluence * vec4(iPos, 1.0f);
	    composedNorm += mat3(boneInfluence) * iNorm;
   	}
   	*/
   	gl_Position = PRE_MVP * composedPos;
   	TexCoord = iUV;
}