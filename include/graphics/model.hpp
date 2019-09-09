#pragma once

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

#include "mesh.hpp"
#include "fileUtils.hpp"

class Model
{
public:
	vector<Texture> texture_loads;
	vector<Mesh> meshes;
	bool gammaCorrection;

	Model(string const &path, bool gamma = false);
	void Draw(Shader shader);

private:
	void loadModel(string const &path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
};
