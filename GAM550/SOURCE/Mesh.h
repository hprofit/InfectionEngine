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
	//const aiMaterial * m_pMaterial;

public:
	//Material(const aiMaterial * pAiMaterial) : m_pMaterial(pAiMaterial) {};
	Material() {};
	~Material() {};
	//inline const aiMaterial * GetMaterial() const { return m_pMaterial; }
	//inline void SetMaterial(const aiMaterial * pAiMaterial) { m_pMaterial = pAiMaterial; }
};

class Mesh;
class Scene {
protected:
	unsigned short m_numMeshes;
	std::vector< std::shared_ptr<Mesh> > m_meshes;

public:
	Scene(unsigned short numMeshes);
	~Scene();

	unsigned short NumMeshes()	const { return m_numMeshes; }
	std::shared_ptr<Mesh>	operator[](const int idx) const { return m_meshes[idx]; };
	std::shared_ptr<Mesh>&	operator[](const int idx) { return m_meshes[idx]; };

	std::shared_ptr<Mesh>	operator[](const unsigned int idx) const { return m_meshes[idx]; };
	std::shared_ptr<Mesh>&	operator[](const unsigned int idx) { return m_meshes[idx]; };

	std::vector< std::shared_ptr<Mesh> >::iterator begin() { return std::begin(m_meshes); }
	std::vector< std::shared_ptr<Mesh> >::iterator end() { return std::end(m_meshes); }
};

class Mesh
{
protected:
	friend class Scene;

	enum VBO_TYPE {
		VBO_VERTICES = 0,
		VBO_NORMALS,
		VBO_TANGENTS,
		VBO_BITANGENTS,
		VBO_FACES,
		VBO_TEX_COORDS,

		NUM_VBO_TYPES
	};

	std::vector<Vertex> m_vertices;
	std::vector<D3DVECTOR> m_normals, m_tangents, m_bitangents;
	std::vector<FLOAT> m_texCoords;
	std::vector<unsigned long> m_vertColors;
	std::vector<Face> m_faces;

	ID3D11Buffer *mp_VBuffer;

public:
	Mesh();
	//Mesh(const aiMesh* mesh);
	~Mesh();

	void AddVertex(FLOAT x, FLOAT y, FLOAT z, D3DXCOLOR color);
	void FinishMesh();

	ID3D11Buffer * const VBuffer() { return mp_VBuffer; }
};

#endif