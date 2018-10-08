/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

#define M (2*size)
#define N (size)
#define NORTH (M*(N-1))
#define SOUTH (M*(N-1)+1)
Sphere::Sphere(unsigned int size)
{
	m_vertices = std::vector<Vertex>(M*(N - 1) + 2);

	for (unsigned int i = 1; i < N; ++i) {
		float theta = PI * i / N;
		for (unsigned int j = 0; j < M; ++j) {
			int index = M * (i - 1) + j;
			float phi = 2 * PI*j / M;
			m_vertices[index].nX = sin(theta)*cos(phi);
			m_vertices[index].nY = sin(theta)*sin(phi);
			m_vertices[index].nZ = cos(theta);

			m_vertices[index].tX = sin(theta)*cos(phi);
			m_vertices[index].tY = sin(theta)*sin(phi);
			m_vertices[index].tZ = cos(theta);

			m_vertices[index].bX = sin(theta)*cos(phi);
			m_vertices[index].bY = sin(theta)*sin(phi);
			m_vertices[index].bZ = cos(theta);
		}
	}
	m_vertices[NORTH].nX = 0;
	m_vertices[NORTH].nY = 0;
	m_vertices[NORTH].nZ = 1;

	m_vertices[SOUTH].nX = 0;
	m_vertices[SOUTH].nY = 0;
	m_vertices[SOUTH].nZ = -1;

	for (unsigned int index = 0; index < m_vertices.size(); ++index) {
		m_vertices[index].x = m_vertices[index].nX * 0.5f;
		m_vertices[index].y = m_vertices[index].nY * 0.5f;
		m_vertices[index].z = m_vertices[index].nZ * 0.5f;
		m_vertices[index].w = 1;

		m_vertices[index].r = m_vertices[index].a = 1;
		m_vertices[index].g = m_vertices[index].b = 0;
	}

	for (unsigned int i = 2; i < N; ++i) {
		for (unsigned int j = 0; j < M; ++j) {
			int jp1 = (j + 1) % M;
			Face face(
				M * (i - 1) + jp1,
				M * (i - 2) + jp1,
				M * (i - 2) + j
			);
			m_faces.push_back(face);

			Face face2(
				M * (i - 1) + j,
				face[0],
				M*(i - 2) + j
			);
			m_faces.push_back(face2);
		}
	}

	for (unsigned int j = 0; j < M; ++j) {
		int jp1 = (j + 1) % M;
		Face face(
			j, 
			jp1, 
			NORTH
		);
		m_faces.push_back(face);
		
		Face face2(
			SOUTH,
			M * (N - 2) + jp1,
			M * (N - 2) + j
		);
		m_faces.push_back(face2);
	}

	Matrix4x4 Std2Unit = Matrix4x4::Scale(0.5f, 0.5f, 1)
		* Matrix4x4::Translate(Vector3D(1, 1, 0, 0))
		* Matrix4x4::Scale(1, -1, 1);
	for (unsigned int i = 0; i < m_vertices.size(); ++i) {
		Vertex v = m_vertices[i];
		Vector3D uv = Std2Unit * Vector3D(v.x, v.y, v.z, 1);
		m_vertices[i].u = uv.x;
		m_vertices[i].v = uv.y;
	}
}
#undef SOUTH
#undef NORTH
#undef N
#undef M

Sphere::~Sphere()
{
}


PolarSphere::PolarSphere(unsigned int size) : Plane(size)
{
	Matrix4x4 Std2Unit = Matrix4x4::Scale(0.5f, 0.5f, 1)
		* Matrix4x4::Translate(Vector3D(1, 1, 0, 0));

	Matrix4x4 PlaneToSphere = Matrix4x4::Scale(PI, PI / 2.0f, 1)
		* Matrix4x4::Translate(Vector3D(1, 1, 0, 0));

	for (int i = 0; i < m_vertices.size(); ++i) {
		Vertex v = m_vertices[i];
		Vector3D uv = Std2Unit * Vector3D(v.x, v.y, v.z, 1);
		m_vertices[i].u = uv.x;
		m_vertices[i].v = uv.y;


		Vector3D newPoint = PlaneToSphere * Vector3D(v.x, v.y, v.z, 1);
		m_vertices[i].x = sinf(newPoint.y) * cosf(newPoint.x);
		m_vertices[i].y = sinf(newPoint.y) * sinf(newPoint.x);
		m_vertices[i].z = cosf(newPoint.y);

		m_vertices[i].nX = m_vertices[i].x;
		m_vertices[i].nY = m_vertices[i].y;
		m_vertices[i].nZ = m_vertices[i].z;
		m_vertices[i].r = m_vertices[i].g = m_vertices[i].b = m_vertices[i].a;
	}
}

PolarSphere::~PolarSphere()
{
}
