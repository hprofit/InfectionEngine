/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

Plane::Plane(unsigned int size)
{
	m_vertices.reserve((size + 1)*(size + 1));
	float d = 2.0f / float(size);
	for (unsigned int j = 0; j <= size; ++j) {
		float y = j * d - 1.0f;
		for (unsigned int i = 0; i <= size; ++i) {
			float x = i * d - 1.0f;
			//int index = (size + 1)*j + i;
			Vertex v;
			v.x = x * 0.5f;
			v.y = y * 0.5f;
			v.z = 0;
			v.w = 1;

			v.nX = 0;
			v.nY = 0;
			v.nZ = 1;

			v.tX = 1;
			v.tY = 0;
			v.tZ = 0;

			v.bX = 0;
			v.bY = 1;
			v.bZ = 0;

			v.u = float(i) / float(size);
			v.v = float(j) / float(size);

			v.r = v.g = v.b = v.a = 1;

			m_vertices.push_back(v);
		}
	}

	m_faces.reserve(2 * size*size);
	for (unsigned int n = 0, j = 0; j < size; ++j) {
		for (unsigned int i = 0; i < size; ++i) {
			Face f(
				(size + 1)*j + i, 
				(size + 1)*j + i + 1,
				(size + 1)*(j + 1) + i + 1
			);
			m_faces.push_back(f);
			++n;

			Face f2(
				(size + 1)*j + i,
				(size + 1)*(j + 1) + i + 1,
				(size + 1)*(j + 1) + i
			);
			m_faces.push_back(f2);
			++n;
		}
	}
}

Plane::~Plane()
{
}

void Plane::Scale(const float & amt)
{
	Matrix4x4 scale = Matrix4x4::Scale(amt, amt, 0);

	for (int i = 0; i < m_vertices.size(); ++i) {
		Vertex v = m_vertices[i];
		Vector3D vert(v.x, v.y, v.z, 1);
		Vector3D normal(v.nX, v.nY, v.nZ, 0);
		Vector3D tangent(v.tX, v.tY, v.tZ, 0);
		Vector3D bitangent(v.bX, v.bY, v.bZ, 0);

		vert = scale * vert;
		normal = Vector3D::Normalize(scale * normal);
		tangent = Vector3D::Normalize(scale * tangent);
		bitangent = Vector3D::Normalize(scale * bitangent);

		v.x = vert.x;
		v.y = vert.y;
		v.z = vert.z;

		v.nX = normal.x;
		v.nY = normal.y;
		v.nZ = normal.z;

		v.tX = tangent.x;
		v.tY = tangent.y;
		v.tZ = tangent.z;

		v.bX = bitangent.x;
		v.bY = bitangent.y;
		v.bZ = bitangent.z;

		m_vertices[i] = v;
	}
}

void Plane::Rotate(const Vector3D & axis, float degrees)
{
	Matrix4x4 r = Matrix4x4::Rotate(degrees, axis);

	for (int i = 0; i < m_vertices.size(); ++i) {
		Vertex v = m_vertices[i];
		Vector3D vert(v.x, v.y, v.z, 1);
		Vector3D normal(v.nX, v.nY, v.nZ, 0);
		Vector3D tangent(v.tX, v.tY, v.tZ, 0);
		Vector3D bitangent(v.bX, v.bY, v.bZ, 0);

		vert = r * vert;
		normal = r * normal;
		tangent = r * tangent;
		bitangent = r * bitangent;
		
		v.x = vert.x;
		v.y = vert.y;
		v.z = vert.z;

		v.nX = normal.x;
		v.nY = normal.y;
		v.nZ = normal.z;

		v.tX = tangent.x;
		v.tY = tangent.y;
		v.tZ = tangent.z;

		v.bX = bitangent.x;
		v.bY = bitangent.y;
		v.bZ = bitangent.z;

		m_vertices[i] = v;
	}
}

void Plane::Translate(const Vector3D & amount)
{
	for (int i = 0; i < m_vertices.size(); ++i) {
		Vertex v = m_vertices[i];
		Vector3D vert(v.x, v.y, v.z, 1);
		Vector3D normal(v.nX, v.nY, v.nZ, 0);
		Vector3D tangent(v.tX, v.tY, v.tZ, 0);
		Vector3D bitangent(v.bX, v.bY, v.bZ, 0);

		vert = amount + vert;
		normal = amount + normal;
		tangent = amount + tangent;
		bitangent = amount + bitangent;

		v.x = vert.x;
		v.y = vert.y;
		v.z = vert.z;

		v.nX = normal.x;
		v.nY = normal.y;
		v.nZ = normal.z;

		v.tX = tangent.x;
		v.tY = tangent.y;
		v.tZ = tangent.z;

		v.bX = bitangent.x;
		v.bY = bitangent.y;
		v.bZ = bitangent.z;

		m_vertices[i] = v;
	}
}
