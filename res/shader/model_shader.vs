#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;
layout(location = 5) in ivec4 aBoneIDs;
layout(location = 6) in vec4 aBoneWeights;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
	vec3 Normal;
	mat3 TBN;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const int MAX_BONES = 64;
uniform mat4 finalTransforms[MAX_BONES];

void main()
{
	mat4 boneTransform = finalTransforms[aBoneIDs[0]] * aBoneWeights[0];
	    boneTransform += finalTransforms[aBoneIDs[1]] * aBoneWeights[1];
		boneTransform += finalTransforms[aBoneIDs[2]] * aBoneWeights[2];
		boneTransform += finalTransforms[aBoneIDs[3]] * aBoneWeights[3];
//	vec4 totalPosition = boneTransform * vec4(aPos, 1.f);
//	FragPos = vec3(model * totalPosition);
//	gl_Position = projection * view * model * totalPosition;


    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 B = normalize(normalMatrix * aBitangent);
    vec3 N = normalize(normalMatrix * aNormal);    
    
    mat3 TBN = mat3(T, B, N);  

    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));   
    vs_out.TexCoords = aTexCoord;
	vs_out.Normal = N;
	vs_out.TBN = TBN;

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}