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

class BoneInfo
{
public:
	glm::mat4 mOffsetTransform;
	int boneID;
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
private:
	void loadModel();
	void processNode(aiNode *node, const aiScene *scene);
	Mesh* loadMesh(aiMesh *mesh, const aiScene *scene, int &boneID);
	vector<Texture> loadMaterialTextures(aiMaterial *material, aiTextureType type, string typeName);

	std::string mPath;
	std::map<std::string, BoneInfo> mBoneMapping;
	Animator *mAnimator;
	vector<Texture> texture_loads;
	vector<Mesh *> meshes;
	bool gammaCorrection;
};
