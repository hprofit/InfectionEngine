/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit, Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

namespace fs = std::experimental::filesystem;

ResourceManager::ResourceManager(){}

ResourceManager::~ResourceManager() 
{
	UnloadAll();
}

bool ResourceManager::Init()
{
	//Loading Prefab Files
	std::string path = INFECT_GAME_CONFIG.PrefabsDir();
	for (auto &p : fs::directory_iterator(path)) {
		json* j = new json();
		*j = OpenJsonFile(p.path().string());
		std::string filename = p.path().filename().string();
		m_prefabs[filename] = j;
	}

	Plane* plane = new Plane();
	plane->FinishMesh();
	Scene* pScenePlane = new Scene(1);
	(*pScenePlane)[0] = plane;
	m_scenes[PLANE_PRIMITIVE] = pScenePlane;

	Plane* quad = new Plane(1);
	quad->FinishMesh();
	Scene* pSceneQuad = new Scene(1);
	(*pSceneQuad)[0] = quad;
	m_scenes[QUAD_PRIMITIVE] = pSceneQuad;

	Cube* pCube = new Cube();
	m_scenes[CUBE_PRIMITIVE] = pCube;

	Skybox* pSkyBox = new Skybox();
	m_scenes[SKYBOX_PRIMITIVE] = pSkyBox;

	Sphere* pSphere = new Sphere();
	pSphere->FinishMesh();
	Scene* pSceneSphere = new Scene(1);
	(*pSceneSphere)[0] = pSphere;
	m_scenes[SPHERE_PRIMITIVE] = pSceneSphere;

	Sphere* pLowPolySphere = new Sphere(10);
	pLowPolySphere->FinishMesh();
	Scene* pSceneLowPolySphere = new Scene(1);
	(*pSceneLowPolySphere)[0] = pLowPolySphere;
	m_scenes[SIMPLE_SPHERE_PRIMITIVE] = pSceneLowPolySphere;

	//PolarSphere* pPolarSphere = new PolarSphere();
	//pPolarSphere->FinishMesh();
	//Scene* pScenePolarSphere = new Scene(1);
	//(*pScenePolarSphere)[0] = pPolarSphere;
	//m_scenes[POLAR_SPHERE_PRIMITIVE] = pScenePolarSphere;




	Mesh* pMesh = new Mesh();

	pMesh->AddVertex(-1.0f, 1.0f, -1.0f, Color(1.0f, 0.0f, 0.0f, 1.0f));
	pMesh->AddVertex(1.0f, 1.0f, -1.0f, Color(0.0f, 1.0f, 0.0f, 1.0f));
	pMesh->AddVertex(-1.0f, -1.0f, -1.0f, Color(0.0f, 0.0f, 1.0f, 1.0f));
	pMesh->AddVertex(1.0f, -1.0f, -1.0f, Color(1.0f, 0.0f, 1.0f, 1.0f));
	pMesh->AddVertex(-1.0f, 1.0f, 1.0f, Color(0.0f, 1.0f, 1.0f, 1.0f));
	pMesh->AddVertex(1.0f, 1.0f, 1.0f, Color(1.0f, 0.0f, 1.0f, 1.0f));
	pMesh->AddVertex(-1.0f, -1.0f, 1.0f, Color(1.0f, 1.0f, 0.0f, 1.0f));
	pMesh->AddVertex(1.0f, -1.0f, 1.0f, Color(1.0f, 1.0f, 1.0f, 1.0f));

	pMesh->AddFace(0, 1, 2);
	pMesh->AddFace(2, 1, 3);
	pMesh->AddFace(4, 0, 6);
	pMesh->AddFace(6, 0, 2);
	pMesh->AddFace(7, 5, 6);
	pMesh->AddFace(6, 5, 4);
	pMesh->AddFace(3, 1, 7);
	pMesh->AddFace(7, 1, 5);
	pMesh->AddFace(4, 5, 0);
	pMesh->AddFace(0, 5, 1);
	pMesh->AddFace(3, 7, 2);
	pMesh->AddFace(2, 7, 6);

	pMesh->FinishMesh();
	Scene* pScene = new Scene(1);
	(*pScene)[0] = pMesh;
	m_scenes["test"] = pScene;


	return true;
}

#pragma region Mesh,Animation Data


//This is a recursive function to read the root node and all it's childern
void ReadRootNodeData(aiNode* aiRootNode, Node& RootNode, Matrix4x4 Transformation, Node* ParentNode)
{
	int i;
	RootNode.NodeName = aiRootNode->mName.C_Str();
	RootNode.Transformations = Transformation * (RootNode.Transformations);
	RootNode.ParentNode = ParentNode;

	RootNode.ChildNodeList.resize(aiRootNode->mNumChildren);

	for (i = 0; i < aiRootNode->mNumChildren; ++i)
	{
		//cout << RootNode.ChildNodeList.size() << " "<< RootNode.NodeName << endl;
		ReadRootNodeData(aiRootNode->mChildren[i], RootNode.ChildNodeList[i], RootNode.Transformations, &RootNode);

	}

}


Scene* ResourceManager::_LoadScene(const std::string& meshSceneName)
{
	const aiScene* scene = m_importer.ReadFile(INFECT_GAME_CONFIG.MeshesDir() + meshSceneName,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType
	);

	if (scene->HasMeshes()) {
		Scene* meshScene = new Scene(scene->mNumMeshes);
		m_scenes[meshSceneName] = meshScene;
		for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
			const aiMesh* aiMeshPtr = scene->mMeshes[i];
			m_meshes[aiMeshPtr->mName.C_Str()] = new Mesh(aiMeshPtr);
			(*meshScene)[i] = m_meshes[aiMeshPtr->mName.C_Str()];
		}

		if (scene->HasAnimations())
		{
			int i;
			Animations animScene(scene->mNumAnimations);

			//This is for the first time 
			//for the root node
			#pragma region Root Node Data
			animScene.m_RootNode.ParentNode = NULL;
			Matrix4x4 tempIdentityMatrix;
			animScene.m_RootNode.Transformations = tempIdentityMatrix.Identity4D();

			ReadRootNodeData(scene->mRootNode, animScene.m_RootNode, animScene.m_RootNode.Transformations, 0);
			#pragma endregion


			#pragma region Animation Data
			int Animation_List_size = animScene.AnimationList.size();
			for (i = 0; i < Animation_List_size; ++i)
			{

				animScene.AnimationList[i].Animation_Name = (scene->mAnimations[i]->mName.C_Str());
				animScene.AnimationList[i].Duration = (scene->mAnimations[i]->mDuration);
				animScene.AnimationList[i].TicksPerSecond = (scene->mAnimations[i]->mTicksPerSecond);

				int num_channels = scene->mAnimations[i]->mNumChannels;
				animScene.AnimationList[i].ChannelList.resize(num_channels);

				int j;

				for (j = 0; j < num_channels; ++j)
				{
					//Name of the node
					animScene.AnimationList[i].ChannelList[j].Name = scene->mAnimations[i]->mChannels[j]->mNodeName.C_Str();

					//Position of the node
					int num_positions = scene->mAnimations[i]->mChannels[i]->mNumPositionKeys;
					animScene.AnimationList[i].ChannelList[j].PositionList.resize(num_positions);

					for (int k = 0; k < num_positions; ++k)
					{
						animScene.AnimationList[i].ChannelList[j].PositionList[k].x = scene->mAnimations[i]->mChannels[j]->mPositionKeys->mValue.x;
						animScene.AnimationList[i].ChannelList[j].PositionList[k].y = scene->mAnimations[i]->mChannels[j]->mPositionKeys->mValue.y;
						animScene.AnimationList[i].ChannelList[j].PositionList[k].z = scene->mAnimations[i]->mChannels[j]->mPositionKeys->mValue.z;

					}

					//Quaternion Rotation values of the node

					int num_rotations = scene->mAnimations[i]->mChannels[i]->mNumRotationKeys;
					animScene.AnimationList[i].ChannelList[j].RotationList.resize(num_rotations);

					for (int k = 0; k < num_rotations; ++k)
					{
						animScene.AnimationList[i].ChannelList[j].RotationList[k].r = scene->mAnimations[i]->mChannels[j]->mRotationKeys->mValue.w;
						animScene.AnimationList[i].ChannelList[j].RotationList[k].i = scene->mAnimations[i]->mChannels[j]->mRotationKeys->mValue.x;
						animScene.AnimationList[i].ChannelList[j].RotationList[k].j = scene->mAnimations[i]->mChannels[j]->mRotationKeys->mValue.y;
						animScene.AnimationList[i].ChannelList[j].RotationList[k].k = scene->mAnimations[i]->mChannels[j]->mRotationKeys->mValue.z;

					}

					//Scale values for the node

					int num_scaling = scene->mAnimations[i]->mChannels[i]->mNumScalingKeys;
					animScene.AnimationList[i].ChannelList[j].UniformScaleList.resize(num_scaling);

					for (int k = 0; k < num_scaling; ++k)
					{
						animScene.AnimationList[i].ChannelList[j].UniformScaleList[k].x = scene->mAnimations[i]->mChannels[j]->mScalingKeys->mValue.x;
						animScene.AnimationList[i].ChannelList[j].UniformScaleList[k].y = scene->mAnimations[i]->mChannels[j]->mScalingKeys->mValue.y;
						animScene.AnimationList[i].ChannelList[j].UniformScaleList[k].z = scene->mAnimations[i]->mChannels[j]->mScalingKeys->mValue.z;

					}
				}

			}
			#pragma endregion



		}


		return meshScene;
	}
	else
		return nullptr;
}

Scene* ResourceManager::GetScene(const std::string& meshSceneName)
{
	Scene* meshScene = m_scenes[meshSceneName];

	if (meshScene)
		return meshScene;
	else
		return _LoadScene(meshSceneName);
}

void ResourceManager::UnloadMesh(const std::string& meshName)
{
	if (m_meshes[meshName]) {
		delete m_meshes[meshName];
		m_meshes.erase(meshName);
	}
}

#pragma endregion

ID3D11ShaderResourceView * ResourceManager::_LoadTexture(const std::string & textureName)
{
	ID3D11ShaderResourceView *pTexture;
	std::string filePath = INFECT_GAME_CONFIG.TexturesDir() + textureName;
	HRESULT result = DirectX::CreateWICTextureFromFile(
		INFECT_RENDERER.Device(),
		std::wstring(filePath.begin(), filePath.end()).c_str(),
		NULL, &pTexture
	);
	if (FAILED(result)) {
		MessageBox(NULL, (std::string("Failed to load texture: ") + filePath).c_str(), "Error", MB_OK);
		return nullptr;
	}

	m_textures[textureName] = pTexture;
	return pTexture;
}

ID3D11ShaderResourceView * ResourceManager::GetTexture(const std::string & textureName)
{
	ID3D11ShaderResourceView* pTexture = m_textures[textureName];

	if (pTexture)
		return pTexture;
	else
		return _LoadTexture(textureName);
}

void ResourceManager::UnloadTexture(const std::string & textureName)
{
	if (m_textures[textureName]) {
		delete m_textures[textureName];
		m_textures.erase(textureName);
	}
}

#pragma region Prefab

json* ResourceManager::GetPrefabFile(const std::string& prefabName) {
	json* file = m_prefabs[prefabName];
	if (file)
		return file;
	return nullptr;
}

#pragma endregion

#pragma region AI
void ConstructTree(BrainComponent* pAgent, BehaviorTree& bt, std::string filePath) {
	// hard coded tree generator for engine demo, TODO: make parser+treegenerator
	bt.m_pRootNode = new MoveToRandom(&bt);
	bt.m_pRootNode->SetAgent(pAgent);
	//bt.m_Scheduler.ScheduleBehavior(bt.m_pRootNode);
	// loop through each line and construct tree
}

BehaviorTree* ResourceManager::GetBehaviorTree(BrainComponent* pAgent, const std::string& treeFileName) {
	std::string filePath = INFECT_GAME_CONFIG.BehaviorTreeDir() + treeFileName;
	BehaviorTree* tree = new BehaviorTree();//static_cast<BehaviorTree*>(INFECT_MEMORY.Alloc(sizeof(BehaviorTree)));
	ConstructTree(pAgent, *tree,filePath);
	return tree;
}

#pragma endregion

void ResourceManager::UnloadAll()
{
	for (auto comp : m_meshes) {
		if (comp.second)
			delete comp.second;
	}
	m_meshes.clear();	
	
	for (auto comp : m_prefabs) {
		if (comp.second) {
			delete comp.second;
		}
	}
	m_prefabs.clear();
}
