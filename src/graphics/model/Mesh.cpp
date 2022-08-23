#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "Mesh.hpp"

void Mesh::initWithAiMesh(aiMesh *mesh)
{
    mVertices.reserve(mesh->mNumVertices);
    
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        vertex.Position = AssimpHelper::convertVec3ToGlm(mesh->mVertices[i]);
        vertex.Normal = AssimpHelper::convertVec3ToGlm(mesh->mNormals[i]);
        if (mesh->mTextureCoords[0])
            vertex.TexCoords = AssimpHelper::convertVec2ToGlm(mesh->mTextureCoords[0][i]);
        else
            vertex.TexCoords = glm::vec2(0.0, 0.0);
        vertex.Tangent = AssimpHelper::convertVec3ToGlm(mesh->mTangents[i]);
        vertex.Bitangent = AssimpHelper::convertVec3ToGlm(mesh->mBitangents[i]);
        mVertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            mIndices.push_back(face.mIndices[j]);
        }
    }
}

void Mesh::addVertexWeight(const aiVertexWeight &weight, unsigned int bondID)
{
    auto vid = weight.mVertexId;
    for (unsigned int i = 0; i < MAX_VETEX_BONES; i++)
    {
        if (mVertices[vid].BoneWeights[i] == 0.0)
        {
			mVertices[vid].BoneWeights[i] = weight.mWeight;
			mVertices[vid].BoneIDs[i] = bondID;
            return;
        }
    }
}

void Mesh::Draw(Shader shader)
{
	shader.use();
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;

    for (unsigned int i = 0; i < mTextures.size(); i++)
    {
        string number;
        string name = mTextures[i].type;
        if (name == "texture_diffuse")
            number = to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = to_string(specularNr++);
        else if (name == "texture_normal")
            number = to_string(normalNr++);
        else if (name == "texture_heigh")
            number = to_string(heightNr++);

        glActiveTexture(GL_TEXTURE0 + i);
        shader.setInt(name + number, i);
        glBindTexture(GL_TEXTURE_2D, mTextures[i].id);
    }

    glBindVertexArray(VAO);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mVertices.size(), &mVertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mIndices.size(), &mIndices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Tangent));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Bitangent));
	glEnableVertexAttribArray(5);
	glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void *)offsetof(Vertex, BoneIDs));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, BoneWeights));

    glBindVertexArray(0);
}