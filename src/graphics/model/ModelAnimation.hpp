#pragma once
#include <string>
#include <vector>
#include <map>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "AssimpHelper.hpp"
#include "Model.hpp"

using namespace std;

class Model;

struct PositionFrame
{
	glm::vec3 position;
	float timeStamp;
};

struct RotationFrame
{
	glm::quat orient;
	float timeStamp;
};

struct ScaleFrame
{
	glm::vec3 scale;
	float timeStamp;
};

class Bone
{
public:
	Bone(const std::string &name, unsigned int ID, const aiNodeAnim* channel);
	~Bone();
	void update(float aniamtionTime);
	glm::mat4 getLoactionTransForm() { return mLocalTransform; }
	std::string getBoneName() { return mName; }
	unsigned int getBoneID() { return mID; }

	unsigned int getPositionIndex(float animationTime);
	unsigned int getRotationIndex(float animationTime);
	unsigned int getScaleIndex(float animationTime);

private:
	float getFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);
	glm::mat4 InterpolatePosition(float animationTime);
	glm::mat4 InterpolateRotation(float animationTime);
	glm::mat4 InterpolateScale(float animationTime);

	vector<PositionFrame> mPositionFrames;
	vector<RotationFrame> mRotationFrames;
	vector<ScaleFrame> mScaleFrames;
	unsigned int mNumPosition;
	unsigned int mNumRotation;
	unsigned int mNumScale;

	glm::mat4 mLocalTransform;
	std::string mName;
	unsigned int mID;
};

struct AssimpNodeData
{
	glm::mat4 transform;
	std::string name;
	std::vector<AssimpNodeData> children;

	~AssimpNodeData()
	{
		children.clear();
		children.shrink_to_fit();
	}
};

class Animation
{
public:
	Animation() = default;
	~Animation();
	Animation(aiNode* rootNode, aiAnimation *animation, Model *model);
	Bone *findBone(const std::string &name);
	const std::string& getName() { return mName; }
	float getTicksPerSecond() { return mTicksPerSecond; }
	float getDuration() { return mDuration; }
	const AssimpNodeData &getRootNode() { return mRootNode; }

private:
	void readMissingBones(const aiAnimation *animation, Model *model);
	void ReadHeirarchyData(AssimpNodeData &dest, const aiNode *node);

	std::string mName;
	float mDuration;
	unsigned int mTicksPerSecond;
	std::vector<Bone> mBones;
	AssimpNodeData mRootNode;
};

class Animator
{
public:
	~Animator();
	Animator(const aiScene* scene, Model* model);
	void updateAnimation(float dt);
	void playAnimation(const std::string &name);
	void calcBoneTransform(const AssimpNodeData* node, const glm::mat4& parentTransform);
	const vector<glm::mat4>& getFinalTranforms() { return mFinalTransforms; }
	bool isPlaying() { return mCurAnimation != nullptr; }
private:
	vector<glm::mat4> mFinalTransforms;
	vector<Animation*> mAnimations;
	Animation *mCurAnimation;
	Model *mModel;
	float mCurrentTime;
};
