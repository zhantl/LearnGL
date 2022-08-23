#pragma once
#include <string>
#include <vector>
#include <map>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Shader.hpp"
#include "AssimpHelper.hpp"

using namespace std;

#define MAX_VETEX_BONES 4

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
	int BoneIDs[MAX_VETEX_BONES];
	float BoneWeights[MAX_VETEX_BONES];

	Vertex()
	{
		for (unsigned int i = 0; i < MAX_VETEX_BONES; i++)
		{
			BoneIDs[i] = 0;
			BoneWeights[i] = 0.;
		}
	}
};

struct Texture
{
	unsigned int id;
	string type;
	string path;
};

class Mesh
{
public:
	void initWithAiMesh(aiMesh *mesh);
	vector<Texture>* getTextures() { return &mTextures; }
	void Mesh::addVertexWeight(const aiVertexWeight &weight, unsigned int bondID);
	void Draw(Shader shader);
	void setupMesh();

private:
	//render data
	unsigned int VBO, EBO;
	//data
	vector<Vertex> mVertices;
	vector<unsigned int> mIndices;
	vector<Texture> mTextures;
	unsigned int VAO;
};
