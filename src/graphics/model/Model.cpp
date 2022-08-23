#include "Model.hpp"

Model::Model(string const &path, bool gamma) : 
    gammaCorrection(gamma),
    mAnimator(nullptr)
{
    mPath = path;
    this->loadModel();
}

void Model::update(float dt)
{
    mAnimator->updateAnimation(dt);
}

void Model::playAnimation(const std::string name)
{
    mAnimator->playAnimation(name);
}

void Model::Draw(Shader shader)
{   
    shader.use();
    if (mAnimator->isPlaying())
    {
        auto finalTransform = mAnimator->getFinalTranforms();
		shader.setMat4("finalTransforms", finalTransform[0], finalTransform.size());
        //for (int i = 0; i < finalTransform.size(); ++i)
        //    shader.setMat4("finalTransforms[" + std::to_string(i) + "]", finalTransform[i]);
    }
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        meshes[i]->Draw(shader);
    }
}

void Model::loadModel()
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(mPath, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
        return;
    }
    this->processNode(scene->mRootNode, scene);
    mAnimator = new Animator(scene, this);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    int boneId = -1;
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(loadMesh(mesh, scene, boneId));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh* Model::loadMesh(aiMesh *aiMesh, const aiScene *scene, int &boneID)
{
    Mesh* mesh = new Mesh();
    mesh->initWithAiMesh(aiMesh);

    for (unsigned int i = 0; i < aiMesh->mNumBones; i++)
    {
        auto bone = aiMesh->mBones[i];
        std::string name(bone->mName.data);

        if (mBoneMapping.find(name) == mBoneMapping.end())
        {
            boneID++;
            auto boneInfo = BoneInfo();
            boneInfo.mOffsetTransform = AssimpHelper::convertMat4ToGlm(bone->mOffsetMatrix);
            boneInfo.boneID = boneID;
            mBoneMapping[name] = boneInfo;
        }
        else
        {
            boneID = mBoneMapping[name].boneID;
        }

        for (unsigned int j = 0; j < bone->mNumWeights; j++)
        {
            mesh->addVertexWeight(bone->mWeights[j], boneID);
        }
    }

    aiMaterial *material = scene->mMaterials[aiMesh->mMaterialIndex];
	auto textures = mesh->getTextures();
    vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures->insert(textures->end(), diffuseMaps.begin(), diffuseMaps.end());

    vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures->insert(textures->end(), specularMaps.begin(), specularMaps.end());

    vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures->insert(textures->end(), normalMaps.begin(), normalMaps.end());

    vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures->insert(textures->end(), heightMaps.begin(), heightMaps.end());

	mesh->setupMesh();
    return mesh;
}

vector<Texture> Model::loadMaterialTextures(aiMaterial *material, aiTextureType type, string typeName)
{
	auto rpos = mPath.rfind("/");
	if (rpos != std::string::npos)
		rpos += 1;
	else
		rpos = mPath.length();
	auto pre_path = mPath.substr(0, rpos);
    vector<Texture> textures;
    for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
    {
        aiString str;
        material->GetTexture(type, i, &str);

        bool skip = false;
        for (unsigned int j = 0; j < texture_loads.size(); j++)
        {
            if (strcmp(texture_loads[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(texture_loads[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {
			auto tex_path = pre_path + str.C_Str();
            Texture texture;
            texture.id = FileUtils::getInstance()->loadTexture(tex_path);
            texture.type = typeName;
            texture.path = tex_path;
            textures.push_back(texture);
            texture_loads.push_back(texture);
        }
    }
    return textures;
}