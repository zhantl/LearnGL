#pragma once

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Mesh.hpp"
#include "AssimpHelper.hpp"
#include "../FileUtils.hpp"
#include "modelAnimation.hpp"

#define MAX_BONES 64

class Animator;

struct BoneInfo
{
	glm::mat4 mOffsetTransform;
	int boneID;
};

struct AssimpNodeData
{
	glm::mat4 transform;
	std::string name;
	std::vector<AssimpNodeData*> children;

	~AssimpNodeData()
	{
		for (auto node : children)
		{
			delete node;
		}
		children.clear();
		children.shrink_to_fit();
	}
};

class Model
{
public:
	Model(string const &path, bool gamma=false);
	~Model();
	void update(float dt);
	void Draw(Shader shader);
	void playAnimation(const std::string name);
	std::map<std::string, BoneInfo> &getBoneMap() { return mBoneMapping; }
	const AssimpNodeData* getRootNode() { return mRootNode; }
private:
	void loadModel();
	void processNode(const aiScene* scene, aiNode *node, AssimpNodeData* dest);
	Mesh* loadMesh(aiMesh *mesh, const aiScene *scene, int &boneID);
	vector<Texture> loadMaterialTextures(aiMaterial *material, aiTextureType type, string typeName);

	std::string mPath;
	vector<Texture> mTextures;
	vector<Mesh*> mMeshes;
	bool gammaCorrection;

	AssimpNodeData* mRootNode;
	std::map<std::string, BoneInfo> mBoneMapping;
	Animator* mAnimator;
};
