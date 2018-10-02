/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef SPHERE_MESH_H
#define SPHERE_MESH_H

#define DEFAULT_SPHERE_SIZE 40
#define SPHERE_PRIMITIVE "Sphere"
#define SIMPLE_SPHERE_PRIMITIVE "SimpleSphere"
//#define POLAR_SPHERE_PRIMITIVE "PolarSphere"

class Sphere : 
	public Mesh
{
public:
	Sphere(unsigned int size = DEFAULT_SPHERE_SIZE);
	~Sphere();
};

class PolarSphere :
	public Plane
{
public:
	PolarSphere(unsigned int size = DEFAULT_SPHERE_SIZE);
	~PolarSphere();
};

#endif