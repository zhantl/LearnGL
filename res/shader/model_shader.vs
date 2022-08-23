#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;
layout(location = 5) in ivec4 aBoneIDs;
layout(location = 6) in vec4 aBoneWeights;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;

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
	vec4 totalPosition = boneTransform * vec4(aPos, 1.f);
	Normal = mat3(transpose(inverse(model))) * aNormal;
	TexCoords = aTexCoord;
	FragPos = vec3(model * totalPosition);
	gl_Position = projection * view * model * totalPosition;
}