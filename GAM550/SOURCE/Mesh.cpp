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
	m_meshes = std::vector < std::shared_ptr<Mesh> >(m_numMeshes);
}

Scene::~Scene() {}

Mesh::Mesh()
{
	//std::fill(m_vboID, m_vboID + NUM_VBO_TYPES / sizeof(GLuint), 0);
}

Mesh::Mesh(const aiMesh * mesh)
{
	//std::fill(m_vboID, m_vboID + NUM_VBO_TYPES / sizeof(GLuint), 0);
	//CreateFromAiMesh(mesh);
}

Mesh::~Mesh()
{
	m_vertices.clear();
	m_normals.clear();
	m_tangents.clear();
	m_bitangents.clear();
	m_faces.clear();
}

void Mesh::AddVertex(FLOAT x, FLOAT y, FLOAT z, D3DXCOLOR color)
{
	Vertex vert;
	vert.x = x;
	vert.y = y;
	vert.z = z;
	vert.color = color;
	m_vertices.push_back(vert);
}

void Mesh::FinishMesh()
{
	Vertex OurVertices[] =
	{
		{ 0.0f, 0.5f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f) },
		{ 0.45f, -0.5, 0.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) },
		{ -0.45f, -0.5f, 0.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f) }
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;							// write access access by CPU and GPU
	//bd.ByteWidth = sizeof(Vertex) * m_vertices.size();		// size is the VERTEX struct * m_vertices.size
	bd.ByteWidth = sizeof(Vertex) * 3;		
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;				// use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;				// allow CPU to write in buffer

	INFECT_RENDERER.Device()->CreateBuffer(&bd, NULL, &mp_VBuffer);       // create the buffer

	D3D11_MAPPED_SUBRESOURCE ms;
	INFECT_RENDERER.DeviceContext()->Map(mp_VBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);   // map the buffer
	//memcpy(ms.pData, &(m_vertices[0]), m_vertices.size() * sizeof(Vertex));	// copy the data
	memcpy(ms.pData, OurVertices, sizeof(OurVertices));	// copy the data
	INFECT_RENDERER.DeviceContext()->Unmap(mp_VBuffer, NULL);				// unmap the buffer

	ID3D11InputLayout *pLayout;
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	ID3D10Blob * const vs = INFECT_RENDERER.VSBlob();
	INFECT_RENDERER.Device()->CreateInputLayout(ied, 2, vs->GetBufferPointer(), vs->GetBufferSize(), &pLayout);
	INFECT_RENDERER.DeviceContext()->IASetInputLayout(pLayout);
}
