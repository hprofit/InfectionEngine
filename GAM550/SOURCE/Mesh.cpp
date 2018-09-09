/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

Scene::Scene(unsigned short numMeshes) : 
	m_numMeshes(numMeshes)
{
	m_meshes = std::vector<Mesh*>(m_numMeshes);
}

Scene::~Scene() {}

void Mesh::_CreateFromAiMesh(const aiMesh * mesh)
{
	m_vertices.reserve(mesh->mNumVertices);
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
		Vertex v;
		v.x = mesh->mVertices[i].x;
		v.y = mesh->mVertices[i].y;
		v.z = mesh->mVertices[i].z;

		v.nX = mesh->mNormals[i].x;
		v.nY = mesh->mNormals[i].y;
		v.nZ = mesh->mNormals[i].z;

		v.tX = mesh->mTangents[i].x;
		v.tY = mesh->mTangents[i].y;
		v.tZ = mesh->mTangents[i].z;

		v.bX = mesh->mBitangents[i].x;
		v.bY = mesh->mBitangents[i].y;
		v.bZ = mesh->mBitangents[i].z;

		v.u = mesh->mTextureCoords[0][i].x;
		v.v = mesh->mTextureCoords[0][i].y;

		//if (mesh->mColors[0][i]) {
		//	aiColor4D color = mesh->mColors[0][i];
		//	v.color = D3DXCOLOR(color.r, color.g, color.b, color.a);
		//}
		v.color = D3DXCOLOR(0,1,1,1);//D3DXCOLOR(color.r, color.g, color.b, color.a);

		m_vertices.push_back(v);
	}

	m_faces.reserve(mesh->mNumFaces);
	for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
		m_faces.push_back(Face(mesh->mFaces[i]));
	}
}

Mesh::Mesh()
{
}

Mesh::Mesh(const aiMesh * mesh)
{
	_CreateFromAiMesh(mesh);
}

Mesh::~Mesh()
{
	m_vertices.clear();
	m_faces.clear();
}

void Mesh::AddVertex(FLOAT x, FLOAT y, FLOAT z, D3DXCOLOR color)
{
	Vertex vert;
	vert.x = x;
	vert.y = y;
	vert.z = z;
	vert.color = color;

	vert.nX = 0;
	vert.nY = 0;
	vert.nZ = 0;

	vert.tX = 0;
	vert.tY = 0;
	vert.tZ = 0;

	vert.bX = 0;
	vert.bY = 0;
	vert.bZ = 0;

	vert.u = 0;
	vert.v = 0;

	m_vertices.push_back(vert);
}

void Mesh::FinishMesh()
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;							// write access access by CPU and GPU
	bd.ByteWidth = sizeof(Vertex) * UINT(m_vertices.size());		// size is the VERTEX struct * m_vertices.size
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;				// use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;				// allow CPU to write in buffer

	INFECT_RENDERER.Device()->CreateBuffer(&bd, NULL, &mp_VBuffer);       // create the buffer

	D3D11_MAPPED_SUBRESOURCE ms;
	INFECT_RENDERER.DeviceContext()->Map(mp_VBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);   // map the buffer
	memcpy(ms.pData, &(m_vertices[0]), m_vertices.size() * sizeof(Vertex));	// copy the data
	INFECT_RENDERER.DeviceContext()->Unmap(mp_VBuffer, NULL);				// unmap the buffer

	ID3D11InputLayout *pLayout;
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORDS", 0, DXGI_FORMAT_R16G16_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	ID3D10Blob * const vs = INFECT_RENDERER.VSBlob();
	INFECT_RENDERER.Device()->CreateInputLayout(ied, 2, vs->GetBufferPointer(), vs->GetBufferSize(), &pLayout);
	INFECT_RENDERER.DeviceContext()->IASetInputLayout(pLayout);
}
