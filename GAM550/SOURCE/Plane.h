/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef PLANE_MESH_H
#define PLANE_MESH_H

#define DEFAULT_PLANE_SIZE 20
#define PLANE_PRIMITIVE "Plane"
#define QUAD_PRIMITIVE "Quad"

class Plane : 
	public Mesh
{
protected:

public:
	Plane(unsigned int size=DEFAULT_PLANE_SIZE);
	~Plane();

	void Scale(const float& amt);
	void Rotate(const Vector3D& axis, float degrees);
	void Translate(const Vector3D& amount);
};

#undef DEFAULT_PLANE_SIZE
#endif