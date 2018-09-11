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
	D3DXCOLOR color;	// color
};

struct TexCoords {
	FLOAT u, v;
	TexCoords(float _u, float _v) : u(_u), v(_v) {}
	TexCoords(const D3DVECTOR* uv) : u(uv->x), v(uv->y) {}
	FLOAT operator[](int idx) {
		return idx == 0 ? u : v;
	}
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
class Scene {
protected:
	unsigned short m_numMeshes;
	std::vector< Mesh* > m_meshes;

public:
	Scene(unsigned short numMeshes);
	~Scene();

	unsigned short NumMeshes()	const { return m_numMeshes; }
	Mesh*	operator[](const int idx) const { return m_meshes[idx]; };
	Mesh*&	operator[](const int idx) { return m_meshes[idx]; };

	Mesh*	operator[](const unsigned int idx) const { return m_meshes[idx]; };
	Mesh*&	operator[](const unsigned int idx) { return m_meshes[idx]; };

	std::vector< Mesh* >::iterator begin() { return std::begin(m_meshes); }
	std::vector< Mesh* >::iterator end() { return std::end(m_meshes); }
};

class Mesh
{
protected:
	friend class Scene;

	std::vector<Vertex> m_vertices;
	std::vector<Face> m_faces;
	std::vector<DWORD> m_faces2;

	ID3D11Buffer *mp_VBuffer;	// Vertex Buffer
	ID3D11Buffer *mp_IBuffer;	// Index Buffer

	void _CreateFromAiMesh(const aiMesh* mesh);

public:
	Mesh();
	Mesh(const aiMesh* mesh);
	~Mesh();

	void AddVertex(FLOAT x, FLOAT y, FLOAT z, D3DXCOLOR color);
	void AddVertex(	FLOAT x, FLOAT y, FLOAT z, 
					FLOAT nx, FLOAT ny, FLOAT nz,
					FLOAT tx, FLOAT ty, FLOAT tz,
					D3DXCOLOR color);

	void AddFace(unsigned int i1, unsigned int i2, unsigned int i3);

	void FinishMesh();
	
	ID3D11Buffer * VBuffer() const { return mp_VBuffer; }
	inline int NumVerts() const { return int(m_vertices.size()); }
	ID3D11Buffer * IBuffer() const { return mp_IBuffer; }
	inline int NumFaces() const { return int(m_faces.size()); }
};

#endif