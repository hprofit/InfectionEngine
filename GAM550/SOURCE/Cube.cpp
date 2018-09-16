/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h> 

Cube::Cube() : 
	Scene(6)
{
	Plane * pLeft = new Plane(1);
	Plane * pFront = new Plane(1);
	Plane * pRight = new Plane(1);
	Plane * pBack = new Plane(1);
	Plane * pTop = new Plane(1);
	Plane * pBottom = new Plane(1);

	pLeft->Rotate(YAXIS, -90.0f);
	pLeft->Translate(Vector3D(-1, 0,0,0));
	pLeft->FinishMesh();
	m_meshes[0] = pLeft;

	pFront->Translate(Vector3D(0, 0, 1, 0));
	pFront->FinishMesh();
	m_meshes[1] = pFront;

	pRight->Rotate(YAXIS, 90.0f);
	pRight->Translate(Vector3D(1, 0, 0, 0));
	pRight->FinishMesh();
	m_meshes[2] = pRight;

	pBack->Rotate(YAXIS, 180.0f);
	pBack->Translate(Vector3D(0, 0, -1, 0));
	pBack->FinishMesh();
	m_meshes[3] = pBack;

	pTop->Rotate(XAXIS, -90.0f);
	pTop->Translate(Vector3D(0, 1, 0, 0));
	pTop->FinishMesh();
	m_meshes[4] = pTop;

	pBottom->Rotate(XAXIS, 90.0f);
	pBottom->Translate(Vector3D(0, -1, 0, 0));
	pBottom->FinishMesh();
	m_meshes[5] = pBottom;

}

Cube::~Cube()
{
}
