/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef MESH_H
#define MESH_H

struct Vertex {
	FLOAT x, y, z;		// position
	FLOAT nX, nY, nZ;	// normal
	FLOAT tX, tY, tZ;	// tangent
	FLOAT bX, bY, bZ;	// bitangent
	FLOAT u, v;			// texture coords
	FLOAT r, g, b, a;	// color
	UINT  VertexID;
};

class Material {
protected:
	const aiMaterial * m_pMaterial;

public:
	Material(const aiMaterial * pAiMaterial) : m_pMaterial(pAiMaterial) {};
	Material() {};
	~Material() {};
	inline const aiMaterial * GetMaterial() const { return m_pMaterial; }
	inline void SetMaterial(const aiMaterial * pAiMaterial) { m_pMaterial = pAiMaterial; }
};

class Mesh;
class Animations;
class Scene {
protected:
	unsigned short m_numMeshes;
	std::vector< Mesh* > m_meshes;

public:
	Scene(unsigned short numMeshes);
	virtual ~Scene();

	unsigned short NumMeshes()	const { return m_numMeshes; }
	Mesh*	operator[](const int idx) const { return m_meshes[idx]; };
	Mesh*&	operator[](const int idx) { return m_meshes[idx]; };

	Mesh*	operator[](const unsigned int idx) const { return m_meshes[idx]; };
	Mesh*&	operator[](const unsigned int idx) { return m_meshes[idx]; };

	std::vector< Mesh* >::iterator begin() { return std::begin(m_meshes); }
	std::vector< Mesh* >::iterator end() { return std::end(m_meshes); }
};


//=======================================================
//Mesh And Bone Data
//=======================================================
struct VertexWeight
{
	UINT	VertexID;
	float	Weight;
};

struct BoneData
{
	unsigned int			  mNumWeights;
	std::string				  BoneName;
	VertexWeight			  WightsList;
	Matrix4x4				  OffsetMatrix;
	//std::vector<VertexWeight> WightsList;

};


class Mesh
{
protected:
	friend class Scene;

	std::vector<Vertex> m_vertices;
	std::vector<Face> m_faces;

	unsigned int m_numBones;
	std::vector<BoneData> m_BoneList;

	ID3D11Buffer *mp_VBuffer;	// Vertex Buffer
	ID3D11Buffer *mp_IBuffer;	// Index Buffer

	void _CreateFromAiMesh(const aiMesh* mesh);

public:
	Mesh();
	Mesh(const aiMesh* mesh);
	virtual ~Mesh();

	void AddVertex(FLOAT x, FLOAT y, FLOAT z, const float* color);
	void AddVertex(	FLOAT x, FLOAT y, FLOAT z, 
					FLOAT nx, FLOAT ny, FLOAT nz,
					FLOAT tx, FLOAT ty, FLOAT tz,
					const float* color);

	void AddFace(unsigned int i1, unsigned int i2, unsigned int i3);

	void FinishMesh();
	
	ID3D11Buffer * VBuffer() const { return mp_VBuffer; }
	inline int NumVerts() const { return int(m_vertices.size()); }
	ID3D11Buffer * IBuffer() const { return mp_IBuffer; }
	inline int NumFaces() const { return int(m_faces.size()); }
};

//=======================================================
//Animation
//=======================================================


struct Node		//(aiNode)
{
	Node() { }
	~Node() { }
	std::string		  NodeName;				//Current Node name
	Matrix4x4		  Transformations;		//transformation which is used to send it to the shader
	Node			  *ParentNode;			//Parent of the current node
	std::vector<Node> ChildNodeList;		//Array of children nodes

};


struct VQS
{

	std::string		 Name;		//name of node which is affected by this animation
	Vector3D		Position;
	Quaternion		Rotation;
	Vector3D		Uniform_scale;

	std::vector<Vector3D> PositionList;
	std::vector<Quaternion> RotationList;
	std::vector<Vector3D> UniformScaleList;

};


struct Animation	//(aiNodeAnimations)
{
	int m_numChannels;

	std::string			Animation_Name;
	double				Duration;
	double				TicksPerSecond;
	std::vector <VQS>  ChannelList;

};


class Animations	//(aiAnimations)
{
public:
	friend class Scene;

	int mNumAnimations = 0;

	Animations(int numAnimations) : mNumAnimations(numAnimations)
	{
		AnimationList.resize(numAnimations);
	}

	~Animations() { }
	//protected:
	std::vector<Animation> AnimationList;
	Node	   m_RootNode;

};



#endif