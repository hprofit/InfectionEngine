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
	for (int j = 0; j <= size; ++j) {
		float y = j * d - 1.0f;
		for (int i = 0; i <= size; ++i) {
			float x = i * d - 1.0f;
			//int index = (size + 1)*j + i;
			Vertex v;
			v.x = x;
			v.y = y;
			v.z = 0;

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

			v.color = D3DXCOLOR(0, 0, 0, 1);

			m_vertices.push_back(v);
		}
	}

	m_faces.reserve(2 * size*size);
	for (int n = 0, j = 0; j < size; ++j) {
		for (int i = 0; i < size; ++i) {
			Face f(
				(size + 1)*j + i, 
				(size + 1)*j + i + 1,
				(size + 1)*(j + 1) + i + 1
			);
			m_faces.push_back(f);
			//m_faces[n][0] = (size + 1)*j + i;
			//m_faces[n][1] = (size + 1)*j + i + 1;
			//m_faces[n][2] = (size + 1)*(j + 1) + i + 1;
			++n;

			Face f2(
				(size + 1)*j + i,
				(size + 1)*(j + 1) + i + 1,
				(size + 1)*(j + 1) + i
			);
			m_faces.push_back(f2);
			//m_faces[n][0] = (size + 1)*j + i;
			//m_faces[n][1] = (size + 1)*(j + 1) + i + 1;
			//m_faces[n][2] = (size + 1)*(j + 1) + i;
			++n;
		}
	}
}

Plane::~Plane()
{
}
