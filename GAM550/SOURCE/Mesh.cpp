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

//Please Dont remove this at any point

void Mesh::ReadBoneVertexWeight(const aiMesh * mesh)
{

	int temp_index = -1;
	int size_list = mesh->mNumVertices;
	m_BoneVertexDataList.resize(size_list);

	for (int i = 0; i < mesh->mNumVertices; ++i)
	{
		
		for (int j = 0; j < mesh->mNumBones; ++j)
		{
			for (int k = 0; k < mesh->mBones[j]->mNumWeights; ++k)
			{
				if (mesh->mBones[j]->mWeights[k].mVertexId == i)
				{
					m_BoneVertexDataList[i].BoneID[temp_index++] = j;
					m_BoneVertexDataList[i].BoneWeights[temp_index++] = mesh->mBones[j]->mWeights[k].mWeight;
				}
			}
		}
		temp_index = 0;
	}


}


void Mesh::_CreateFromAiMesh(const aiMesh * mesh)
{
	m_vertices.reserve(mesh->mNumVertices);
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex v;
		v.x = mesh->mVertices[i].x;
		v.y = mesh->mVertices[i].y;
		v.z = mesh->mVertices[i].z;
		v.w = 1;

		//v.nX = mesh->mNormals[i].x;
		//v.nY = mesh->mNormals[i].y;
		//v.nZ = mesh->mNormals[i].z;

		//v.tX = mesh->mTangents[i].x;
		//v.tY = mesh->mTangents[i].y;
		//v.tZ = mesh->mTangents[i].z;
		//
		//v.bX = mesh->mBitangents[i].x;
		//v.bY = mesh->mBitangents[i].y;
		//v.bZ = mesh->mBitangents[i].z;
		//
		//v.u = mesh->mTextureCoords[0][i].x;
		//v.v = mesh->mTextureCoords[0][i].y;

		if (mesh->mColors && mesh->mColors[0]) {
			aiColor4D color = mesh->mColors[0][i];
			v.r = color.r;
			v.g = color.g;
			v.b = color.b;
			v.a = color.a;
		}
		for (int j = 0; j < 4; ++j)
		{
			v.BoneID0 = m_BoneVertexDataList[i].BoneID[0];
			v.BoneID1 = m_BoneVertexDataList[i].BoneID[1];
			v.BoneID2 = m_BoneVertexDataList[i].BoneID[2];
			v.BoneID3 = m_BoneVertexDataList[i].BoneID[3];

			v.BoneWeights0 = m_BoneVertexDataList[i].BoneWeights[0];
			v.BoneWeights1 = m_BoneVertexDataList[i].BoneWeights[1];
			v.BoneWeights2 = m_BoneVertexDataList[i].BoneWeights[2];
			v.BoneWeights3 = m_BoneVertexDataList[i].BoneWeights[3];

			//v.BoneID[j] = m_BoneVertexDataList[i].BoneID[j];
			//v.BoneWeights[j] = m_BoneVertexDataList[i].BoneWeights[j];

		}

		m_vertices.push_back(v);
	}

	m_faces.reserve(mesh->mNumFaces);
	for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
	{
		m_faces.push_back(Face(mesh->mFaces[i]));
	}

#pragma region Bone Data

		
	m_BoneList.resize(mesh->mNumBones);
	for (unsigned int i = 0; i < mesh->mNumBones; ++i)
	{
		m_BoneList[i].BoneName = mesh->mBones[i]->mName.C_Str();
		m_BoneList[i].mNumWeights = mesh->mBones[i]->mNumWeights;

		//Offset Matrix Data

		m_BoneList[i].OffsetMatrix.Set(0, 0, mesh->mBones[i]->mOffsetMatrix.a1);
		m_BoneList[i].OffsetMatrix.Set(0, 1, mesh->mBones[i]->mOffsetMatrix.a2);
		m_BoneList[i].OffsetMatrix.Set(0, 2, mesh->mBones[i]->mOffsetMatrix.a3);
		m_BoneList[i].OffsetMatrix.Set(0, 3, mesh->mBones[i]->mOffsetMatrix.a4);

		m_BoneList[i].OffsetMatrix.Set(1, 0, mesh->mBones[i]->mOffsetMatrix.b1);
		m_BoneList[i].OffsetMatrix.Set(1, 1, mesh->mBones[i]->mOffsetMatrix.b2);
		m_BoneList[i].OffsetMatrix.Set(1, 2, mesh->mBones[i]->mOffsetMatrix.b3);
		m_BoneList[i].OffsetMatrix.Set(1, 3, mesh->mBones[i]->mOffsetMatrix.b4);

		m_BoneList[i].OffsetMatrix.Set(2, 0, mesh->mBones[i]->mOffsetMatrix.c1);
		m_BoneList[i].OffsetMatrix.Set(2, 1, mesh->mBones[i]->mOffsetMatrix.c2);
		m_BoneList[i].OffsetMatrix.Set(2, 2, mesh->mBones[i]->mOffsetMatrix.c3);
		m_BoneList[i].OffsetMatrix.Set(2, 3, mesh->mBones[i]->mOffsetMatrix.c4);

		m_BoneList[i].OffsetMatrix.Set(3, 0, mesh->mBones[i]->mOffsetMatrix.d1);
		m_BoneList[i].OffsetMatrix.Set(3, 1, mesh->mBones[i]->mOffsetMatrix.d2);
		m_BoneList[i].OffsetMatrix.Set(3, 2, mesh->mBones[i]->mOffsetMatrix.d3);
		m_BoneList[i].OffsetMatrix.Set(3, 3, mesh->mBones[i]->mOffsetMatrix.d4);


		//Vertex Data

		m_BoneList[i].WightsList.VertexID = mesh->mBones[i]->mWeights->mVertexId;
		m_BoneList[i].WightsList.Weight = mesh->mBones[i]->mWeights->mWeight;

		
	}
#pragma endregion

	//cout << "The size of Bone data : " << mesh->mNumBones << endl;
	//cout << "The size of Vertices data : " << mesh->mNumVertices * mesh->mNumBones << endl;
	//cout << temp_index << endl;

	FinishMesh();
}

Mesh::Mesh()
{

}

Mesh::Mesh(const aiMesh * mesh)
{
	
	ReadBoneVertexWeight(mesh);
	_CreateFromAiMesh(mesh);
}

Mesh::~Mesh()
{
	m_vertices.clear();
	m_faces.clear();
	mp_VBuffer->Release();
}

void Mesh::AddVertex(FLOAT x, FLOAT y, FLOAT z, const float* color)
{
	Vertex vert;
	vert.x = x;
	vert.y = y;
	vert.z = z;
	vert.w = 1;

	vert.r = color[0];
	vert.g = color[1];
	vert.b = color[2];
	vert.a = color[3];

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

void Mesh::AddVertex(FLOAT x, FLOAT y, FLOAT z, FLOAT nx, FLOAT ny, FLOAT nz, FLOAT tx, FLOAT ty, FLOAT tz, const float* color)
{
	Vertex vert;
	vert.x = x;
	vert.y = y;
	vert.z = z;
	vert.w = 1;

	vert.r = color[0];
	vert.g = color[1];
	vert.b = color[2];
	vert.a = color[3];

	vert.nX = nx;
	vert.nY = ny;
	vert.nZ = nz;

	vert.tX = tx;
	vert.tY = ty;
	vert.tZ = tz;

	Vector3D bitangent = Vector3D::Cross(Vector3D(nx, ny, nz, 0), Vector3D(tx, ty, tz, 0));

	vert.bX = bitangent.x;
	vert.bY = bitangent.y;
	vert.bZ = bitangent.z;

	vert.u = 0;
	vert.v = 0;

	m_vertices.push_back(vert);
}

void Mesh::AddFace(unsigned int i1, unsigned int i2, unsigned int i3)
{
	m_faces.push_back(Face(i1, i2, i3));
}

void Mesh::FinishMesh()
{
	D3D11_BUFFER_DESC vertexbd;
	ZeroMemory(&vertexbd, sizeof(vertexbd));
	int vertSize = sizeof(Vertex) * UINT(m_vertices.size());
	vertexbd.Usage = D3D11_USAGE_DYNAMIC;				// write access access by CPU and GPU
	vertexbd.ByteWidth = vertSize;						// size is the Vertex struct * m_vertices.size
	vertexbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// use as a vertex buffer
	vertexbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// allow CPU to write in buffer

	INFECT_RENDERER.Device()->CreateBuffer(&vertexbd, NULL, &mp_VBuffer);       // create the buffer

	D3D11_MAPPED_SUBRESOURCE vMS;
	INFECT_RENDERER.DeviceContext()->Map(mp_VBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &vMS);	// map the buffer
	memcpy(vMS.pData, &(m_vertices[0]), vertSize);													// copy the data
	INFECT_RENDERER.DeviceContext()->Unmap(mp_VBuffer, NULL);										// unmap the buffer


	D3D11_BUFFER_DESC indexbd;
	ZeroMemory(&indexbd, sizeof(indexbd));
	int indexSize = sizeof(Face) * UINT(m_faces.size());

	indexbd.Usage = D3D11_USAGE_DYNAMIC;				// write access access by CPU and GPU
	indexbd.ByteWidth = indexSize;						// size is the Face struct * m_faces.size
	indexbd.BindFlags = D3D11_BIND_INDEX_BUFFER;		// use as a vertex buffer
	indexbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// allow CPU to write in buffer
	indexbd.MiscFlags = 0;

	INFECT_RENDERER.Device()->CreateBuffer(&indexbd, NULL, &mp_IBuffer);	// create the buffer

	D3D11_MAPPED_SUBRESOURCE iMS;
	INFECT_RENDERER.DeviceContext()->Map(mp_IBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &iMS);   // map the buffer
	memcpy(iMS.pData, &(m_faces[0]), indexSize);	// copy the data
	INFECT_RENDERER.DeviceContext()->Unmap(mp_IBuffer, NULL);					// unmap the buffer


	ID3D11InputLayout *pLayout;
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORDS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONEID", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONEWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	ID3D10Blob * const vs = INFECT_RENDERER.VSBlob();
	INFECT_RENDERER.Device()->CreateInputLayout(ied, 6, vs->GetBufferPointer(), vs->GetBufferSize(), &pLayout);
	INFECT_RENDERER.DeviceContext()->IASetInputLayout(pLayout);
}