#include "ModelAnimation.hpp"

Bone::Bone(const std::string &name, unsigned int ID , const aiNodeAnim* channel):
mName(name),
mID(ID),
mLocalTransform(1.0f)
{
    mNumPosition = channel->mNumPositionKeys;
    mPositionFrames.reserve(mNumPosition);
    for (int i = 0; i < mNumPosition; ++i)
    {
        aiVector3D aiPos = channel->mPositionKeys[i].mValue;
        float timeStamp = channel->mPositionKeys[i].mTime;
        PositionFrame frame;
        frame.position = AssimpHelper::convertVec3ToGlm(aiPos);
        frame.timeStamp = timeStamp;
        mPositionFrames.push_back(frame);
    }

    mNumRotation = channel->mNumRotationKeys;
    mRotationFrames.reserve(mNumRotation);
    for (int i = 0; i < mNumRotation; ++i)
    {
        aiQuaternion mRotationKeys = channel->mRotationKeys[i].mValue;
        float timeStamp = channel->mRotationKeys[i].mTime;
        RotationFrame frame;
        frame.orient = AssimpHelper::convertQuatToGlm(mRotationKeys);
        frame.timeStamp = timeStamp;
        mRotationFrames.push_back(frame);
    }

    mNumScale = channel->mNumScalingKeys;
    mScaleFrames.reserve(mNumScale);
    for (int i = 0; i < mNumScale; ++i)
    {
        aiVector3D aiScale = channel->mScalingKeys[i].mValue;
        float timeStamp = channel->mScalingKeys[i].mTime;
        ScaleFrame frame;
        frame.scale = AssimpHelper::convertVec3ToGlm(aiScale);
        frame.timeStamp = timeStamp;
        mScaleFrames.push_back(frame);
    }
}

Bone::~Bone()
{
    mPositionFrames.clear();
    mPositionFrames.shrink_to_fit();
    mRotationFrames.clear();
    mRotationFrames.shrink_to_fit();
    mScaleFrames.clear();
    mScaleFrames.shrink_to_fit();
}

void Bone::update(float aniamtionTime)
{
    glm::mat4 translation = InterpolatePosition(aniamtionTime);
    glm::mat4 rotation = InterpolateRotation(aniamtionTime);
    glm::mat4 scale = InterpolateScale(aniamtionTime);
    mLocalTransform = translation * rotation * scale;
}

unsigned int Bone::getPositionIndex(float animationTime)
{
    for (int i = 0; i < mNumPosition - 1; ++i)
    {
        if (animationTime < mPositionFrames[i + 1].timeStamp)
            return i;
    }
    assert(0);
}

unsigned int Bone::getRotationIndex(float animationTime)
{
    for (int i = 0; i < mNumRotation - 1; ++i)
    {
        if (animationTime < mRotationFrames[i + 1].timeStamp)
            return i;
    }
    assert(0);
}

unsigned int Bone::getScaleIndex(float animationTime)
{
    for (int i = 0; i < mNumScale - 1; ++i)
    {
        if (animationTime < mScaleFrames[i + 1].timeStamp)
            return i;
    }
    assert(0);
}

float Bone::getFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
{
    float factor = 1.;
    float mid = animationTime - lastTimeStamp;
    float diff = nextTimeStamp - lastTimeStamp;
    factor = mid / diff;
    return factor;
}

glm::mat4 Bone::InterpolatePosition(float animationTime)
{
    if (mNumPosition == 1)
        return glm::translate(glm::mat4(1.), mPositionFrames[0].position);
    int idx0 = getPositionIndex(animationTime);
    int idx1 = idx0 + 1;
    float factor = getFactor(mPositionFrames[idx0].timeStamp, mPositionFrames[idx1].timeStamp, animationTime);
    glm::vec3 finalPos = glm::mix(mPositionFrames[idx0].position, mPositionFrames[idx1].position, factor);
    return glm::translate(glm::mat4(1.), finalPos);
}

glm::mat4 Bone::InterpolateRotation(float animationTime)
{
    if (mNumRotation == 1)
    {
        auto rotation = glm::normalize(mRotationFrames[0].orient);
        return glm::toMat4(rotation);
    }
        
    int idx0 = getRotationIndex(animationTime);
    int idx1 = idx0 + 1;
    float factor = getFactor(mRotationFrames[idx0].timeStamp, mRotationFrames[idx1].timeStamp, animationTime);
    glm::quat finalRotation = glm::slerp(mRotationFrames[idx0].orient, mRotationFrames[idx1].orient, factor);
    finalRotation = glm::normalize(finalRotation);
    return glm::toMat4(finalRotation);
}

glm::mat4 Bone::InterpolateScale(float animationTime)
{
    if (mNumScale == 1)
        return glm::scale(glm::mat4(1.), mScaleFrames[0].scale);
    int idx0 = getScaleIndex(animationTime);
    int idx1 = idx0 + 1;
    float factor = getFactor(mScaleFrames[idx0].timeStamp, mScaleFrames[idx1].timeStamp, animationTime);
    glm::vec3 finalScale = glm::mix(mScaleFrames[idx0].scale, mScaleFrames[idx1].scale, factor);
    return glm::scale(glm::mat4(1.), finalScale);
}


Animation::Animation(aiNode* rootNode, aiAnimation *animation, Model* model)
{
    mName = animation->mName.data;
    mDuration = animation->mDuration;
    mTicksPerSecond = animation->mTicksPerSecond;
    ReadHeirarchyData(mRootNode, rootNode);
    readMissingBones(animation, model);
}

Animation::~Animation()
{
    mBones.clear();
    mBones.shrink_to_fit();
}

Bone *Animation::findBone(const std::string &name)
{
	for (auto& bone : mBones)
	{
		if (bone.getBoneName() == name)
			return &bone;
	}
    return nullptr;
}


void Animation::readMissingBones(const aiAnimation *animation, Model *model)
{
    auto &boneInfoMap = model->getBoneMap();
    int boneId = boneInfoMap.size();
    mBones.reserve(animation->mNumChannels);
    for (int i = 0; i < animation->mNumChannels; ++i)
    {
        auto channel = animation->mChannels[i];
        std::string boneName = channel->mNodeName.data;
        if (boneInfoMap.find(boneName) == boneInfoMap.end())
        {
            boneInfoMap[boneName].boneID = boneId;
            boneId++;
        }
        mBones.push_back(Bone(boneName, boneInfoMap[boneName].boneID, channel));
    }
}

void Animation::ReadHeirarchyData(AssimpNodeData &dest, const aiNode *node)
{
    assert(node);
    dest.name = node->mName.data;
    dest.transform = AssimpHelper::convertMat4ToGlm(node->mTransformation);

    dest.children.reserve(node->mNumChildren);
    for (int i = 0; i < node->mNumChildren; ++i)
    {
        AssimpNodeData newData;
        ReadHeirarchyData(newData, node->mChildren[i]);
        dest.children.push_back(newData);
    }
}


Animator::Animator(const aiScene* scene, Model* model)
{
    mModel = model;
    mCurAnimation = nullptr;
    mCurrentTime = 0.f;
    mAnimations.reserve(scene->mNumAnimations);
    for (int i = 0; i < scene->mNumAnimations; ++i)
    {
        mAnimations.push_back(new Animation(scene->mRootNode, scene->mAnimations[i], model));
    }
    mFinalTransforms.reserve(MAX_BONES);
    for (int i = 0; i < MAX_BONES; ++i)
    {
        mFinalTransforms.push_back(glm::mat4(1.f));
    }
}

Animator::~Animator()
{
    mFinalTransforms.clear();
    mFinalTransforms.shrink_to_fit();
    mModel = nullptr;
    for (auto animation: mAnimations)
    {
        delete animation;
    }
    mAnimations.clear();
    mAnimations.shrink_to_fit();
	mCurAnimation = nullptr;
}

void Animator::updateAnimation(float dt)
{
    if (mCurAnimation)
    {
        mCurrentTime += mCurAnimation->getTicksPerSecond() * dt;
        mCurrentTime = fmod(mCurrentTime, mCurAnimation->getDuration());
        calcBoneTransform(&mCurAnimation->getRootNode(), glm::mat4(1.f));
    }
}

void Animator::playAnimation(const std::string& name)
{
	mCurrentTime = 0.;
	mCurAnimation = nullptr;
	for (auto anim : mAnimations)
	{
		if (anim->getName() == name)
		{
			mCurAnimation = anim;
			break;
		}
	}
}

void Animator::calcBoneTransform(const AssimpNodeData* node, const glm::mat4& parentTransform)
{
    std::string nodeName = node->name;
    glm::mat4 nodeTransform = node->transform;

    Bone *bone = mCurAnimation->findBone(nodeName);
    if (bone)
    {
        bone->update(mCurrentTime);
        nodeTransform = bone->getLoactionTransForm();
    }

    glm::mat4 globalTransform = parentTransform * nodeTransform;

    auto boneInfoMap = mModel->getBoneMap();
    if (boneInfoMap.find(nodeName) != boneInfoMap.end())
    {
        int boneId = boneInfoMap[nodeName].boneID;
        mFinalTransforms[boneId] = globalTransform * boneInfoMap[nodeName].mOffsetTransform;
    }

    for (auto &child : node->children)
    {
        calcBoneTransform(&child, globalTransform);
    }
}







