#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <assimp/types.h>
#include <glm/gtx/quaternion.hpp>

class AssimpHelper
{
public:
	static glm::mat4 convertMat4ToGlm(const aiMatrix4x4 &from)
	{
		glm::mat4 to;
		//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
		to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
		to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
		to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
		to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
		return to;
	}

	static glm::vec2 convertVec2ToGlm(const aiVector3D& vec2)
	{
		return glm::vec2(vec2.x, vec2.y);
	}

	static glm::vec3 convertVec3ToGlm(const aiVector3D& vec3)
	{
		return glm::vec3(vec3.x, vec3.y, vec3.z);
	}

	static glm::quat convertQuatToGlm(const aiQuaternion& quat)
	{
		return glm::quat(quat.w, quat.x, quat.y, quat.z);
	}
};


